from rotor.extensions import *

import pycommand
import sys
import signal
import os


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

class Server:
  def __init__( self, parameterFile ):
    self.options  = BaseOptions()
    f = open( parameterFile )
    self.options.fromString( f.read() )
    f.close()
    
    serverIp = self.options.getString( "rotor_server", "serverIp", rotorc.hostIp() )
    
    self.options.setInt( "rotor_server", "serverPort", 60709 )
    
    self.defaultRegistryClass = self.options.getString( "rotor_server", "registry" )
    self.options.setString( self.defaultRegistryClass, "serverIp", serverIp )

    signal.signal( signal.SIGTERM, self.shutdown )
    
    self.processes = []
    self.setupExternalServer()
    rotorc.Thread.sleep( 0.1 )
    self.setupBroadcastServer()
    self.setupDefaultServer()
    
#-------------------------------------------------------------------------------

  def shutdown( self, signal, frame  = None ):
    self.terminated = True
    for p in self.processes:
      p.kill()
    self.join()
    
#-------------------------------------------------------------------------------

  def setupExternalServer( self ):
    if self.defaultRegistryClass == "CarmenRegistry":
      Logger.info( "Starting central" )
      p = pycommand.Command( "central" )
      self.processes.append( p )
      p.start()

#-------------------------------------------------------------------------------

  def write_string( self, channel, stream ):
    while True:
      try:
        line = channel.readline()
        if not line:
          break
        if stream == "error":
          Logger.error( line[:-1] )
        else:
          Logger.info( line[:-1] )
      except Exception, e:
        print e.message

#-------------------------------------------------------------------------------

  def commandHandler( self, registry, name ):
    try:
      Logger.spam( "receiving: %s %d" % ( name, seconds() ) )
      message = registry.receiveQuery( 1 )
      if message.name() == "SERVER_COMMAND":
        data = message.data()
        Logger.spam( "%s>%s %s" % ( name, data.command, data.arguments ) )
        if data.command == "GET_OPTIONS":
          reply       = registry.newStructure( "OptionString" )
          if data.arguments == "*":
            reply.value = self.options.toString()
          else:
            reply.value = self.options.toString( data.arguments )
          registry.reply( Message( "OPTION_STRING", reply ) )
          Logger.spam( "%s" % reply.toString() )
    except MessagingTimeout, e:
      Logger.spam( "Message timed out" )
      pass
    
#-------------------------------------------------------------------------------

  def setupBroadcastServer( self ):
    self.broadcastRegistry = RemoteRegistry( "BroadcastRegistry", "rotor_server", self.options, "" )
    for typeDefinitions in types:
      self.broadcastRegistry.registerType( typeDefinitions )
    for message in messages:
      self.broadcastRegistry.registerMessage( message[0], message[1] )
    self.broadcastRegistry.subscribeToQuery( "SERVER_COMMAND" )
    self.terminated = False

#-------------------------------------------------------------------------------

  def setupDefaultServer( self ):
    self.defaultRegistry = RemoteRegistry( self.defaultRegistryClass, "rotor", self.options, "" )
    for typeDefinitions in types:
      self.defaultRegistry.registerType( typeDefinitions )
    for message in messages:
      self.defaultRegistry.registerMessage( message[0], message[1] )
    self.defaultRegistry.subscribeToQuery( "SERVER_COMMAND" )
    self.terminated = False

#-------------------------------------------------------------------------------

  def terminate( self ):
    self.terminated = True
    
#-------------------------------------------------------------------------------

  def join( self ):
    try:
      while not self.terminated:
        self.commandHandler( self.broadcastRegistry, "broadcast" )
        if self.terminated:
          break
        self.commandHandler( self.defaultRegistry, "default" )
      for p in self.processes:
        p.join()
    except KeyboardInterrupt:
      self.shutdown( 15 )
