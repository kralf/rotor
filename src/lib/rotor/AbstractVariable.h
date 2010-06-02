#ifndef ROTOR_ABSTRACT_VARIABLE_H
#define ROTOR_ABSTRACT_VARIABLE_H


#include "Memory.h"


namespace Rotor {

//------------------------------------------------------------------------------

enum GenericType 
{
  IntType,
  DoubleType,
  ArrayType,
  StructureType,
  StringType
};

//------------------------------------------------------------------------------

class AbstractVariable : public Memory
{
public:
  virtual AbstractVariable & operator=( const AbstractVariable & value );
  virtual AbstractVariable & operator=( int value );
  virtual AbstractVariable & operator=( double value );
  virtual AbstractVariable & operator=( const char * value );
  virtual operator int() const;
  virtual operator double() const;
  virtual operator const char *() const;
  virtual AbstractVariable & operator[]( int index );
  virtual AbstractVariable & operator[]( const char * fieldName );
  virtual const AbstractVariable & operator[]( int index ) const;
  virtual const AbstractVariable & operator[]( const char * fieldName ) const;
  virtual size_t size() const;
  virtual void resize( size_t newSize );  
  virtual GenericType type() const = 0;
  virtual size_t footprint() const;
  
protected:
  AbstractVariable( void * address, size_t size );
};


}

#include "AbstractVariableOperators.h"


#endif //ROTOR_ABSTRACT_VARIABLE_H
