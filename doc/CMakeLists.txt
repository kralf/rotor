remake_add_directories(rotor)
remake_add_directories(
  carmen
  COMPONENT carmen
  IF CARMEN_CORE_FOUND
)
remake_add_directories(
  yarp
  COMPONENT yarp
  IF YARP_FOUND
)
