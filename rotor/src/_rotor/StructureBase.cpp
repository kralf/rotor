#include "StructureBase.h"
#include <rotor/BasicTypes.h>
#include <rotor/Debug.h>
#include <rotor/Logger.h>
#include <rotor/Serialization.h>
#include <sstream>
#include <stdint.h>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

StructureBase::StructureBase( 
  const std::string & typeName, 
  void * address, 
  const Registry & registry ) 
  : AbstractVariable( address, registry[typeName].size() ),
    _registry( registry ),
    _type( registry[typeName] )
{
  initialize();
}

//------------------------------------------------------------------------------

void
StructureBase::initialize()
{
  const MemberDefinitions & definitions = _registry[ _type.name() ].members();
  uint8_t * bytePointer = static_cast<uint8_t *>( buffer() );
  AbstractVariable * variableArraySize = NULL;
  for ( size_t i = 0; i < definitions.size(); i++ ) {
    void * pointer = static_cast<void *>( bytePointer );
    const MemberDefinition & definition = definitions[i];
    
    AbstractVariable * variable;
    // For owned pointer arrays, the default capacity is 10
    // TODO: What to do with the corresponding count?
    if ( owner() ) {
      variable = newVariable( _registry, pointer, definition, 0, 0 ); //HERE
    } else {
      void * variableArrayPointer = *( reinterpret_cast<void **>( pointer ) );
      if ( ! variableArraySize ) {
        variable = newVariable( _registry, pointer, definition, variableArrayPointer, 0 );
      } else {
        int arraySize = *variableArraySize;
        variable = newVariable( _registry, pointer, definition, variableArrayPointer, arraySize );
      }
    }
    
    _members[definition.name]   = variable;
    _memberArray.push_back( variable );
    
    // Assume that the last integer type before a pointer array is its index
    if ( variable->type() == IntType ) {
      variableArraySize = variable;  
    }
    if ( definition.cardinality == VARIABLE ) {
      _variableArrayPointers[variable] = reinterpret_cast<size_t *>( pointer );
      *_variableArrayPointers[variable] = reinterpret_cast<size_t>( variable->buffer() );
      _variableArraySizes[variable] = variableArraySize;
    }
    bytePointer += definition.realSize;
  }
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureBase::operator=( const StructureBase & structure )
{
  if (  structure.type() != StructureType || structure.typeData().name() != typeData().name() ) {
    throw TypeError( "Trying to assign incompatible type to Structure:" + typeData().name() );
  }
  
  for ( size_t i = 0; i < size(); i++ ) {
    AbstractVariable & member = operator[]( i );
    if ( _variableArrayPointers.find( &member ) != _variableArrayPointers.end() ) {
      if ( ! owner() ) {
        throw InvalidAssignmentError( "Non owner destination structure contains pointer" );
      }
      member = structure[i];
      *_variableArrayPointers[&member] = reinterpret_cast<size_t>( member.buffer() );
    } else {
      member = structure[i];
    }
  }

  return * this;
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureBase::operator=( const AbstractVariable & value )
{
  if (  value.type() != StructureType  ) {
    throw TypeError( "Trying to assign incompatible type to Structure:" + typeData().name() );
  }
  
  const StructureBase & sValue = *reinterpret_cast<const StructureBase *>( &value );
  return operator=( sValue );
}

//------------------------------------------------------------------------------

StructureBase::~StructureBase() 
{
  for ( std::map<std::string, AbstractVariable *>::iterator i = _members.begin();
        i != _members.end();
        i++ ) {
    delete i->second;
  }
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureBase::operator[]( int index )
{
  AbstractVariable & member = *_memberArray[index];
  adjustPointers( member);
  return member;
}

//------------------------------------------------------------------------------

const AbstractVariable & 
StructureBase::operator[]( int index ) const
{
  AbstractVariable & member = *_memberArray[index];
  adjustPointers( member);
  return member;
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureBase::operator[]( const char * fieldName ) 
{
  MemberTable::iterator iMember = _members.find( fieldName );
  if (  iMember == _members.end() ) {
    throw  TypeError( "Structure does not have field: " + std::string( fieldName ) );
  }
  AbstractVariable & member = *iMember->second;
  adjustPointers( member );
  return member;
}

//------------------------------------------------------------------------------

const AbstractVariable & 
StructureBase::operator[]( const char * fieldName ) const
{
  MemberTable::const_iterator iMember = _members.find( fieldName );
  if (  iMember == _members.end() ) {
    throw  TypeError( "Structure does not have field: " + std::string( fieldName ) );
  }
  
  AbstractVariable & member = *iMember->second;
  adjustPointers( member );
  return member;
}

//------------------------------------------------------------------------------

size_t 
StructureBase::size() const
{
  return _memberArray.size();
}

//------------------------------------------------------------------------------

GenericType 
StructureBase::type() const 
{
  return StructureType;
}  

//------------------------------------------------------------------------------

const Type &
StructureBase::typeData() const 
{
  return _type;
}  

//------------------------------------------------------------------------------

const Registry &
StructureBase::registry() const 
{
  return _registry;
}  

//------------------------------------------------------------------------------

string
StructureBase::toString() const 
{
  stringstream result;
  result << _type.name() << " {\n";
  for ( size_t i = 0; i < _type.members().size(); ++i ) {
    result << "  "  << _type.members()[i].name << ": ";
    if ( (*this)[i].bufferSize() > 0 ) {
      result << marshall( (*this)[i] ) <<";\n";
    } else {
      result << "(empty);\n";
    }
  }
  result << "}\n";
  return result.str();
}  

//------------------------------------------------------------------------------

void 
StructureBase::adjust() const
{
  for ( size_t i = 0; i < size(); ++i ) {
    adjustPointers( (*this)[i] );
  } 
}

//------------------------------------------------------------------------------

//NOTE: Need to find a way to solve this const correctness mess.
void 
StructureBase::adjustPointers( const AbstractVariable & member ) const
{
  StructureBase & self = *const_cast<StructureBase *>( this );
  AbstractVariable & cmember = *const_cast<AbstractVariable *>( &member );
  if ( member.owner() ) {
    if ( member.type() == StringType ) {
      *(self._variableArrayPointers[&cmember]) = reinterpret_cast<size_t>( member.buffer() );
    } else if ( member.type() == ArrayType ) {
      VariableTable::const_iterator iArraySize = _variableArraySizes.find( &cmember );
      if ( iArraySize != _variableArraySizes.end() ) {
        cmember.resize( iArraySize->second->operator int() );
        *(self._variableArrayPointers[&cmember]) = reinterpret_cast<size_t>( member.buffer() );
      }
    }
  }
}
