#ifndef ROTOR_BASE_OPTIONS_H
#define ROTOR_BASE_OPTIONS_H


#include "Options.h"
#include <map>


namespace Rotor {


class BaseOptions : public Options
{
public:

  virtual ~BaseOptions();

  virtual void setString(
    const std::string & sectionName,
    const std::string & optionName,
    const std::string & value )
  throw ( OptionError );

  virtual void setInt(
    const std::string & sectionName,
    const std::string & optionName,
    int value )
  throw ( OptionError );

  virtual void setDouble(
    const std::string & sectionName,
    const std::string & optionName,
    double & value )
  throw ( OptionError );

  virtual void setBool(
    const std::string & sectionName,
    const std::string & optionName,
    bool  value )
  throw ( OptionError );

  virtual const std::string & getString(
    const std::string & sectionName,
    const std::string & optionName,
    const std::string & defaultValue )
  throw ( OptionError );

  virtual int getInt(
    const std::string & sectionName,
    const std::string & optionName,
    int defaultValue )
  throw ( OptionError );

  virtual double getDouble(
    const std::string & sectionName,
    const std::string & optionName,
    double defaultValue )
  throw ( OptionError );

  virtual bool getBool(
    const std::string & sectionName,
    const std::string & optionName,
    bool defaultValue )
  throw ( OptionError );

  virtual const std::string & getString(
    const std::string & sectionName,
    const std::string & optionName )
  const throw ( OptionError );

  virtual int getInt(
    const std::string & sectionName,
    const std::string & optionName )
  const throw ( OptionError );

  virtual double getDouble(
    const std::string & sectionName,
    const std::string & optionName )
  const throw ( OptionError );

  virtual bool getBool(
    const std::string & sectionName,
    const std::string & optionName )
  const throw ( OptionError );

  virtual void fromString( const std::string & input );
  virtual std::string toString( const std::string & sectionName = "" ) const;

private:
  typedef std::map<std::string, std::string>  OptionTable;
  typedef std::map<std::string, OptionTable > SectionTable;

  const OptionTable & section( const std::string & sectionName ) const;
  OptionTable & section( const std::string & sectionName );

  SectionTable                                     _sections;
  std::vector<std::string>                         _sectionList;
  std::map<std::string, std::vector<std::string> > _optionList;
};


}


#endif //ROTOR_BASE_OPTIONS_H
