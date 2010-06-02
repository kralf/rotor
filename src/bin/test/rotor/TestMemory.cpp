#include <unittest++/UnitTest++.h>
#include <rotor/Memory.h>

SUITE( Memory ) {

  //----------------------------------------------------------------------------

  TEST( OwnedMemory )
  {
    int x;
    Rotor::Memory memory( 0, sizeof x );
    CHECK_EQUAL( sizeof x, memory.bufferSize() );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedMemory )
  {
    int x;
    Rotor::Memory memory( &x, sizeof x );
    CHECK_EQUAL( sizeof x, memory.bufferSize() );
  }
}

