import paramiko
import subprocess
import threading
import os
from   Queue import *
import time


#-------------------------------------------------------------------------------

class Command( object ):
  def __init__( self, command, host = None, username = None, password = None, output_lines = 300 ):
    object.__init__( self )
    self.host         = host
    self.remote       = host
    self.username     = username
    self.password     = password
    self.command      = command
    self.running      = False
    self.pid          = None
    self.return_code  = 0
    self.out          = Queue()
    self.err          = Queue()
    self._output      = []
    self.output_lines = output_lines
        
  #-----
  
  def check_running( self, stream ):
    self.running = False
    if not self.remote:
      poll = self.popen.poll()
      if type( poll ) == int:
        self.return_code = poll
    else:
      if stream.channel.exit_status_ready():
        self.return_code = stream.channel.exit_status
        self.ssh.close()
  
  #-----
  
  def update( self ):
    while True:
      done = True
      if not self.out.empty():
        line = self.out.get()
        self._output.append( ( 0, time.time(), line ) )
        done = False
      if not self.err.empty():
        line = self.err.get()
        self._output.append( ( 1, time.time(), line ) )
        done = False
      if done:
          break
    if self.output_lines != None:
      self._output = self._output[-self.output_lines:]
  
  #-----
  
  def getOutput( self ):
    self.update()
    return self._output
    
  output = property( getOutput )
    
  #-----
  
  def append_string( self, stream, queue ):
    while True:
      try:
        line = stream.readline()
        if not line or line == "":
          break
        queue.put( line )
      except Exception, e:
        print e.message
    self.check_running( stream )
    
  #-----
  
  def start( self ):
    if self.running:
      return
    self._output = []
    if not self.remote:
      self.popen = subprocess.Popen( 
        self.command.split(), 
        shell  = False, 
        stdout = subprocess.PIPE, 
        stdin  = subprocess.PIPE, 
        stderr = subprocess.PIPE )
      self.pid     = self.popen.pid
      self.running = True
      self.thread1 = threading.Thread( target = self.append_string, args = ( self.popen.stdout, self.out ) )    
      self.thread2 = threading.Thread( target = self.append_string, args = ( self.popen.stderr, self.err ) )
      self.thread1.start()
      self.thread2.start()
    else:
      try:
        self.ssh = paramiko.SSHClient()
        self.ssh.set_missing_host_key_policy( paramiko.AutoAddPolicy() )
        if self.username:
          self.ssh.connect( self.host, username = self.username, password = self.password )
        else:
          self.ssh.connect( self.host )
        self.sin, self.sout, self.serr    = self.ssh.exec_command( self.command )
        self.running = True
        self.thread1 = threading.Thread( target = self.append_string, args = ( self.sout, self.out ) )    
        self.thread2 = threading.Thread( target = self.append_string, args = ( self.serr, self.err ) )
        self.thread1.start()
        self.thread2.start()
      except Exception, e:
        print type( e ), e, e.message
        exit( 1 )
  #-----
  
  def join( self ):
    if self.running:
      self.thread1.join()
      self.thread2.join()
    self.stop()
  
  #-----
  
  def stop( self ):
    if self.running:
      if not self.remote:
        os.kill( self.pid, 15 )
      else:
        self.ssh.close()
      self.running = False
  
  #-----
  
  def restart( self ):
    self.stop()
    self.start()
  
  #-----
  
  def clear( self ):
    self._output = []
  
  #-----
  
  def kill( self ):
    if self.running:
      if not self.remote:
        os.kill( self.pid, 9 )
      else:
        self.stop()
      self.running = False