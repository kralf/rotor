#ifndef ROTOR_YARP_CONVERSIONS_H
#define ROTOR_YARP_CONVERSIONS_H


#include <rotor/AbstractVariable.h>
#include <rotor/Message.h>
#include <rotor/Type.h>
#include <yarp/os/Bottle.h>


namespace Rotor {

void 
appendToBottle( yarp::os::Bottle & bottle, const AbstractVariable & value );

Structure * 
bottleToStructure( const yarp::os::Bottle & bottle, const Type & type );
  
};


#endif //ROTOR_YARP_CONVERSIONS_H