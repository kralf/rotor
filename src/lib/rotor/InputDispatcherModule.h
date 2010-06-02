#ifndef ROTOR_INPUT_DISPATCHER_MODULE_H
#define ROTOR_INPUT_DISPATCHER_MODULE_H


#include "DispatcherModule.h"


namespace Rotor {


class InputDispatcherModule : public DispatcherModule
{
public:
  InputDispatcherModule( ModuleRunner & runner );
  virtual void * operator()();
};


}


#endif //ROTOR_INPUT_DISPATCHER_MODULE_H
