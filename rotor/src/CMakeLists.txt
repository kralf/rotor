#-------------------------------------------------------------------------------
# Main library
#-------------------------------------------------------------------------------

file( GLOB ROTOR_SRC rotor/*.cpp )
file( GLOB _ROTOR_SRC _rotor/*.cpp )
find_library( HAPY_LIB hapy )
find_library( POCONET_LIB PocoNet )

add_library( rotor SHARED ${ROTOR_SRC} ${_ROTOR_SRC} )

target_link_libraries( rotor ${HAPY_LIB} pthread dl )


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
# Broadcast
#-------------------------------------------------------------------------------

add_library( 
  BroadcastRegistry SHARED 
  broadcast/BroadcastRegistry.cpp
)

target_link_libraries( BroadcastRegistry rotor ${POCONET_LIB} )
sm_cpp_install_binaries( BroadcastRegistry )

#-------------------------------------------------------------------------------
# Sandbox
#-------------------------------------------------------------------------------

# add_executable( print_laser sandbox/laser_receiver.cpp )

# target_link_libraries( print_laser rotor )

add_executable( structurePerformance sandbox/structurePerformance.cpp )
target_link_libraries( structurePerformance rotor )
