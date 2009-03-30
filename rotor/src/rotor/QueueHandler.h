#ifndef ROTOR_QUEUE_HANDLER_H
#define ROTOR_QUEUE_HANDLER_H


#include "Exceptions.h"
#include "Queue.h"
#include "Structure.h"
#include <map>
#include <string>


namespace Rotor {


class QueueHandler
{
public:
  QueueHandler( size_t mainQueueCapacity, QueuePolicy mainQueuePolicy );
  virtual ~QueueHandler();
  
  void subscribeToMessage( 
    const std::string & messageName, 
    bool queueOwner,
    size_t queueCapacity, 
    QueuePolicy queuePolicy );
  
  void enqueueMessage( Message & message );
  
  void enqueueMessage( const std::string & messageName, Structure * structure );
  
  Message dequeueMessage( double timeout ) throw ( MessagingTimeout );
  
  Structure * dequeueMessage( const std::string & messageName, double timeout )
  throw( MessagingTimeout );
  
private:
  typedef Queue< Message > MessageQueue;
  typedef Queue< Structure *> StructureQueue;
  typedef std::map< std::string, StructureQueue * > StructureQueues;
  
  MessageQueue    _mainQueue;
  StructureQueues _messageQueues;
};


}


#endif //ROTOR_QUEUE_HANDLER_H
