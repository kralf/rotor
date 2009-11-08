#ifndef ROTOR_ARRAY_H
#define ROTOR_ARRAY_H


#include "Variable.h"
#include "Exceptions.h"
#include "Debug.h"
#include <vector>


namespace Rotor {


template < typename T, GenericType TYPE >
class Array : public AbstractVariable
{
public:
  explicit Array( int size );
  Array( T * address, int size );
  virtual ~Array();
  virtual AbstractVariable & operator=( const Array & value );
  virtual AbstractVariable & operator=( const AbstractVariable & value );
  virtual AbstractVariable & operator[]( int index );
  virtual const AbstractVariable & operator[]( int index ) const;
  virtual void resize( size_t newSize );
  virtual size_t size() const;
  GenericType type() const;

private:
  void initializeMembers();

  int                              _size;
  std::vector<Variable<T, TYPE> *> _members;
};

#include "Array.tpp"

}



#endif //ROTOR_ARRAY_H
