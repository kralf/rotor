#include "LightweightStructure.h"
#include "Type.h"
#include "NullDeleter.h"
#include "Logger.h"

using namespace Rotor;

//------------------------------------------------------------------------------

LightweightStructure::LightweightStructure( const Structure & structure )
  : Structure( structure.typeData().name(), 0, structure.registry() ),
    _structure( const_cast<Structure*>( &structure ), NullDeleter() )
{
}

//------------------------------------------------------------------------------

LightweightStructure::LightweightStructure( const LightweightStructure & structure )
  : Structure( structure.typeData().name(), 0, structure.registry() ),
    _structure( structure._structure )
{
}

//------------------------------------------------------------------------------

LightweightStructure::LightweightStructure( const StructurePtr & structure )
  : Structure( structure->typeData().name(), 0, structure->registry() ),
    _structure( structure )
{
}

//------------------------------------------------------------------------------

AbstractVariable & 
LightweightStructure::operator=( const LightweightStructure & value )
{
  _structure = StructurePtr( value._structure.get() );
  return *this;
}

//------------------------------------------------------------------------------

AbstractVariable & 
LightweightStructure::operator=( const Structure & value )
{
  _structure = StructurePtr( const_cast<Structure *>( &value ), NullDeleter() );
  return *this;
}

//------------------------------------------------------------------------------

AbstractVariable & 
LightweightStructure::operator=( const AbstractVariable & value )
{
  return (*_structure) = value;
}

//------------------------------------------------------------------------------

AbstractVariable & 
LightweightStructure::operator[]( int index )
{
  return (*_structure )[index];
}

//------------------------------------------------------------------------------

AbstractVariable & 
LightweightStructure::operator[]( const char * fieldName )
{
  return (*_structure )[fieldName];
}

//------------------------------------------------------------------------------

const AbstractVariable & 
LightweightStructure::operator[]( int index ) const
{
  return (*_structure )[index];
}

//------------------------------------------------------------------------------

const AbstractVariable & 
LightweightStructure::operator[]( const char * fieldName ) const
{
  return (*_structure )[fieldName];
}

//------------------------------------------------------------------------------

size_t 
LightweightStructure::size() const
{
  return (*_structure ).size();
}

//------------------------------------------------------------------------------

GenericType 
LightweightStructure::type() const
{
  return (*_structure ).type();

}

//------------------------------------------------------------------------------

const Type & 
LightweightStructure::typeData() const
{
  return (*_structure ).typeData();
}

//------------------------------------------------------------------------------

const Registry & 
LightweightStructure::registry() const
{
  return (*_structure ).registry();
}

//------------------------------------------------------------------------------

std::string 
LightweightStructure::toString() const
{
  return (*_structure ).toString();
}

//------------------------------------------------------------------------------

void 
LightweightStructure::adjust() const
{
  (*_structure ).adjust();
}

//------------------------------------------------------------------------------

const size_t 
LightweightStructure::bufferSize() const
{
  return (*_structure).bufferSize();
}

//------------------------------------------------------------------------------

void * 
LightweightStructure::buffer() const
{
  return (*_structure).buffer();
}

//------------------------------------------------------------------------------

const bool 
LightweightStructure::owner() const
{
  return (*_structure).owner();
}
