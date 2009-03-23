#ifndef URUS_MESSAGE_HANDLER_H
#define URUS_MESSAGE_HANDLER_H


namespace Rotor
{


class UrusMessageHandler : 
public yarp::os::TypedReaderCallback<yarp::os::Bottle>
{
public:
  virtual void onRead( yarp::os::Bottle & bottle );
  Message dequeue( double timeout );
};


}


#endif //URUS_MESSAGE_HANDLER_H