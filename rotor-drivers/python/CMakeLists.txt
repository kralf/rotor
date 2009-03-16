install(
  DIRECTORY rotor
  DESTINATION share/rotor/python/
  COMPONENT default
  PATTERN .* EXCLUDE
  PATTERN *.pyc EXCLUDE
)
