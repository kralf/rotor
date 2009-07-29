project( rotor-runner )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.1.0 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Rotor executable launch helper" )
set( PACKAGE_DESCRIPTION "Standalone application to centralize process management." )
set( PACKAGE_DEPENDS python-urwid,python-paramiko )

include( SimpleCMake )
sm_configure_project()
