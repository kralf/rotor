#ifndef ROTOR_REGISTRY_MACROS_H
#define ROTOR_REGISTRY_MACROS_H


#include "Structure.h"


#define ROTOR_DEFINE_TYPE( NAME, DEFINITION ) \
DEFINITION \
const char * ROTOR_DEFINITION_##NAME = #DEFINITION; \
NAME & operator<<( NAME & value, Rotor::Structure & structure ) { \
  if ( structure.typeData().name() != #NAME ) { \
    throw Rotor::InvalidCastError( "Structure of type " + structure.typeData().name() + " could not be cast to " + #NAME ); \
  } \
  for ( int i = 0; i < structure.size(); i++ ) structure[i];\
  value =  *( reinterpret_cast<NAME*>( structure.buffer() ) ); \
  return value; \
} \
NAME & operator<<( NAME & value, Rotor::Structure * structure ) { \
  return value << *structure; \
} \
Rotor::Structure & operator>>( NAME & value, Rotor::Structure & structure ) { \
  if ( structure.typeData().name() != #NAME ) { \
    throw Rotor::InvalidCastError( "Structure of type " + structure.typeData().name() + " could not be cast to " + #NAME ); \
  } \
  Rotor::Structure tmp( #NAME, &value, structure.registry() );\
  structure = tmp; \
} \
Rotor::Structure & operator>>( NAME & value, Rotor::Structure * structure ) { \
  return value >> *structure; \
} 

#define ROTOR_DEFINITION_STRING( NAME ) \
ROTOR_DEFINITION_##NAME

#define ROTOR_REGISTRY_FACTORY( CLASS ) \
extern "C" { \
  Rotor::Registry * CLASS##Factory( const string & name, Options & options ) \
  { \
    return new CLASS( name, options ); \
  } \
}

#endif //ROTOR_REGISTRY_MACROS_H
