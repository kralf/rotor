#include "String.h"

using namespace Rotor;

//------------------------------------------------------------------------------

String::String()
  : AbstractVariable( 0, 0 ), 
    _ownerAddress( 0 ),
    _size( 0 ) 
{
}

//------------------------------------------------------------------------------

String::String( const std::string & s )
  : AbstractVariable( const_cast< char * >( s.c_str() ), s.size() + 1 ), 
    _ownerAddress( 0 ),
    _size( s.size() ) 
{
}

//------------------------------------------------------------------------------

String::String( const char * address )
  : AbstractVariable( const_cast< char * >( address ), strlen( address ) + 1 ), 
    _ownerAddress( 0 ),
    _size( strlen( address ) ) 
{
}

//------------------------------------------------------------------------------

String::String( void * ownerAddress )
  : AbstractVariable( 0, 0 ), 
    _ownerAddress( static_cast< void ** >( ownerAddress ) ),
    _size( 0 ) 
{
}

//------------------------------------------------------------------------------

AbstractVariable & 
String::operator=( const AbstractVariable & variable ) 
{
  if ( variable.type() != StringType ) {
    throw InvalidAssignmentError( "Attempting to assign from a non String type" );
  }
  return operator=( variable.operator const char *() );
}

//------------------------------------------------------------------------------

AbstractVariable & 
String::operator=( const char * address ) 
{
  if ( not owner() ) {
    throw InvalidAssignmentError( "Attempting to assign to a non owned String" );
  }
  _size = strlen( address );
  resizeBuffer( _size + 1 );
  if ( _ownerAddress ) {
    (*_ownerAddress ) = static_cast<char *>( buffer() );
  }
  strcpy( reinterpret_cast< char * >( buffer() ), address );
  return *this;
}

//------------------------------------------------------------------------------

String::operator const char*() const 
{
  return reinterpret_cast< char * >( buffer() );
}

//------------------------------------------------------------------------------

size_t 
String::size() const 
{
  return _size;
}

//------------------------------------------------------------------------------

GenericType 
String::type() const 
{
  return StringType;
}
