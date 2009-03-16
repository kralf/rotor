
//------------------------------------------------------------------------------

template <typename T>
Queue<T>::Queue( size_t capacity, Policy policy )
  : _capacity( capacity ),
    _policy( policy )
{
}

//------------------------------------------------------------------------------
  
template <typename T>
void 
Queue<T>::push( const T & value, double timeout ) throw ( TimeoutException)
{
  _mutex.lock();
  if ( _capacity > 0 && _queue.size() == _capacity ) {
    if ( _policy == WAIT_WHEN_FULL ) {
      while ( _queue.size() == _capacity ) {
        if ( ! _notFull.wait( _mutex, timeout ) ) {
          throw TimeoutException( "Timeout in queue" );
        }
      }
    } else if ( _policy == DISCARD_OLDEST ) {
      _queue.pop();
      _queue.push( value );
      _mutex.unlock();
      return;
    } else {
      _mutex.unlock();
      return;
    }
  }
  _queue.push( value );
  _mutex.unlock();
  _notEmpty.notify();
}

//------------------------------------------------------------------------------
  
template <typename T>
T &
Queue<T>::next( double timeout ) throw ( TimeoutException )
{
  Lock lock( _mutex );
  while ( _queue.empty() ) {
    if ( ! _notEmpty.wait( _mutex, timeout ) ) {
      throw TimeoutException( "Timeout in queue" );
    }
  }
  return _queue.front();
}

//------------------------------------------------------------------------------
  
template <typename T>
void
Queue<T>::pop( double timeout ) throw ( TimeoutException )
{
  Lock lock( _mutex );
  while ( _queue.empty() ) {
    if ( ! _notEmpty.wait( _mutex, timeout ) ) {
      throw TimeoutException( "Timeout in queue" );
    }
  }
  _queue.pop();
  _notFull.notify();
}
