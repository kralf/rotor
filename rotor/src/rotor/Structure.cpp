#include "Structure.h"
#include <_rotor/StructureBase.h>
#include "Type.h"
#include "NullDeleter.h"
#include "Logger.h"

using namespace Rotor;

//------------------------------------------------------------------------------

Structure::Structure( 
  const std::string & typeName, 
  void * address, 
  const Registry & registry )
  : AbstractVariable( reinterpret_cast<void*>( 1 ), 0 ),
    _structure( new StructureBase( typeName, address, registry ) )
    
{
}

//------------------------------------------------------------------------------

Structure::Structure( const Structure & structure )
  : AbstractVariable( reinterpret_cast<void*>( 1 ), 0 ),
    _structure( structure._structure )
{
}

//------------------------------------------------------------------------------

AbstractVariable & 
Structure::operator=( const Structure & value )
{
/*  _structure = StructurePtr( value._structure.get() );*/
  return *_structure = *( value._structure );
}

//------------------------------------------------------------------------------

AbstractVariable & 
Structure::operator=( const AbstractVariable & value )
{
  return *_structure = value;
}

//------------------------------------------------------------------------------

AbstractVariable & 
Structure::operator[]( int index )
{
  return (*_structure )[index];
}

//------------------------------------------------------------------------------

AbstractVariable & 
Structure::operator[]( const char * fieldName )
{
  return (*_structure )[fieldName];
}

//------------------------------------------------------------------------------

const AbstractVariable & 
Structure::operator[]( int index ) const
{
  return (*_structure )[index];
}

//------------------------------------------------------------------------------

const AbstractVariable & 
Structure::operator[]( const char * fieldName ) const
{
  return (*_structure )[fieldName];
}

//------------------------------------------------------------------------------

size_t 
Structure::size() const
{
  return (*_structure ).size();
}

//------------------------------------------------------------------------------

GenericType 
Structure::type() const
{
  return (*_structure ).type();

}

//------------------------------------------------------------------------------

const Type & 
Structure::typeData() const
{
  return (*_structure ).typeData();
}

//------------------------------------------------------------------------------

const Registry & 
Structure::registry() const
{
  return (*_structure ).registry();
}

//------------------------------------------------------------------------------

std::string 
Structure::toString() const
{
  return (*_structure ).toString();
}

//------------------------------------------------------------------------------

void 
Structure::adjust() const
{
  (*_structure ).adjust();
}

//------------------------------------------------------------------------------

void 
Structure::referTo( const Structure & structure )
{
  _structure = structure._structure;
}

//------------------------------------------------------------------------------

const size_t 
Structure::bufferSize() const
{
  return (*_structure).bufferSize();
}

//------------------------------------------------------------------------------

void * 
Structure::buffer() const
{
  return (*_structure).buffer();
}

//------------------------------------------------------------------------------

const bool 
Structure::owner() const
{
  return (*_structure).owner();
}
