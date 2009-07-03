#ifndef ROTOR_QUEUE_HANDLER_H
#define ROTOR_QUEUE_HANDLER_H


#include "Exceptions.h"
#include "Queue.h"
#include "Message.h"
#include "PointerQueueReleaser.h"
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
  
  Message dequeueMessage( double timeout ) throw ( TimeoutException );
  
  StructurePtr dequeueMessage( const std::string & messageName, double timeout )
  throw( TimeoutException );
  
private:
  typedef Queue< Message > MessageQueue;
  typedef Queue< StructurePtr > StructureQueue;
  typedef std::map< std::string, StructureQueue * > StructureQueues;
  
  MessageQueue    _mainQueue;
  StructureQueues _structureQueues;
};


}


#endif //ROTOR_QUEUE_HANDLER_H
