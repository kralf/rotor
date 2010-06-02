#include <unittest++/UnitTest++.h>
#include <rotor/Exceptions.h>
#include <rotor/String.h>

SUITE( String ) {

  //----------------------------------------------------------------------------

  TEST( OwnedMemory )
  {
    Rotor::String s;
    CHECK_EQUAL( 0 , s.size() );
    
    s = "12345";
    CHECK_EQUAL( strlen( "12345" ) , s.size() );
    
    Rotor::AbstractVariable & rs = s;
    rs = "ABC";
    CHECK_EQUAL( strcmp( "ABC", s ), 0 );
    const char * ts = rs;
    CHECK_EQUAL( strcmp( "ABC", ts ), 0 );
  }

  //----------------------------------------------------------------------------

  TEST( PreallocatedMemory )
  {
    std::string ss = "";
    Rotor::String rs = ss;
    CHECK_EQUAL( 0 , rs.size() );
    
    CHECK_THROW( rs = "12345", Rotor::InvalidAssignmentError );
  }
}

