#ifndef ROTOR_OPTIONS_H
#define ROTOR_OPTIONS_H


#include "Exceptions.h"
#include <string>
#include <vector>


namespace Rotor {


class Options
{
public:
  virtual void setString( 
    const std::string & sectionName, 
    const std::string & optionName,
    const std::string & value ) 
  throw ( OptionError ) = 0;
    
  virtual void setInt( 
    const std::string & sectionName, 
    const std::string & optionName,
    int value )
  throw ( OptionError ) = 0;
    
  virtual void setDouble( 
    const std::string & sectionName, 
    const std::string & optionName,
    double & value )
  throw ( OptionError ) = 0;
    
  virtual void setBool( 
    const std::string & sectionName, 
    const std::string & optionName,
    bool  value )
  throw ( OptionError ) = 0;
    
  virtual const std::string & getString(   
    const std::string & sectionName, 
    const std::string & optionName,
    const std::string & defaultValue )
  throw ( OptionError ) = 0;

  virtual int getInt(   
    const std::string & sectionName, 
    const std::string & optionName,
    int defaultValue )
  throw ( OptionError ) = 0;

  virtual double getDouble(   
    const std::string & sectionName, 
    const std::string & optionName,
    double defaultValue )
  throw ( OptionError ) = 0;

  virtual bool getBool(   
    const std::string & sectionName, 
    const std::string & optionName,
    bool defaultValue ) 
  throw ( OptionError ) = 0;

  virtual const std::string & getString(   
    const std::string & sectionName, 
    const std::string & optionName )
  const throw ( OptionError ) = 0;

  virtual int getInt(   
    const std::string & sectionName, 
    const std::string & optionName )
  const throw ( OptionError ) = 0;

  virtual double getDouble(   
    const std::string & sectionName, 
    const std::string & optionName )
  const throw ( OptionError ) = 0;

  virtual bool getBool(   
    const std::string & sectionName, 
    const std::string & optionName )
  const throw ( OptionError ) = 0;
  
  virtual void fromString( const std::string & input ) = 0;
  virtual std::string toString( const std::string & sectionName = "" ) const = 0;
};


}


#endif //ROTOR_OPTIONS_H