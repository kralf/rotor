from rotorc import *
import rotorc
from threading import *
from time import *
import sys
import socket

types = [
  '''
    struct RemoteCommand {
      char * command;
      char * arguments;
    };
  ''',
  '''
    struct OptionString {
      char * value;
    };
  '''
]

messages = [
  ["SERVER_COMMAND", "RemoteCommand"],
  ["OPTION_STRING", "OptionString"]
]

#-------------------------------------------------------------------------------

def out( string ):
  sys.stdout.write( string )
  sys.stdout.flush()

#-------------------------------------------------------------------------------

def getHostName():
  s = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
  s.connect( ( '1.2.3.4', 56 ) )
  ip = s.getsockname()[0]
  return ip

#-------------------------------------------------------------------------------

class Server:
  def __init__( self, parameterFile ):
    self.options  = BaseOptions()
    
    f = open( parameterFile )
    self.options.fromString( f.read() )
    f.close()
    
    self.options.setString( "BOOTSTRAP", "server", getHostName() )
    self.options.setInt( "rotor", "listenPort", 60709 )
    
    self.defaultRegistryClass = self.options.getString( "BOOTSTRAP", "registry" )
    self.options.setString( self.defaultRegistryClass, "server", getHostName() )
    sys.stdout.flush()

    self.setupBroadcastServer()
    self.setupDefaultServer()
    
#-------------------------------------------------------------------------------

  def commandHandler( self, registry, name ):
    while not self.terminated:
      try:
        Logger.spam( "receiving: %s" % name )
        message = registry.receiveQuery( 0.2 )
        if message.name == "SERVER_COMMAND":
          Logger.info( "%s>%s %s" % ( name, message.data.command, message.data.arguments ) )
          if message.data.command == "GET_OPTIONS":
            reply       = Structure( "OptionString", None, registry )
            if message.data.arguments == "*":
              reply.value = self.options.toString()
            else:
              reply.value = self.options.toString( message.data.arguments )
            registry.reply( Message( "OPTION_STRING", reply ) )
            Logger.debug( "%s" % reply.toString() )
      except Exception, e:
        if e.message != "No message was received":
          print e.message
          raise
    print "Handler has been terminated"
    
#-------------------------------------------------------------------------------

  def setupBroadcastServer( self ):
    self.broadcastRegistry = Registry.load( "BroadcastRegistry", "rotor", self.options, "" )
    for typeDefinitions in types:
      self.broadcastRegistry.registerType( typeDefinitions )
    for message in messages:
      self.broadcastRegistry.registerMessage( message[0], message[1] )
    self.broadcastRegistry.subscribeToQuery( "SERVER_COMMAND" )
    self.terminated = False
    self.broadcastThread = Thread( target = self.commandHandler, args = ( self.broadcastRegistry, "broadcast" ) ) 
    self.broadcastThread.start()

#-------------------------------------------------------------------------------

  def setupDefaultServer( self ):
    self.defaultRegistry = Registry.load( self.defaultRegistryClass, "rotor", self.options, "" )
    for typeDefinitions in types:
      self.defaultRegistry.registerType( typeDefinitions )
    for message in messages:
      self.defaultRegistry.registerMessage( message[0], message[1] )
    self.defaultRegistry.subscribeToQuery( "SERVER_COMMAND" )
    self.terminated = False
    self.defaultThread = Thread( target = self.commandHandler, args = ( self.defaultRegistry, "default" ) ) 
    self.defaultThread.start()

#-------------------------------------------------------------------------------

  def terminate( self ):
    self.terminated = True
    
#-------------------------------------------------------------------------------

  def join( self ):
    try:
      while True:
        if self.broadcastThread.isAlive:
          self.broadcastThread.join( 1 )
        elif self.defaultThread.isAlive:
          self.defaultThread.join( 1 )
        else:
          break
    except KeyboardInterrupt:
      self.terminate()
        