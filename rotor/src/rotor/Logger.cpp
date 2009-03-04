#include "Logger.h"
#include <ostream>
#include <iostream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

Logger::Level Logger::_level     = Logger::INFO;
ostream * Logger::_errorStream   = &cerr; 
ostream * Logger::_warningStream = &cerr; 
ostream * Logger::_infoStream    = &cout; 
ostream * Logger::_debugStream   = &cout; 
ostream * Logger::_spamStream    = &cout; 

//------------------------------------------------------------------------------

void 
Logger::setLevel( Level level )
{
  _level = level;
}

//------------------------------------------------------------------------------

void 
Logger::setErrorOutput( ostream & stream )
{
  _errorStream = &stream;
}

//------------------------------------------------------------------------------

void 
Logger::setWarningOutput( ostream & stream )
{
  _warningStream = &stream;
}

//------------------------------------------------------------------------------

void 
Logger::setInfoOutput( ostream & stream )
{
  _infoStream = &stream;
}

//------------------------------------------------------------------------------

void 
Logger::setDebugOutput( ostream & stream )
{
  _debugStream = &stream;
}

//------------------------------------------------------------------------------

void 
Logger::setSpamOutput( ostream & stream )
{
  _spamStream = &stream;
}

//------------------------------------------------------------------------------

void 
Logger::error( const std::string & message )
{
  if ( _level >= ERROR ) {
    *_errorStream << "ERROR  : " << message << endl;
    _errorStream->flush();
  }
}

//------------------------------------------------------------------------------

void 
Logger::warning( const std::string & message )
{
  if ( _level >= WARNING ) {
    *_warningStream << "WARNING: "<< message << endl;
    _warningStream->flush();
  }
}

//------------------------------------------------------------------------------

void 
Logger::info( const std::string & message )
{
  if ( _level >= INFO ) {
    *_infoStream << "INFO   : " << message << endl;
    _infoStream->flush();
  }
}

//------------------------------------------------------------------------------

void 
Logger::debug( const std::string & message )
{
  if ( _level >= DEBUG ) {
    *_debugStream << "DEBUG  : " << message << endl;
    _debugStream->flush();
  }
}

//------------------------------------------------------------------------------

void 
Logger::spam( const std::string & message )
{
  if ( _level >= SPAM ) {
    *_spamStream << "SPAM   : " << message << endl;
    _spamStream->flush();
  }
}
