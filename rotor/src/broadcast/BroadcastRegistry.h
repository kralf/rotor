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
  
  virtual const std::string & name() const;
  
  virtual Options & options() const;
  
  virtual const Type & 
  registerType( const std::string & definitionString );
  
  virtual const Type & 
  operator[]( const std::string & typeName ) const;

  virtual void 
  registerMessage( 
    const std::string & messageName, 
    const std::string & typeName );

  virtual void subscribeToMessage( 
    const std::string & messageName,
    bool queueOwner = false,
    size_t queueCapacity = 0,
    QueuePolicy queuePolicy = DISCARD_OLDEST );  

  virtual void subscribeToQuery( const std::string & messageName );  

  virtual const Type & 
  messageType( const std::string & messageName ) const;
  
  virtual void sendMessage( const Message & message );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout );
  
  virtual Message receiveMessage( 
    const std::string & messageName, 
    double timeout = 0 ) 
  throw( MessagingTimeout );

  virtual Structure query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );
  
  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout );

  virtual void reply( const Message & message );
private:
  std::string                _name;
  Options &                  _options;
  BaseRegistry               _registry;
  Poco::Net::SocketAddress   _destination;
  Poco::Net::DatagramSocket  _socket;
};


}


#endif //ROTOR_BROADCAST_REGISTRY_H