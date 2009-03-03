#ifndef ROTOR_CORE_MESSAGES_H
#define ROTOR_CORE_MESSAGES_H


#include "RegistryMacros.h"


namespace Rotor {


  ROTOR_DEFINE_TYPE( RemoteCommand,
    struct RemoteCommand {
      char * command;
      char * arguments;
    };
  )

  ROTOR_DEFINE_TYPE( OptionString,
    struct OptionString {
      char * value;
    };
  )

}


#endif //ROTOR_CORE_MESSAGES_H