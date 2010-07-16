#include "example_types.h"

#include <rotor/RemoteRegistry.h>
#include <rotor/Serialization.h>
#include <iostream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  RemoteRegistry registry( "rotor_example_receiver" );
  registry.registerType( ROTOR_DEFINITION_STRING( point_6d_t ) );
  registry.registerMessageType( "pos_message",
    ROTOR_DEFINITION_STRING( pos_message ) );
  registry.subscribeToMessage( "pos_message", true );

  while ( true ) {
    Message m = registry.receiveMessage();
    cout << m.data().toString() << endl;
  }
}
