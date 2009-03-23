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

velocity = 0
oldTime = rotorc.seconds()

while True:
  message = registry.receiveMessage()
  joystick = message.data
  command.velocity      = -joystick.axes[1] * options.getDouble( robotName, "maxSpeed" )
  command.steeringangle = -joystick.axes[0] * options.getDouble( robotName, "maxSteeringAngle" )
  command.timestamp     = rotorc.microseconds();
  command.host          = "ventisca"

  time    = rotorc.seconds()
  deltaT  = time - oldTime
  deltaV  = command.velocity - velocity
  acceleration    = deltaV / deltaT
  maxAcceleration = options.getDouble( robotName, "maxAcceleration" )
  maxDeceleration = options.getDouble( robotName, "maxDeceleration" )
  if velocity == 0:
    if acceleration > maxAcceleration:
      rotorc.Logger.debug( "Starting forward" )
      command.velocity = maxAcceleration * deltaT
    elif acceleration < -maxAcceleration:
      rotorc.Logger.debug( "Starting backwards" )
      command.velocity = -maxAcceleration * deltaT
  elif velocity > 0:
    if acceleration > maxAcceleration:
      rotorc.Logger.debug( "Accelerating forward" )
      command.velocity = velocity + maxAcceleration * deltaT
    elif acceleration < -maxDeceleration:
      rotorc.Logger.debug( "Braking forward" )
      command.velocity = velocity - maxDeceleration * deltaT
  elif velocity < 0:
    if acceleration < -maxAcceleration:
      rotorc.Logger.debug( "Accelerating backwards" )
      command.velocity = velocity - maxAcceleration * deltaT
    elif acceleration > maxDeceleration:
      rotorc.Logger.debug( "Braking backwards" )
      command.velocity = velocity + maxDeceleration * deltaT
      
  velocity = command.velocity


  registry.sendStructure( "elrob_smart_steering_message", command )
  oldTime = time 
