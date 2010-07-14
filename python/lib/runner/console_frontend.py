from command_list    import *
from command_dialog  import *
from memory_info     import *
from output_list     import *
from selectable_text import *

from rotor.pycommand import *

import threading
import urwid.curses_display
import urwid
import time
import os
import sys

#-------------------------------------------------------------------------------

class ConsoleFrontend():
  def __init__( self, commands = [] ):
    self.screen = urwid.curses_display.Screen()    
    self.screen.register_palette( [
      ( "body", "dark gray", "light gray" ),
      ( "header", "white", "dark blue" ),
      ( "list_header", "white", "black" ),
      ( "command", "yellow", "dark blue" ),
      ( "selected_command", "black", "light gray" ),
      ( "prompt", "black", "default" ), 
      ( "stdout", "light green", "default" ), 
      ( "stderr", "light red", "default" ),
      ( "running", "light green", "default" ),
      ( "error", "light red", "default" ),
      ( "stopped", "yellow", "default" )
    ] )
    
    self.commands        = commands
    self.commandOutputs  = {}
    self.command_list    = CommandList( self.commands )
    self.lock            = threading.Lock()
    
    self.output_lists    = []
    for c in self.commands:
      self.commandOutputs[c] = []
      c.handler = self.processOutput
      self.output_lists.append( OutputList( c, self.commandOutputs, self.lock ) )
      
    self.output_box      = urwid.LineBox( urwid.Filler( urwid.Text( "" ) ) )

    self.columns = urwid.Columns( [
      ( "fixed", 60, self.command_list ), 
      self.output_box
    ], dividechars = 1 )
    
    
    self.header_text = urwid.Text( self.header() )
    header = urwid.AttrWrap( self.header_text, 'header' )
    
    instruction_text = urwid.Text( "Keys <esc: quit> | <s: start> | <t: stop> | <k: kill> | <c: clear> | <tab: next panel>" )
    footer = urwid.AttrWrap( instruction_text, 'header' )
    
    self.new_dialog = urwid.LineBox( CommandDialog( self.commands, self.output_lists ) )
    
    footer = urwid.Pile( [self.new_dialog, footer] )

    self.top = urwid.Frame( self.columns, header, footer )
    
    self.selection = 0
    self.update_focus()

    self.screen.set_input_timeouts( 0.5 )
    self.screen.run_wrapper( self.run )
    
  #-----------------------------------------------------------------------------

  def processOutput( self, command, source, line ):
    s = 0
    if source == "stderr":
      s = 1
    self.lock.acquire()
    output = self.commandOutputs[command]
    output.append( [s, time.time(), line] )
    if len( output ) > 1000:
      output.pop( 0 )
    self.lock.release()
    
  #-----------------------------------------------------------------------------

  def header( self ):
    return "Process Runner v1.0 %s memory:%i K" % ( time.asctime(), resident() / 1024 )   
  
  #-----------------------------------------------------------------------------

  def update_focus( self ):
    if self.selection == 0:
      self.top.set_focus( "body" )
      self.columns.set_focus( 0 )
    elif self.selection == 1:
      self.top.set_focus( "body" )
      self.columns.set_focus( 1 )
    elif self.selection == 2:
      self.top.set_focus( "footer" )
  
  #-----------------------------------------------------------------------------

  def run( self ):
    while True:
      self.size = self.screen.get_cols_rows()
      pos = self.command_list.get_focus()[1]
      
      self.header_text.set_text( self.header() )
      self.command_list.update()
      if pos != None:
        output = self.output_lists[pos]
        output.update()
        self.output_box._w = urwid.LineBox( output )

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
        if "s" in keys:
          self.lock.acquire()
          self.commandOutputs[self.commands[pos]] = []
          self.lock.release()
          self.commands[pos].start()
        if "t" in keys:
          self.commands[pos].stop()
        if "k" in keys:
          self.commands[pos].kill()
        if "c" in keys:
          self.lock.acquire()
          self.commandOutputs[self.commands[pos]] = []
          self.lock.release()
      for k in keys:
        self.top.keypress( self.size, k )
