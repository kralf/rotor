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
  enum Policy {
    WAIT_WHEN_FULL = 0,
    DISCARD_NEWEST = 1,
    DISCARD_OLDEST = 2
  };

  
  Queue( size_t capacity = 0, Policy policy = WAIT_WHEN_FULL );
  
  void push( const T & value, double timeout = 0 ) throw ( TimeoutException );
  T & next( double timeout = 0 ) throw ( TimeoutException );
  void pop( double timeout = 0 ) throw ( TimeoutException );

private:
  Queue( const Queue & );
  void operator=( const Queue & );
  
  size_t        _capacity;
  Policy        _policy;
  Mutex         _mutex;
  Condition     _notEmpty;
  Condition     _notFull;
  std::queue<T> _queue;
};

#include "Queue.tpp"

}


#endif //ROTOR_QUEUE_H