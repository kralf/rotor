#include "YarpRegistry.h"
#include "Conversions.ipp"
#include <rotor/Lock.h>
#include <rotor/Logger.h>
#include <rotor/Message.h>
#include <rotor/Options.h>
#include <rotor/Rotor.h>
#include <rotor/Structure.h>
#include <rotor/Conversion.h>
#include <rotor/Time.h>
#include <rotor/TypedThread.h>
#include <yarp/os/all.h>
#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

ROTOR_REGISTRY_FACTORY( YarpRegistry )

//------------------------------------------------------------------------------

YarpRegistry::YarpRegistry( const string & name, Options & options)
  : Registry( name, options ),
    _name( name ), 
    _options( options ),
    _registry( name, options )
{
}

//------------------------------------------------------------------------------

YarpRegistry::~YarpRegistry()
{
}

//------------------------------------------------------------------------------
  
const std::string & 
YarpRegistry::name() const
{
  return _name;
}
  
//------------------------------------------------------------------------------
  
Options & 
YarpRegistry::options() const
{
  return _options;
}

//------------------------------------------------------------------------------

const Type & 
YarpRegistry::registerType( const string & definitionString )
{
  return _registry.registerType( definitionString );
}

//------------------------------------------------------------------------------

const Type & 
YarpRegistry::operator[]( const std::string & typeName ) const
{
  return _registry[typeName];
}

//------------------------------------------------------------------------------

void 
YarpRegistry::registerMessage(
  const string & messageName, 
  const string & typeName )
{
  _registry.registerMessage( messageName, typeName );
}

//------------------------------------------------------------------------------

void
YarpRegistry::subscribeToMessage(
  const std::string &,
  bool,
  size_t,
  QueuePolicy )
{
  throw NotImplementedError( "Not implemented" );
}

//------------------------------------------------------------------------------

void
YarpRegistry::subscribeToMessage( const string & messageName )
{
  const Type & type = _registry.messageType( messageName );
  size_t splitPoint = messageName.rfind( "/" );
  string serverString   = messageName.substr( 0, splitPoint );
  string datafeedString = messageName.substr( splitPoint );
  
  string inputPort  = _name + "/data-feed/input/" + messageName;
  string outputPort = serverString + "/data-feed/output/" + datafeedString;
  PortTable::iterator it = _ports.find( inputPort );
  if ( it == _ports.end() ) {
    yarp::os::Port & port = _ports[inputPort];
    _network.connect( outputPort.c_str(), inputPort.c_str() );
  }
}

//------------------------------------------------------------------------------

void
YarpRegistry::subscribeToQuery( const string & messageName )
{
  const Type & type = _registry.messageType( messageName );
  string completeName = _name + "/service-request/input";
  PortTable::iterator it = _ports.find( completeName );
  if ( it == _ports.end() ) {
    yarp::os::Port & port = _ports[completeName];
  }
}

//------------------------------------------------------------------------------

const Type & 
YarpRegistry::messageType( const string & messageName ) const
{
  return _registry.messageType( messageName );
}

//------------------------------------------------------------------------------

void 
YarpRegistry::sendMessage( const Message & message )
{
  const Type & type       = _registry.messageType( message.name() );
  size_t splitPoint       = message.name().rfind( "/" );
  string serverString     = message.name().substr( 0, splitPoint );
  string datafeedString   = message.name().substr( splitPoint );
  string outputPort       = serverString + "/data-feed/output/" + datafeedString;
  
  PortTable::iterator it  = _ports.find( outputPort );
  yarp::os::Port & port   = _ports[outputPort];
  
  yarp::os::Bottle bottle;
  appendToBottle( bottle, message.data() );
  port.write( bottle );
}

//------------------------------------------------------------------------------

Message 
YarpRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "Receiving message" );
//     return _handler->dequeueMessage( timeout );
  } catch ( TimeoutException & e ) {
    Logger::spam( "Receive message timed out" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message
YarpRegistry::receiveMessage(
  const string &,
  double )
throw( MessagingTimeout )
{
  throw NotImplementedError( "Not implemented" );
}

//------------------------------------------------------------------------------

Structure
YarpRegistry::query( const Message & message, double timeout )
throw( MessagingTimeout )
{
  try {
//     return _handler->dequeueReply( timeout );
  } catch ( TimeoutException & e ) {
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

Message 
YarpRegistry::receiveQuery( double timeout ) throw( MessagingTimeout )
{
  try {
    Logger::spam( "ReceivingQuery" );
//     pair<Message, MSG_INSTANCE> result = _handler->dequeueQuery( timeout );
//     Logger::spam( "Received query name: " + result.first.name );
//     return result.first;
  } catch ( TimeoutException & e ) {
    Logger::spam( "Receive query timed out" );
    throw MessagingTimeout( "No message was received" );
  }
}

//------------------------------------------------------------------------------

void
YarpRegistry::reply( const Message & message )
{
//   _handler->reply( message );
}
