#ifndef ROTOR_REGISTRY_MACROS_H
#define ROTOR_REGISTRY_MACROS_H


#include "Structure.h"
#include "Type.h"


#define ROTOR_DEFINE_TYPE( NAME, DEFINITION ) \
DEFINITION \
const char * ROTOR_DEFINITION_##NAME = #DEFINITION; \
inline \
NAME & operator<<( NAME & value, Rotor::Structure & structure ) { \
  if ( structure.typeData().name() != #NAME ) { \
    throw Rotor::InvalidCastError( "Structure of type " + structure.typeData().name() + " could not be cast to " + #NAME ); \
  } \
  value =  *( reinterpret_cast<NAME*>( structure.buffer() ) ); \
  return value; \
} \
inline \
Rotor::Structure & operator>>( NAME & value, Rotor::Structure & structure ) { \
  if ( structure.typeData().name() != #NAME ) { \
    throw Rotor::InvalidCastError( "Structure of type " + structure.typeData().name() + " could not be cast to " + #NAME ); \
  } \
  Rotor::Structure tmp( #NAME, &value, structure.registry() );\
  structure = tmp; \
  return structure; \
} 

#define ROTOR_VARIABLE( NAME, STRUCTURE ) \
* reinterpret_cast<NAME *>( STRUCTURE.buffer() )

#define ROTOR_DEFINITION_STRING( NAME ) \
ROTOR_DEFINITION_##NAME

#define ROTOR_REGISTRY_FACTORY( CLASS ) \
extern "C" { \
  Rotor::RegistryPtr CLASS##Factory( const string & name, Options & options ) \
  { \
    return RegistryPtr( new CLASS( name, options ) ); \
  } \
}

#endif //ROTOR_REGISTRY_MACROS_H
