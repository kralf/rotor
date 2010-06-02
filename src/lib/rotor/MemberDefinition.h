#ifndef ROTOR_MEMBER_DEFINITION_H
#define ROTOR_MEMBER_DEFINITION_H


#include <vector>
#include <string>


namespace Rotor {


enum Cardinality
{
  ONE      = 0,
  VARIABLE = -1
};


class MemberDefinition 
{
public:
  std::string name;
  std::string type;
  int         cardinality;
  size_t      size;
  size_t      realSize;
  bool operator==( const MemberDefinition & value ) const;
};


typedef std::vector<MemberDefinition> MemberDefinitions;


}

#endif //ROTOR_MEMBER_DEFINITION_H
