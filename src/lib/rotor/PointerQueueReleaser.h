#ifndef ROTOR_POINTER_QUEUE_RELEASER_H
#define ROTOR_POINTER_QUEUE_RELEASER_H


namespace Rotor
{


template <typename T>
class PointerQueueReleaser
{
public:
  static void release( T value );
};

#include "PointerQueueReleaser.tpp"

}

#endif //ROTOR_POINTER_QUEUE_RELEASER_H
