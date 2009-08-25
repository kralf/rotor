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
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <cstdlib>


using namespace Rotor;
using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace asio;
using namespace asio::ip;

//------------------------------------------------------------------------------

ROTOR_REGISTRY_FACTORY( BroadcastRegistry )

//------------------------------------------------------------------------------

void 
setResult( optional<boost::system::error_code>* a, boost::system::error_code b )
{
  a->reset( b );
}

//------------------------------------------------------------------------------

template <typename SOCKET, typename BUFFER>
void readTimeout( 
  SOCKET & socket, 
  const BUFFER & buffers,
  udp::endpoint & address,
  double timeout )
{
  optional<boost::system::error_code> timerResult;
  deadline_timer timer( socket.io_service() );
  timer.expires_from_now( seconds( timeout ) );
  timer.async_wait( boost::bind( setResult, &timerResult, _1 ) );

  optional<boost::system::error_code> readResult;
  socket.async_receive_from( buffers, address, boost::bind( setResult, &readResult, _1 ) );

  socket.io_service().reset();

  bool timedOut = false;
  while ( socket.io_service().run_one() ) {
    if ( readResult ) {
      timer.cancel();
    } else if ( timerResult ) {
      timedOut = true;
      socket.cancel();
    }
  }

  if ( timedOut ) {
    throw MessagingTimeout( "No message was received" );
  }
} 

//------------------------------------------------------------------------------

BroadcastRegistry::BroadcastRegistry( const string & name, Options & options )
  : Registry( name, options ),
    _name( name ), 
    _options( options ),
    _registry( name, options ),
    _socket( _service ),
    _destination( address::from_string( "255.255.255.255" ), 60709 )
{
  Logger::setLevel(
    static_cast<Logger::Level>( options.getInt( "BroadcastRegistry", "loggingLevel",  3 ) ),
    "BroadcastRegistry"
  );
  
  int port  = options.getInt( name, "serverPort", 0 );

  _socket.open( _destination.protocol() );
  _socket.set_option( socket_base::broadcast( true ) );
  
  udp::endpoint listenAddress;
  if ( port ) {
    listenAddress = udp::endpoint( ip::udp::v4(), port );
  } else {
    listenAddress = udp::endpoint( address::from_string( hostIp() ), 0 );
  }
  boost::system::error_code error = error::host_not_found;
  _socket.bind( listenAddress, error );
  if ( error ) { 
    throw boost::system::system_error( error );  
  }
  Logger::info(
      "Registry bound to:" 
    + listenAddress.address().to_string()
    + ":"
    + toString( listenAddress.port() ),
    "BroadcastRegistry"
  );
}

//------------------------------------------------------------------------------

BroadcastRegistry::~BroadcastRegistry()
{
  _socket.close();
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
  const std::string &,
  bool,
  size_t,
  QueuePolicy )
{
  throw NotImplementedError( "Not implemented" );
}

//------------------------------------------------------------------------------

void 
BroadcastRegistry::subscribeToQuery( const std::string & )
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
    _socket.send_to( buffer( s.c_str(), s.size() + 1 ), _destination );
  } catch ( boost::system::system_error & e ) {
    cerr << e.what() << endl;
  }
}

//------------------------------------------------------------------------------

Message 
BroadcastRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  udp::endpoint address;
  char input[2048];
  while ( true ) {
    readTimeout( _socket, buffer( input, 2048 ), address, timeout );
    _destination = address;
    Logger::spam( string( "Raw message:" ) + input, "BroadcastRegistry" );
    //NOTE: Here is necessary to check for subscribed messages.
    return unmarshall( _registry, input );
  }
}

//------------------------------------------------------------------------------

Message 
BroadcastRegistry::receiveMessage( 
  const string &,
  double ) 
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
