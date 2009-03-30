#include "QueueHandler.h"

//------------------------------------------------------------------------------

QueueHandler::QueueHandler( 
  size_t mainQueueCapacity, 
  QueuePolicy mainQueuePolicy )
  : _mainQueue( mainQueueCapacity, mainQueuePolicy )
{
}

//------------------------------------------------------------------------------

QueueHandler::~QueueHandler()
{
}

//------------------------------------------------------------------------------

void 
QueueHandler::subscribeToMessage( 
  const std::string & messageName, 
  bool queueOwner,
  size_t queueCapacity, 
  QueuePolicy queuePolicy )
{
  StructureQueues::iterator it = _structureQueues.find( messageName );
  if ( it != _structureQueues.end() ) {
    StructureQueue * queue = 0;
    if ( queueOwner ) {
      queue = new StructureQueue( queueCapacity, queuePolicy );
    }
    _structureQueues[messageName] = queue;
  }
}

//------------------------------------------------------------------------------

void 
QueueHandler::enqueueMessage( Message & message )
{
}

//------------------------------------------------------------------------------

void 
QueueHandler::enqueueMessage( 
  const string & messageName, 
  Structure * structure )
{
}

//------------------------------------------------------------------------------

Message 
QueueHandler::dequeueMessage( double timeout ) throw ( MessagingTimeout )
{
}

//------------------------------------------------------------------------------

Structure * 
QueueHandler::dequeueMessage( const std::string & messageName, double timeout )
throw( MessagingTimeout )
{
}
  
private:
  typedef Queue< Message > MessageQueue;
  typedef Queue< Structure *> StructureQueue;
  typedef std::map< std::string, StructureQueue * > StructureQueues;
