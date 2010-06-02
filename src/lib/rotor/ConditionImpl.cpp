#include "ConditionImpl.ipp"
#include "MutexImpl.ipp" 
#include <cmath>
#include <rotor/Time.h>
#include <cstdio>

using namespace Rotor;

//------------------------------------------------------------------------------

ConditionImpl::ConditionImpl()
{
  pthread_cond_init( &_condition, NULL );
}

//------------------------------------------------------------------------------

ConditionImpl::~ConditionImpl()
{ 
  pthread_cond_destroy( &_condition );
}
  
//------------------------------------------------------------------------------

bool 
ConditionImpl::wait( MutexImpl & mutex, double seconds )
{
  if ( ! seconds ) {
    pthread_cond_wait( &_condition, &mutex._mutex );
    return true;
  } else {
    struct timespec time;
    seconds += Rotor::seconds();
    time.tv_sec  = floor( seconds );
    time.tv_nsec = ( seconds - time.tv_sec ) * 1E9;
    int result = pthread_cond_timedwait( &_condition, &mutex._mutex, &time );
    return result == 0;
  }
}

//------------------------------------------------------------------------------

void 
ConditionImpl::notify()
{
  pthread_cond_signal( &_condition );
}

//------------------------------------------------------------------------------

void 
ConditionImpl::notifyAll()
{
  pthread_cond_broadcast( &_condition );
}
