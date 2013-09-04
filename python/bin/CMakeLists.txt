remake_add_directories(rotor)
remake_add_directories(
  runner
  COMPONENT runner
)
remake_add_directories(
  carmen
  COMPONENT carmen
  IF CARMEN_CORE_FOUND
)
remake_add_directories(
  joystick
  COMPONENT joystick
)
