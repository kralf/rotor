#ifndef ROTOR_FILEUTILS_H
#define ROTOR_FILEUTILS_H


#include <string>


namespace Rotor {

std::string fileContents( const std::string & path );

bool fileExists( const std::string & fullPath );

const std::string findFile( 
  const std::string & fileName,
  const std::string & searchPath );


}


#endif //ROTOR_FILEUTILS_H
