#include "Condition.h"
#include "Mutex.h"
#include "ConditionImpl.ipp"


using namespace Rotor;

//------------------------------------------------------------------------------

Condition::Condition()
  : _condition( new ConditionImpl() )
{
}

//------------------------------------------------------------------------------

Condition::~Condition()
{ 
  delete _condition;
}
  
//------------------------------------------------------------------------------

bool 
Condition::wait( Mutex & mutex, double seconds )
{
  return _condition->wait( *( mutex._mutex ), seconds );
}

//------------------------------------------------------------------------------

void 
Condition::notify()
{
  _condition->notify();
}

//------------------------------------------------------------------------------

void 
Condition::notifyAll()
{
  _condition->notifyAll();
}
