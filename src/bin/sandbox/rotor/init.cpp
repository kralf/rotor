#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <stdint.h>

int main() {
  void * buffer = malloc( 10 );
  memset( buffer, 0, 10 );
  uint8_t * bytePointer = static_cast<uint8_t *>( buffer );
  bytePointer += 4;
  void * pointer = static_cast<void *>( bytePointer );
  size_t * vap = *( reinterpret_cast<size_t **>( pointer ) );
  if ( vap ) {
    printf( "Vap\n" );
  }
  free( buffer );
}
