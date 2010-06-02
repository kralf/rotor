#include <unittest++/UnitTest++.h>
#include <rotor/BaseOptions.h>
#include <rotor/BaseRegistry.h>
#include <rotor/Structure.h>
#include <rotor/Rotor.h>
#include <stdint.h>

ROTOR_DEFINE_TYPE( Complete,
  struct Complete {
    int8_t count;
    double * x;
    float y[10];
  };
)

ROTOR_DEFINE_TYPE( FixedPointArray,
  struct FixedPointArray {
    double x[10];
    float  y[10];
  };
)

ROTOR_DEFINE_TYPE( VariablePointArray,
  struct VariablePointArray {
    uint8_t   count;
    double  * x;
    float   * y;
  };
)

ROTOR_DEFINE_TYPE( IntAndString,
  struct IntAndString {
    uint8_t  intValue;
    char  *  stringValue;
  };
)

ROTOR_DEFINE_TYPE( IntAndStructure,
  struct IntAndStructure {
    uint8_t   intValue;
    Complete  structureValue;
  };
)

ROTOR_DEFINE_TYPE( RealCase1,
  struct RealCase1 {
    double         globalpos_xy_cov;
    int32_t        converged;
    double         timestamp;
  };
)


SUITE( Structure ) {
  
  //----------------------------------------------------------------------------

//   TEST( Initialization )
//   {
//     Rotor::BaseOptions options;
//     Rotor::BaseRegistry registry( "test", options );
//     registry.registerType( ROTOR_DEFINITION_STRING( Complete ) );
//     Rotor::Structure complete( "Complete", 0, registry );
//   }

  //----------------------------------------------------------------------------

  TEST( FixedPointArray )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( FixedPointArray ) );
    Rotor::Structure structure( "FixedPointArray", 0, registry );

    for ( int i = 0; i < 10; i++ ) {
      structure["x"][i] = i;
      structure["y"][i] = structure["x"][i] + i;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i );
      CHECK( structure["y"][i] == 2 * i );
    }
    
    FixedPointArray * realStruct = reinterpret_cast<FixedPointArray *>( structure.buffer() );
    for ( int i = 0; i < 10; i++ ) {
      CHECK( realStruct->x[i] == i );
      CHECK( realStruct->y[i] == 2 * i );
    }

    for ( int i = 0; i < 10; i++ ) {
      realStruct->x[i] = i * 3;
      realStruct->y[i] = i - 3;
    }

    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i * 3 );
      CHECK( structure["y"][i] == i - 3 );
      CHECK( structure["x"][i] != i - 3 );
    }    
    
    CHECK_THROW( structure["x"][10] = 3, std::out_of_range );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedFixedPointArray )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( FixedPointArray ) );
    
    FixedPointArray realStruct;
    Rotor::Structure structure( "FixedPointArray", &realStruct, registry );

    for ( int i = 0; i < 10; i++ ) {
      structure["x"][i] = i;
      structure["y"][i] = structure["x"][i] + i;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i );
      CHECK( structure["y"][i] == 2 * i );
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( realStruct.x[i] == i );
      CHECK( realStruct.y[i] == 2 * i );
    }

    for ( int i = 0; i < 10; i++ ) {
      realStruct.x[i] = i * 3;
      realStruct.y[i] = i - 3;
    }

    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i * 3 );
      CHECK( structure["y"][i] == i - 3 );
      CHECK( structure["x"][i] != i - 3 );
    }    
    
    CHECK_THROW( structure["x"][10] = 3, std::out_of_range );
  }

  //----------------------------------------------------------------------------

  TEST( VariablePointArray )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( VariablePointArray ) );
    Rotor::Structure structure( "VariablePointArray", 0, registry );

    structure["count"] = 10;
    for ( int i = 0; i < 10; i++ ) {
      structure["x"][i] = i;
      structure["y"][i] = structure["x"][i] + i;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i );
      CHECK( structure["y"][i] == 2 * i );
    }
    
    CHECK_THROW( structure["x"][10] = 3, std::out_of_range );
    
    structure["count"] = 100;
    VariablePointArray * realStruct = reinterpret_cast<VariablePointArray *>( structure.buffer() );
    for ( int i = 0; i < 10; i++ ) {
      CHECK( realStruct->x[i] == i );
      CHECK( realStruct->y[i] == 2 * i );
    }

    for ( int i = 0; i < 10; i++ ) {
      realStruct->x[i] = i * 3;
      realStruct->y[i] = i - 3;
    }

    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i * 3 );
      CHECK( structure["y"][i] == i - 3 );
      CHECK( structure["x"][i] != i - 3 );
    }    
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedVariablePointArray )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( VariablePointArray ) );
    
    double x[10];
    float  y[10];
    VariablePointArray realStruct = { 10, &x[0], &y[0] };
    Rotor::Structure structure( "VariablePointArray", &realStruct, registry );

    for ( int i = 0; i < 10; i++ ) {
      structure["x"][i] = i;
      structure["y"][i] = structure["x"][i] + i;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i );
      CHECK( structure["y"][i] == 2 * i );
    }
    
    CHECK_THROW( structure["x"][10] = 3, std::out_of_range );
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( realStruct.x[i] == i );
      CHECK( realStruct.y[i] == 2 * i );
    }

    for ( int i = 0; i < 10; i++ ) {
      realStruct.x[i] = i * 3;
      realStruct.y[i] = i - 3;
    }

    for ( int i = 0; i < 10; i++ ) {
      CHECK( structure["x"][i] == i * 3 );
      CHECK( structure["y"][i] == i - 3 );
      CHECK( x[i] == i * 3 );
      CHECK( y[i] == i - 3 );
    }    
  }

  //----------------------------------------------------------------------------

  TEST( IntAndString )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( IntAndString ) );
    Rotor::Structure structure( "IntAndString", 0, registry );  
    structure["intValue"]    = 38;
    structure["stringValue"] = "fistandantilus";
    CHECK( structure["intValue"] == 38 );
    CHECK( structure["stringValue"] == "fistandantilus" );
    IntAndString & realStruct = *( reinterpret_cast<IntAndString*>( structure.buffer() ) );
    CHECK( realStruct.intValue == 38 );
    CHECK( std::string( realStruct.stringValue ) == std::string( "fistandantilus" ) );
    CHECK( std::string( structure["stringValue"] ) == std::string( realStruct.stringValue )  );
    
    //Check what happens when we change string size
    structure["stringValue"] = "Era la arzavesperia y los flexilimosos toves giroscopiaban taledrando en el vade";
