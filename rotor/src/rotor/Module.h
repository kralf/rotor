#ifndef ROTOR_MODULE_H
#define ROTOR_MODULE_H


#include "Message.h"
#include "Queue.h"
#include <string>

namespace Rotor {


class Registry;
class Structure;


class Module
{
public:
  typedef Queue<Message> MessageQueue;
  
  Module();
  
  Registry & registry();
  
  void registry( Registry & registry );
  
  void outputQueue( MessageQueue & queue );
  
  virtual bool operator()();
  
  virtual bool operator()( Message & message );
  
protected:
  void sendMessage( const Message & message );
  
private:
  Registry * _registry;
  MessageQueue * _outputQueue;
};


}


#endif //ROTOR_MODULE_H
