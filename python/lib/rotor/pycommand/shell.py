import time
import subprocess
import threading
import select
import re
import sys
import exceptions
import Queue 

#-------------------------------------------------------------------------------

class Shell( object ):
  '''A class implementing asynchronous communications with a bash shell.'''
  def __init__( self, host = None, username = None, debug = False ):
    command = "bash -l"
    if host != None:
      if username != None:
        command = "ssh -X %s@%s" % ( username, host )
      else:
        command = "ssh -X %s" % ( host )
    self.popen = subprocess.Popen( 
      command,
      shell  = True, 
      stdout = subprocess.PIPE, 
      stdin  = subprocess.PIPE, 
      stderr = subprocess.PIPE )
    self.queue = Queue.Queue()
    self.thread1 = threading.Thread( target = self.enqueueOutput, args = ( self.popen.stdout, self.queue, "stdout" ) )    
    self.thread2 = threading.Thread( target = self.enqueueOutput, args = ( self.popen.stderr, self.queue, "stderr" ) )
    self.thread1.setDaemon( True )
    self.thread2.setDaemon( True )
    self.thread1.start()
    self.thread2.start()
    
    self.lock = threading.Lock()
    self._running  = False
    self._exitCode = False
    self._pid      = 0
    self._debug    = False
      
  #-----------------------------------------------------------------------------

  def __del__( self ):
    if self.popen.poll() == -1:
      self.popen.stdin.write( "exit\n" )
        
  #-----------------------------------------------------------------------------

  def getExitCode( self ):
    self.lock.acquire()
    result = self._exitCode
    self.lock.release()
    return result
    
  #-----------------------------------------------------------------------------

  def setExitCode( self, value ):
    self.lock.acquire()
    self._exitCode = value
    self.lock.release()
    
  exitCode = property( getExitCode, setExitCode )
  
  #-----------------------------------------------------------------------------

  def getPid( self ):
    self.lock.acquire()
    result = self._pid
    self.lock.release()
    return result
    
  #-----------------------------------------------------------------------------

  def setPid( self, value ):
    self.lock.acquire()
    self._pid = value
    self.lock.release()
    
  pid = property( getPid, setPid )

  #-----------------------------------------------------------------------------

  def getRunning( self ):
    self.lock.acquire()
    result = self._running
    self.lock.release()
    return result
    
  #-----------------------------------------------------------------------------

  def setRunning( self, value ):
    self.lock.acquire()
    self._running = value
    self.lock.release()
    
  running = property( getRunning, setRunning )

  #-----------------------------------------------------------------------------

  def enqueueOutput( self, stream, queue, source ):
    '''Thread handler that transfer stream contents to a queue'''
    while True:
      try:
        select.select( [stream], [], [] )
        line = stream.readline()
        if not line:
          break
        if len( line ) > 0:
          queue.put( ( source, line[:-1] ) )
      except KeyboardInterrupt:
        print "Interrupt in enqueueOutput"
        thread.interrupt_main()
      
  #-----------------------------------------------------------------------------

  def set( self, name, value ):
    '''Sets the value of environment variable'''
    self.execute( "export %s=%s" % ( name, value ) )

  #-----------------------------------------------------------------------------

  def get( self, name ):
    '''Gets the value of an environment variable'''
    output, errors, exit_code = self.execute( "echo $%s" % name )
    return output[:-1]

  #-----------------------------------------------------------------------------
  
  def send( self, line ):
    if self._debug:
      print "-> %s" % line.strip()
    self.popen.stdin.write( line )
  
  #-----------------------------------------------------------------------------
  
  def executionTask( self, command, handler = None ):
    self.send( "%s & export __PID=$!\n" % command )
    self.send( "echo __COMMAND_STARTED_$__PID\n" )
    self.send( "wait $__PID\n" )
    self.send( "echo __COMMAND_FINISHED_$?\n\n\n" )
    commandStarted  = re.compile( "__COMMAND_STARTED_(.+)" )
    commandFinished = re.compile( "__COMMAND_FINISHED_(.+)" )
    count          = 0
    self.exitCode  = False
    self.pid       = False
    match          = None
    
    while self.running:
      try:
        if count > 1:
          try:
            source, line = self.queue.get( True, 0.2 )
          except Queue.Empty:
            break
        else:
          source, line = self.queue.get()
        match = commandStarted.match( line )
        if match:
          if self._debug:
            print "S: %s" % line.strip()
          self.pid = int( match.group( 1 ) )
          match = None
          count += 1
        else:
          match = commandFinished.match( line )
          if not match:
            if handler:
              handler( source, line )
          else:
            if self._debug:
              print "E: %s" % line.strip()
            self.exitCode = int( match.group( 1 ) )
            count += 1
      except KeyboardInterrupt:
        print "Interrupt in executionTask"
        thread.interrupt_main()
    self.running = False
    
    
  #-----------------------------------------------------------------------------
  
  def execute( self, command, handler = None, wait = False ):
    '''Executes a command.
    output_handler and error_handler are functions that handle a text line at a
    time. Returns two strings containing the output and the error messages, 
    respectively.'''
    
    if self.running:
      return False
    else:
      self.running = True
      self.executionThread = threading.Thread( 
        target = self.executionTask, 
        args   = ( command, handler ) 
      )
      self.executionThread.setDaemon( True )
      self.executionThread.start()
    if wait:
      self.executionThread.join()
    return True

  #-----------------------------------------------------------------------------
  
  def join( self, timeout = None ):
    if self.running:
      self.executionThread.join( timeout )
      
  #-----------------------------------------------------------------------------
  
  def kill( self ):
    if self.running:
      self.running = False
    while self.executionThread.isAlive():
      time.sleep( 0.1 )

