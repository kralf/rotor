#include "BasicTypes.h"
#include "AbstractVariable.h"
#include "Registry.h"
#include "Structure.h"
#include "StructureArray.h"
#include "Exceptions.h"
#include "Array.h"
#include "String.h"
#include <stdint.h>

using namespace Rotor;

//------------------------------------------------------------------------------

size_t 
Rotor::size( const MemberDefinition & definition, const Registry & registry )
{
  if ( definition.cardinality == VARIABLE ) {
    return sizeof( void * );
  }
  size_t baseSize = 0;
  
  if ( definition.type == "int8_t" ) {
    baseSize = sizeof( int8_t );
  } else if ( definition.type == "uint8_t" ) {
    baseSize = sizeof( uint8_t );
  } else if ( definition.type == "int16_t" ) {
    baseSize = sizeof( int16_t );
  } else if ( definition.type == "uint16_t" ) {
    baseSize = sizeof( uint16_t );
  } else if ( definition.type == "int32_t" ) {
    baseSize = sizeof( int32_t );
  } else if ( definition.type == "uint32_t" ) {
    baseSize = sizeof( uint32_t );
  } else if ( definition.type == "float" ) {
    baseSize = sizeof( float );
  } else if ( definition.type == "double" ) {
    baseSize = sizeof( double );
  } else {
    return registry[definition.type].size();
  }
  return baseSize;
}

//------------------------------------------------------------------------------
  
AbstractVariable * 
Rotor::newVariable( 
  const Registry & registry,
  void *& address, 
  const MemberDefinition & definition,
  void * variableArrayAddress,
  int variableArraySize )
{
  if ( definition.cardinality == VARIABLE ) {
    const int size = variableArraySize;
    if ( definition.type == "int8_t" ) {
      return new Array<int8_t, IntType>( static_cast<int8_t*>( variableArrayAddress ), size );
    } else if ( definition.type == "uint8_t" ) {
      return new Array<uint8_t, IntType>( static_cast<uint8_t*>( variableArrayAddress ), size );
    } else if ( definition.type == "int16_t" ) {
      return new Array<int16_t, IntType>( static_cast<int16_t*>( variableArrayAddress ), size );
    } else if ( definition.type == "uint16_t" ) {
      return new Array<uint16_t, IntType>( static_cast<uint16_t*>( variableArrayAddress ), size );
    } else if ( definition.type == "int32_t" ) {
      return new Array<int32_t, IntType>( static_cast<int32_t*>( variableArrayAddress ), size );
    } else if ( definition.type == "uint32_t" ) {
      return new Array<uint32_t, IntType>( static_cast<uint32_t*>( variableArrayAddress ), size );
    } else if ( definition.type == "float" ) {
      return new Array<float, DoubleType>( static_cast<float*>( variableArrayAddress ), size );
    } else if ( definition.type == "double" ) {
      return new Array<double, DoubleType>( static_cast<double*>( variableArrayAddress ), size );
    } else if ( definition.type == "char" ) {
      if ( variableArrayAddress ) {
        char * tmpString = static_cast<char *>( variableArrayAddress );
        if ( strlen( tmpString ) > 0 ) {
          return new String( tmpString  );
        }
      }
      return new String( address );
    } else {
      return new StructureArray( definition.type, static_cast<void *>( variableArrayAddress ), size, registry );
    }
  } else if ( definition.cardinality != ONE ) {
    const int size = definition.cardinality;
    if ( definition.type == "int8_t" ) {
      return new Array<int8_t, IntType>( static_cast<int8_t*>( address ), size );
    } else if ( definition.type == "uint8_t" ) {
      return new Array<uint8_t, IntType>( static_cast<uint8_t*>( address ), size );
    } else if ( definition.type == "int16_t" ) {
      return new Array<int16_t, IntType>( static_cast<int16_t*>( address ), size );
    } else if ( definition.type == "uint16_t" ) {
      return new Array<uint16_t, IntType>( static_cast<uint16_t*>( address ), size );
    } else if ( definition.type == "int32_t" ) {
      return new Array<int32_t, IntType>( static_cast<int32_t*>( address ), size );
    } else if ( definition.type == "uint32_t" ) {
      return new Array<uint32_t, IntType>( static_cast<uint32_t*>( address ), size );
    } else if ( definition.type == "float" ) {
      return new Array<float, DoubleType>( static_cast<float*>( address ), size );
    } else if ( definition.type == "double" ) {
      return new Array<double, DoubleType>( static_cast<double*>( address ), size );
    } else {
      return new StructureArray( definition.type, address, size, registry );
    }
  } else {
    if ( definition.type == "int8_t" ) {
      return new Variable<int8_t, IntType>( static_cast<int8_t*>( address ) );
    } else if ( definition.type == "uint8_t" ) {
      return new Variable<uint8_t, IntType>( static_cast<uint8_t*>( address ) );
    } else if ( definition.type == "int16_t" ) {
      return new Variable<int16_t, IntType>( static_cast<int16_t*>( address ) );
    } else if ( definition.type == "uint16_t" ) {
      return new Variable<uint16_t, IntType>( static_cast<uint16_t*>( address ) );
    } else if ( definition.type == "int32_t" ) {
      return new Variable<int32_t, IntType>( static_cast<int32_t*>( address ) );
    } else if ( definition.type == "uint32_t" ) {
      return new Variable<uint32_t, IntType>( static_cast<uint32_t*>( address ) );
    } else if ( definition.type == "float" ) {
      return new Variable<float, DoubleType>( static_cast<float*>( address ) );
    } else if ( definition.type == "double" ) {
      return new Variable<double, DoubleType>( static_cast<double*>( address ) );
    } else if ( definition.type == "double" ) {
      return new Variable<double, DoubleType>( static_cast<double*>( address ) );
    } else {
      AbstractVariable * result = new Structure( definition.type, address, registry );
      return result;
    }
  }
}
