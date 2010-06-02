#include <boost/spirit.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace boost::spirit;


//------------------------------------------------------------------------------

struct StructureGrammar : public grammar<StructureGrammar>
{
  StructureGrammar() 
    : grammar<StructureGrammar>()
  {
  }

  template <typename SCANNER>
  struct definition
  {
    typedef rule< SCANNER > Rule;
    Rule  grammar, structure, identifier, 
          declaration, typeDefinition, 
          normalType, fixedArrayType, variableArrayType, integer;
    
    Rule const &
    start() const 
    {
      return grammar;
    }
    
    
    definition ( StructureGrammar const & self )
    {
      grammar           = *space_p 
                          >> structure 
                          >> *space_p 
                          >>';' 
                          >> *space_p 
                          >> end_p;
      structure         = "struct" 
                          >> +space_p 
                          >> identifier 
                          >> *space_p 
                          >> '{' 
                          >> *declaration 
                          >> '}';
      identifier        = ( alpha_p | '_' ) >> *( alnum_p | '_' );
      declaration       = *space_p
                          >> typeDefinition 
                          >> *space_p;
      typeDefinition    =   normalType 
                          | fixedArrayType 
                          | variableArrayType;
      normalType        = identifier 
                          >> +space_p 
                          >> identifier
                          >> *space_p 
                          >> ';';
      fixedArrayType    = identifier 
                          >> +space_p 
                          >> identifier 
                          >> *space_p 
                          >> '[' 
                          >> *space_p 
                          >> integer 
                          >> *space_p 
                          >> ']'
                          >> *space_p 
                          >> ';';
      variableArrayType = identifier 
                          >> *space_p 
                          >> '*' 
                          >> *space_p 
                          >> identifier
                          >> *space_p 
                          >> ';';
      integer           = digit_p >> *digit_p;
    }
    
  };
  

};

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  StructureGrammar grammar;
  string input;
  input =  input 
      + "struct Point {\n" 
      + "  int count;\n" 
      + "  double * x;\n" 
      + "  float y[10];\n" 
      + "  char * _stringValue;\n" 
      + "};"
      ;
  
  parse_info<> info = parse( input.c_str(), grammar );

  if (info.full)
  {
      cout << "-------------------------\n";
      cout << "Parsing succeeded\n";
      cout << "-------------------------\n";
  }
  else
  {
      cout << "-------------------------\n";
      cout << "Parsing failed\n";
      cout << "stopped at: \"" << info.stop << "\"\n";
      cout << "-------------------------\n";
  }
}
