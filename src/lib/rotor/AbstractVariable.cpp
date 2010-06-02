#include "AbstractVariable.h"
#include "Exceptions.h"
#include "Conversion.h"

using namespace Rotor;

//------------------------------------------------------------------------------

AbstractVariable::AbstractVariable( void * address, size_t size ) 
  : Memory( address, size )
{
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( const AbstractVariable & )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( int )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( double )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( const char * )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable::operator int() const
{
  throw InvalidCastError( "Variable of type " + toString( type() ) + " cannot be cast to int" );
}

//------------------------------------------------------------------------------

AbstractVariable::operator double() const
{
  throw InvalidCastError( "Variable of type " + toString( type() ) + " cannot be cast to double" );
}

//------------------------------------------------------------------------------

AbstractVariable::operator const char *() const
{
  throw InvalidCastError( "Variable of type " + toString( type() ) + " is not a string" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator[]( int ) {
  throw InvalidOperatorError( "Variable of type " + toString( type() ) + " is not an Array" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator[]( const char * ) {
  throw InvalidOperatorError( "Variable of type " + toString( type() ) + " is not a Structure" );
}

//------------------------------------------------------------------------------
  
const AbstractVariable & 
AbstractVariable::operator[]( int ) const
{
  throw InvalidOperatorError( "Variable of type " + toString( type() ) + " is not an Array" );
}

//------------------------------------------------------------------------------

const AbstractVariable & 
AbstractVariable::operator[]( const char * ) const
{
  throw InvalidOperatorError( "Variable of type " + toString( type() ) + " is not a Structure" );
}

//------------------------------------------------------------------------------

size_t 
AbstractVariable::size() const
{
  return 1;
}

//------------------------------------------------------------------------------

void 
AbstractVariable::resize( size_t )
{
  throw InvalidOperatorError( "Variable is not an Array" );
}

//------------------------------------------------------------------------------

size_t 
AbstractVariable::footprint() const
{
  if ( type() == IntType ) {
    return bufferSize();
  } else if ( type() == DoubleType ) {
    return bufferSize();
  } else if ( type() == StringType ) {
    return size();
  }
  size_t result = 0;
  for ( size_t i = 0; i < size(); ++i ) {
    result += operator[](i).footprint();
  }
  return result;
}

//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

IMPLEMENT_BOOL_OPERATOR(==)
IMPLEMENT_BOOL_OPERATOR(!=)
IMPLEMENT_BOOL_OPERATOR(<)
IMPLEMENT_BOOL_OPERATOR(>)
IMPLEMENT_BOOL_OPERATOR(<=)

IMPLEMENT_NUMERIC_OPERATOR(-)
IMPLEMENT_NUMERIC_OPERATOR(*)
IMPLEMENT_NUMERIC_OPERATOR(+)
IMPLEMENT_NUMERIC_OPERATOR(/)
