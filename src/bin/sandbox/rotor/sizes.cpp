#include <stdint.h>
#include <cstdio>


struct Test1 {
  int8_t c1;
  int8_t c2[10];
};

struct Test2 {
  int8_t c1;
  int8_t * c2;
};

struct Test3 {
  int8_t c1;
  int16_t c2;
};

struct Test4 {
  int8_t c1;
  int16_t c2;
  int32_t c3;
};

struct Test5 {
  int8_t c1;
  int16_t c2;
  int8_t c3;
};

struct Test6 {
  int8_t c1;
  int16_t c2;
  int8_t c3;
  int32_t c4;
};

struct Test7 {
  int8_t c1;
  int8_t c2;
  int8_t c3;
};

struct Test8 {
  int16_t c1;
  int8_t c2;
};

struct Test9 {
  int16_t c1;
  int8_t c2;
  int8_t a1;
  int8_t a2;
};

struct Test10 {
  int8_t c2;
  int8_t a1;
  int16_t c1;
  int8_t a2;
};

struct Test11 {
  int32_t c1;
  int8_t c2;
  int8_t a1;
  int8_t a2;
};

struct Test12 {
  int8_t c2;
  int8_t a1;
  int32_t c1;
  int8_t a2;
};

int main() 
{
  printf( "Size Test1 (11): %i\n", sizeof( Test1 ) );
  printf( "Size Test2 (8) : %i\n", sizeof( Test2 ) );
  printf( "Size Test3 (4) : %i\n", sizeof( Test3 ) );
  printf( "Size Test4 (12): %i\n", sizeof( Test4 ) );
  printf( "Size Test5 (6) : %i\n", sizeof( Test5 ) );
  printf( "Size Test6 (16): %i\n", sizeof( Test6 ) );
  printf( "Size Test7 (3): %i\n", sizeof( Test7 ) );
  printf( "Size Test8 (3): %i\n", sizeof( Test8 ) );
  printf( "Size Test7[2] (6): %i\n", sizeof( Test7[2] ) );
  printf( "Size Test8[2] (8): %i\n", sizeof( Test8[2] ) );
  printf( "Size Test9 (6): %i\n", sizeof( Test9 ) );
  printf( "Size Test10 (5): %i\n", sizeof( Test10 ) );
  printf( "Size Test11 (8): %i\n", sizeof( Test11 ) );
  printf( "Size Test12 (12): %i\n", sizeof( Test12 ) );
/*  
  Test2 t2;
  t2.c1 = 6;
  t2.c2 = (int8_t *)   12;
  printf( "t2.c1: %i\n", t2.c1 );
  for ( int i = 0; i < 5; i++ ) {
    printf( "t2 + %i: %u\n",  i, *( reinterpret_cast<int8_t*>( &t2 ) + i ) );
  }*/
}
