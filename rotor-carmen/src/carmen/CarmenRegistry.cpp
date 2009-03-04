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
    _registry( name, options )
{
  stringstream tmpName;
  tmpName << setprecision( 10 );
  tmpName << name << "_" << seconds();

  Logger::spam( "Establishing temporary connection" );
  // Temporary connection, needed to call IPC_isModuleConnected
  if ( IPC_connectModule( tmpName.str().c_str(), options.getString( "BOOTSTRAP", "server" ).c_str() ) == IPC_Error ) {
    fprintf( stderr, "Could not connect IPC\n" );
    exit( 1 );
  }

  Logger::spam( "Checking with same name is already connected" );
  if ( IPC_isModuleConnected( name.c_str() ) == 1 ) {
    fprintf( stderr, "Module '%s' is already connected\n", name.c_str() );
    exit( 1 );
  }
  
  Logger::spam( "Disconnecting temporary connection" );
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Error connecting to IPC\n" );
    exit( 1 );
  }
  
  Logger::spam( "Establishing definitive connection" );
  if ( IPC_connectModule( name.c_str(), options.getString( "BOOTSTRAP", "server" ).c_str() ) == IPC_Error ) {
    fprintf( stderr, "Could not connect IPC\n" );
    exit( 1 );
  }
  
  IPC_setCapacity( 4 );
  
  Logger::spam( "Setting up carmen handler" );
  _handler = new CarmenHandler( *this );
  Logger::debug( "Created CarmenRegistry for " + name );
}

//------------------------------------------------------------------------------

CarmenRegistry::~CarmenRegistry()
{
  delete _handler;
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Could not disconnect IPC\n" );
    exit( 1 );
  }
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
CarmenRegistry::subscribeToMessage( const string & messageName )
{
  Lock lock( _ipcMutex );
  if ( IPC_subscribeData(  messageName.c_str(), CarmenHandler::handleMessage, _handler ) == IPC_Error ) {
    fprintf( stderr, "Could not define message\n" );
    exit( 1 );
  }
  IPC_setMsgQueueLength( messageName.c_str(), 1 );
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
  Logger::spam( "Publishing message name:" + message.name );
  if (  IPC_publishData( 
          message.name.c_str(), 
          message.data->buffer() ) == IPC_Error ) 
  {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
  Logger::spam( "Message published" );
}

//------------------------------------------------------------------------------

Message 
CarmenRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "Receiving message" );
    return _handler->dequeueMessage( timeout );
  } catch ( TimeoutException & e ) {
    Logger::spam( "Receive message timed out" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Structure *
CarmenRegistry::query( const Message & message, double timeout ) 
throw( MessagingTimeout )
{
  void * reply;
  IPC_RETURN_TYPE result =  IPC_queryNotifyData(
                              message.name.c_str(),
                              message.data->buffer(),
                              CarmenHandler::handleReply,
                              _handler );
                            
  if (  result == IPC_Error ) {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
  try {
    return _handler->dequeueReply( timeout );
  } catch ( TimeoutException & e ) {
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
    Logger::spam( "Received query name: " + result.first.name );
    return result.first;
  } catch ( TimeoutException & e ) {
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