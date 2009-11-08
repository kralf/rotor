#ifndef ROTOR_DISPATCHER_MODULE_H
#define ROTOR_DISPATCHER_MODULE_H


namespace Rotor {


class ModuleRunner;
class Thread;


class DispatcherModule
{
public:
  DispatcherModule( ModuleRunner & runner );
  virtual ~DispatcherModule();
  void start();
  virtual void * operator()() = 0;
  Thread & thread();

protected:
  ModuleRunner & _runner;

private:
  Thread *       _dispatchThread;
};


}


#endif //ROTOR_DISPATCHER_MODULE_H
