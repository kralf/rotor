#ifndef ROTOR_STRUCTURE_PARSER_H
#define ROTOR_STRUCTURE_PARSER_H


#include "Registry.h"
#include <string>


namespace Rotor {


class StructureParserImpl;

/**
 * \brief Parser for C++ style struct declarations.
 * 
 * Parses C++ style structures, building a table of member information.
 */

class StructureParser {
public:
  StructureParser();
  virtual ~StructureParser();
  
  bool parse( const std::string & definitionString );
  
  const MemberDefinitions & members() const;
  
  const std::string & name() const;
  
  const std::string & error() const;
  
private:
  StructureParser( const StructureParser & );
  void operator=( const StructureParser & );
  StructureParserImpl * _impl;
};


}


#endif //ROTOR_STRUCTURE_PARSER_H
