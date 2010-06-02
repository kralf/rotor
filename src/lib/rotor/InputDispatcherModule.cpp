#include "InputDispatcherModule.h"
#include "ModuleRunner.h"

using namespace Rotor;

//------------------------------------------------------------------------------

InputDispatcherModule::InputDispatcherModule( ModuleRunner & runner )
  : DispatcherModule( runner )
{
}

//------------------------------------------------------------------------------

void *
InputDispatcherModule::operator()()
{
  _runner.inputDispatch();
  return NULL;
}
