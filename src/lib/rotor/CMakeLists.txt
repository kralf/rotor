remake_find_package(Threads)
if(NOT ${CMAKE_USE_PTHREADS_INIT})
  message(FATAL_ERROR "Missing POSIX thread support!")
endif(NOT ${CMAKE_USE_PTHREADS_INIT})

remake_project_get(LIBRARY_DESTINATION DESTINATION OUTPUT ROTOR_LIBRARY_PATH)
remake_project_header(Config.h.remake)

remake_add_library(rotor PREFIX OFF LINK dl ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY})
remake_add_headers()
