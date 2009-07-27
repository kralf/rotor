# Based on C code from:
# "Hildon Tutorial" version 2009-04-28
# Example 3.1, "Example of a Hildon application menu"

import sys

import gtk
import hildon

def menu_button_clicked(button, label):
    buttontext = button.get_label()
    text = "Last option selected:\n%s" % buttontext
    label.set_text(text)
    print >>sys.stderr, "Button clicked: %s" % buttontext

def create_menu(label):
    menu = hildon.AppMenu()

    for i in xrange(1, 6):
        # Create menu entries
        button = hildon.GtkButton(gtk.HILDON_SIZE_AUTO)
        command_id = "Menu command %d" % i
        button.set_label(command_id)

        # Attach callback to clicked signal
        button.connect("clicked", menu_button_clicked, label)

        # Add entry to the view menu
        menu.append(button)

    # Create filters
    # FIXME: hildon_gtk_radio_button_new() is not exposed to Python, using a
    # plain gtk.RadioButton for now
    #button = hildon.GtkRadioButton(gtk.HILDON_SIZE_AUTO, None)
    button = gtk.RadioButton()
    button.set_label("filter one")
    button.connect("clicked", menu_button_clicked, label)
    menu.add_filter(button)
    button.set_mode(False)

    button = gtk.RadioButton(button)
    button.set_label("filter two")
    button.connect("clicked", menu_button_clicked, label)
    menu.add_filter(button)
    button.set_mode(False)

    menu.show_all()

    return menu

def main():
    win = hildon.StackableWindow()

    # Create and pack labels
    label = gtk.Label("This is an example of the\nHildonAppMenu widget.\n\n"
                      "Click on the titlebar\nto pop up the menu.")
    label2 = gtk.Label("No menu option has been selected yet.")

    label.set_justify(gtk.JUSTIFY_CENTER)
    label2.set_justify(gtk.JUSTIFY_CENTER)

    vbox = gtk.VBox(False, 10)

    vbox.pack_start(label, True, True, 0)
    vbox.pack_start(label2, True, True, 0)

    # Create menu
    menu = create_menu(label2)

    # Attach menu to the window
    win.set_app_menu(menu)

    # Add label's box to window
    win.add(vbox)

    win.connect("delete_event", gtk.main_quit)

    win.show_all()

    gtk.main()

if __name__ == "__main__":
    main()

