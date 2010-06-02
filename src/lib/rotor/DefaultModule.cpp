#include "DefaultModule.h"
#include "Message.h"
#include "Registry.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

bool 
DefaultModule::operator()()
{
  sendMessage( registry().receiveMessage() );
  return true;
}

//------------------------------------------------------------------------------

bool 
DefaultModule::operator()( Message & message )
{
  registry().sendMessage( message );
  return true;
}

