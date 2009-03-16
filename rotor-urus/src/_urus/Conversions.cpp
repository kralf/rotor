#include "Conversions.h"

using namespace Rotor;
using yarp::os::Bottle;
using namespace std;

//------------------------------------------------------------------------------


void 
appendToBottle( Bottle & bottle, const AbstractVariable & value )
{
  if ( value.type() == IntType )  {
    bottle->addInt(  value );
  } else if ( value.type() == DoubleType ) {
    bottle->addDouble(  value );
  } else if ( value.type() == StringType ) {
    bottle->addString(  value );
  } else if ( value.type() == ArrayType ) {
    Bottle & subBottle = bottle.addList();
    for ( int i = 0; i < value.size(); i++ ) {
      appendToBottle( subBottle, value[i] );
    }
  } else if ( value.type() == StructureType ) {
    Bottle & subBottle = bottle.addList();
    for ( int i = 0; i < value.size(); i++ ) {
      appendToBottle( subBottle, value[i] );
    }
  }
}

//------------------------------------------------------------------------------

Structure * 
bottleToStructure( const Bottle & bottle, const Type & type )
{
}
