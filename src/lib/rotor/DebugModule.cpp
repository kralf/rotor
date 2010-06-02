#include "DebugModule.h"
#include "Message.h"
#include "Structure.h"
#include "Time.h"
#include "Registry.h"
#include <iostream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

bool 
DebugModule::operator()( Message & message )
{
  const Structure & s  = message.data();
  const MemberDefinitions & members = s.typeData().members();
  cout << "--------[" << ( microseconds() / 1000 ) << "]--------" << endl;
  cout << s.typeData().name() << ":" << endl;
  for ( MemberDefinitions::const_iterator i = members.begin(); 
        i != members.end(); i++ )
  {
    cout << "  " << i->name << ": " << s[i->name.c_str()] << endl;
  }
  return true;
}
