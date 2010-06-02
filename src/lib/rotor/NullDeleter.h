#ifndef ROTOR_NULL_DELETER_H
#define ROTOR_NULL_DELETER_H


namespace Rotor{


class NullDeleter
{
public:
  void operator()(void const *) const;
};


}


#endif //ROTOR_NULL_DELETER_H
