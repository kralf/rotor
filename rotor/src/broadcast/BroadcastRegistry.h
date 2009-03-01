#ifndef ROTOR_BROADCAST_REGISTRY_H
#define ROTOR_BROADCAST_REGISTRY_H


#include <rotor/BaseRegistry.h>
#include <rotor/Options.h>
#include <Poco/Net/MulticastSocket.h>


typedef struct addrinfo Addrinfo;


namespace Rotor {


class BroadcastRegistry : public Registry
{
public:
  BroadcastRegistry( const std::string & name, Options & options );
  
  virtual ~BroadcastRegistry();
  
  virtual const Type & 
  registerType( const std::string & definitionString );
  
  virtual const Type & 
  operator[]( const std::string & typeName ) const;

  virtual void 
  registerMessage( 
    const std::string & messageName, 
    const std::string & typeName );

  virtual void subscribeToMessage( const std::string & messageName );

  virtual const Type & 
  messageType( const std::string & messageName ) const;
  
  virtual void sendMessage( const Message & message );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout );
  
  virtual Message query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );

  virtual void reply( const Message & message );
private:
  BaseRegistry               _registry;
  Poco::Net::SocketAddress   _destination;
  Poco::Net::DatagramSocket  _socket;
};


}


#endif //ROTOR_BROADCAST_REGISTRY_H