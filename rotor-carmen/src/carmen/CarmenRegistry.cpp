#include "CarmenRegistry.h"
#include <_carmen/CarmenHandler.h>
#include <_carmen/FormatString.h>
#include <rotor/Lock.h>
#include <rotor/Logger.h>
#include <rotor/Message.h>
#include <rotor/Options.h>
#include <rotor/Rotor.h>
#include <rotor/Structure.h>
#include <rotor/Conversion.h>
#include <rotor/Time.h>
#include <rotor/TypedThread.h>
#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

ROTOR_REGISTRY_FACTORY( CarmenRegistry )

//------------------------------------------------------------------------------

CarmenRegistry::CarmenRegistry( const string & name, Options & options)
  : Registry( name, options ),
    _name( name ),
    _options( options ),
    _registry( name, options ),
    _queueHandler( 2, DISCARD_OLDEST )
{
  Logger::setLevel(
    static_cast<Logger::Level>( options.getInt( "CarmenRegistry", "loggingLevel",  3 ) ),
    "CarmenRegistry"
  );
  stringstream tmpName;
  tmpName << setprecision( 10 );
  tmpName << name << "_" << seconds();

  Logger::info( "Establishing temporary connection", "CarmenRegistry" );
  // Temporary connection, needed to call IPC_isModuleConnected
  try {
    if ( IPC_connectModule( tmpName.str().c_str(), options.getString( "rotor_server", "serverIp" ).c_str() ) == IPC_Error ) {
      fprintf( stderr, "Could not connect IPC\n" );
      exit( 1 );
    }
  } catch ( ... ) {
    if ( IPC_connectModule( tmpName.str().c_str(), NULL ) == IPC_Error ) {
      fprintf( stderr, "Could not connect IPC\n" );
      exit( 1 );
    }
  }

  Logger::info(
    "Checking if module with same name is already connected",
    "CarmenRegistry"
  );
  if ( IPC_isModuleConnected( name.c_str() ) == 1 ) {
    fprintf( stderr, "Module '%s' is already connected\n", name.c_str() );
    exit( 1 );
  }

  Logger::info( "Disconnecting temporary connection", "CarmenRegistry" );
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Error connecting to IPC\n" );
    exit( 1 );
  }

  Logger::info( "Establishing definitive connection", "CarmenRegistry" );
  try {
    if ( IPC_connectModule( name.c_str(), options.getString( "rotor_server", "serverIp" ).c_str() ) == IPC_Error ) {
      fprintf( stderr, "Could not connect IPC\n" );
      exit( 1 );
    }
  } catch ( ... ) {
    if ( IPC_connectModule( name.c_str(), NULL ) == IPC_Error ) {
      fprintf( stderr, "Could not connect IPC\n" );
      exit( 1 );
    }
  }

  IPC_setCapacity( 4 );

  Logger::spam( "Setting up carmen handler", "CarmenRegistry" );
  _handler = new CarmenHandler( *this );
  Logger::debug( "Created CarmenRegistry for " + name, "CarmenRegistry" );
}

//------------------------------------------------------------------------------

CarmenRegistry::~CarmenRegistry()
{
  Logger::info( "Deleting carmen handler", "CarmenRegistry" );
  delete _handler;
  Logger::info( "Disconnecting from IPC", "CarmenRegistry" );
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Could not disconnect IPC\n" );
    exit( 1 );
  }
  Logger::info( "Carmen registry was successfully destroyed", "CarmenRegistry" );
}

//------------------------------------------------------------------------------

const std::string &
CarmenRegistry::name() const
{
  return _name;
}

//------------------------------------------------------------------------------

Options &
CarmenRegistry::options() const
{
  return _options;
}

//------------------------------------------------------------------------------

const Type &
CarmenRegistry::registerType( const string & definitionString )
{
  return _registry.registerType( definitionString );
}

//------------------------------------------------------------------------------

const Type &
CarmenRegistry::operator[]( const std::string & typeName ) const
{
  return _registry[typeName];
}

//------------------------------------------------------------------------------

void
CarmenRegistry::registerMessage(
  const string & messageName,
  const string & typeName )
{
  Lock lock( _ipcMutex );
  Logger::debug(
    string( "Registered " ) + messageName + " with type: " + typeName +
    " and format: " + formatString( *this, typeName ),
    "CarmenRegistry"
  );
  _registry.registerMessage( messageName, typeName );
  if ( ! IPC_isMsgDefined( messageName.c_str() ) ) {
    if (  IPC_defineMsg(
            messageName.c_str(),
            IPC_VARIABLE_LENGTH,
            formatString( *this, typeName ).c_str() )
          == IPC_Error )
    {
      fprintf( stderr, "Could not define message\n" );
      exit( 1 );
    }
  }
}

