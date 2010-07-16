#ifndef ROTOR_YARP_HANDLER_H
#define ROTOR_YARP_HANDLER_H


namespace Rotor
{


class 3Handler :
public yarp::os::TypedReaderCallback<yarp::os::Bottle>
{
public:
  virtual void onRead( yarp::os::Bottle & bottle );
  Message dequeue( double timeout );
};


}


#endif //ROTOR_YARP_HANDLER_H