#ifndef ROTOR_SERIALIZATION_H
#define ROTOR_SERIALIZATION_H


#include <rotor/AbstractVariable.h>
#include <rotor/Message.h>
#include <rotor/Registry.h>


namespace Rotor {
const std::string marshall( const Message & message );
Message unmarshall( const Registry & registry, const std::string & data );
const std::string marshall( const AbstractVariable & variable );
}


#endif //ROTOR_SERIALIZATION_H
