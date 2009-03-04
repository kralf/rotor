#ifndef ROTOR_LOGGER_H
#define ROTOR_LOGGER_H


#include <ostream>


namespace Rotor{


class Logger
{
public:
  enum Level {
    NONE    = 0,
    ERROR   = 1,
    WARNING = 2,
    INFO    = 3,
    DEBUG   = 4,
    SPAM    = 5
  };

  static void setLevel( Level level );
  static void setErrorOutput( std::ostream & stream );
  static void setWarningOutput( std::ostream & stream );
  static void setInfoOutput( std::ostream & stream );
  static void setDebugOutput( std::ostream & stream );
  static void setSpamOutput( std::ostream & stream );
  
  static void error( const std::string & message );
  static void warning( const std::string & message );
  static void info( const std::string & message );
  static void debug( const std::string & message );
  static void spam( const std::string & message );
  
private:
  static Level          _level;
  static std::ostream * _errorStream; 
  static std::ostream * _warningStream; 
  static std::ostream * _infoStream; 
  static std::ostream * _debugStream; 
  static std::ostream * _spamStream; 
};


}


#endif //ROTOR_LOGGER_H