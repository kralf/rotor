#include <rotor/Rotor.h>
#include <rotor/BaseRegistry.h>
#include <rotor/Structure.h>
#include <rotor/BaseOptions.h>


using namespace Rotor;

//------------------------------------------------------------------------------

ROTOR_DEFINE_TYPE( axt_message, 
  struct axt_message {
    uint32_t num_points;
    uint32_t version;
    uint32_t scanner_type;
    uint32_t ecu_id;
    uint32_t timestamp_sensor;
    double start_angle;
    double end_angle;
    uint32_t scan_counter;

    int32_t * scanner_id;
    int32_t * channel;
    int32_t * point_status;
    
    float *x;
    float *y;
    float *z;
    
    uint32_t *echo_pulse_width;

    double timestamp;
  };
)

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  BaseOptions options;
  options.setInt( "BOOTSTRAP", "loggingLevel", 3 );
  
  BaseRegistry registry( "test", options );
  registry.registerType( ROTOR_DEFINITION_STRING( axt_message ) );

  Structure structure = Structure( "axt_message", 0, registry );
  structure["scan_counter"] = 1000;
  for ( size_t i = 0; i < 1000; ++i ) {
    structure["x"][i] = i * 1.0;
    structure["y"][i] = i * 1.0;
    structure["z"][i] = i * 1.0;
  }
}
