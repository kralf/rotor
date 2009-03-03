#-------------------------------------------------------------------------------
# Main library
#-------------------------------------------------------------------------------

file( GLOB ROTOR_SRC rotor/*.cpp )
file( GLOB _ROTOR_SRC _rotor/*.cpp )

add_library( rotor SHARED ${ROTOR_SRC} ${_ROTOR_SRC} )

target_link_libraries( rotor Hapy pthread dl )


sm_cpp_install( rotor )

# Python bindings
include( SwigPython )
swig_python( rotorc rotor )

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------

file( GLOB TEST_SRC test/*.cpp )

add_executable( unit_tests ${TEST_SRC} ) 

target_link_libraries( unit_tests rotor BroadcastRegistry unittest++ )

add_test( unit_tests ${EXECUTABLE_OUTPUT_PATH}/unit_tests xml )

#-------------------------------------------------------------------------------
# Carmen
#-------------------------------------------------------------------------------

file( GLOB CARMEN_SRC carmen/*.cpp )

add_library( CarmenRegistry SHARED ${CARMEN_SRC} )

target_link_libraries( CarmenRegistry rotor ipc )
sm_cpp_install_binaries( CarmenRegistry )

#-------------------------------------------------------------------------------
# Broadcast
#-------------------------------------------------------------------------------

add_library( 
  BroadcastRegistry SHARED 
  broadcast/BroadcastRegistry.cpp
)

target_link_libraries( BroadcastRegistry rotor PocoNet )
sm_cpp_install_binaries( BroadcastRegistry )

#-------------------------------------------------------------------------------
# Sandbox
#-------------------------------------------------------------------------------

add_executable(
  broadcastSerialization
  sandbox/broadcastSerialization.cpp
)

target_link_libraries( broadcastSerialization BroadcastRegistry ipc )

add_executable(
  carmenSender
  sandbox/carmenSender.cpp
)

target_link_libraries( carmenSender rotor )

add_executable(
  carmenReceiver
  sandbox/carmenReceiver.cpp
)

target_link_libraries( carmenReceiver rotor )
