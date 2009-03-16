#!/usr/bin/python

import rotorc
import time
import sys
from math import *


steeringDefinition = '''
  struct elrob_smart_steering_message {
    double velocity;
    double steeringangle;
    double timestamp;
    char* host;
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

name      = sys.argv[1]
registry  = rotorc.RemoteRegistry( name )
options   = registry.options()
robotName = options.getString( name, "robot" )

registry.registerType( joystickDefinition )
registry.registerMessage( "JOYSTICK", "Joystick" )
registry.subscribeToMessage( "JOYSTICK" )

registry.registerMessageType( "elrob_smart_steering_message", steeringDefinition )
command = rotorc.Structure( "elrob_smart_steering_message", None, registry )

oldTime = 0

while True:
  message = registry.receiveMessage()
  time    = rotorc.seconds()
  joystick = message.data
  command.velocity      = -joystick.axes[1] * options.getDouble( robotName, "maxSpeed" )
  command.steeringangle = -joystick.axes[0] * options.getDouble( robotName, "maxSteeringAngle" )
  command.timestamp     = rotorc.microseconds();
  command.host          = "ventisca"
  registry.sendStructure( "elrob_smart_steering_message", command )
  sys.stdout.write( "\r%f\t%f \t%f" % ( command.velocity, command.steeringangle, time - oldTime )  )
  sys.stdout.flush()
  oldTime = time 
