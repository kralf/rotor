#ifndef ROTOR_MESSAGE_H
#define ROTOR_MESSAGE_H


#include "Structure.h"
#include <string>


namespace Rotor {

struct Message
{
public:
  Message( const std::string & name, Structure & data );
  const std::string & name() const;
  const Structure & data() const;
  Structure & data();

private:
  std::string _name;
  Structure   _data;
};


}


#endif //ROTOR_MESSAGE_H
