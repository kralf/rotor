#!/usr/bin/python

import rotorc
import time
import sys

baseVelocityDefinition = '''
  struct Command {
    double tv;
    double rv;
    double timestamp;
    char *host;
  };
'''

joystickDefinition = '''
  struct Joystick {
    uint8_t   axesCount;
    float   * axes;
    uint8_t   buttonCount;
    uint8_t * buttons;
  };
'''

name = sys.argv[1]
options  = rotorc.BaseOptions()
options.setString( "BOOTSTRAP", "server", "172.16.0.10" )
#options.setString( "BOOTSTRAP", "server", "localhost" )
registry = rotorc.Registry.load( "CarmenRegistry", name, options, "/usr/lib" )

registry.registerType( joystickDefinition )
registry.registerMessage( "JOYSTICK", "Joystick" )
registry.subscribeToMessage( "JOYSTICK" )

registry.registerMessageType( "carmen_base_velocity", baseVelocityDefinition )
command = rotorc.Structure( "Command", None, registry )

oldTime = 0

while True:
  message = registry.receiveMessage()
  time    = rotorc.seconds()
  joystick = message.data
  command.tv = -joystick.axes[1] * 0.4;
  command.rv = -joystick.axes[0] * 1;
  command.timestamp = rotorc.microseconds();
  command.host  = "ventisca"
  registry.sendStructure( "carmen_base_velocity", command )
  sys.stdout.write( "\r%f\t%f \t%f" % ( command.tv, command.rv, time - oldTime )  )
  sys.stdout.flush()
  oldTime = time 
