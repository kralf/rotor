#include <unittest++/UnitTest++.h>
#include <rotor/Rotor.h>
#include <rotor/BaseOptions.h>
#include <rotor/BaseRegistry.h>
#include <stdint.h>

#define TEST_ALIGNMENT( NAME ) \
iRegistry.registerType( ROTOR_DEFINITION_STRING( NAME ) ); \
CHECK_EQUAL( sizeof( NAME ), iRegistry[#NAME].size() );

ROTOR_DEFINE_TYPE( Test1, 
  struct Test1 {
    int8_t c1;
    int8_t c2[10];
  };
)

ROTOR_DEFINE_TYPE( Test2, 
  struct Test2 {
    int8_t c1;
    int8_t * c2;
  };
)

ROTOR_DEFINE_TYPE( Test3, 
  struct Test3 {
    int8_t c1;
    int16_t c2;
  };
)

ROTOR_DEFINE_TYPE( Test4, 
  struct Test4 {
    int8_t c1;
    int16_t c2;
    int32_t c3;
  };
)

ROTOR_DEFINE_TYPE( Test5, 
  struct Test5 {
    int8_t c1;
    int16_t c2;
    int8_t c3;
  };
)

ROTOR_DEFINE_TYPE( Test6, 
  struct Test6 {
    int8_t c1;
    int16_t c2;
    int8_t c3;
    int32_t c4;
  };
)

ROTOR_DEFINE_TYPE( Test7, 
  struct Test7 {
    int8_t c1;
    int8_t c2;
    int8_t c3;
  };
)

ROTOR_DEFINE_TYPE( Test8, 
  struct Test8 {
    int16_t c1;
    int8_t c2;
  };
)

ROTOR_DEFINE_TYPE( Test9, 
  struct Test9 {
    int16_t c1;
    int8_t c2;
    int8_t a1;
    int8_t a2;
  };
)

ROTOR_DEFINE_TYPE( Test10, 
  struct Test10 {
    int8_t c2;
    int8_t a1;
    int16_t c1;
    int8_t a2;
  };
)

ROTOR_DEFINE_TYPE( Test11, 
  struct Test11 {
    int32_t c1;
    int8_t c2;
    int8_t a1;
    int8_t a2;
  };
)

ROTOR_DEFINE_TYPE( Test12, 
  struct Test12 {
    int8_t c2;
    int8_t a1;
    int32_t c1;
    int8_t a2;
  };
)

ROTOR_DEFINE_TYPE( Test13, 
  struct Test13 {
    int8_t intValue;
    char * stringValue;
  };
)

ROTOR_DEFINE_TYPE( Test14,
  struct Test14 {
    int8_t count;
    double * x;
    float y[10];
  };
)

ROTOR_DEFINE_TYPE( Test15,
  struct Test15 {
    uint8_t intValue;
    Test14  structureValue;
  };
)



SUITE( TypeRegistry ) {

  TEST( Alignment )
  {
    Rotor::BaseOptions options;
    Rotor::BaseRegistry registry( "test", options );
    Rotor::Registry & iRegistry = registry;
    TEST_ALIGNMENT( Test1 );
    TEST_ALIGNMENT( Test2 );
    TEST_ALIGNMENT( Test3 );
    TEST_ALIGNMENT( Test4 );
    TEST_ALIGNMENT( Test5 );
    TEST_ALIGNMENT( Test6 );
    TEST_ALIGNMENT( Test7 );
    TEST_ALIGNMENT( Test8 );
    TEST_ALIGNMENT( Test9 );
    TEST_ALIGNMENT( Test10 );
    TEST_ALIGNMENT( Test11 );
    TEST_ALIGNMENT( Test12 );
    TEST_ALIGNMENT( Test13 );
    TEST_ALIGNMENT( Test14 );
    TEST_ALIGNMENT( Test15 );
  }

}
