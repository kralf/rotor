#-------------------------------------------------------------------------------
# Main library
#-------------------------------------------------------------------------------

add_library( 
  rotor SHARED 
  rotor/AbstractVariable.cpp
  rotor/BaseOptions.cpp
  rotor/BaseRegistry.cpp
  rotor/BasicTypes.cpp
  rotor/Condition.cpp
  _rotor/ConditionImpl.cpp
  rotor/Debug.cpp
  rotor/DebugModule.cpp
  rotor/DefaultModule.cpp
  rotor/FileUtils.cpp
  rotor/DispatcherModule.cpp
  rotor/InputDispatcherModule.cpp
  rotor/Lock.cpp
  rotor/MemberDefinition.cpp
  rotor/Memory.cpp
  rotor/Message.cpp
  rotor/Module.cpp
  rotor/ModuleRunner.cpp
  rotor/Mutex.cpp
  _rotor/MutexImpl.cpp
  _rotor/OptionParser.cpp
  rotor/PeriodicTask.cpp
  rotor/Registry.cpp  
  rotor/Serialization.cpp
  rotor/String.cpp 
  rotor/Structure.cpp 
  rotor/StructureArray.cpp 
  rotor/StructureParser.cpp 
  _rotor/StructureParserImpl.cpp
  rotor/Type.cpp
  rotor/Thread.cpp
  _rotor/ThreadImpl.cpp
)


target_link_libraries( rotor Hapy pthread dl )


sm_cpp_install( rotor )

# Python bindings
include( SwigPython )
swig_python( rotorc rotor )

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------

add_executable(
  unit_tests
  test/main.cpp
#   test/TestArray.cpp
  test/TestMemory.cpp
#   test/TestOptions.cpp
#   test/TestOptionParser.cpp
#   test/TestVariable.cpp
#   test/TestString.cpp
#   test/TestStructure.cpp
#   test/TestStructureArray.cpp
#   test/TestStructureParser.cpp
  test/TestRegistry.cpp
#   test/TestQueue.cpp
#   test/TestBroadcastSerialization.cpp
) 

target_link_libraries( unit_tests rotor BroadcastRegistry unittest++ )

add_test( unit_tests ${EXECUTABLE_OUTPUT_PATH}/unit_tests xml )

#-------------------------------------------------------------------------------

add_library( 
  CarmenRegistry SHARED 
  carmen/CarmenRegistry.cpp
)

target_link_libraries( CarmenRegistry rotor ipc )
sm_cpp_install_binaries( CarmenRegistry )

#-------------------------------------------------------------------------------

add_library( 
  BroadcastRegistry SHARED 
  broadcast/BroadcastRegistry.cpp
)

target_link_libraries( BroadcastRegistry rotor PocoNet )

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