#include <unittest++/UnitTest++.h>
#include <rotor/Exceptions.h>
#include <rotor/BaseOptions.h>


SUITE( Options ) {

  TEST( Everything )
  {
    Rotor::BaseOptions options;
    CHECK_THROW( options.getString( "aSection", "anOption" ), Rotor::OptionError );
    options.setInt( "aSection", "anOption", 1 );
    CHECK_EQUAL( options.getInt( "aSection", "anOption" ), 1 );
    CHECK_THROW( options.getBool( "aSection", "anotherOption" ), Rotor::OptionError );
    CHECK_EQUAL( options.getBool( "aSection", "anotherOption", true ), true );
    CHECK_EQUAL( options.getBool( "aSection", "anotherOption" ), true );
  }

}
