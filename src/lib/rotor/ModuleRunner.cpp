#include "ModuleRunner.h"
#include "Message.h"
#include "Module.h"
#include "PeriodicTask.h"
#include "Thread.h"
#include "Structure.h"
#include "InputDispatcherModule.h"
#include "Registry.h"
#include "Lock.h"

using namespace Rotor;
using namespace std;


//------------------------------------------------------------------------------

ModuleRunner::ModuleRunner( Registry & registry )
  : _registry( registry )
{
  _inputDispatcherModule = new InputDispatcherModule( *this );
  _inputDispatcherModule->start();
}

//------------------------------------------------------------------------------

ModuleRunner::~ModuleRunner()
{
  delete _inputDispatcherModule;
}

//------------------------------------------------------------------------------

void 
ModuleRunner::join()
{
  _inputDispatcherModule->thread().join();
}

//------------------------------------------------------------------------------

void 
ModuleRunner::addInput( Module * input, int period ) 
{
  input->registry( _registry );
  input->outputQueue( _inputQueue );
  
  PeriodicTask * task = new PeriodicTask( input, period );
  task->thread().start();
  _tasks.insert( task );
}

//------------------------------------------------------------------------------

void 
ModuleRunner::addOutput( Module * output ) 
{
  output->registry( _registry );
  _outputs.insert( output );
}

//------------------------------------------------------------------------------

void 
ModuleRunner::subscribe( const string & messageName, Module * subscriber )
{
  subscriber->registry( _registry );
  Lock lock( _mutex );
  _subscribers[messageName].insert( subscriber );
}

//------------------------------------------------------------------------------

void 
ModuleRunner::inputDispatch()
{
  while ( true ) {
    Message & message = _inputQueue.next();
    
    Lock lock( _mutex );
    ModuleMap::iterator iSubscribers;
    iSubscribers = _subscribers.find( message.name().c_str() );
    if ( iSubscribers != _subscribers.end() ) {
      Modules & subscribers =  iSubscribers->second;
      for ( Modules::iterator i = subscribers.begin(); i != subscribers.end(); ) {
        Modules::iterator current = i;
        if (! (**i)( message ) ) {
          i++;
          subscribers.erase( current );
        } else {
          i++;
        }
      }
    } else {
    }
    _inputQueue.pop();
  }
}
