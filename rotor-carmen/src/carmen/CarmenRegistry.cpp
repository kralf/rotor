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
#include <carmen/ipc.h>
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
    _queueHandler( 1, DISCARD_OLDEST )
{
  stringstream tmpName;
  tmpName << setprecision( 10 );
  tmpName << name << "_" << seconds();

  Logger::info( "Establishing temporary connection" );
  // Temporary connection, needed to call IPC_isModuleConnected
  try {
    if ( IPC_connectModule( tmpName.str().c_str(), options.getString( "BOOTSTRAP", "server" ).c_str() ) == IPC_Error ) {
      fprintf( stderr, "Could not connect IPC\n" );
      exit( 1 );
    }
  } catch ( ... ) {  
    if ( IPC_connectModule( tmpName.str().c_str(), NULL ) == IPC_Error ) {
      fprintf( stderr, "Could not connect IPC\n" );
      exit( 1 );
    }
  }

  Logger::info( "Checking with same name is already connected" );
  if ( IPC_isModuleConnected( name.c_str() ) == 1 ) {
    fprintf( stderr, "Module '%s' is already connected\n", name.c_str() );
    exit( 1 );
  }
  
  Logger::info( "Disconnecting temporary connection" );
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Error connecting to IPC\n" );
    exit( 1 );
  }
  
  Logger::info( "Establishing definitive connection" );
  try {
    if ( IPC_connectModule( name.c_str(), options.getString( "BOOTSTRAP", "server" ).c_str() ) == IPC_Error ) {
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
  
  Logger::spam( "Setting up carmen handler" );
  _handler = new CarmenHandler( *this );
  Logger::debug( "Created CarmenRegistry for " + name );
}

//------------------------------------------------------------------------------

CarmenRegistry::~CarmenRegistry()
{
  Logger::info( "Deleting carmen handler" );
  delete _handler;
  Logger::info( "Disconnecting from IPC" );
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Could not disconnect IPC\n" );
    exit( 1 );
  }
  Logger::info( "Carmen registry was successfully destroyed" );
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
    " and format: " + formatString( *this, typeName ) );
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
  Lock lock( _ipcMutex );
  _queueHandler.subscribeToMessage( 
    messageName, queueOwner, queueCapacity, queuePolicy );
  if ( IPC_subscribeData(  messageName.c_str(), CarmenHandler::handleMessage, _handler ) == IPC_Error ) {
    fprintf( stderr, "Could not define message\n" );
    exit( 1 );
  }
  IPC_setMsgQueueLength( messageName.c_str(), 10 );
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
  IPC_setMsgQueueLength( messageName.c_str(), 1 );
}

//------------------------------------------------------------------------------

const Type & 
CarmenRegistry::messageType( const string & messageName ) const
{
  return _registry.messageType( messageName );
}

//------------------------------------------------------------------------------

void 
CarmenRegistry::sendMessage( const Message & message )
{
  Logger::spam( "Publishing message name:" + message.name() );
  _outputQueue.push( message );
  Logger::spam( "Message published" );
}

//------------------------------------------------------------------------------

Message 
CarmenRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "Receiving message" );
    return _queueHandler.dequeueMessage( timeout );
  } catch ( ... ) {
    Logger::spam( "Receive message timed out" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message 
CarmenRegistry::receiveMessage( const string & messageName, double timeout ) 
throw( MessagingTimeout )
{
  try {
    Logger::spam( "Receiving message" );
    LightweightStructure data( _queueHandler.dequeueMessage( messageName, timeout ) );
    return Message( messageName, data );
  } catch ( ... ) {
    Logger::spam( "Receive message timed out" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

LightweightStructure
CarmenRegistry::query( const Message & message, double timeout ) 
throw( MessagingTimeout )
{
  _ipcMutex.lock();
  IPC_RETURN_TYPE result =  IPC_queryNotifyData(
                              message.name().c_str(),
                              message.data().buffer(),
                              CarmenHandler::handleReply,
                              _handler );
                            
  _ipcMutex.unlock();
  if (  result == IPC_Error ) {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
  try {
    return _handler->dequeueReply( timeout );
  } catch ( ... ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message 
CarmenRegistry::receiveQuery( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "ReceivingQuery" );
    pair<Message, MSG_INSTANCE> result = _handler->dequeueQuery( timeout );
    Logger::spam( "Received query name: " + result.first.name() );
    return result.first;
  } catch ( ... ) {
    Logger::spam( "Receive query timed out" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

void
CarmenRegistry::reply( const Message & message ) 
{
  _handler->reply( message );
}
