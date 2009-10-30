project( rotor )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 1.0.0 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Robot to robot communications" )
set( PACKAGE_DESCRIPTION "A unifying middleware framework for robotics applications." )
set( PACKAGE_BUILD_DEPENDS libboost-dev,libasio-dev )

execute_process(
  COMMAND dpkg --print-architecture
  OUTPUT_VARIABLE DEBIAN_ARCHITECTURE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

include( SimpleCMake )
sm_configure_project()
