#ifndef ROTOR_DEFAULT_QUEUE_RELEASER_H
#define ROTOR_DEFAULT_QUEUE_RELEASER_H


namespace Rotor
{


template <typename T>
class DefaultQueueReleaser
{
public:
  static void release( T value );
};

#include "DefaultQueueReleaser.tpp"

}

#endif //ROTOR_DEFAULT_QUEUE_RELEASER_H
