#ifndef ROTOR_TYPED_THREAD_H
#define ROTOR_TYPED_THREAD_H


#include "Thread.h"


namespace Rotor{

template <typename T>
class TypedThread : public Thread
{
public:
  TypedThread( T & functor );
  T & functor();
  
private:
  virtual void * run();
  
  T & _functor;
};

template <typename T>
TypedThread<T> * typedThread( T & functor );

#include "TypedThread.tpp"

}


#endif //ROTOR_TYPED_THREAD_H

