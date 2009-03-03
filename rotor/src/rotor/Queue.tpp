
//------------------------------------------------------------------------------

template <typename T>
Queue<T>::Queue()
{
}

//------------------------------------------------------------------------------
  
template <typename T>
void 
Queue<T>::push( const T & value )
{
  _mutex.lock();
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
}
