#include "NetUtils.h"
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/DNS.h>
#include <Poco/Net/NetException.h>

using namespace std;

//------------------------------------------------------------------------------

std::string 
Rotor::hostName()
{
  return Poco::Net::DNS::thisHost().name();
}

//------------------------------------------------------------------------------

std::string 
Rotor::hostIp()
{
  Poco::Net::DatagramSocket s;
  string ip;
  try {
    s.connect( Poco::Net::SocketAddress( "1.2.3.4", "56" ) );
    ip = s.address().host().toString();
    s.close();
  } catch ( ... ) {
    ip = "127.0.0.1";
  }
  return ip;
}
