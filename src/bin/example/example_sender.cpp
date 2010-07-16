#include "example_types.h"

#include <rotor/Logger.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/Thread.h>
#include <rotor/Time.h>

using namespace std;
using namespace Rotor;

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  RemoteRegistry registry( "rotor_example_sender" );
  registry.registerType( ROTOR_DEFINITION_STRING( point_6d_t ) );
  registry.registerMessageType( "pos_message",
    ROTOR_DEFINITION_STRING( pos_message ) );

  // Second way
  pos_message p;
  p.host = "fotoinforsic";
  
  // Structure pose is an 'alias' of p
  Structure pose = registry.newStructure( "pos_message", &p ); 
  
  
  int value = 0;
  while ( true ) {
    Logger::spam( pose.toString() );
    Thread::sleep( 0.1 );
    value += 1;
    p.pose.x = value;
    p.pose.y = value * 2;
    p.velocity = value * 1;
    p.steering_angle = value * 2;
    p.timestamp = seconds();
    Logger::debug( pose.toString() );
    registry.sendStructure( "pos_message", pose );
  }
}
