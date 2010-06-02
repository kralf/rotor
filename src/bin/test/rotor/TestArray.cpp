#include <unittest++/UnitTest++.h>
#include <rotor/Array.h>
#include <cmath>
#include <stdint.h>

int abs( int num );

SUITE( Array ) {

  //----------------------------------------------------------------------------

  TEST( OwnedMemory )
  {
    Rotor::Array<int8_t, Rotor::IntType> intArray( 10 );
    CHECK_EQUAL( 10 * sizeof( int8_t ), intArray.bufferSize() );
    CHECK_EQUAL( 10 , intArray.size() );

    for ( int i = 0; i < 10; i++ ) {
      intArray[i] = i * 2;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( i * 2 == intArray[i] );
    }
    
    intArray.resize( 20 );
    CHECK_EQUAL( 20, intArray.size() );
    intArray[19] = 5;
    for ( int i = 0; i < 10; i++ ) {
      int temp = intArray[i];
      CHECK( i * 2 == temp );
    }
    CHECK( 5 == intArray[19] );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedMemory )
  {
    int8_t values[5] = { 0, 3, 6, 9, 12 };
    Rotor::Array<int8_t, Rotor::IntType> intArray( &values[0], 5 );
    CHECK_EQUAL( 5 * sizeof( int8_t ), intArray.bufferSize() );
    CHECK_EQUAL( 5 , intArray.size() );
    
    for ( int i = 0; i < 5; i++ ) {
      CHECK( values[i] == intArray[i] );
    }
    
    for ( int i = 0; i < 5; i++ ) {
      intArray[i] = i * 2;
    }
    
    for ( int i = 0; i < 5; i++ ) {
      CHECK( i * 2 == intArray[i] );
    }
    
    for ( int i = 0; i < 5; i++ ) {
      CHECK( values[i] == intArray[i] );
    }

    CHECK_THROW( intArray.resize( 20 ), Rotor::MemoryAllocationError );
  }

  //----------------------------------------------------------------------------

  TEST( Assignment ) {
    Rotor::Array<int8_t, Rotor::IntType> intArray( 10 );
    Rotor::AbstractVariable & aIntArray = intArray;
    
    for ( int i = 0; i < 10; i++ ) {
      aIntArray[i] = i * 2;
    }
    
    for ( int i = 0; i < 10; i++ ) {
      CHECK( i * 2 == aIntArray[i] );
    }
    
    int intX = aIntArray[2];
    CHECK( intX == aIntArray[2] );
    double doubleX = aIntArray[2];
    CHECK( abs( doubleX - aIntArray[2] ) < 0.00001 );
    const char * x;
    CHECK_THROW( x = intArray[2], Rotor::InvalidCastError );

    Rotor::Variable<int8_t, Rotor::IntType> intVar;
    CHECK_THROW( intX = intVar[0], Rotor::InvalidOperatorError );
  }
}

