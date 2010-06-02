#ifndef ROTOR_DEFAULT_MODULE_H
#define ROTOR_DEFAULT_MODULE_H


#include <rotor/MemberDefinition.h>
#include <rotor/Module.h>


namespace Rotor {


class Message;


class DefaultModule : public Module
{
public: 
  virtual bool operator()();
  virtual bool operator()( Message & message );
};


}


#endif //ROTOR_DEFAULT_MODULE_H