//------------------------------------------------------------------------------

void
CarmenRegistry::subscribeToMessage(
  const std::string & messageName,
  bool queueOwner,
  size_t queueCapacity,
  QueuePolicy queuePolicy )
{
  printf("%s %d\n", messageName.c_str(), queueOwner);
  Lock lock( _ipcMutex );
  _queueHandler.subscribeToMessage(
    messageName, queueOwner, queueCapacity, queuePolicy );
  if ( IPC_subscribeData(  messageName.c_str(), CarmenHandler::handleMessage, _handler ) == IPC_Error ) {
    fprintf( stderr, "Could not define message\n" );
    exit( 1 );
  }
  IPC_setMsgQueueLength( messageName.c_str(), 10 );

  TimestampQueues::iterator it = _timestampQueues.find( messageName );
  if ( it == _timestampQueues.end() ) {
    TimestampQueue * queue = 0;
    queue = new TimestampQueue();
    _timestampQueues[messageName] = queue;
  }
}

//------------------------------------------------------------------------------

void
CarmenRegistry::subscribeToQuery( const string & messageName )
{
  Lock lock( _ipcMutex );
  if ( IPC_subscribeData(  messageName.c_str(), CarmenHandler::handleQuery, _handler ) == IPC_Error ) {
    fprintf( stderr, "Could not define message\n" );
    exit( 1 );
  }
  IPC_setMsgQueueLength( messageName.c_str(), 10 );
}

//------------------------------------------------------------------------------

const Type &
CarmenRegistry::messageType( const string & messageName ) const
{
  return _registry.messageType( messageName );
}

//------------------------------------------------------------------------------

double
CarmenRegistry::messageFrequency( const std::string & messageName ) const
{
  Lock lock( _ipcMutex );
  double frequency = 0.0;
  TimestampQueues::const_iterator it = _timestampQueues.find( messageName );

  if ( it != _timestampQueues.end() ) {
    TimestampQueue* timestampQueue = it->second;
    double now = seconds();

    while ( !timestampQueue->empty() && ( now-timestampQueue->back() > 1 ) )
      timestampQueue->pop_back();

    if (!timestampQueue->empty())
      frequency = timestampQueue->size()/(now-timestampQueue->back());
  }

  return frequency;
}

//------------------------------------------------------------------------------

void
CarmenRegistry::sendMessage( const Message & message )
{
  Logger::spam( "Publishing message name:" + message.name(), "CarmenRegistry" );
  _outputQueue.push( message );
  Logger::spam( "Message published", "CarmenRegistry" );
}

//------------------------------------------------------------------------------

Message
CarmenRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "Receiving message", "CarmenRegistry" );
    return _queueHandler.dequeueMessage( timeout );
  } catch ( ... ) {
    Logger::spam( "Receive message timed out", "CarmenRegistry" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message
CarmenRegistry::receiveMessage( const string & messageName, double timeout )
throw( MessagingTimeout )
{
  try {
    Logger::spam( "Receiving message", "CarmenRegistry" );
    Structure data( _queueHandler.dequeueMessage( messageName, timeout ) );
    return Message( messageName, data );
  } catch ( TimeoutException ) {
    Logger::spam( "Receive message timed out", "CarmenRegistry" );
    throw MessagingTimeout( "No message was received" );
  } catch ( ... ) {
    //TODO: Fix this, python does not work without this ugly workaround
    Logger::spam( "Receive message timed out", "CarmenRegistry" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Structure
CarmenRegistry::query( const Message & message, double timeout )
throw( MessagingTimeout )
{
  _ipcMutex.lock();
  Logger::spam( "Calling IPC query notify", "CarmenRegistry" );
  IPC_RETURN_TYPE result =  IPC_queryNotifyData(
                              message.name().c_str(),
                              message.data().buffer(),
                              CarmenHandler::handleReply,
                              _handler );

  Logger::spam( "IPC query notify returned from call", "CarmenRegistry" );
  _ipcMutex.unlock();
  if (  result == IPC_Error ) {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
  try {
    Logger::spam( "Dequeuing reply", "CarmenRegistry" );
    Structure result =  _handler->dequeueReply( timeout );
    return result;
  } catch ( TimeoutException ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message
CarmenRegistry::receiveQuery( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "ReceivingQuery", "CarmenRegistry" );
    pair<Message, MSG_INSTANCE> result = _handler->dequeueQuery( timeout );
    Logger::spam( "Received query name: " + result.first.name(), "CarmenRegistry" );
    return result.first;
  } catch ( TimeoutException ) {
    Logger::spam( "Receive query timed out", "CarmenRegistry" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

void
CarmenRegistry::reply( const Message & message )
{
  _handler->reply( message );
}
