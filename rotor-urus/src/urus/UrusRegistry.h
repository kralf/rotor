#ifndef ROTOR_URUS_REGISTRY_H
#define ROTOR_URUS_REGISTRY_H


#include <rotor/BaseRegistry.h>
#include <rotor/Mutex.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>


namespace Rotor {


class UrusHandler;


class UrusRegistry : public Registry
{
public:
  UrusRegistry( const std::string & name, Options & options );
  
  virtual ~UrusRegistry();
  
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

  virtual void subscribeToMessage( const std::string & messageName );

  virtual void subscribeToQuery( const std::string & messageName );

  virtual const Type & 
  messageType( const std::string & messageName ) const;
  
  virtual void sendMessage( const Message & message );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout );

  virtual Structure * query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );

  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout );

  virtual void reply( const Message & message );

private:
  typedef std::map<std::string, yarp::os::Port> PortTable;
  std::string       _name;
  Options &         _options;
  BaseRegistry      _registry;
  yarp::os::Network _network;
  PortTable         _ports;
  Message           _message;
  Mutex             _ipcMutex;
};


}


#endif //ROTOR_CARMEN_REGISTRY_H