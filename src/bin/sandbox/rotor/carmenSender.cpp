#include "Point.h"
#include <rotor/BaseOptions.h>
#include <rotor/Registry.h>
#include <rotor/DebugModule.h>
#include <rotor/DefaultModule.h>
#include <rotor/ModuleRunner.h>
#include <rotor/Structure.h>

using namespace std;
using namespace Rotor;

//------------------------------------------------------------------------------

class PointGenerator : public Module
{
public:
  PointGenerator()
  : _count( 0 )
  {
  }
  
  virtual bool operator()()
  {
    Point p;
    p.x = _count;
    p.y = 3.5 * _count;
    _count += 1;
    Structure tmp( "Point", &p, registry() );
    Structure * out = new Structure( "Point", 0, registry() );
    *out = tmp;
    sendMessage( Message( "TEST_POINT", out ) );
    return true;
  }
  
private:
  Message          _message;
  int              _count;
};

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  BaseOptions options;
  Registry * registry = Registry::load( "CarmenRegistry", argv[1], options, "lib" );
  registry->registerType( ROTOR_DEFINITION_STRING( Point ) );
  registry->registerMessage( "TEST_POINT", "Point" );

  ModuleRunner runner( *registry );
  
  runner.addInput( new PointGenerator(), 10 );
  runner.subscribe( "TEST_POINT", new DebugModule() );
  runner.subscribe( "TEST_POINT", new DefaultModule() );
  runner.join();
  
  delete registry;
}