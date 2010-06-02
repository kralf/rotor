
template < typename T, GenericType TYPE >
Array<T, TYPE>::Array( int size ) 
  : AbstractVariable( 0, size * sizeof( T ) ), 
    _size( size ), 
    _members( size ) 
{
  initializeMembers();
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Array<T, TYPE>::Array( T * address, int size ) 
  : AbstractVariable( address, size * sizeof( T ) ), 
    _size( size ), 
    _members( size ) 
{
  initializeMembers();
}
  
//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
Array<T, TYPE>::~Array() 
{
  for ( int i = 0; i < _size; i++ ) {
    delete _members[i];
  }
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
AbstractVariable & 
Array<T, TYPE>::operator=( const Array & value )
{
  for ( size_t i = 0; i < value.size(); i++ ) {
    *_members[i] = value[i];
  }
  return * this;  
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
AbstractVariable & 
Array<T, TYPE>::operator=( const AbstractVariable & value )
{
  if (  value.type() != ArrayType  ) {
    throw TypeError( "Trying to assign incompatible type to Array" );
  }
  
  const Array & sValue = *reinterpret_cast<const Array *>( &value );
  return operator=( sValue );
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
AbstractVariable & 
Array<T, TYPE>::operator[]( int index ) 
{
  if ( index >= _size || index < 0 ) {
    throw( std::out_of_range( "Index out of range while accesing a Variable array" ) );
  }
  return *_members[index];
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
const AbstractVariable & 
Array<T, TYPE>::operator[]( int index ) const
{
  if ( index >= _size || index < 0 ) {
    throw( std::out_of_range( "Index out of range while accesing a Variable array" ) );
  }
  return *_members[index];
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
void 
Array<T, TYPE>::resize( size_t newSize ) 
{
  if ( newSize != static_cast<size_t>( _size ) ) {
    resizeBuffer( newSize * sizeof( T ) );
    for ( int i = 0; i < _size; i++ ) {
      delete _members[i];
    }
    _size = newSize;
    initializeMembers();
  }
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
size_t 
Array<T, TYPE>::size() const 
{
  return _size;
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
GenericType 
Array<T, TYPE>::type() const 
{
  return ArrayType;
}

//------------------------------------------------------------------------------

template < typename T, GenericType TYPE >
void 
Array<T, TYPE>::initializeMembers() 
{
  _members.resize( _size );
  T * pointer = reinterpret_cast<T *>( buffer() );
  for ( int i = 0; i < _size; i++ ) {
    _members[i] = new Variable<T, TYPE>( &pointer[i] );
  }
}
