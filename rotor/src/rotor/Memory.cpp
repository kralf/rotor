#include "Memory.h"
#include "Exceptions.h"
#include <cstdlib>

using namespace Rotor;

//------------------------------------------------------------------------------

Memory::Memory( void * address, size_t bufferSize )
  : _buffer( address ),
    _owner( address != 0 ),
    _bufferSize( bufferSize )
{
  if ( address ) {
    _owner = false;
  } else {
    _owner  = true;
    if ( bufferSize > 0 ) {
      _buffer = malloc( bufferSize );
      memset( _buffer, 0, bufferSize );
    }
  }
}

//------------------------------------------------------------------------------

Memory::~Memory()
{
  if ( _owner && _buffer ) {
    free( _buffer );
  }
}

//------------------------------------------------------------------------------

size_t
Memory::bufferSize() const
{
  return _bufferSize;
}

//------------------------------------------------------------------------------

bool 
Memory::owner() const
{
  return _owner;
}

//------------------------------------------------------------------------------

void * 
Memory::buffer() const
{
  return _buffer;
}


//------------------------------------------------------------------------------

void
Memory::resizeBuffer( size_t newSize )
{
  if ( _owner ) {
    if ( newSize > 0 ) {
      _buffer = realloc( _buffer, newSize );
      _bufferSize = newSize;
    }
    if ( ! _buffer && newSize != 0 ) {
      throw MemoryAllocationError( "Buffer resize has failed." );
    }
  } else {
    if ( newSize != _bufferSize ) {
      throw MemoryAllocationError( "Cannot resize buffer because has no ownership." );
    }
  }
}

//------------------------------------------------------------------------------

void 
Memory::copyBuffer( const Memory & memory )
{
  if ( _owner ) {
    resizeBuffer( memory._bufferSize );
    memcpy( memory._buffer, _buffer, _bufferSize );
  }
}
