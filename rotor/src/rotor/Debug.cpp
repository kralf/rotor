#include "Debug.h"

using namespace Rotor;
using namespace std;


int Debug::_classDepth = 0;


//------------------------------------------------------------------------------

Debug::Debug( const string & message ) 
  : _depth( _classDepth ),
    _message( message )
{
  _classDepth += 1;
  string tabs;
  tabs.append( _depth * 2, ' ' );
  cerr << tabs << message << "{\n";
}

//------------------------------------------------------------------------------

Debug::~Debug()
{
  string tabs;
  tabs.append( _depth * 2, ' ' );
  cerr << tabs << "} //" << _message << endl;
  _classDepth -= 1;
}
