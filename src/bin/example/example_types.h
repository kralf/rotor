#ifndef TYPES_H
#define TYPES_H


#include <rotor/Rotor.h>


ROTOR_DEFINE_TYPE( Point,
  struct Point {
    double x;
    double y;
  };
)

ROTOR_DEFINE_TYPE( point_6d_t,
  struct point_6d_t {
    double x;
    double y;
    double z;
    double phi;
    double theta;
    double psi;
  };
)

ROTOR_DEFINE_TYPE( pos_message,
  struct pos_message {
    point_6d_t pose;
    double velocity;
    double steering_angle;
    double timestamp;
    char * host;
  };
)

ROTOR_DEFINE_TYPE( steering_message,
  struct steering_message {
    double velocity;
    double steeringangle;
    double timestamp;
    char* host;
  };
)


#endif //TYPES_H