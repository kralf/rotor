#include "StructureArray.h"
#include "Registry.h"
#include "Structure.h"
#include "Type.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

StructureArray::StructureArray( 
  const string & typeName, 
  void * address, 
  int size,
  const Registry & registry ) 
  : AbstractVariable( address, size * registry[typeName].size() ), 
    _size( size ), 
    _members( size ),
    _registry( registry ),
    _type( registry[typeName] )
{
  initializeMembers();
}
  
//------------------------------------------------------------------------------

StructureArray::~StructureArray() 
{
  for ( size_t i = 0; i < _size; i++ ) {
    delete _members[i];
  }
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureArray::operator=( const StructureArray & value )
{
  for ( size_t i = 0; i < value.size(); i++ ) {
    *_members[i] = value[i];
  }
  return * this;  
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureArray::operator=( const AbstractVariable & value )
{
  if (  value.type() != ArrayType  ) {
    throw TypeError( "Trying to assign incompatible type to StructureArray" );
  }
  
  // TODO: Find a better way to check for this
  if ( value[0].type() != StructureType ) {
    throw TypeError( "Cannot assign Array to StructureArray" );
  }
  
  const StructureArray & sValue = *reinterpret_cast<const StructureArray *>( &value );
  return operator=( sValue );
}

//------------------------------------------------------------------------------

AbstractVariable & 
StructureArray::operator[]( size_t index ) 
{
  if ( index >= _size || index < 0 ) {
    throw( std::out_of_range( "Index out of range while accesing a StructureArray" ) );
  }
  return *_members[index];
}

//------------------------------------------------------------------------------

const AbstractVariable & 
StructureArray::operator[]( size_t index ) const
{
  if ( index >= _size || index < 0 ) {
    throw( std::out_of_range( "Index out of range while accesing a StructureArray" ) );
  }
  return *_members[index];
}

//------------------------------------------------------------------------------

void 
StructureArray::resize( size_t newSize ) 
{
  if ( _size == newSize ) {
    return;
  }
  resizeBuffer( newSize * _type.size() );
  for ( size_t i = 0; i < _size; i++ ) {
    delete _members[i];
  }
  if ( newSize > _size ) {
    memset( 
      static_cast<int8_t*>( buffer() ) + _size * _type.size(), 
      0, 
      _type.size() * ( newSize - _size ) );
  }
  _size = newSize;
  initializeMembers();
}

//------------------------------------------------------------------------------

size_t 
StructureArray::size() const 
{
  return _size;
}

//------------------------------------------------------------------------------

GenericType 
StructureArray::type() const 
{
  return ArrayType;
}

//------------------------------------------------------------------------------

void 
StructureArray::initializeMembers() 
{
  _members.resize( _size );
  int8_t * pointer = reinterpret_cast<int8_t *>( buffer() );
  for ( size_t i = 0; i < _size; i++ ) {
    _members[i] = new Structure( _type.name(), pointer, _registry );
    pointer    += _members[i]->bufferSize();
  }
}
