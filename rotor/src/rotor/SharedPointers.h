#ifndef ROTOR_SHARED_POINTERS_H
#define ROTOR_SHARED_POINTERS_H


#include <boost/shared_ptr.hpp>


namespace Rotor {


class Registry;
class Structure;
typedef boost::shared_ptr<Registry> RegistryPtr;
typedef boost::shared_ptr<Structure> StructurePtr;


}


#endif //ROTOR_SHARED_POINTERS_H
