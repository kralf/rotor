#ifndef ROTOR_DEBUG_MODULE_H
#define ROTOR_DEBUG_MODULE_H


#include "Module.h"


namespace Rotor {


class DebugModule : public Module
{
public:
  virtual bool operator()( Message & message );
};


}


#endif //ROTOR_DEBUG_MODULE_H
