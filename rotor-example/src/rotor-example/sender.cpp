#include "Point.h"
#include <rotor/Logger.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/Thread.h>
#include <rotor/Time.h>

using namespace std;
using namespace Rotor;

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  RemoteRegistry registry( argv[0] );
  registry.registerType( ROTOR_DEFINITION_STRING( elrob_point_6d_t ) );
  registry.registerMessageType( "elrob_smart_pos_message", ROTOR_DEFINITION_STRING( elrob_smart_pos_message ) );

  {
    // First way
    Structure pose = registry.newStructure( "elrob_smart_pos_message" );
    pose["pose"]["x"] = 12;
    pose["pose"]["y"] = 12;
    pose["host"] = "fotoinforsic";
    registry.sendStructure( "elrob_smart_pos_message", pose );
  }
  {
    // Second way
    elrob_smart_pos_message p;
    p.host = "fotoinforsic";
    
    // Structure pose is an 'alias' of p
    Structure pose = registry.newStructure( "elrob_smart_pos_message", &p ); 
    
    
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
      registry.sendStructure( "elrob_smart_pos_message", pose );
    }
  }
}
 