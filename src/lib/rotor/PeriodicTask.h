#ifndef ROTOR_PERIODIC_TASK_H
#define ROTOR_PERIODIC_TASK_H


namespace Rotor {


class Module;
class Thread;


class PeriodicTask
{
public:
  PeriodicTask( Module * module, unsigned long int period = 0 );
  virtual ~PeriodicTask();
  
  void * operator()();
  
  Thread & thread();
  
private:
  Module *       _module;
  unsigned int   _period;
  Thread *       _thread;
};


}


#endif //ROTOR_PERIODIC_TASK_H
