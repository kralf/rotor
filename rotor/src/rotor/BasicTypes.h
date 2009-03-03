#ifndef ROTOR_BASIC_TYPES_H
#define ROTOR_BASIC_TYPES_H

#include "MemberDefinition.h"

namespace Rotor {

class AbstractVariable;
class Registry;

size_t size( const MemberDefinition & definition, const Registry & registry );

AbstractVariable * newVariable( 
  const Registry & registry,
  void *& address, 
  const MemberDefinition & definition,
  void * variableArrayAddress,
  int variableArraySize );
}

#endif //ROTOR_BASIC_TYPES_H
