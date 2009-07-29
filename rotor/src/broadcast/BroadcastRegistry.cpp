#include "BroadcastRegistry.h"
#include <rotor/NetUtils.h>
#include <rotor/Conversion.h>
#include <rotor/Exceptions.h>
#include <rotor/Lock.h>
#include <rotor/Logger.h>
#include <rotor/Message.h>
#include <rotor/Rotor.h>
#include <rotor/Serialization.h>
#include <rotor/Structure.h>
#include <rotor/TypedThread.h>
#include <Poco/Net/NetException.h>
#include <iostream>
#include <cstdlib>


using namespace Rotor;
using namespace std;
using namespace Poco::Net;

//------------------------------------------------------------------------------

ROTOR_REGISTRY_FACTORY( BroadcastRegistry )

//------------------------------------------------------------------------------

BroadcastRegistry::BroadcastRegistry( const string & name, Options & options )
  : Registry( name, options ),
    _name( name ), 
    _options( options ),
    _registry( name, options ),
    _destination( "255.255.255.255:60709" ) 
{
  Logger::setLevel(
    static_cast<Logger::Level>( options.getInt( "BroadcastRegistry", "loggingLevel",  3 ) ),
    "BroadcastRegistry"
  );
  _socket.setBroadcast( true );
  int port  = options.getInt( name, "serverPort", 0 );
 
  if ( port ) {
    _socket.bind( SocketAddress( "0", port ) );
  } else {
    _socket.bind( SocketAddress( hostIp(), "0" ) );
  }
  Logger::info(
    "Broadcast registry bound to:" + _socket.address().toString(),
    "BroadCastRegistry"
  );
}

//------------------------------------------------------------------------------

BroadcastRegistry::~BroadcastRegistry()
{
}

//------------------------------------------------------------------------------
  
const std::string & 
BroadcastRegistry::name() const
{
  return _name;
}
  
//------------------------------------------------------------------------------
  
Options & 
BroadcastRegistry::options() const
{
  return _options;
}

//------------------------------------------------------------------------------

const Type & 
BroadcastRegistry::registerType( const string & definitionString )
{
  return _registry.registerType( definitionString );
}

//------------------------------------------------------------------------------

const Type & 
BroadcastRegistry::operator[]( const std::string & typeName ) const
{
  return _registry[typeName];
}

//------------------------------------------------------------------------------

void 
BroadcastRegistry::registerMessage( 
  const string & messageName, 
  const string & typeName )
{
  _registry.registerMessage( messageName, typeName );
}

//------------------------------------------------------------------------------

void
BroadcastRegistry::subscribeToMessage(
  const std::string & messageName,
  bool queueOwner,
  size_t queueCapacity,
  QueuePolicy queuePolicy )
{
}

//------------------------------------------------------------------------------

void 
BroadcastRegistry::subscribeToQuery( const std::string & messageName )
{
}

//------------------------------------------------------------------------------

const Type & 
BroadcastRegistry::messageType( const string & messageName ) const
{
  return _registry.messageType( messageName );
}

//------------------------------------------------------------------------------

void 
BroadcastRegistry::sendMessage( const Message & message )
{
  string s = marshall( message );
  try {
    _socket.sendTo( s.c_str(), s.size() + 1, _destination );
  } catch ( NetException & e ) {
    cerr << e.message() << endl;
  }
}

//------------------------------------------------------------------------------

Message 
BroadcastRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  SocketAddress address;
  char buffer[2048];
  if ( timeout ) {
    _socket.setReceiveTimeout( timeout * 1000000 );
  } else {
    _socket.setReceiveTimeout( 0 );
  }
  while ( true ) {
    try {
      _socket.receiveFrom( buffer, 2048, address );
      _destination = address;
      Logger::spam( string( "Raw message:" ) + buffer, "BroadcastRegistry" );
      //NOTE: Here is necessary to check for subscribed messages.
      return unmarshall( _registry, buffer );
    } catch ( Poco::TimeoutException ) {
      throw MessagingTimeout( "No message was received" );
    }
  }
}

//------------------------------------------------------------------------------

Message 
BroadcastRegistry::receiveMessage( 
  const string & messageName,
  double timeout ) 
throw( MessagingTimeout )
{
  throw NotImplementedError( "Not implemented" );
}

//------------------------------------------------------------------------------

Structure
BroadcastRegistry::query( const Message & message, double timeout ) 
throw( MessagingTimeout )
{
  for ( int i = 0; i < 3; ++i ) {
    sendMessage( message );
    try {
      return receiveMessage( timeout ).data();
    } catch ( MessagingTimeout ) {
    }
  }
  throw MessagingTimeout( "No message was received" );
}

//------------------------------------------------------------------------------

Message 
BroadcastRegistry::receiveQuery( double timeout ) throw( MessagingTimeout )
{
  return receiveMessage( timeout );
}

//------------------------------------------------------------------------------

void
BroadcastRegistry::reply( const Message & message )
{
  sendMessage( message );
}
