install(
  DIRECTORY urwid
  DESTINATION share/python-support/python-urwid
  COMPONENT default
  PATTERN .* EXCLUDE
)