remake_find_package(Boost COMPONENTS system RESULT_VAR Boost_FOUND)
remake_find_package(carmen-core CONFIG OPTIONAL)
remake_find_library(YARP_OS os/all.h PACKAGE YARP OPTIONAL)

remake_add_directories(lib)
remake_add_directories(bin)

remake_pkg_config_generate()
remake_pkg_config_generate(COMPONENT carmen REQUIRES carmen-core)
