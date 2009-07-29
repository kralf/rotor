#include "RemoteRegistry.h"
#include "BaseOptions.h"
#include "Conversion.h"
#include "CoreMessages.h"
#include "FileUtils.h"
#include "Logger.h"
#include <dlfcn.h>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

RemoteRegistry::RemoteRegistry( 
  const string & className, 
  const string & name, 
  Options & options, 
  const string & path
) : _registry( load( className, name, options, path ) )
{
}

//------------------------------------------------------------------------------

RemoteRegistry::RemoteRegistry( const std::string & name )
{
  _registry = load( "BroadcastRegistry", "bootstrap", _options, "" );
  _registry->registerMessageType( "SERVER_COMMAND", ROTOR_DEFINITION_STRING( RemoteCommand ) );
  _registry->registerMessageType( "OPTION_STRING", ROTOR_DEFINITION_STRING( OptionString ) );

  Structure request( "RemoteCommand", 0, *_registry );
  request["command"]   = "GET_OPTIONS";
  request["arguments"] = "rotor_server";
  Structure reply = _registry->queryStructure( "SERVER_COMMAND", request, 3 );
  Logger::info( "Main server found", "RemoteRegistry" );
  
  OptionString sReply;
  sReply << reply;  
  _options.fromString( sReply.value );

  Logger::info( "Loading definitive registry", "RemoteRegistry" );
  _registry = load( _options.getString( "rotor_server", "registry" ), name, _options, "" );
  _registry->registerMessageType( "SERVER_COMMAND", ROTOR_DEFINITION_STRING( RemoteCommand ) );
  _registry->registerMessageType( "OPTION_STRING", ROTOR_DEFINITION_STRING( OptionString ) );
  Logger::debug( "Retrieving options" );
  
  request["command"]   = "GET_OPTIONS";
  request["arguments"] = "*";
  
  reply.referTo( _registry->queryStructure( "SERVER_COMMAND", request, 3 ) );
  
  Logger::debug( reply.toString() );
  
  sReply << reply;
  _options.fromString( sReply.value );
  
  Logger::info( 
    string( "Connected to server with transport: " ) + _options.getString( "rotor_server", "registry" ),
    "RemoteRegistry" 
  );
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

Structure 
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

//------------------------------------------------------------------------------

RegistryPtr 
RemoteRegistry::load( 
  const string & className, 
  const string & registryName, 
  Options & options,
  const string & searchPath )
{
  size_t flags = RTLD_NOW | RTLD_GLOBAL;
  void * handle = dlopen( findFile( "lib" + className + ".so", searchPath ).c_str(), flags );  
  if( handle == 0 ) {
    fprintf( stderr, "Error: %s\n", dlerror() );
    handle = dlopen( findFile( className + ".so", searchPath ).c_str(), flags );  
    if ( handle == 0 ) {
      throw ClassLoadingError( "Unable to open class lib for registry: " + className + "\nError:" + dlerror() );
    }
  }
  
  std::string factoryName = className + "Factory";
  RegistryFactory factory = reinterpret_cast<RegistryFactory>( dlsym( handle, factoryName.c_str() ) );

  if( factory == 0 ) {
    dlclose( handle );
    throw ClassLoadingError( "Factory function not found for registry: " + className );
  }
  return RegistryPtr( factory( registryName, options ) );
}
