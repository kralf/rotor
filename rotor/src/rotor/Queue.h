#ifndef ROTOR_QUEUE_H
#define ROTOR_QUEUE_H


#include "Mutex.h"
#include "Condition.h"
#include "Exceptions.h"
#include "Lock.h"
#include "QueuePolicy.h"
#include "DefaultQueueReleaser.h"
#include <queue>


namespace Rotor {

template < typename T, class RELEASER = DefaultQueueReleaser<T> >
class Queue
{
public:
  Queue( size_t capacity = 0, QueuePolicy policy = WAIT_WHEN_FULL );

  void push( const T & value, double timeout = 0 ) throw ( TimeoutException );
  T & next( double timeout = 0 ) throw ( TimeoutException );
  void pop( double timeout = 0 ) throw ( TimeoutException );
  T popNext( double timeout = 0 ) throw ( TimeoutException );
  bool empty();

private:
  Queue( const Queue & );
  void operator=( const Queue & );

  size_t        _capacity;
  QueuePolicy   _policy;
  Mutex         _mutex;
  Condition     _notEmpty;
  Condition     _notFull;
  std::queue<T> _queue;
};

#include "Queue.tpp"

}


#endif //ROTOR_QUEUE_H
