from shell import *
import time

class Command:

  #-----------------------------------------------------------------------------
  
  def __init__( self, command, handler = None, host = None, username = None ):
    self.command      = command
    self.host         = host
    self.username     = username
    self.commandShell = Shell( host, username )
    self.statusShell  = Shell( host, username )
    self.handler      = handler
    self.statusInfo   = {}

  #-----------------------------------------------------------------------------
  
  def __del__( self ):
    self.kill()
  
  #-----------------------------------------------------------------------------
  
  def handlerProxy( self, source, line  ):
    if self.handler != None:
      self.handler( self, source, line )
      
  #-----------------------------------------------------------------------------
  
  def statusHandler( self, source, line  ):
    if source == "stderr":
      print "Error:", line
    else:
      parts = line.split( "=" )
      self.statusInfo[parts[0]] = parts[1]
    
  #-----------------------------------------------------------------------------
  
  def getStatusVariable( self, name, key ):
    while self.commandShell.pid == False and self.commandShell.running:
      time.sleep( 0.1 )
    if self.commandShell.pid != False and self.commandShell.running:
      self.statusShell.execute( 
        "echo %s=`cat /proc/%i/status | grep \"%s\" | sed -e \"s/^%s:[ \t]*\(.*\)/\\1/g\"`" 
        % ( name, self.commandShell.pid, key, key ),
        self.statusHandler
      )
      self.statusShell.join()
      if self.statusShell.exitCode == 0:
        return self.statusInfo[name]
      else:
        return None
    return None
  
  #-----------------------------------------------------------------------------
  
  def getPsVariable( self, name, key ):
    while self.commandShell.pid == False and self.commandShell.running:
      time.sleep( 0.1 )
    if self.commandShell.pid != False and self.commandShell.running:
      self.statusShell.execute( 
        "echo %s=`ps u %i| grep %i`" 
        % ( name, self.commandShell.pid, self.commandShell.pid ),
        self.statusHandler
      )
      self.statusShell.join()
      if self.statusShell.exitCode == 0:
        self.statusInfo[name] = self.statusInfo[name].split()[key]
        return self.statusInfo[name]
      else:
        return None
    return None
  
  #-----------------------------------------------------------------------------
  
  def memory( self ):
    result = self.getPsVariable( "memory", 3 )
    if result != None:
      return float( result ) / 100
    return 0
    
  #-----------------------------------------------------------------------------
  
  def status( self ):
    result = self.getPsVariable( "status", 7)
    if result != None:
      return result
    return "?"
    
  #-----------------------------------------------------------------------------
  
  def cpu( self ):
    result = self.getPsVariable( "cpu", 2 )
    if result != None:
      return float( result ) / 100
    return 0
    
  #-----------------------------------------------------------------------------
  
  def getRunning( self):
    return self.commandShell.running
    
  running = property( getRunning )
    
  #-----------------------------------------------------------------------------
  
  def getExitCode( self):
    return self.commandShell.exitCode
    
  exitCode = property( getExitCode )
    
  #-----------------------------------------------------------------------------
  
  def getPid( self):
    return self.commandShell.pid
    
  pid = property( getPid )
    
  #-----------------------------------------------------------------------------
  
  def start( self ):
    self.commandShell.execute( self.command, self.handlerProxy )
    
  #-----------------------------------------------------------------------------
  
  def join( self ):
    while self.commandShell.running:
      time.sleep( 0.5 )
      
  #-----------------------------------------------------------------------------
  
  def kill( self ):
    if self.commandShell.running and self.commandShell.pid != False:
      killshell = Shell( self.host, self.username )
      killshell.execute( "kill -9 %i" % self.commandShell.pid )
      while killshell.running:
        time.sleep( 0.5 )

  #-----------------------------------------------------------------------------
  
  def stop( self ):
    if self.commandShell.running and self.commandShell.pid != False:
      killshell = Shell( self.host, self.username )
      killshell.execute( "kill -15 %i" % self.commandShell.pid )
      while killshell.running:
        time.sleep( 0.5 )
