#ifndef ROTOR_CARMEN_HANDLER_H
#define ROTOR_CARMEN_HANDLER_H


#include <rotor/Message.h>
#include <rotor/Registry.h>
#include <rotor/Queue.h>
#include <carmen/ipc.h>
#include <queue>
#include <map>


namespace Rotor {


class Thread;


class CarmenHandler
{
public:
  typedef std::pair< Message, MSG_INSTANCE> QueryInfo;
  
  CarmenHandler( Registry & registry );
  virtual ~CarmenHandler();
  
  Registry & registry();
  
  void reply( const Message & message );
  
  void enqueueMessage( Message & message );
  void enqueueQuery( Message & message, MSG_INSTANCE msgInstance );
  void enqueueReply( Message & message );
  
  Message dequeueMessage( double timeout );
  QueryInfo dequeueQuery( double timeout );
  Structure * dequeueReply( double timeout );

  static void * dispatcher( void * data );
  static void handleMessage( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  static void handleQuery( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  static void handleReply( MSG_INSTANCE msgInstance, BYTE_ARRAY byteArray, void * handlerPtr );
  
private:
  typedef std::queue< MSG_INSTANCE > InstanceQueue;
  Registry &         _registry;
  Thread *           _dispatchThread; 
  Queue<Message>     _messageQueue;
  Queue<QueryInfo >  _queryQueue;
  Queue<Message>     _replyQueue;
  InstanceQueue      _instanceQueue;
};


}


#endif //ROTOR_CARMEN_HANDLER_H