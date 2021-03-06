#ifndef ROTOR_STRUCTURE_BASE_H
#define ROTOR_STRUCTURE_BASE_H


#include <rotor/AbstractVariable.h>
#include <rotor/SharedPointers.h>
#include <rotor/Exceptions.h>
#include <map>
#include <vector>

namespace Rotor {


class Type;
class Registry;


class StructureBase : public AbstractVariable
{
public:
  StructureBase( 
    const std::string & typeName, 
    void * address, 
    const Registry & registry );
  virtual ~StructureBase();
  virtual AbstractVariable & operator=( const StructureBase & value );
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

  
private:
  typedef std::map<std::string, AbstractVariable *> MemberTable;
  typedef std::vector<AbstractVariable *> MemberArray;
  typedef std::map<AbstractVariable *, AbstractVariable * > VariableTable;
  typedef std::map<AbstractVariable*, size_t *> SizeTable;
  
  void initialize();
  void adjustPointers( const AbstractVariable & member ) const;

  const Registry &  _registry;
  const Type &      _type;
  MemberTable       _members;
  MemberArray       _memberArray;
  VariableTable     _variableArraySizes;
  SizeTable         _variableArrayPointers;
};


}


#endif //ROTOR_STRUCTURE_BASE_H
