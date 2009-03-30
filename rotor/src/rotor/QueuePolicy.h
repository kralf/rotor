#ifndef ROTOR_QUEUE_POLICY_H
#define ROTOR_QUEUE_POLICY_H


namespace Rotor {


  enum QueuePolicy {
    WAIT_WHEN_FULL = 0,
    DISCARD_NEWEST = 1,
    DISCARD_OLDEST = 2
  };


}


#endif //ROTOR_QUEUE_POLICY_H