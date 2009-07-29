from selectable_text import *

import urwid

#-------------------------------------------------------------------------------

class CommandList( urwid.WidgetWrap ):
  def __init__( self, commands ):
    self.commands = commands
    self.walker   = urwid.SimpleListWalker( [] )
    list          = urwid.ListBox( self.walker )
    titles        = urwid.Columns( [
        ( "weight", 1, urwid.Text( "STATUS" ) ), 
        ( "weight", 5, urwid.Text( "COMMAND" ) )      
    ] )
    list          = urwid.Frame( list, urwid.AttrWrap( titles, "list_header" ) )
    box           = urwid.LineBox( list )
    urwid.WidgetWrap.__init__( self, box )
    self.update()
    
  #-----------------------------------------------------------------------------

  def update( self ):
    pos = self.walker.get_focus()
    del self.walker.contents[:]
    for c in self.commands:
      if c.running:
        status = "running"
        text   = status
      elif c.exitCode == 0:
        status = "stopped"
        text   = status
      else:
        status = "error" 
        text   = status + " %i" % c.exitCode
      widget = urwid.Columns( [
        ( "weight", 1, SelectableText( ( status, text ) ) ), 
        ( "weight", 5, SelectableText( c.command ) )
      ] )
      self.walker.contents.append( urwid.AttrWrap( widget, None, "selected_command" ) )
    if type( pos[1] ) == type( 1 ):
      self.walker.set_focus( pos[1] )
    else:
      self.walker.set_focus( 0 )
  
  #-----------------------------------------------------------------------------

  def get_focus( self ):
    return self.walker.get_focus()
