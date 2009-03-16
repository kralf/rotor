#-------------------------------------------------------------------------------
# Urus
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )
find_library( YARP_LIB YARP_OS )

file( GLOB URUS_SRC urus/*.cpp )
file( GLOB _URUS_SRC _urus/*.cpp )

add_library( UrusRegistry SHARED ${URUS_SRC} ${_URUS_SRC} )

target_link_libraries( UrusRegistry ${ROTOR_LIB} ${YARP_LIB} )
sm_cpp_install_binaries( UrusRegistry )
