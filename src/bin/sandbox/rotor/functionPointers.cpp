#include <cstdio>

typedef void * (* FunctionPointer)( int );

class A
{
public:
  static void * print( int argument )
  {
    fprintf( stderr, "Hi %i!\n", argument );
  }
};

void call( FunctionPointer function, int argument )
{
  function( argument );
}


int main() {
  FunctionPointer function = &A::print;
  function( 0 );
  function( 1 );
  call( function, 2 );
  call( function, 3 );
}
