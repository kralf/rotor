#include "Point.h"
#include <rotor/RemoteRegistry.h>
#include <rotor/Serialization.h>
#include <iostream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  RemoteRegistry registry( argv[0] );
  registry.registerType( ROTOR_DEFINITION_STRING( elrob_point_6d_t ) );
  registry.subscribeToMessageType( "elrob_smart_pos_message", ROTOR_DEFINITION_STRING( elrob_smart_pos_message ) );

  while ( true ) {
    Message m = registry.receiveMessage();
    cout << m.data->toString() << endl;
//     delete m.data;
  }
}
