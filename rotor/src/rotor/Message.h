#ifndef ROTOR_MESSAGE_H
#define ROTOR_MESSAGE_H


#include <string>


namespace Rotor {

class Structure;

struct Message
{
  Message();
  Message( const std::string & name, Structure * data );
  std::string name;
  Structure * data;
};


}


#endif //ROTOR_MESSAGE_H
