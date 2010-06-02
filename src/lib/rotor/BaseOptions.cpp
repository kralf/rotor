#include "BaseOptions.h"
#include "Conversion.h"
#include "Exceptions.h"
#include "OptionParser.ipp"
#include <sstream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

BaseOptions::~BaseOptions()
{
}

//------------------------------------------------------------------------------

void 
BaseOptions::setString( 
  const string & sectionName, 
  const string & optionName,
  const string & value ) throw ( OptionError )
{
  section( sectionName )[optionName] = value;
}
  
//------------------------------------------------------------------------------

void 
BaseOptions::setInt( 
  const string & sectionName, 
  const string & optionName,
  int value ) throw ( OptionError )
{
  section( sectionName )[optionName] = ::toString( value );
}
  
//------------------------------------------------------------------------------

void 
BaseOptions::setDouble( 
  const string & sectionName, 
  const string & optionName,
  double & value ) throw ( OptionError )
{
  section( sectionName )[optionName] = ::toString( value );
}
  
//------------------------------------------------------------------------------

void 
BaseOptions::setBool( 
  const string & sectionName, 
  const string & optionName,
  bool  value ) throw ( OptionError )
{
  section( sectionName )[optionName] = ::toString( value );
}
  
//------------------------------------------------------------------------------

const string & 
BaseOptions::getString(   
  const string & sectionName, 
  const string & optionName,
  const string & defaultValue ) throw ( OptionError )
{
  OptionTable & options = section( sectionName );
  OptionTable::iterator option = options.find( optionName );
  if ( option == options.end() ) {
    options[optionName] = defaultValue;
    return defaultValue;
  }
  return option->second;
}

//------------------------------------------------------------------------------

int 
BaseOptions::getInt(   
  const string & sectionName, 
  const string & optionName,
  int defaultValue ) throw ( OptionError )
{
  return ::fromString<int>( getString( sectionName, optionName, ::toString( defaultValue ) ) );
}

//------------------------------------------------------------------------------

double 
BaseOptions::getDouble(   
  const string & sectionName, 
  const string & optionName,
  double defaultValue ) throw ( OptionError )
{
  return ::fromString<double>( getString( sectionName, optionName, ::toString( defaultValue ) ) );
}

//------------------------------------------------------------------------------

bool 
BaseOptions::getBool(   
  const string & sectionName, 
  const string & optionName,
  bool defaultValue ) throw ( OptionError )
{
  return ::fromString<bool>( getString( sectionName, optionName, ::toString( defaultValue ) ) );
}

//------------------------------------------------------------------------------

const string & 
BaseOptions::getString(   
  const string & sectionName, 
  const string & optionName ) const throw ( OptionError )
{
  const OptionTable & options = section( sectionName );
  OptionTable::const_iterator option = options.find( optionName );
  if ( option == options.end() ) {
    throw OptionError( "Option " + optionName + " not found in section " + sectionName );
  }
  return option->second;
}

//------------------------------------------------------------------------------

int 
BaseOptions::getInt(   
  const string & sectionName, 
  const string & optionName ) const throw ( OptionError )
{
  return ::fromString<int>( getString( sectionName, optionName ) );
}

//------------------------------------------------------------------------------

double 
BaseOptions::getDouble(   
  const string & sectionName, 
  const string & optionName ) const throw ( OptionError )
{
  return ::fromString<double>( getString( sectionName, optionName ) );
}

//------------------------------------------------------------------------------

bool 
BaseOptions::getBool(   
  const string & sectionName, 
  const string & optionName ) const throw ( OptionError )
{
  return ::fromString<bool>( getString( sectionName, optionName ) );
}

//------------------------------------------------------------------------------
  
const BaseOptions::OptionTable &
BaseOptions::section( const string & sectionName ) const
{
  SectionTable::const_iterator section = _sections.find( sectionName );
  if ( section == _sections.end() ) {
    throw OptionError( "Section " + sectionName + " not found" );
  }
  return section->second;  
}

//------------------------------------------------------------------------------
  
BaseOptions::OptionTable &
BaseOptions::section( const string & sectionName )
{
  return _sections[sectionName];  
}

//------------------------------------------------------------------------------

void 
BaseOptions::fromString( const string & input )
{
  OptionParser parser;
  if ( ! parser.parse( input, *this ) ) {
    throw ParsingError( parser.error() );
  }
}

//------------------------------------------------------------------------------

string 
BaseOptions::toString( const string & sectionName ) const
{
  stringstream ss;
  SectionTable::const_iterator section = _sections.begin();
  for (; section != _sections.end(); section++ ) {
    if ( sectionName.empty() || sectionName == section->first ) {
      OptionTable::const_iterator option = section->second.begin();
      ss << "[" << section->first << "]" << endl;
      for ( ; option != section->second.end(); option++ ) {
        ss << option->first << " = " << option->second << endl;
      }
    }
  }
  return ss.str();
}
