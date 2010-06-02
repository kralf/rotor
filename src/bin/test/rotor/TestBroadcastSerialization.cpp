#include <unittest++/UnitTest++.h>
#include <rotor/Rotor.h>
#include <rotor/Logger.h>
#include <rotor/BaseOptions.h>
#include <rotor/BaseRegistry.h>
#include <rotor/Structure.h>
#include <rotor/Serialization.h>
#include <stdint.h>

ROTOR_DEFINE_TYPE( Test,
  struct Test {
    char * name;
    double  x[2];
    uint8_t count;
    double * y;
  };
)

using namespace Rotor;

SUITE( Serialization ) {

  TEST( RoundTrip )
  {
    BaseOptions options;
    BaseRegistry registry( "Test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( Test ) );
    registry.registerMessage( "TEST", "Test" );
    Structure structure = registry.newStructure( "Test", 0 );
    Message m( "TEST", structure );
    m.data()["name"] = "This is a test";
    for ( int i = 0; i < 2; i++ ) {
      m.data()["x"][i] = i;
    }
    m.data()["count"] = 3;
    for ( int i = 0; i < 3; i++ ) {
      m.data()["y"][i] = i * 3.14159265;
    }
    Message m1 = unmarshall( registry, marshall( m ) );
    CHECK_EQUAL( marshall( m ), marshall( m1 ) );
  }

}
