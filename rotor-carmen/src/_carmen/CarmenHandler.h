#ifndef ROTOR_CARMEN_HANDLER_H
#define ROTOR_CARMEN_HANDLER_H


#include <rotor/Message.h>
#include <rotor/Registry.h>
#include <rotor/Queue.h>
#include <carmen/ipc.h>
#include <queue>
#include <map>


namespace Rotor {


class CarmenRegistry;
class Thread;


class CarmenHandler
{
public:
  typedef std::pair< Message, MSG_INSTANCE> QueryInfo;
  
  CarmenHandler( CarmenRegistry & registry );
  virtual ~CarmenHandler();
  
  Registry & registry();
  
  void reply( const Message & message );
  
  void enqueueQuery( Message & message, MSG_INSTANCE msgInstance );
  void enqueueReply( Message & message );
  
  QueryInfo dequeueQuery( double timeout );
  Structure dequeueReply( double timeout );

  static void * dispatcher( void * data );
  static void handleMessage( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  static void handleQuery( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  static void handleReply( MSG_INSTANCE msgInstance, BYTE_ARRAY byteArray, void * handlerPtr );
  
private:
  typedef std::queue< MSG_INSTANCE > InstanceQueue;
  CarmenRegistry &   _registry;
  Thread *           _dispatchThread; 
  Queue<QueryInfo >  _queryQueue;
  Queue<Message>     _replyQueue;
  InstanceQueue      _instanceQueue;
};


}


#endif //ROTOR_CARMEN_HANDLER_H