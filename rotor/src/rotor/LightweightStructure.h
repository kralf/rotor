#ifndef ROTOR_LIGHTWEIGHT_STRUCTURE_H
#define ROTOR_LIGHTWEIGHT_STRUCTURE_H


#include "Structure.h"


namespace Rotor {


class LightweightStructure : public Structure
{
public:
  LightweightStructure( const Structure & structure );
  LightweightStructure( const LightweightStructure & structure );
  LightweightStructure( const StructurePtr & structure );
  virtual AbstractVariable & operator=( const LightweightStructure & value );
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
  
  virtual const size_t bufferSize() const;
  virtual void * buffer() const;
  virtual const bool owner() const;


private:
  StructurePtr _structure;
};


}



#endif //ROTOR_LIGHTWEIGHT_STRUCTURE_H
