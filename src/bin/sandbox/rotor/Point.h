#ifndef POINT_H
#define POINT_H


#include <rotor/Rotor.h>


ROTOR_DEFINE_TYPE( Point,
  struct Point {
    double x;
    double y;
  };
)

ROTOR_DEFINE_TYPE( QueryMessage,
  struct QueryMessage{
    int map_is_global_likelihood;
    double timestamp;
    char *host;
  };
)


#endif //POINT_H