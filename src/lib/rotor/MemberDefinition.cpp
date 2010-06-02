#include "MemberDefinition.h"

using namespace Rotor;

//------------------------------------------------------------------------------

bool 
MemberDefinition::operator==( const MemberDefinition & value ) const
{
  return  name        == value.name &&
          type        == value.type &&
          cardinality == value.cardinality;
}
