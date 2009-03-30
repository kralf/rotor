from rotorc import *
import rotorc
import subprocess
import threading
import sys
import socket
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

def getHostName():
  s = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
  try:
    s.connect( ( '1.2.3.4', 56 ) )
    return s.getsockname()[0]
  except socket.error:
    return "127.0.0.1"

#-------------------------------------------------------------------------------

class Server:
  def __init__( self, parameterFile ):
    self.options  = BaseOptions()
    f = open( parameterFile )
    self.options.fromString( f.read() )
    f.close()
    
    server = self.options.getString( "BOOTSTRAP", "server", "" )
    if server == "":
      server = getHostName()
    
    self.options.setString( "BOOTSTRAP", "server", server )
    self.options.setInt( "rotor", "listenPort", 60709 )
    out( self.options.toString() )
    
    self.defaultRegistryClass = self.options.getString( "BOOTSTRAP", "registry" )
    self.options.setString( self.defaultRegistryClass, "server", server )

    signal.signal( signal.SIGTERM, self.shutdown )
    
    self.processes = []
    self.setupExternalServer()
    self.setupBroadcastServer()
    self.setupDefaultServer()
    
#-------------------------------------------------------------------------------

  def shutdown( self, signal, frame  = None ):
    self.terminated = True
    for p in self.processes:
      os.kill( p[0].pid, 15 )
    self.join()
    
#-------------------------------------------------------------------------------

  def setupExternalServer( self ):
    if self.defaultRegistryClass == "CarmenRegistry":
      Logger.info( "Starting central" )
      self.startProcess( "central" )

#-------------------------------------------------------------------------------

  def startProcess( self, command ):
    popen = subprocess.Popen( 
      command.split(), 
      shell  = False, 
      stdout = subprocess.PIPE, 
      stdin  = subprocess.PIPE, 
      stderr = subprocess.PIPE )
    pid     = popen.pid
    thread1 = threading.Thread( target = self.write_string, args = ( popen.stdout, "out" ) )    
    thread2 = threading.Thread( target = self.write_string, args = ( popen.stderr, "error" ) )
    thread1.start()
    thread2.start()
    self.processes.append( ( popen, thread1, thread2 ) )
    
#-------------------------------------------------------------------------------

  def write_string( self, channel, stream ):
    while True:
      try:
        line = channel.readline()
        if not line or line == "":
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
    
#-------------------------------------------------------------------------------

  def setupBroadcastServer( self ):
    self.broadcastRegistry = Registry.load( "BroadcastRegistry", "rotor", self.options, "" )
    for typeDefinitions in types:
      self.broadcastRegistry.registerType( typeDefinitions )
    for message in messages:
      self.broadcastRegistry.registerMessage( message[0], message[1] )
    self.broadcastRegistry.subscribeToQuery( "SERVER_COMMAND" )
    self.terminated = False

#-------------------------------------------------------------------------------

  def setupDefaultServer( self ):
    self.defaultRegistry = Registry.load( self.defaultRegistryClass, "rotor", self.options, "" )
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
        p[1].join()
        p[2].join()
    except KeyboardInterrupt:
      self.shutdown( 15 )
