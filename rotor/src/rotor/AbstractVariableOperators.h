#ifndef ROTOR_ABSTRACT_VARIABLE_OPERATORS_H
#define ROTOR_ABSTRACT_VARIABLE_OPERATORS_H


#include "OperatorMacros.h"


namespace Rotor {


class AbstractVariable;


DECLARE_BOOL_OPERATOR(==)
DECLARE_BOOL_OPERATOR(!=)
DECLARE_BOOL_OPERATOR(<)
DECLARE_BOOL_OPERATOR(>)
DECLARE_BOOL_OPERATOR(<=)

DECLARE_NUMERIC_OPERATOR(-)
DECLARE_NUMERIC_OPERATOR(*)
DECLARE_NUMERIC_OPERATOR(+)
DECLARE_NUMERIC_OPERATOR(/)

template < typename T >
T & operator<<( T & v1, const AbstractVariable & v2 ) {
  if ( v2.type() == IntType ) {
    v1 << v2.operator int();
  } else if ( v2.type() == DoubleType ) {
    v1 << v2.operator double();
  } else if ( v2.type() == StringType ) {
    v1 << v2.operator const char *();
  }
  return v1;
}

template < typename T >
T & operator<<( T & v1, AbstractVariable & v2 ) {
  if ( v2.type() == IntType ) {
    v1 << v2.operator int();
  } else if ( v2.type() == DoubleType ) {
    v1 << v2.operator double();
  } else if ( v2.type() == StringType ) {
    v1 << v2.operator const char *();
  }
  return v1;
}

}


#endif //ROTOR_ABSTRACT_VARIABLE_OPERATORS_H