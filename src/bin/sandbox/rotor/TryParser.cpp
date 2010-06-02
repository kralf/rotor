#include <rotor/StructureParser.h>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

void 
print( const MemberDefinition & member )
{
  cout << "\t- Name: " << member.name << endl;
  cout << "\t  Type: " << member.type << endl;
  cout << "\t  Cardinality: " << member.cardinality << endl;
}


//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  string content = "";
  content =  content +  "struct Point {\n" +  
                        "  int count;\n" +
                        "  double * x;\n" +
                        "  float y[10];\n" +
                        "};";

  StructureParser parser;
  parser.parse( content );
  
  cout << "Name: " << parser.name() << endl;
  for_each( parser.members().begin(), parser.members().end(), ptr_fun( &print ) );
}
