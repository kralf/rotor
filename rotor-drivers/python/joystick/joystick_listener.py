#!/usr/bin/python

import rotorc
import sys

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

while True:
  message = registry.receiveMessage()
  print rotorc.marshall( message )
