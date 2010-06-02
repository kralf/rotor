#include "Point.h"
#include <rotor/BaseOptions.h>
#include <rotor/Registry.h>
#include <rotor/DebugModule.h>
#include <rotor/DefaultModule.h>
#include <rotor/ModuleRunner.h>

using namespace std;
using namespace Rotor;

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  BaseOptions options;
  options.setString( argv[1], "serverName", "127.0.0.1" );
  Registry * registry = Registry::load( "CarmenRegistry", argv[1], options, "lib" );
  registry->registerType( ROTOR_DEFINITION_STRING( Point ) );
  registry->registerMessage( "TEST_POINT", "Point" );
  registry->subscribeToMessage( "TEST_POINT" );

  ModuleRunner runner( *registry );
  
  runner.addInput( new DefaultModule(), 0 );
  runner.subscribe( "TEST_POINT", new DebugModule() );
  runner.join();
  delete registry;
}
