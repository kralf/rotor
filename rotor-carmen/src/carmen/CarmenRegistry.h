#ifndef ROTOR_CARMEN_REGISTRY_H
#define ROTOR_CARMEN_REGISTRY_H


#include <rotor/BaseRegistry.h>
#include <rotor/Mutex.h>
#include <rotor/QueueHandler.h>


namespace Rotor {


class CarmenHandler;


class CarmenRegistry : public Registry
{
public:
  CarmenRegistry( const std::string & name, Options & options );
  
  virtual ~CarmenRegistry();
  
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

  virtual LightweightStructure query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );

  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout );

  virtual void reply( const Message & message );

private:
  typedef Queue<Message> MessageQueue;

  std::string      _name;
  Options &        _options;
  BaseRegistry     _registry;
  CarmenHandler *  _handler;
  Mutex            _ipcMutex;
  QueueHandler     _queueHandler;
  MessageQueue     _outputQueue;
  
  friend class CarmenHandler;
};


}


#endif //ROTOR_CARMEN_REGISTRY_H