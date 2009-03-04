#include "Serialization.h"
#include "AbstractVariable.h"
#include "MemberDefinition.h"
#include "Registry.h"
#include "Structure.h"
#include <iomanip>
#include <sstream>
#include <ostream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

const AbstractVariable & unmarshall( istream & stream, AbstractVariable & variable ) {
  if ( variable.type() == StructureType ) {
    stream.ignore();
    for ( int i = 0; i < variable.size(); i++ ) {
      stream.ignore();
      unmarshall( stream, variable[i] );
    }
    stream.ignore( 2 );
  } else if ( variable.type() == ArrayType ) {
    stream.ignore();
    for ( int i = 0; i < variable.size(); i++ ) {
      stream.ignore();
      unmarshall( stream, variable[i] );
    }
    stream.ignore( 2 );
  } else if ( variable.type() == IntType ) {
    int value;
    stream >> value;
    variable = value;
  } else if ( variable.type() == DoubleType ) {
    double value;
    stream >> value;
    variable = value;
  } else if ( variable.type() == StringType ) {
    int count;
    stream >> count;
    stream.ignore();
    stream.ignore();
    string s;
    for ( int i = 0; i < count; i++ ) {
      s += stream.get();
    }
    variable = s.c_str();
  }
  return variable;
}

//------------------------------------------------------------------------------

const AbstractVariable & marshall( ostream & stream, const AbstractVariable & variable ) {
  if ( variable.type() == StructureType ) {
    stream << "{";
    for ( int i = 0; i < variable.size(); i++ ) {
      stream << " ";
      marshall( stream, variable[i] );
    }
    stream << " }";
  } else if ( variable.type() == ArrayType ) {
    stream << "[";
    for ( int i = 0; i < variable.size(); i++ ) {
      stream << " ";
      marshall( stream, variable[i] );
    }
    stream << " ]";
  } else if ( variable.type() == IntType ) {
    stream << variable.operator int();
  } else if ( variable.type() == DoubleType ) {
    stream << variable.operator double();
  } else if ( variable.type() == StringType ) {
    string out( variable );
    stream << out.size() << " :" << out.c_str();
  }
  return variable;
}

//------------------------------------------------------------------------------

const string 
Rotor::marshall( const Message & message )
{
  stringstream stream;
  stream << setprecision( 10 );
  stream << message.name;
  ::marshall( stream, *message.data );
  return stream.str();
}
  
//------------------------------------------------------------------------------

const string 
Rotor::marshall( const AbstractVariable & variable )
{
  stringstream stream;
  stream << setprecision( 10 );
  ::marshall( stream, variable );
  return stream.str();
}
  
//------------------------------------------------------------------------------

Message 
Rotor::unmarshall( const Registry & registry, const string & input )
{
  Structure * data;
  string::size_type last = input.find_first_of( "{", 0 );
  string name = input.substr( 0, last );
  string type = registry.messageType( name ).name();
  data = new Structure( type, 0, registry );
  
  stringstream stream( input.substr( last, input.size() - last ) );
  ::unmarshall( stream, *data );
  return Message( name, data );
}
