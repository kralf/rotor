cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  RoToR
  VERSION 1.1
  RELEASE beta
  SUMMARY "Robot-to-robot communications."
  AUTHOR "Ralf Kaestner"
  AUTHOR "Dizan Vasquez"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://aslforge.ethz.ch/projects/rotor"
  LICENSE "GNU General Public License (GPL)"
  INSTALL /usr
)

remake_doc(
  html
)
remake_add_directories(
  python
)

remake_add_package(
  DEPENDS libboost-system[0-9.]*
)

remake_add_package(
  COMPONENT python
  DESCRIPTION "Python modules"
  DEPENDS rotor
)
remake_add_package(
  COMPONENT runner
  DESCRIPTION "process runner"
  DEPENDS rotor-python python-urwid
)

remake_add_package(
  COMPONENT carmen
  DESCRIPTION "CARMEN driver"
  DEPENDS "rotor (>= 0.2)" rotor-python carmen-core
  IF CARMEN_FOUND
)
remake_add_package(
  COMPONENT joystick
  DESCRIPTION "CARMEN joystick driver"
  DEPENDS rotor-carmen python-pygame
  IF CARMEN_FOUND
)
remake_add_package(
  COMPONENT example
  DESCRIPTION "CARMEN example"
  DEPENDS rotor-carmen rotor-runner
  IF CARMEN_FOUND
)
remake_add_package(
  COMPONENT yarp
  DESCRIPTION "YARP driver"
  DEPENDS rotor-python carmen-core
  IF YARP_FOUND
)

remake_add_package(
  COMPONENT dev
  DESCRIPTION "development headers"
  DEPENDS rotor libboost-system-dev
)
remake_add_package(
  COMPONENT carmen-dev
  DESCRIPTION "CARMEN driver development headers"
  DEPENDS rotor-carmen "rotor-dev (>= 0.2)" carmen-core-dev
  IF CARMEN_FOUND
)
remake_add_package(
  COMPONENT yarp-dev
  DESCRIPTION "YARP driver development headers"
  DEPENDS rotor-yarp rotor-dev
  IF YARP_FOUND
)

remake_add_package(
  ARCH all
  COMPONENT doc
  DESCRIPTION "documentation"
)
remake_add_package(
  ARCH all
  COMPONENT carmen-doc
  DESCRIPTION "CARMEN driver documentation"
  DEPENDS rotor-doc
  IF CARMEN_FOUND
)
remake_add_package(
  ARCH all
  COMPONENT yarp-doc
  DESCRIPTION "YARP driver documentation"
  DEPENDS rotor-doc
  IF YARP_FOUND
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake libboost-system-dev python-urwid python-pygame
    carmen-core-dev libunittest++-dev swig pkg-config doxygen
  PASS CMAKE_BUILD_TYPE ROTOR_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION precise
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake libboost-system-dev python-urwid python-pygame
    carmen-core-dev libunittest++-dev swig pkg-config doxygen
  PASS CMAKE_BUILD_TYPE ROTOR_GIT_REVISION
)
