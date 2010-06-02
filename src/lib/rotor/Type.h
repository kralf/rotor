#ifndef ROTOR_TYPE_H
#define ROTOR_TYPE_H


#include "MemberDefinition.h"
#include <string>


namespace Rotor {


class Registry;


class Type
{
public:
  Type(
    const Registry & registry,
    const std::string & name,
    const MemberDefinitions & members );

  const Registry & registry() const;
  const std::string & name() const;
  const MemberDefinitions & members() const;
  MemberDefinitions & members();
  size_t size() const;

private:
  const Registry &  _registry;
  std::string       _name;
  MemberDefinitions _members;
  size_t            _size;
};


}


#endif //ROTOR_TYPE_H
