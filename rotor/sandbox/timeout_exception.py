import rotorc


elrob_point_6d_t = '''
  struct elrob_point_6d_t {
    double x;
    double y;
    double z;
    double phi;
    double theta;
    double psi;
  };
'''

elrob_smart_pos_message = '''
  struct elrob_smart_pos_message {
    elrob_point_6d_t pose;
    double velocity;
    double steering_angle;
    double timestamp;
    char * host;
  };
'''

elrob_smart_steering_message = '''
  struct elrob_smart_steering_message {
    double velocity;
    double steeringangle;
    double timestamp;
    char* host;
  };
'''

rotor_registry  = rotorc.RemoteRegistry( "morsel-rotor" )
options         = rotor_registry.options()
rotor_registry.registerType( elrob_point_6d_t )
rotor_registry.registerType( elrob_smart_pos_message )
rotor_registry.registerType( elrob_smart_steering_message )

while True:
  try:
    message = rotor_registry.receiveMessage( 0.001 )
    if message.name == "elrob_smart_steering_message":
      v = [ message.data.velocity, message.data.steeringangle * 180 / pi ]      
      print v
  except rotorc.MessagingTimeout:
    pass
