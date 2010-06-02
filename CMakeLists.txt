cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  RoToR
  VERSION 1.1
  RELEASE beta
  SUMMARY "Robot-to-robot communications."
  AUTHOR "Dizan Vasquez"
  AUTHOR "Ralf Kaestner"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://aslforge.ethz.ch/projects/rotor"
  LICENSE "GNU General Public License (GPL)"
  INSTALL /usr
)

remake_doc(
  man INSTALL share
  html
)

remake_pack_deb()
remake_pack_deb(COMPONENT dev rotor)
remake_pack_deb(ARCH all COMPONENT doc)
