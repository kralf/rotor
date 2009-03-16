install(
  DIRECTORY rotor
  DESTINATION share/${PROJECT_NAME}/python/
  COMPONENT default
  PATTERN .* EXCLUDE
  PATTERN *.pyc EXCLUDE
)

install(
  DIRECTORY runner
  DESTINATION share/${PROJECT_NAME}/python/
  COMPONENT default
  PATTERN .* EXCLUDE
  PATTERN *.pyc EXCLUDE
)

install(
  FILES rotor.pth
  DESTINATION ${CMAKE_INSTALL_ROOT}/usr/lib/python2.5/site-packages/
  COMPONENT default
)