#include <unittest++/UnitTest++.h>
#include <rotor/StructureParser.h>
#include <string>


SUITE( StructureParser ) {

  /**
   * Parsing a structure with all possible types of member declarations.
   */

  TEST( CompleteStructure )
  {
    std::string input = "";
    input =  input +
      "struct Point {\n" +
      "  int count;\n" +
      "  double * x;\n" +
      "  float y[10];\n" +
      "  char * _stringValue;\n" +
      "};";
    Rotor::StructureParser parser;
    CHECK( parser.parse( input ) );
    CHECK_EQUAL( 4, parser.members().size() );
    CHECK_EQUAL( "Point", parser.name() );

    CHECK_EQUAL( "int", parser.members()[0].type );
    CHECK_EQUAL( "count", parser.members()[0].name );
    CHECK_EQUAL( 0, parser.members()[0].cardinality );

    CHECK_EQUAL( "double", parser.members()[1].type );
    CHECK_EQUAL( "x", parser.members()[1].name );
    CHECK_EQUAL( -1, parser.members()[1].cardinality );

    CHECK_EQUAL( "float", parser.members()[2].type );
    CHECK_EQUAL( "y", parser.members()[2].name );
    CHECK_EQUAL( 10, parser.members()[2].cardinality );

    CHECK_EQUAL( "char", parser.members()[3].type );
    CHECK_EQUAL( "_stringValue", parser.members()[3].name );
    CHECK_EQUAL( -1, parser.members()[3].cardinality );
  }

  /**
   * Verify that incorrect input leads to parse errors.
   */

  TEST( ParseErrors )
  {
    std::string input = "";
    input =  input +
      "struct Point {\n" +
      "  int count\n" +
      "  double * x;\n" +
      "  float y[10];\n" +
      "};";
    Rotor::StructureParser parser;
    CHECK( ! parser.parse( input ) );

    input = "";
    input =  input +
      "struct Point {\n" +
      "  int;\n" +
      "  double * x;\n" +
      "  float y[10];\n" +
      "};";

    CHECK( ! parser.parse( input ) );

    input = "";
    input =  input +
      "struct Point {\n" +
      "  int;\n" +
      "  double * x;\n" +
      "  float y[10];\n" +
      "}";

    CHECK( ! parser.parse( input ) );

    input = "";
    input =  input +
      "struct Point {\n" +
      "  int count;\n" +
      "  double * x;\n" +
      "  float y[10];\n" +
      "};";

    CHECK( parser.parse( input ) );
  }
}
