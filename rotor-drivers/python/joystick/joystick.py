#!/usr/bin/python

import pygame.joystick
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

registry.registerMessageType( "JOYSTICK", joystickDefinition )
frequency = options.getDouble( name, "frequency", 10 )



joystickInfo = rotorc.Structure( "Joystick", None, registry )

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick( 0 )
joystick.init()
time = rotorc.seconds()

while True:
  delta   = rotorc.seconds()
  sys.stdout.write( "\r[%f]          " % ( rotorc.seconds() - time ) )
  sys.stdout.flush()
  time = delta
  pygame.event.pump()
  joystickInfo.axesCount = joystick.get_numaxes()
  for i in xrange( joystickInfo.axesCount ):
    joystickInfo.axes[i] = joystick.get_axis( i )
  joystickInfo.buttonCount = joystick.get_numbuttons()
  for i in xrange( joystickInfo.buttonCount ):
    joystickInfo.buttons[i] = joystick.get_button( i )
  registry.sendStructure( "JOYSTICK", joystickInfo )
  rotorc.Thread.sleep( 0.1 )
