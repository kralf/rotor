#include "AbstractVariable.h"
#include "Exceptions.h"

using namespace Rotor;

//------------------------------------------------------------------------------

AbstractVariable::AbstractVariable( void * address, size_t size ) 
  : Memory( address, size )
{
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( const AbstractVariable & value )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( int value )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( double value )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator=( const char * value )
{
  throw InvalidAssignmentError( "Value cannot be assigned to variable" );
}

//------------------------------------------------------------------------------

AbstractVariable::operator int() const
{
  throw InvalidCastError( "Variable cannot be cast to int" );
}

//------------------------------------------------------------------------------

AbstractVariable::operator double() const
{
  throw InvalidCastError( "Variable cannot be cast to double" );
}

//------------------------------------------------------------------------------

AbstractVariable::operator const char *() const
{
  throw InvalidCastError( "Variable is not a string" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator[]( int index ) {
  throw InvalidOperatorError( "Variable is not an Array" );
}

//------------------------------------------------------------------------------

AbstractVariable & 
AbstractVariable::operator[]( const char * fieldName ) {
  throw InvalidOperatorError( "Variable is not a Structure" );
}

//------------------------------------------------------------------------------
  
const AbstractVariable & 
AbstractVariable::operator[]( int index ) const
{
  throw InvalidOperatorError( "Variable is not an Array" );
}

//------------------------------------------------------------------------------

const AbstractVariable & 
AbstractVariable::operator[]( const char * fieldName ) const
{
  throw InvalidOperatorError( "Variable is not a Structure" );
}

//------------------------------------------------------------------------------

size_t 
AbstractVariable::size() const
{
  return 1;
}

//------------------------------------------------------------------------------

void 
AbstractVariable::resize( size_t newSize )
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
  for ( int i = 0; i < size(); ++i ) {
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
