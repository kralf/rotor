#include "Point.h"
#include <rotor/RemoteRegistry.h>
#include <rotor/Serialization.h>
#include <iostream>


ROTOR_DEFINE_TYPE( carmen_point_rt,
  struct carmen_point_rt {
    double x;
    double y;
    double theta;
  };
)

ROTOR_DEFINE_TYPE( carmen_laser_laser_config_rt,
  struct carmen_laser_laser_config_rt {
    int32_t laser_type;
    double start_angle;
    double fov;
    double angular_resolution;
    double maximum_range;
    double accuracy;
    int32_t remission_mode;
  };
)

ROTOR_DEFINE_TYPE( carmen_robot_laser_message_rt,
   struct carmen_robot_laser_message_rt {
    int32_t id;
    carmen_laser_laser_config_rt config;
    int32_t num_readings;
    float *range;
    int32_t num_remissions;
    float *remission;
    double timestamp;
    char *host;
   };
) 

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  RemoteRegistry registry( argv[1] );
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_rt ) );
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_laser_laser_config_rt ) );
  registry.registerMessageType( "carmen_laser_laser1", ROTOR_DEFINITION_STRING( carmen_robot_laser_message_rt ) );
  registry.subscribeToMessage( "carmen_laser_laser1", true );

  while ( true ) {
    Message m = registry.receiveMessage();
    cerr << m.data->toString() << endl;
    delete m.data;
  }
}