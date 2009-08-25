#ifndef ROTOR_STRUCTURE_H
#define ROTOR_STRUCTURE_H


#include "AbstractVariable.h"
#include "SharedPointers.h"


namespace Rotor {


class Type;


class Structure : public AbstractVariable
{
public:
  Structure( 
    const std::string & typeName, 
    void * address, 
    const Registry & registry );
  Structure( const Structure & structure );
  virtual ~Structure();
  
  virtual AbstractVariable & operator=( const Structure & value );
  virtual AbstractVariable & operator=( const AbstractVariable & value );
  virtual AbstractVariable & operator[]( int index );
  virtual AbstractVariable & operator[]( const char * fieldName );
  virtual const AbstractVariable & operator[]( int index ) const;
  virtual const AbstractVariable & operator[]( const char * fieldName ) const;
  virtual size_t size() const;
  GenericType type() const;

  virtual const Type & typeData() const;
  virtual const Registry & registry() const;
  virtual std::string toString() const;
  virtual void adjust() const;
  virtual void referTo( const Structure & value );
  
  virtual size_t bufferSize() const;
  virtual void * buffer() const;
  virtual bool owner() const;


private:
  StructurePtr _structure;
};


}


#endif //ROTOR_STRUCTURE_H
