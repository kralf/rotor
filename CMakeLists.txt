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

remake_python()
remake_doc(
  html
)

remake_pack_deb()
remake_pack_deb(
  COMPONENT carmen
  DESCRIPTION "CARMEN driver"
  "rotor (>= 0.2)" carmen-core
)
remake_pack_deb(
  COMPONENT runner
  DESCRIPTION "process runner"
  rotor python-urwid
)

remake_pack_deb(
  COMPONENT python
  DESCRIPTION "Python modules"
  rotor
)

remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  rotor libboost-system-dev
)
remake_pack_deb(
  COMPONENT carmen-dev
  DESCRIPTION "CARMEN driver development headers"
  "rotor-dev (>= 0.2)" carmen-core-dev
)

remake_pack_deb(
  ARCH all
  COMPONENT doc
  DESCRIPTION "documentation"
)
remake_pack_deb(
  ARCH all
  COMPONENT carmen-doc
  DESCRIPTION "CARMEN driver documentation"
  rotor-doc
)
