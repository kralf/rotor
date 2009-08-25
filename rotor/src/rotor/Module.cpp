#include "Module.h"
#include "Message.h"
#include "Structure.h"
#include "Registry.h"
#include "Exceptions.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

Module::Module()
  : _registry( 0 ),
    _outputQueue( 0 )
{
}

//------------------------------------------------------------------------------

Module::~Module()
{
}

//------------------------------------------------------------------------------

Registry & 
Module::registry()
{
  return *_registry;
}

//------------------------------------------------------------------------------

void 
Module::registry( Registry & registry )
{
  _registry = &registry;
}

//------------------------------------------------------------------------------

void 
Module::outputQueue( MessageQueue & queue )
{
  _outputQueue = & queue;
}

//------------------------------------------------------------------------------

bool 
Module::operator()() 
{ 
  return false; 
}

//------------------------------------------------------------------------------

bool 
Module::operator()( Message & ) 
{ 
  return false; 
}

//------------------------------------------------------------------------------

void 
Module::sendMessage( const Message & message )
{
  if ( ! _outputQueue ) {
    throw MessagingError( "No output queue has been registered for process" );
  }
  
  _outputQueue->push( message );
}
