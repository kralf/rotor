#ifndef ROTOR_TIME_H
#define ROTOR_TIME_H


#include <sys/time.h>


namespace Rotor {
  inline unsigned int microseconds();
  inline double seconds();
}


#include "Time.tpp"


#endif //ROTOR_TIME_H
