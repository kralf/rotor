#include "PeriodicTask.h"
#include "Module.h"
#include "Time.h"
#include "Thread.h"
#include "TypedThread.h"

using namespace Rotor;

//------------------------------------------------------------------------------

PeriodicTask::PeriodicTask( Module * module, unsigned long int period )
  : _module( module ),
    _period( period )
{
  _thread = typedThread( *this );
}

//------------------------------------------------------------------------------

PeriodicTask::~PeriodicTask()
{
  delete _thread;
  delete _module;
}

//------------------------------------------------------------------------------

Thread & 
PeriodicTask::thread()
{
  return *_thread;
}

//------------------------------------------------------------------------------

void * 
PeriodicTask::operator()()
{
  double time;
  while ( true ) {
    time = microseconds();
    if ( ! (*_module)() ) {
      break;
    }
    time = _period - ( microseconds() - time ) / 1000;
    if ( time > 0 ) {
      Thread::sleep( time );
    }
  }
  return NULL;
}