//TODO: Correct valgrind error here
    CHECK( std::string( realStruct.stringValue ) != std::string( "fistandantilus" ) );
    std::string s1( structure["stringValue"] );
    CHECK( s1 == std::string( realStruct.stringValue )  );
    CHECK( s1 == std::string( "Era la arzavesperia y los flexilimosos toves giroscopiaban taledrando en el vade" )  );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedIntAndString )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( IntAndString ) );
    
    const char * fistandantilus = "fistandantilus";
    
    IntAndString realStruct = {38, const_cast<char*>( fistandantilus ) };

    Rotor::Structure structure( "IntAndString", &realStruct, registry );  
    
    CHECK( structure["intValue"] == 38 );
    CHECK( structure["stringValue"] == "fistandantilus" );
    CHECK( realStruct.intValue == 38 );
    CHECK( std::string( realStruct.stringValue ) == std::string( "fistandantilus" ) );
//     CHECK( "fistandantilus" == realStruct.stringValue );
    
    CHECK_THROW( structure["stringValue"] = "anotherThing", Rotor::InvalidAssignmentError );
  }  

  //----------------------------------------------------------------------------

  TEST( IntAndStructure )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( Complete ) );
    registry.registerType( ROTOR_DEFINITION_STRING( IntAndStructure ) );
    Rotor::Structure structure( "IntAndStructure", 0, registry );  
    structure["structureValue"]["count"] = 1;
    structure["structureValue"]["x"][0] = 2;
    structure["intValue"] = 3;
    CHECK( 1 == structure["structureValue"]["count"] );
    CHECK( 2 == structure["structureValue"]["x"][0] );
    CHECK( 3 == structure["intValue"] );
  }
  
  //----------------------------------------------------------------------------

  TEST( PreallocatedIntAndStructure )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( Complete ) );
    registry.registerType( ROTOR_DEFINITION_STRING( IntAndStructure ) );
    IntAndStructure ias = {0, {0, 0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}}};
    Rotor::Structure structure( "IntAndStructure", &ias, registry );  
    structure["structureValue"]["count"] = 1;
    structure["structureValue"]["y"][0] = 2;
    structure["intValue"] = 3;
    CHECK_EQUAL( 1, ias.structureValue.count );
    CHECK_EQUAL( 2, ias.structureValue.y[0] );
    CHECK_EQUAL( 3, ias.intValue );
  }
  
  //----------------------------------------------------------------------------

  TEST( Assignments )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( FixedPointArray ) );
    Rotor::Structure originalFPA( "FixedPointArray", 0, registry );  
    Rotor::Structure copyFPA( "FixedPointArray", 0, registry );  
    
    
    //Test in one sense
    for ( int i = 0; i < 10; i++ ) {
      originalFPA["x"][i] = i;
      originalFPA["y"][i] = i * 2;
    }

    copyFPA = originalFPA;
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( originalFPA["x"][i] == copyFPA["x"][i] );
      CHECK( originalFPA["y"][i] == copyFPA["y"][i] );
    }
    
    //Now in the other
    for ( int i = 0; i < 10; i++ ) {
      originalFPA["x"][i] = i + 1;
      originalFPA["y"][i] = i + 2;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( originalFPA["x"][i] != copyFPA["x"][i] );
      CHECK( originalFPA["y"][i] != copyFPA["x"][i] );
    }
    
    //Test exceptions
    FixedPointArray realFPA;
    
    Rotor::Structure preallocatedFPA( "FixedPointArray", &realFPA, registry );  
    preallocatedFPA = originalFPA;
    
    //-------------------------------------
    //Now test for variable arrays
    //-------------------------------------
    
    registry.registerType( ROTOR_DEFINITION_STRING( VariablePointArray ) );
    Rotor::Structure originalVPA( "VariablePointArray", 0, registry );  
    Rotor::Structure copyVPA( "VariablePointArray", 0, registry );  
    
    
    originalVPA["count"] = 10;
    //Test in one sense
    for ( int i = 0; i < 10; i++ ) {
      originalVPA["x"][i] = i;
      originalVPA["y"][i] = i * 2;
    }
    
    copyVPA = originalVPA;
    
    CHECK( originalVPA["count"] == copyVPA["count"] );
    for ( int i = 0; i < 10; i++ ) {
      CHECK( originalVPA["x"][i] == copyVPA["x"][i] );
      CHECK( originalVPA["y"][i] == copyVPA["y"][i] );
    }
    
    //Now in the other
    for ( int i = 0; i < 10; i++ ) {
      originalVPA["x"][i] = i + 1;
      originalVPA["y"][i] = i + 2;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( originalVPA["x"][i] != copyVPA["x"][i] );
      CHECK( originalVPA["y"][i] != copyVPA["x"][i] );
    }
    
    //Test exceptions

    VariablePointArray realVPA = {0, 0, 0}; 
    
    Rotor::Structure preallocatedVPA( "VariablePointArray", &realVPA, registry );  
    CHECK_THROW( preallocatedVPA = originalVPA, Rotor::InvalidAssignmentError );

  }

    double         globalpos_xy_cov;
    int32_t        converged;
    double         timestamp;
  //----------------------------------------------------------------------------

  TEST( RealCase1 )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    registry.registerType( ROTOR_DEFINITION_STRING( RealCase1 ) );
    Rotor::Structure structure( "RealCase1", 0, registry );  
    structure["globalpos_xy_cov"]    = 1.1;
    structure["converged"]           = 2;
    structure["timestamp"]           = 2.2;
    RealCase1 & realStruct = *( reinterpret_cast<RealCase1*>( structure.buffer() ) );
    CHECK_EQUAL( sizeof realStruct, structure.typeData().size() );
    CHECK( realStruct.globalpos_xy_cov == 1.1 );
    CHECK( realStruct.converged        == 2 );
    CHECK( realStruct.timestamp        == 2.2 );
  }
}
