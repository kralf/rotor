#include "NetUtils.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

//------------------------------------------------------------------------------

void connectHandler( const boost::system::error_code & )
{
}

//------------------------------------------------------------------------------

std::string 
Rotor::hostName()
{
  return  ip::host_name();
}

//------------------------------------------------------------------------------

std::string 
Rotor::hostIp()
{
  string ip;

  tcp::endpoint  testAddress( address::from_string( "1.2.3.4" ), 56 );
  io_service     service;
  tcp::socket    socket( service );
  deadline_timer timer( service );
  
  socket.open( testAddress.protocol() );
  
  timer.expires_from_now( boost::posix_time::seconds( 0.1 ) );
  socket.async_connect( testAddress, &connectHandler );
  timer.wait();
  
  ip = socket.local_endpoint().address().to_string();
  socket.close();
  return ip;
}
