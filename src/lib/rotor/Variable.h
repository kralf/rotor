#ifndef ROTOR_VARIABLE_H
#define ROTOR_VARIABLE_H


#include "AbstractVariable.h"


namespace Rotor {


template < typename T, GenericType TYPE >
class Variable : public AbstractVariable
{
public:
  Variable();
  Variable( const AbstractVariable & value );
  Variable( const Variable & variable );
  Variable( T * address );
  Variable( const T & value );
  virtual AbstractVariable & operator=( const AbstractVariable & value );
  virtual AbstractVariable & operator=( int value );
  virtual AbstractVariable & operator=( double value );
  virtual operator int() const;
  virtual operator double() const;
  virtual GenericType type() const;
};

#include "Variable.tpp"

}

#endif //ROTOR_VARIABLE_H
