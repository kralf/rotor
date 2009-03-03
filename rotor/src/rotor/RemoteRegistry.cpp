#include "RemoteRegistry.h"
#include "BaseOptions.h"
#include "CoreMessages.h"


using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

RemoteRegistry::RemoteRegistry( const std::string & name )
{
  BaseOptions options;
  Registry * _registry = Registry::load( "BroadcastRegistry", "bootstrap", options, "" );
  _registry->registerMessageType( "SERVER_COMMAND", ROTOR_DEFINITION_STRING( RemoteCommand ) );
  _registry->registerMessageType( "OPTION_STRING", ROTOR_DEFINITION_STRING( OptionString ) );

  Structure request = _registry->newStructure( "RemoteCommand" );
  request["command"]   = "GET_OPTIONS";
  request["arguments"] = "BOOTSTRAP";
  Structure * reply = _registry->queryStructure( "SERVER_COMMAND", request, 3 );
  OptionString sReply;
  sReply << reply;
  
  options.fromString( sReply.value );
  
  delete _registry;
  delete reply;

  _registry = Registry::load( options.getString( "BOOTSTRAP", "registry" ), name, options, "" );
  _registry->registerMessageType( "SERVER_COMMAND", ROTOR_DEFINITION_STRING( RemoteCommand ) );
  _registry->registerMessageType( "OPTION_STRING", ROTOR_DEFINITION_STRING( OptionString ) );
  
  Structure request1 = _registry->newStructure( "RemoteCommand" );
  request1["command"]   = "GET_OPTIONS";
  request1["arguments"] = "*";
  
  Structure * reply1 = _registry->queryStructure( "SERVER_COMMAND", request1, 3 );
  sReply << reply1;
  
  options.fromString( sReply.value );
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
RemoteRegistry::subscribeToMessage( const std::string & messageName )
{
  _registry->subscribeToMessage( messageName );
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
  return _registry->receiveMessage( timeout );
}

//------------------------------------------------------------------------------

Structure * 
RemoteRegistry::query( const Message & message, double timeout ) 
throw( MessagingTimeout )
{
  return _registry->query( message, timeout );
}

//------------------------------------------------------------------------------

Message 
RemoteRegistry::receiveQuery( double timeout ) throw( MessagingTimeout )

{
  return _registry->receiveQuery( timeout );
}

//------------------------------------------------------------------------------

void 
RemoteRegistry::reply( const Message & message )
{
  _registry->reply( message );
}
