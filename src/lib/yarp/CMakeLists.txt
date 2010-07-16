remake_find_library(YARP_OS os/all.h PACKAGE YARP)

remake_add_library(yarp_registry LINK rotor ${YARP_LIBRARY})
remake_add_headers()
