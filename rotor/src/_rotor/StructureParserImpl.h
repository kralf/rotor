#ifndef ROTOR_STRUCTURE_PARSER_IMPL_H
#define ROTOR_STRUCTURE_PARSER_IMPL_H


#include <rotor/StructureParser.h>
#include <string>


namespace Rotor {


class StructureParserImpl {
public:
  StructureParserImpl();

  bool parse( const std::string & definitionString );

  const MemberDefinitions & members() const;

  const std::string & name() const;

  const std::string & error() const;

private:
  std::string        _name;
  MemberDefinitions  _members;
  std::string        _error;
};


}

#endif //ROTOR_STRUCTURE_PARSER_IMPL_H
