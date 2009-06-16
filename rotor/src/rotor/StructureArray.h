#ifndef ROTOR_STRUCTURE_ARRAY_H
#define ROTOR_STRUCTURE_ARRAY_H


#include "AbstractVariable.h"
#include <string>
#include <vector>


namespace Rotor {


class Registry;
class Structure;
class Type;


class StructureArray : public AbstractVariable
{
public:
  StructureArray( 
    const std::string & typeName, 
    void * address, 
    int size, 
    const Registry & registry );
  virtual ~StructureArray();
  virtual AbstractVariable & operator=( const StructureArray & value );
  virtual AbstractVariable & operator=( const AbstractVariable & value );
  virtual AbstractVariable & operator[]( size_t index );
  virtual const AbstractVariable & operator[]( size_t index ) const;
  virtual void resize( size_t newSize );
  virtual size_t size() const;
  GenericType type() const;
  
private:
  void initializeMembers();
  
  size_t                   _size;
  std::vector<Structure *> _members;
  const Registry &         _registry;
  const Type &             _type;

};


}


#endif //ROTOR_STRUCTURE_ARRAY_H