#ifndef ROTOR_CARMEN_HANDLER_H
#define ROTOR_CARMEN_HANDLER_H


#include <rotor/Message.h>
#include <rotor/Registry.h>
#include <rotor/Queue.h>
#include <carmen/ipc.h>


namespace Rotor {


class Thread;


class CarmenHandler
{
public:
  CarmenHandler( Registry & registry );
  virtual ~CarmenHandler();
  
  Registry & registry();
  
  void enqueueMessage( Message & message );
  void enqueueQuery( Message & message, MSG_INSTANCE msgInstance );
  void enqueueReply( Message & message );
  
  Message dequeueMessage( double timeout );
  std::pair<Message, MSG_INSTANCE> dequeueQuery( double timeout );
  Structure * dequeueReply( double timeout );

  static void * dispatcher( void * data );
  static void handleMessage( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  static void handleQuery( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  static void handleReply( MSG_INSTANCE msgInstance, void * data, void * handlerPtr );
  
private:
  Registry &                                 _registry;
  Thread *                                   _dispatchThread; 
  Queue<Message>                             _messageQueue;
  Queue<std::pair< Message, MSG_INSTANCE > > _queryQueue;
  Queue<Message>                             _replyQueue;
  
};


}


#endif //ROTOR_CARMEN_HANDLER_H