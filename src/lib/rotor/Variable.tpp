template < typename T, GenericType TYPE >
Variable<T, TYPE>::Variable() 
  : AbstractVariable( 0, sizeof( T ) ) 
{
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Variable<T, TYPE>::Variable( const AbstractVariable & value ) 
  : AbstractVariable( 0, sizeof( T ) )  
{
  if ( TYPE == IntType ) {
    operator=( value.operator int() );
  } else if ( TYPE == DoubleType ) {
    operator=( value.operator double() );
  }
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Variable<T, TYPE>::Variable( const Variable & variable ) 
  : AbstractVariable( 0, sizeof( T ) ) 
{
  *( reinterpret_cast<T*>( buffer() ) ) = *( reinterpret_cast<T*>( variable.buffer() ) );
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Variable<T, TYPE>::Variable( T * address ) 
  : AbstractVariable( address, sizeof( T ) ) 
{
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Variable<T, TYPE>::Variable( const T & value ) 
  : AbstractVariable( 0, sizeof( T ) ) 
{
  *( reinterpret_cast<T*>( buffer() ) ) = value;
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
AbstractVariable & 
Variable<T, TYPE>::operator=( const AbstractVariable & value ) 
{
  if (  ( type() == IntType || type() == DoubleType ) 
        && ( value.type() == IntType || value.type() == DoubleType ) ) {
    if ( value.type() == IntType ) {
      operator=( value.operator int() );
      return * this;
    } else {
      operator=( value.operator double() );
      return * this;
    }
  }
  AbstractVariable::operator=( value );
  return *this;
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
AbstractVariable & 
Variable<T, TYPE>::operator=( int value ) 
{
  if ( TYPE == IntType || TYPE == DoubleType ) {
    *( reinterpret_cast<T*>( buffer() ) ) = value;
    return *this;
  }
  AbstractVariable::operator=( value );
  return *this;
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
AbstractVariable & 
Variable<T, TYPE>::operator=( double value ) 
{
  if ( TYPE == IntType || TYPE == DoubleType ) {
    *( reinterpret_cast<T*>( buffer() ) ) = value;
    return *this;
  }
  AbstractVariable::operator=( value );
  return *this;
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Variable<T, TYPE>::operator int() const 
{
  if ( TYPE == IntType || TYPE == DoubleType ) {
    return *( reinterpret_cast<T*>( buffer() ) );
  }
  return AbstractVariable::operator int();
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Variable<T, TYPE>::operator double() const 
{
  if ( TYPE == IntType || TYPE == DoubleType ) {
    return *( reinterpret_cast<T*>( buffer() ) );
  }
  return AbstractVariable::operator double();
}

//------------------------------------------------------------------------------


template < typename T, GenericType TYPE >
GenericType 
Variable<T, TYPE>::type() const 
{
  return TYPE;
}
