#ifndef ROTOR_CONVERSION_H
#define ROTOR_CONVERSION_H


#include <sstream>
#include <iomanip>

namespace Rotor {


template <typename T>
std::string toString( const T & value );

template <typename T>
T fromString( const std::string & input );


#include "Conversion.tpp"

}


#endif //ROTOR_CONVERSION_H
