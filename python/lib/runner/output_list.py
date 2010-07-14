from selectable_text import *

import urwid
import time
import os

#-------------------------------------------------------------------------------

class OutputList( urwid.WidgetWrap ):
  def __init__( self, command, commandOutputs, lock ):
    self.command        = command
    self.commandOutputs = commandOutputs
    self.lock           = lock
    self.walker         = urwid.SimpleListWalker( [] )
    list                = urwid.ListBox( self.walker )
    self.lastTime       = time.time()
    urwid.WidgetWrap.__init__( self, list )
    
  #-----------------------------------------------------------------------------

  def update( self ):
    last = None
    self.lock.acquire()
    if len( self.commandOutputs[self.command] ) > 0:
      last = self.commandOutputs[self.command][-1]
    else:
      del self.walker.contents[:]
    self.lock.release()
    if not last or last[1] <= self.lastTime:
      return
    self.lastTime = last[1]
    self.lock.acquire()
    lines = self.commandOutputs[self.command][:]
    self.lock.release()
    del self.walker.contents[:]
    for line in lines:
      host = self.command.host
      if not host:
        host     = os.uname()[1]        
      if line[0] == 0:
        prompt = SelectableText( ( "stdout", line[2] ) )
      else:
        prompt = SelectableText( ( "stderr", line[2] ) )
      widget = urwid.Columns( [
        ( "fixed", len( host ) + 2, urwid.Text( ( "prompt", "%s$ " % host ) ) ), 
        prompt
      ] )
      self.walker.contents.append( urwid.AttrWrap( widget, None, "selected_command" ) )
    self.walker.set_focus( len( self.walker.contents ) - 1 )
  
  #-----------------------------------------------------------------------------

  def get_focus( self ):
    return self.walker.get_focus()

