#-------------------------------------------------------------------------------
# Carmen
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )
find_library( IPC_LIB NAMES carmen-ipc ipc )

file( GLOB CARMEN_SRC carmen/*.cpp )
file( GLOB _CARMEN_SRC _carmen/*.cpp )

add_library( CarmenRegistry SHARED ${CARMEN_SRC} ${_CARMEN_SRC} )

target_link_libraries( CarmenRegistry ${ROTOR_LIB} ${IPC_LIB} )
sm_cpp_install_binaries( CarmenRegistry )
