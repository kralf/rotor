#ifndef ROTOR_QUEUE_H
#define ROTOR_QUEUE_H


#include "Mutex.h"
#include "Condition.h"
#include "Exceptions.h"
#include "Lock.h"
#include <queue>


namespace Rotor {

template < typename T >
class Queue
{
public:
  Queue();
  
  void push( const T & value );
  T & next( double timeout = 0 ) throw ( TimeoutException );
  void pop( double timeout = 0 ) throw ( TimeoutException );

private:
  Queue( const Queue & );
  void operator=( const Queue & );
  
  Mutex         _mutex;
  Condition     _notEmpty;
  std::queue<T> _queue;
};

#include "Queue.tpp"

}


#endif //ROTOR_QUEUE_H