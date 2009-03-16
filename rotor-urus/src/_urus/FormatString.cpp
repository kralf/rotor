#include "FormatString.h"
#include <rotor/Conversion.h>
#include <rotor/Registry.h>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

string 
Rotor::formatString( const Registry & registry, const std::string & typeName )
{
  const MemberDefinitions & definitions = registry[typeName].members();
  string result = "{ ";
  int lastIntIndex = -1;
  for ( int i = 0; i < definitions.size(); i++ ) {
    const MemberDefinition & definition = definitions[i];
    if ( i > 0 ) {
      result += ", ";
    }
    if ( definition.cardinality == ONE ) {
      if ( definition.type == "int8_t" ) {
        result += "byte";
        lastIntIndex = i;
      } else if ( definition.type == "uint8_t" ) {
        result += "ubyte";
        lastIntIndex = i;
      } else if ( definition.type == "int16_t" ) {
        result += "short";
        lastIntIndex = i;
      } else if ( definition.type == "uint16_t" ) {
        result += "ushort";
        lastIntIndex = i;
      } else if ( definition.type == "int32_t" ) {
        result += "int";
        lastIntIndex = i;
      } else if ( definition.type == "uint32_t" ) {
        result += "uint";
        lastIntIndex = i;
      } else if ( definition.type == "float" ) {
        result += "float";
      } else if ( definition.type == "double" ) {
        result += "double";
      } else {
        result += formatString( registry, definition.type );
      }
    } else if ( definition.cardinality == VARIABLE ) {
      if ( definition.type == "int8_t" ) {
        result += "<byte: " + toString( lastIntIndex + 1) + ">";
      } else if ( definition.type == "uint8_t" ) {
        result += "<ubyte: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "int16_t" ) {
        result += "<short: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "uint16_t" ) {
        result += "<ushort: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "int32_t" ) {
        result += "<int: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "uint32_t" ) {
        result += "<uint: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "float" ) {
        result += "<float: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "double" ) {
        result += "<double: " + toString( lastIntIndex + 1 ) + ">";
      } else if ( definition.type == "char" ) {
        result += "string";
      }      
    }
  }
  result += " }";
  return result;
}
