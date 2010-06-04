#include <unittest++/UnitTest++.h>
#include <rotor/BaseOptions.h>
#include "rotor/OptionParser.ipp"
#include <string>


SUITE( OptionParser ) {

  /**
   * Parsing a structure with all possible types of member declarations.
   */

  TEST( Parse )
  {
    std::string input = "";
    input =  input +
      "[section1]\n" +
      "  alpha = alpha content\n" +
      "  beta =     beta content\n" +
      "anumber = 34\n" +
      "  [section2] \n" +
      " ; this should not count\n" +
      "anumber = 12\n";
    Rotor::BaseOptions options;
    Rotor::OptionParser parser;
    CHECK( parser.parse( input, options ) );
    CHECK_EQUAL( "alpha content", options.getString( "section1", "alpha" ) );
    CHECK_EQUAL( "beta content", options.getString( "section1", "beta" ) );
    CHECK_EQUAL( 34, options.getInt( "section1", "anumber" ) );
    CHECK_EQUAL( 12, options.getInt( "section2", "anumber" ) );
  }

  /**
   * Verify that incorrect input leads to parse errors.
   */

  TEST( ParseErrors )
  {
    Rotor::BaseOptions options;
    Rotor::OptionParser parser;

    std::string input = "";
    input =  input +
      "[section1 1]\n" +
      "  alpha = alpha content\n" +
      "  beta =     beta content\n" +
      "anumber = 34\n" +
      "  [section2] \n" +
      " ; this should not count\n" +
      "anumber = 12\n";
    CHECK( ! parser.parse( input, options ) );

    input = "";
    input =  input +
      "[section1]\n" +
      "  alpha = alpha content\n" +
      "  beta =     beta content\n" +
      " trheta\n"
      "anumber = 34\n" +
      "  [section2] \n" +
      " ; this should not count\n" +
      "anumber = 12\n";
    CHECK( ! parser.parse( input, options ) );
  }
}
