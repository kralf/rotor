#include <unittest++/UnitTest++.h>
#include <unittest++/XmlTestReporter.h>
#include <iostream>
#include <fstream>

int main( int argc, char ** ) {
  if ( argc == 2 ) {
    UnitTest::XmlTestReporter reporter( std::cout );
    return UnitTest::RunAllTests(
      reporter,
      UnitTest::Test::GetTestList(),
      NULL,
      0);
  }
    
  return UnitTest::RunAllTests();
}
