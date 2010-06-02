#ifndef ROTOR_CONDITION_IMPL_H
#define ROTOR_CONDITION_IMPL_H


#include<pthread.h>


namespace Rotor {


class MutexImpl;

class ConditionImpl
{
public:
  ConditionImpl();
  virtual ~ConditionImpl();
  
  bool wait( MutexImpl & mutex, double seconds );
  void notify();
  void notifyAll();
  
private:
  ConditionImpl( const ConditionImpl & );
  void operator=( const ConditionImpl & );
  
  pthread_cond_t _condition;
  
};


}


#endif //ROTOR_CONDITION_IMPL_H
