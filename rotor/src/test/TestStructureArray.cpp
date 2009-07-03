#include <unittest++/UnitTest++.h>
#include <rotor/BaseOptions.h>
#include <rotor/BaseRegistry.h>
#include <rotor/RegistryMacros.h>
#include <rotor/Serialization.h>
#include <rotor/Structure.h>
#include <rotor/StructureArray.h>
#include <rotor/Debug.h>
#include <cmath>

int abs( int num );

ROTOR_DEFINE_TYPE( Point,
  struct Point {
    double x;
    double y;
  };
)

ROTOR_DEFINE_TYPE( PointList,
  struct PointList {
    uint8_t count;
    Point * points;
  };
)

SUITE( StructureArray ) {

  //----------------------------------------------------------------------------

  TEST( OwnedMemory )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( Point ) );
    registry.registerType( ROTOR_DEFINITION_STRING( PointList ) );
    Rotor::Structure structure( "PointList", 0, registry );
    structure["count"] = 10;
    CHECK_EQUAL( 10 * sizeof( Point ), structure["points"].bufferSize() );
    CHECK_EQUAL( 10 , structure["points"].size() );
  
    for ( int i = 0; i < 10; i++ ) {
      structure["points"][i]["x"] = i * 2;
      structure["points"][i]["y"] = i * 2 + 1;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( i * 2     == structure["points"][i]["x"] );
      CHECK( i * 2 + 1 == structure["points"][i]["y"] );
    }
    structure["count"] = 20;
    CHECK_EQUAL( 20, structure["points"].size() );
    structure["points"][19]["x"] = 5;
    for ( int i = 0; i < 10; i++ ) {
      CHECK( i * 2 == structure["points"][i]["x"] );
    }
    CHECK( 5 == structure["points"][19]["x"] );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedMemory )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( Point ) );
    registry.registerType( ROTOR_DEFINITION_STRING( PointList ) );
    
    PointList list = {0, 0};
    Rotor::Structure structure( "PointList", &list, registry );
    structure["count"] = 10;
    for ( int i = 0; i < 10; i++ ) {
      structure["points"][i]["x"] = i * 2;
      structure["points"][i]["y"] = i * 2 + 1;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( i * 2     == list.points[i].x );
      CHECK( i * 2 + 1 == list.points[i].y );
    }

    for ( int i = 0; i < 10; i++ ) {
      list.points[i].x = i * 2 - 1;
      list.points[i].y = i * 2;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( i * 2 - 1 == structure["points"][i]["x"] );
      CHECK( i * 2     == structure["points"][i]["y"] );
    }
  }
}

