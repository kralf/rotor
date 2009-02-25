import paramiko
import subprocess
import threading
import os
from   Queue import *
import cmd
import snack

class Command( object ):
  def __init__( self, command, host = None, username = None, password = None ):
    object.__init__( self )
    self.host        = host
    self.remote      = host
    self.username    = username
    self.password    = password
    self.command     = command
    self.running     = False
    self.pid         = None
    self.return_code = None
    self.out         = Queue()
    self.err         = Queue()
    self._stdout     = []
    self._stderr     = []
        
  #-----
  
  def check_running( self, stream ):
    if not self.running:
      return False
    if not self.remote:
      poll = self.popen.poll()
      if poll != -1 and poll != None:
        self.running = False
        self.return_code = self.popen.returncode
    else:
      if stream.channel.exit_status_ready():
        self.running     = False
        self.return_code = stream.channel.exit_status()
  
  #-----
  
  def update( self ):
    while not self.out.empty():
      line = self.out.get()
      self._stdout.append( line )
    while not self.err.empty():
      line = self.err.get()
      self._stderr.append( line )
  
  #-----
  
  def getStdOut( self ):
    self.update()
    return self._stdout
    
  stdout = property( getStdOut )
    
  #-----
  
  def getStdErr( self ):
    self.update()
    return self._stderr

  stderr = property( getStdErr )

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
    if not self.host:
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
    self.close()
  
  #-----
  
  def close( self ):
    if self.remote:
      self.ssh.close()

  #-----
  
  def stop( self ):
    if self.running:
      self.running = False
      os.kill( self.pid, 15 )
  
  #-----
  
  def restart( self ):
    self.stop()
    self.start()
  
  #-----
  
  def kill( self ):
    if self.running:
      self.running = False
      os.kill( self.pid, 9 )
      
class Application( cmd.Cmd ):
  def __init__( self ):
    cmd.Cmd.__init__( self )
    self.commands = []
    self.local = True
    self.host  = "local"
    
    
  def do_host( self, info ):
    if info == "":
      print "Host set to %s" % self.host
    elif info == "local" or info == "localhost":
      self.local = True
      self.host  = "local"
      print "Set host to local"
    else:
      info = info.split()
      self.local = False
      self.host     = info[0]
      self.username = info[1]
      self.password = info[2]
      print "Set host to %s" % self.host
    
  def do_exec( self, command_string ):
    command = None
    if self.local:
      command       = Command( command_string )
    else:
      command       = Command( command_string, self.host, self.username, self.password )
    self.commands.append( command )
    command.start()
    print "ok"
      
  def do_list( self, args ):
    print "-" * 80
    print "No.\tRunning\tCommand"
    print "-" * 80
    for i in xrange( len( self.commands ) ):
      c = self.commands[i]
      print i, "\t", c.running, "\t", c.command
      
  def do_out( self, args ):
    print "-" * 80
    for line in self.commands[int( args )].stdout:
      print line[:-1]
    print "-" * 80

  def do_err( self, args ):
    '''
      err command
    '''
    print "-" * 80
    for line in self.commands[int( args )].stderr:
      print line[:-1]
    print "-" * 80

  def do_exit( self, args ):
    for c in self.commands:
      c.join()
    exit()



class TextGui:
  def __init__( self ):
    self.screen = snack.SnackScreen()
    self.initWindow1()
    
  def initWindow1( self ):
    self.window1 = snack.EntryWindow(
      self.screen, 
      'Host Info', 
      'Remote host data',
      ['Name', 'user', 'password*'] )
    
  def __del__( self ):
    self.screen.finish()
    print self.window1




gui = TextGui()

#application = Application()
#application.cmdloop()
      



##command = Command( "ls %s/Desktop" % os.environ["HOME"], host = "192.168.0.173", username = "vasquezg", password = "watusi" )
##command = Command( "DISPLAY=:0.0 firefox", host = "192.168.0.173", username = "vasquezg", password = "watusi" )
##command = Command( "ls %s/Desktop" % os.environ["HOME"] )
#command.start()
#command.join()
##command.restart()
##command.join()
#for line in command.stdout:
  #print line[:-1]
#for line in command.stderr:
  #print line[:-1]



  
