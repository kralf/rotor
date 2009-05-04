#include "Type.h"
#include "BasicTypes.h"
#include "Registry.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

Type::Type( 
  const Registry & registry, 
  const string & name, 
  const MemberDefinitions & members )
  : _registry( registry ),
    _name( name ),
    _members( members )
{
  size_t offset = 0;
  size_t structurePad = 1;
  for ( int i = 0; i < _members.size(); i++ ) {
    MemberDefinition & definition = _members[i];
    definition.size = ::size( definition, registry );
        
    if ( definition.size > structurePad ) {
      structurePad = definition.size;
    }

    if ( definition.cardinality > 0 ) {
      definition.realSize = definition.size * definition.cardinality;
    } else {
      definition.realSize = definition.size;
    }
    
    size_t nextPad = ( definition.size - offset % definition.size ) % definition.size;
    if ( nextPad >= 4 ) {
      nextPad = nextPad % 4;
    }
    offset += nextPad;
    if ( i > 0 ) {
      _members[i-1].realSize += nextPad;
    }
    offset += definition.realSize;
  }
  _size = offset;
  size_t finalPad = ( structurePad - offset % structurePad ) %  structurePad; 
  if ( finalPad ) {
    if ( finalPad >= 4) {
      finalPad = finalPad % 4;
    }
    _members[_members.size() - 1].realSize += finalPad;
    _size += finalPad;
  }
}

//------------------------------------------------------------------------------

const Registry & 
Type::registry() const
{
  return _registry;
}

//------------------------------------------------------------------------------

const string & 
Type::name() const
{
  return _name;
}

//------------------------------------------------------------------------------

const MemberDefinitions & 
Type::members() const
{
  return _members;
}

//------------------------------------------------------------------------------

MemberDefinitions & 
Type::members()
{
  return _members;
}

//------------------------------------------------------------------------------

size_t 
Type::size() const
{
  return _size;
}
