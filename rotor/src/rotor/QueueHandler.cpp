#include "QueueHandler.h"
#include "Logger.h"

using namespace Rotor;
using namespace std;

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
  if ( queueOwner ) {
    StructureQueues::iterator it = _structureQueues.find( messageName );
    if ( it != _structureQueues.end() ) {
      StructureQueue * queue = 0;
      queue = new StructureQueue( queueCapacity, queuePolicy );
      _structureQueues[messageName] = queue;
    }
  }
}

//------------------------------------------------------------------------------

void 
QueueHandler::enqueueMessage( Message & message )
{
  //TODO: Check if not owning queues have been subscribed
  StructureQueues::iterator it = _structureQueues.find( message.name );
  if ( it != _structureQueues.end() ) {
    Logger::debug( "Enqueing owned " + message.name );
    it->second->push( message.data );
  } else {
    Logger::debug( "Enqueing not owned " + message.name );
    _mainQueue.push( message );
  }
}

//------------------------------------------------------------------------------

Message 
QueueHandler::dequeueMessage( double timeout ) throw ( TimeoutException )
{
  return _mainQueue.popNext( timeout );
}

//------------------------------------------------------------------------------

Structure * 
QueueHandler::dequeueMessage( const std::string & messageName, double timeout )
throw( TimeoutException )
{
  StructureQueues::iterator it = _structureQueues.find( messageName );
  if ( it != _structureQueues.end() ) {
    return it->second->popNext( timeout );
  }
}
