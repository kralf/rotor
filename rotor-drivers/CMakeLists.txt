project( rotor-drivers )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.2 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Diverse hardware driverse for Rotor" )
set( PACKAGE_DESCRIPTION "Default set of hardware drivers for the Rotor communication middleware." )
set( PACKAGE_DEPENDS "rotor, python-pygame" )

include( SimpleCMake )
sm_configure_project()

add_subdirectory( python )