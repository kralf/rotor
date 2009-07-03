#ifndef ROTOR_MESSAGE_H
#define ROTOR_MESSAGE_H


#include "LightweightStructure.h"
#include <string>


namespace Rotor {

struct Message
{
public:
  Message( const std::string & name, Structure & data );
  Message( const std::string & name, LightweightStructure & data );
  const std::string & name() const;
  const Structure & data() const;
  Structure & data();

private:
  std::string          _name;
  LightweightStructure _data;
};


}


#endif //ROTOR_MESSAGE_H
