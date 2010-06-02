#include <unittest++/UnitTest++.h>
#include <unittest++/XmlTestReporter.h>
#include <iostream>
#include <fstream>

int main( int argc, char ** ) {
  if ( argc == 2 ) {
    UnitTest::XmlTestReporter reporter( std::cout );
    UnitTest::TestRunner runner( reporter );
    return runner.RunTestsIf(
      UnitTest::Test::GetTestList(),
      NULL,
      UnitTest::True(),
      0);
  }
    
  return UnitTest::RunAllTests();
}
