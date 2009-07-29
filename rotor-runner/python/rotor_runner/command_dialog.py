import urwid

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
  
  #-----------------------------------------------------------------------------

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
    
    command = pycommand.Command( command, host = host, username = username )
    command.start()
    self.commands.append( command )
    self.output_lists.append( OutputList( command ) )
    self.host_text.set_edit_text( "" )
    self.command_text.set_edit_text( "" )
    self.username_text.set_edit_text( "" )
    self.password_text.set_edit_text( "" )
    self.columns.set_focus( 0 )
    
