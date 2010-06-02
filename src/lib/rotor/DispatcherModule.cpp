#include "DispatcherModule.h"
#include "ModuleRunner.h"
#include "TypedThread.h"

using namespace Rotor;

//------------------------------------------------------------------------------

DispatcherModule::DispatcherModule( ModuleRunner & runner )
  : _runner( runner )
{
  _dispatchThread =  typedThread( *this );
}

//------------------------------------------------------------------------------

DispatcherModule::~DispatcherModule()
{
  delete _dispatchThread;
}

//------------------------------------------------------------------------------

void
DispatcherModule::start()
{
  _dispatchThread->start();
}

//------------------------------------------------------------------------------

Thread & 
DispatcherModule::thread()
{
  return *_dispatchThread;
}
