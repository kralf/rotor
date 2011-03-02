from rotor import *
from rotor import rotorc
from rotor.rotorc import *

from threading import *
from time import *
import sys

types = [
'''
struct RegistryRequest {
  char * moduleName;
};
''',
'''
struct RegistryResponse {
  uint8_t nameOk;
  char * options;
};
'''
]

messages = [
  ["REGISTRY_REQUEST", "RegistryRequest"],
  ["REGISTRY_RESPONSE", "RegistryResponse"]
]

class Client:
  def __init__( self, name ):
    self.name = name
    self.options  = BaseOptions() # TODO: Read from files
    self.connectToCentralServer()
    
  def connectToCentralServer( self ):
    try:
      registry = Registry.load( "BroadcastRegistry", self.name, self.options, "/home/vasquezg/dev/c++/robotics/rotor/build/lib" )
      for typeDefinitions in types:
        registry.registerType( typeDefinitions )
      for message in messages:
        registry.registerMessage( message[0], message[1] )
      request = Structure( "RegistryRequest", None, registry )
      request.moduleName = self.name
      response = registry.sendReceiveMessage( Message( "REGISTRY_REQUEST", request ) )
      if response.data.nameOk == 1:
        self.options.fromString( response.data.options )
        print self.options.toString()
        self.registry = Registry.load( self.options.getString( self.name, "registry" ), self.name, self.options )
    except OptionError, e:
      print type( e ).__dict__
      
  def join( self ):
    pass
