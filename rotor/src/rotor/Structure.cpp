#include "Structure.h"
#include "Debug.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

Structure::Structure( 
  const std::string & typeName, 
  void * address, 
  const Registry & registry ) 
  : AbstractVariable( address, registry[typeName].size() ),
    _type( registry[typeName] ),
    _registry( registry )
{
  const MemberDefinitions & definitions = registry[ typeName ].members();
  uint8_t * bytePointer = static_cast<uint8_t *>( buffer() );
  AbstractVariable * variableArraySize = NULL;
  for ( int i = 0; i < definitions.size(); i++ ) {
    void * pointer = static_cast<void *>( bytePointer );
    const MemberDefinition & definition = definitions[i];
    
    AbstractVariable * variable;
    // For owned pointer arrays, the default capacity is 10
    // TODO: What to do with the corresponding count?
    if ( owner() ) {
      variable = newVariable( registry, pointer, definition, 0, 0 ); //HERE
    } else {
      void * variableArrayPointer = *( reinterpret_cast<void **>( pointer ) );
      if ( ! variableArraySize ) {
        variable = newVariable( registry, pointer, definition, variableArrayPointer, 0 );
      } else {
        int arraySize = *variableArraySize;
        variable = newVariable( registry, pointer, definition, variableArrayPointer, arraySize );
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
Structure::operator=( const Structure & structure )
{
  if (  structure.type() != StructureType || structure.typeData().name() != typeData().name() ) {
    throw TypeError( "Trying to assign incompatible type to Structure:" + typeData().name() );
  }
  
  for ( int i = 0; i < size(); i++ ) {
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
Structure::operator=( const AbstractVariable & value )
{
  if (  value.type() != StructureType  ) {
    throw TypeError( "Trying to assign incompatible type to Structure:" + typeData().name() );
  }
  
  const Structure & sValue = *reinterpret_cast<const Structure *>( &value );
  return operator=( sValue );
}

//------------------------------------------------------------------------------

Structure::~Structure() 
{
  for ( std::map<std::string, AbstractVariable *>::iterator i = _members.begin();
        i != _members.end();
        i++ ) {
    delete i->second;
  }
}

//------------------------------------------------------------------------------

AbstractVariable & 
Structure::operator[]( int index )
{
  AbstractVariable & member = *_memberArray[index];
  adjustPointers( member);
  return member;
}

//------------------------------------------------------------------------------

const AbstractVariable & 
Structure::operator[]( int index ) const
{
  AbstractVariable & member = *_memberArray[index];
  adjustPointers( member);
  return member;
}

//------------------------------------------------------------------------------

AbstractVariable & 
Structure::operator[]( const char * fieldName ) 
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
Structure::operator[]( const char * fieldName ) const
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
Structure::size() const
{
  return _memberArray.size();
}

//------------------------------------------------------------------------------

GenericType 
Structure::type() const 
{
  return StructureType;
}  

//------------------------------------------------------------------------------

const Type &
Structure::typeData() const 
{
  return _type;
}  

//------------------------------------------------------------------------------

const Registry &
Structure::registry() const 
{
  return _registry;
}  

//------------------------------------------------------------------------------

//NOTE: Need to find a way to solve this const correctness mess.
void 
Structure::adjustPointers( const AbstractVariable & member ) const
{
  Structure & self = *const_cast<Structure *>( this );
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