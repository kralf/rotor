import cmd

#-------------------------------------------------------------------------------

class ShellFrontend( cmd.Cmd ):
  def __init__( self ):
    cmd.Cmd.__init__( self )
    self.commands = []
    self.local = True
    self.host  = "local"
    
  #-----
    
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
    
  #-----
    
  def do_exec( self, command_string ):
    command = None
    if self.local:
      command       = Command( command_string )
    else:
      command       = Command( command_string, self.host, self.username, self.password )
    self.commands.append( command )
    command.start()
    print "ok"
      
  #-----
    
  def do_list( self, args ):
    print "-" * 80
    print "No.\tRunning\tCommand"
    print "-" * 80
    for i in xrange( len( self.commands ) ):
      c = self.commands[i]
      print i, "\t", c.running, "\t", c.command
      
  #-----
    
  def do_out( self, args ):
    print "-" * 80
    for line in self.commands[int( args )].output[1]:
      print line[:-1]
    print "-" * 80

  #-----
    
  def do_err( self, args ):
    '''
      err command
    '''
    print "-" * 80
    for line in self.commands[int( args )].stderr:
      print line[:-1]
    print "-" * 80

  #-----
    
  def do_exit( self, args ):
    for c in self.commands:
      c.join()
    exit()
