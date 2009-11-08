#ifndef ROTOR_OPTION_PARSER_H
#define ROTOR_OPTION_PARSER_H


#include <rotor/Options.h>
#include <string>


namespace Rotor {


class OptionParser
{
public:
  OptionParser();

  bool parse( const std::string & input, Options & options );

  const std::string & error() const;

private:
  std::string  _error;
};


}


#endif //ROTOR_OPTION_PARSER_H
