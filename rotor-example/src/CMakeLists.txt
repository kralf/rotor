find_library( LIB_ROTOR rotor )

add_executable( 
  sender 
  rotor-example/sender.cpp
)
target_link_libraries( sender ${LIB_ROTOR} )

add_executable( 
  receiver
  rotor-example/receiver.cpp
)
target_link_libraries( receiver ${LIB_ROTOR} )
