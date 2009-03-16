project( rotor-urus )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.1 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Urus drivers for rotor" )
set( PACKAGE_DESCRIPTION "Urus YARP drivers for the rotor communications framework" )
set( PACKAGE_DEPENDS "rotor (>= 0.0.11)"  )

include( SimpleCMake )
sm_configure_project()
