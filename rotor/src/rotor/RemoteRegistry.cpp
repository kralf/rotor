#include "RemoteRegistry.h"
#include "BaseOptions.h"
#include "CoreMessages.h"
#include "Logger.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

RemoteRegistry::RemoteRegistry( const std::string & name )
{
  _registry = Registry::load( "BroadcastRegistry", "bootstrap", _options, "" );
  _registry->registerMessageType( "SERVER_COMMAND", ROTOR_DEFINITION_STRING( RemoteCommand ) );
  _registry->registerMessageType( "OPTION_STRING", ROTOR_DEFINITION_STRING( OptionString ) );

  Structure request = _registry->newStructure( "RemoteCommand" );
  request["command"]   = "GET_OPTIONS";
  request["arguments"] = "BOOTSTRAP";
  Structure * reply = _registry->queryStructure( "SERVER_COMMAND", request, 3 );
  Logger::info( "Main server found" );
  
  OptionString sReply;
  sReply << reply;  
  _options.fromString( sReply.value );
  
  delete _registry;
  delete reply;

  _registry = Registry::load( _options.getString( "BOOTSTRAP", "registry" ), name, _options, "" );
  _registry->registerMessageType( "SERVER_COMMAND", ROTOR_DEFINITION_STRING( RemoteCommand ) );
  _registry->registerMessageType( "OPTION_STRING", ROTOR_DEFINITION_STRING( OptionString ) );
  Logger::debug( "Retrieving options" );
  
  request["command"]   = "GET_OPTIONS";
  request["arguments"] = "*";
  
  reply = _registry->queryStructure( "SERVER_COMMAND", request, 3 );
  
  sReply << reply;
  _options.fromString( sReply.value );
  
  delete reply;
  Logger::info( string( "Connected to server with transport: " ) + _options.getString( "BOOTSTRAP", "registry" ) );
}

//------------------------------------------------------------------------------

RemoteRegistry::~RemoteRegistry()
{
  delete _registry;
}
//------------------------------------------------------------------------------

const std::string & 
RemoteRegistry::name() const
{
  return _registry->name();
}

//------------------------------------------------------------------------------

Options & 
RemoteRegistry::options() const
{
  return _registry->options();
}

//------------------------------------------------------------------------------

const Type & 
RemoteRegistry::registerType( const std::string & definition )
{
  return _registry->registerType( definition );
}

//------------------------------------------------------------------------------

const Type & 
RemoteRegistry::operator[]( const std::string & typeName ) const
{
  return (*_registry)[typeName];
}

//------------------------------------------------------------------------------
  
void 
RemoteRegistry::registerMessage( 
  const std::string & messageName, 
  const std::string & typeName )
{
  _registry->registerMessage( messageName, typeName );
}

//------------------------------------------------------------------------------

void 
RemoteRegistry::subscribeToMessage(
  const std::string & messageName,
  bool queueOwner,
  size_t queueCapacity,
  QueuePolicy queuePolicy )
{
  _registry->subscribeToMessage( 
    messageName, 
    queueOwner, 
    queueCapacity, 
    queuePolicy );
}

//------------------------------------------------------------------------------

void
RemoteRegistry::subscribeToQuery( const string & messageName )
{
  _registry->subscribeToQuery( messageName );
}

//------------------------------------------------------------------------------

const Type & 
RemoteRegistry::messageType( const std::string & messageName ) const
{
  return _registry->messageType( messageName );
}

//------------------------------------------------------------------------------

void 
RemoteRegistry::sendMessage( const Message & message )
{
  _registry->sendMessage( message );
}

//------------------------------------------------------------------------------

Message 
RemoteRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  try {
    return _registry->receiveMessage( timeout );
  } catch ( ... ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message 
RemoteRegistry::receiveMessage( 
  const string & messageName,
  double timeout ) 
throw( MessagingTimeout )
{
  try {
    return _registry->receiveMessage( messageName, timeout );
  } catch ( ... ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Structure * 
RemoteRegistry::query( const Message & message, double timeout ) 
throw( MessagingTimeout )
{
  try {
    return _registry->query( message, timeout );
  } catch ( ... ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message 
RemoteRegistry::receiveQuery( double timeout ) throw( MessagingTimeout )

{
  try {
    return _registry->receiveQuery( timeout );
  } catch ( ... ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

void 
RemoteRegistry::reply( const Message & message )
{
  _registry->reply( message );
}
