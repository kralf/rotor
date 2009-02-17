#ifndef ROTOR_MODULE_RUNNER_H
#define ROTOR_MODULE_RUNNER_H


#include "Message.h"
#include "Queue.h"
#include "Mutex.h"
#include <string>
#include <set>
#include <map>


namespace Rotor {


class InputDispatcherModule;
class Module;
class PeriodicTask;
class Thread;
class Registry;


class ModuleRunner
{
public:
  ModuleRunner( Registry & registry );
  
  virtual ~ModuleRunner();
  
  void join();
  
  void addInput( Module * input, int period );
  
  void addOutput( Module * output );
  
  void subscribe( const std::string & messageName, Module * subscriber );
  
  void inputDispatch();


private:
  ModuleRunner( const ModuleRunner & );
  void operator=( const ModuleRunner & );
  
  typedef Queue<Message> MessageQueue;
  typedef std::set<Module *> Modules;
  typedef std::map< std::string, Modules > ModuleMap;
  typedef std::set<PeriodicTask *> Tasks;
  
  Registry &              _registry;
  Mutex                   _mutex;
  MessageQueue            _inputQueue;
  MessageQueue            _outputQueue;
  InputDispatcherModule * _inputDispatcherModule;
  Tasks                   _tasks;
  Modules                 _inputs;
  Modules                 _outputs;
  ModuleMap               _subscribers;
};



}


#endif //ROTOR_MODULE_RUNNER_H
