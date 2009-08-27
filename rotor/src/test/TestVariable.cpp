#include <unittest++/UnitTest++.h>
#include <rotor/Variable.h>
#include <stdint.h>

using namespace Rotor;

SUITE( Variable ) {

  //----------------------------------------------------------------------------

  TEST( OwnedMemory )
  {
    int integerX = 10;
    Rotor::Variable<int8_t, IntType> integerVariable( integerX );
    CHECK_EQUAL( 10, integerVariable );
    integerVariable = 5;
    CHECK_EQUAL( 5, integerVariable );
    CHECK( integerVariable != integerX );

    double doubleX = 10.1;
    Rotor::Variable<double, DoubleType> doubleVariable( doubleX );
    CHECK_EQUAL( 10.1, doubleVariable );
    doubleVariable = 5.3;
    CHECK_EQUAL( 5.3, doubleVariable );
    CHECK( doubleVariable != doubleX );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedMemory )
  {
    int8_t integerX = 10;
    Rotor::Variable<int8_t, IntType> integerVariable( &integerX );
    Rotor::AbstractVariable & aiVariable = integerVariable;
    CHECK( 10 == aiVariable );
    CHECK_EQUAL( 10, integerVariable );
    integerVariable = 5;
    CHECK_EQUAL( 5, integerVariable );
    aiVariable = 6.1;
    CHECK( 6 == aiVariable );
    CHECK_EQUAL( integerVariable, integerX );

    double doubleX = 10.1;
    Rotor::Variable<double, DoubleType> doubleVariable( &doubleX );
    CHECK_EQUAL( 10.1, doubleVariable );
    doubleVariable = 5.3;
    CHECK_EQUAL( 5.3, doubleVariable );
    CHECK_EQUAL( doubleVariable, doubleX );
  }

  //----------------------------------------------------------------------------

  TEST( Assignment )
  {
    Rotor::Variable<int16_t, IntType> intX = 10;
    int z = intX;
    CHECK_EQUAL( intX, z );
    
    Rotor::Variable<int8_t, DoubleType> intY( intX );
    CHECK_EQUAL( intX, intY );
    
    intX = 1024;
    intY = 1024;
    
    CHECK( intX != intY ); //Numeric overflow in intY
    
    intY = 5;
    CHECK( intX != intY );
    
    intX = 6;
    intY = intX;
    CHECK_EQUAL( intX, intY );
  }

}

