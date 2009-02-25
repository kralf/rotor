#ifndef ROTOR_CARMEN_REGISTRY_H
#define ROTOR_CARMEN_REGISTRY_H


#include <rotor/BaseRegistry.h>
#include <rotor/Mutex.h>
#include <rotor/Condition.h>
#include <rotor/Queue.h>


namespace Rotor {


class Options;
class Thread;


class CarmenRegistry : public Registry
{
public:
  CarmenRegistry( const std::string & name, Options & options );
  
  virtual ~CarmenRegistry();
  
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

  std::string formatString( const std::string & typeName ) const;

  void setMessage( Message & message );
  int operator()();
  Queue<Message> & responseQueue();
  
private:
  BaseRegistry   _registry;
  Message        _message;
  bool           _hasMessage;
  Condition      _messageAvailable;
  Mutex          _mutex;
  Mutex          _ipcMutex;
  Thread *       _dispatchThread;
  Queue<Message> _responseQueue;
};


}


#endif //ROTOR_CARMEN_REGISTRY_H