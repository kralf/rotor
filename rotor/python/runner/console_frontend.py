import runner
import urwid.curses_display
import urwid
import time
import os

#-------------------------------------------------------------------------------

class SelectableText( urwid.Text ):
  def selectable(self):
    return True
    
  #-----

  def keypress(self, size, key):
    return key
  
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
    
  #-----
  
  def update( self ):
    pos = self.walker.get_focus()
    del self.walker.contents[:]
    for c in self.commands:
      if c.running:
        status = "running"
      elif c.return_code == 0:
        status = "stopped"
      else:
        status = "error"
      widget = urwid.Columns( [
        ( "weight", 1, SelectableText( ( status, status ) ) ), 
        ( "weight", 5, SelectableText( c.command ) )
      ] )
      self.walker.contents.append( urwid.AttrWrap( widget, None, "selected_command" ) )
    if type( pos[1] ) == type( 1 ):
      self.walker.set_focus( pos[1] )
    else:
      self.walker.set_focus( 0 )
  
  #-----
  
  def get_focus( self ):
    return self.walker.get_focus()

    
#-------------------------------------------------------------------------------

class OutputList( urwid.WidgetWrap ):
  def __init__( self, command ):
    self.command  = command
    self.walker   = urwid.SimpleListWalker( [] )
    list          = urwid.ListBox( self.walker )
    self.lastTime = time.time()
    urwid.WidgetWrap.__init__( self, list )
    
  #-----
  
  def update( self ):
    last = None
    if len( self.command.output ) > 0:
      last = self.command.output[-1]
    if not last or last[1] <= self.lastTime:
      return
    self.lastTime = last[1]
    lines = self.command.output[:]
    del self.walker.contents[:]
    for line in lines:
      host = self.command.host
      if not host:
        host     = os.uname()[1]        
      if line[0] == 0:
        prompt = SelectableText( ( "stdout", line[2][:-1] ) )
      else:
        prompt = SelectableText( ( "stderr", line[2][:-1] ) )
      widget = urwid.Columns( [
        ( "fixed", len( host ) + 2, urwid.Text( ( "prompt", "%s$ " % host ) ) ), 
        prompt
      ] )
      self.walker.contents.append( urwid.AttrWrap( widget, None, "selected_command" ) )
    self.walker.set_focus( len( self.walker.contents ) - 1 )
  
  #-----
  
  def get_focus( self ):
    return self.walker.get_focus()

#-------------------------------------------------------------------------------

class CommandDialog( urwid.WidgetWrap ):
  def __init__( self, commands, output_lists ):
    self.commands      = commands
    self.output_lists  = output_lists
    self.host_text     = urwid.Edit( caption = "Host:" )
    self.command_text  = urwid.Edit( caption = "Command:" )
    self.username_text = urwid.Edit( caption = "User name:" )
    self.password_text = urwid.Edit( caption = "Password:" )
    self.ok_button     = urwid.Button( "Add", self.ok_pressed )
    self.columns = urwid.Columns( [
      ( "weight", 15, self.host_text ),
      ( "weight", 25, self.command_text ),
      ( "weight", 15, self.username_text ),
      ( "weight", 15, self.password_text ),
      ( "fixed", 8, self.ok_button )
    ] )
    urwid.WidgetWrap.__init__( self, self.columns )
  
  def ok_pressed( self, button ):
    host     = self.host_text.get_edit_text()
    command  = self.command_text.get_edit_text()
    username = self.username_text.get_edit_text()
    password = self.password_text.get_edit_text()
    if host == "":
      host = None
    if username == "":
      username = None
    if password == "":
      password
    
    command = runner.Command( command, host = host, username = username, password = password )
    command.start()
    self.commands.append( command )
    self.output_lists.append( OutputList( command ) )
    self.host_text.set_edit_text( "" )
    self.command_text.set_edit_text( "" )
    self.username_text.set_edit_text( "" )
    self.password_text.set_edit_text( "" )
    self.columns.set_focus( 0 )
    
#-------------------------------------------------------------------------------

class ConsoleFrontend():
  def __init__( self, commands = [] ):
    self.screen = urwid.curses_display.Screen()    
    self.screen.register_palette( [
      ( "body", "black", "light gray" ),
      ( "header", "white", "dark blue" ),
      ( "list_header", "white", "black" ),
      ( "command", "yellow", "dark blue" ),
      ( "selected_command", "black", "light gray" ),
      ( "prompt", "light gray", "black" ), 
      ( "stdout", "light green", "black" ), 
      ( "stderr", "light red", "black" ),
      ( "running", "light green", "black" ),
      ( "error", "light red", "black" ),
      ( "stopped", "yellow", "black" )
    ] )
    
    self.commands  = commands
    self.command_list    = CommandList( self.commands )
    
    self.output_lists    = []
    for c in self.commands:
      self.output_lists.append( OutputList( c ) )
      
    self.output_box      = urwid.LineBox( urwid.Filler( urwid.Text( "" ) ) )

    self.columns = urwid.Columns( [
      ( "fixed", 60, self.command_list ), 
      self.output_box
    ], dividechars = 1 )
    
    
    self.header_text = urwid.Text( "Process Runner v1.0 %s" % time.asctime() )
    header = urwid.AttrWrap( self.header_text, 'header' )
    
    instruction_text = urwid.Text( "Keys <esc: quit> | <s: start> | <t: stop> | <k: kill> | <a: kill all>" )
    footer = urwid.AttrWrap( instruction_text, 'header' )
    
    self.new_dialog = urwid.LineBox( CommandDialog( self.commands, self.output_lists ) )
    
    footer = urwid.Pile( [self.new_dialog, footer] )

    self.top = urwid.Frame( self.columns, header, footer )
    
    self.selection = 0
    self.update_focus()

    self.screen.run_wrapper( self.run )
    
  #-----
  
  def update_focus( self ):
    if self.selection == 0:
      self.top.set_focus( "body" )
      self.columns.set_focus( 0 )
    elif self.selection == 1:
      self.top.set_focus( "body" )
      self.columns.set_focus( 1 )
    elif self.selection == 2:
      self.top.set_focus( "footer" )
  
  #-----

  def run( self ):
    while True:
      self.size = self.screen.get_cols_rows()
      pos = self.command_list.get_focus()[1]
      
      self.header_text.set_text( "Process Runner v1.0 %s" % time.asctime() )
      self.command_list.update()
      if pos != None:
        output = self.output_lists[pos]
        output.update()
        self.output_box.set_w( urwid.LineBox( output ) )

      canvas = self.top.render( self.size, focus=True )
      self.screen.draw_screen( self.size, canvas )
      keys = self.screen.get_input()
      if "esc" in keys:
        self.screen.stop()
        for c in self.commands:
          c.stop()
          c.join()
        break
      if "tab" in keys:
        self.selection = ( self.selection + 1 ) % 3
        self.update_focus()
      if self.selection != 2:
        if "k" in keys:
          self.commands[pos].kill()
        if "s" in keys:
          self.commands[pos].start()
        if "t" in keys:
          self.commands[pos].stop()
      for k in keys:
        self.top.keypress( self.size, k )