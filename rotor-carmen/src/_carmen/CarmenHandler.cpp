#include "CarmenHandler.h"
#include <carmen/CarmenRegistry.h>
#include <rotor/Message.h>
#include <rotor/Structure.h>
#include <rotor/TypedThread.h>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

CarmenHandler::CarmenHandler( Registry & registry )
  : _registry( registry )
{
  _dispatchThread = new Thread();
  _dispatchThread->start( CarmenHandler::dispatcher, &registry );
}

//------------------------------------------------------------------------------

CarmenHandler::~CarmenHandler()
{
  delete _dispatchThread;
}

//------------------------------------------------------------------------------

Registry & 
CarmenHandler::registry()
{
  return _registry;
}
  
//------------------------------------------------------------------------------

void 
CarmenHandler::enqueueMessage( Message & message )
{
  _messageQueue.push( message );
}

//------------------------------------------------------------------------------

void 
CarmenHandler::enqueueQuery( Message & message, MSG_INSTANCE msgInstance )
{
  _queryQueue.push( make_pair( message, msgInstance ) );
}
  
//------------------------------------------------------------------------------

void 
CarmenHandler::enqueueReply( Message & message )
{
  _replyQueue.push( message );
}
  
//------------------------------------------------------------------------------

Message 
CarmenHandler::dequeueMessage( double timeout )
{
  Message result = _messageQueue.next( timeout );
  _messageQueue.pop();
  return result;
}

//------------------------------------------------------------------------------

pair< Message, MSG_INSTANCE >
CarmenHandler::dequeueQuery( double timeout )
{
  pair< Message, MSG_INSTANCE> result = _queryQueue.next( timeout );
  _queryQueue.pop();
  return result;
}

//------------------------------------------------------------------------------

Structure * 
CarmenHandler::dequeueReply( double timeout )
{
  Message result = _messageQueue.next( timeout );
  _replyQueue.pop();
  return result.data;
}

//------------------------------------------------------------------------------

void * 
CarmenHandler::dispatcher( void * data )
{
  CarmenRegistry * registry = reinterpret_cast<CarmenRegistry *>( data );
  while ( true ) {
    registry->_ipcMutex.lock(); 
    IPC_listenClear( 100 );
    registry->_ipcMutex.unlock(); 
    Thread::sleep( 0.01 );
  }
  return 0;
}

//------------------------------------------------------------------------------

void 
CarmenHandler::handleMessage( 
  MSG_INSTANCE msgInstance, 
  void * data, 
  void * handlerPtr )
{
  CarmenHandler * handler = reinterpret_cast<CarmenHandler *>( handlerPtr );
  Message message;
  message.name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( message.name ).name();
  Structure tmp( typeName, data, handler->registry() );
  message.data = new Structure( typeName, 0, handler->registry() );
  *(message.data) = tmp;

  handler->enqueueMessage( message );
  FORMATTER_PTR formatter = IPC_msgInstanceFormatter( msgInstance );
  IPC_freeData( formatter, data );
}

//------------------------------------------------------------------------------

void 
CarmenHandler::handleQuery( 
  MSG_INSTANCE msgInstance, 
  void * data, 
  void * handlerPtr )
{
  CarmenHandler * handler = reinterpret_cast<CarmenHandler *>( handlerPtr );
  Message message;
  message.name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( message.name ).name();
  Structure tmp( typeName, data, handler->registry() );
  message.data = new Structure( typeName, 0, handler->registry() );
  *(message.data) = tmp;
  
  IPC_delayResponse( msgInstance );
  handler->enqueueQuery( message, msgInstance );
  FORMATTER_PTR formatter = IPC_msgInstanceFormatter( msgInstance );
  IPC_freeData( formatter, data );
}

//------------------------------------------------------------------------------

void 
CarmenHandler::handleReply( 
  MSG_INSTANCE msgInstance, 
  void * data, 
  void * handlerPtr )
{
  CarmenHandler * handler = reinterpret_cast<CarmenHandler *>( handlerPtr );
  Message message;
  message.name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( message.name ).name();
  Structure tmp( typeName, data, handler->registry() );
  message.data = new Structure( typeName, 0, handler->registry() );
  *(message.data) = tmp;
  
  IPC_delayResponse( msgInstance );
  handler->enqueueReply( message );
  FORMATTER_PTR formatter = IPC_msgInstanceFormatter( msgInstance );
  IPC_freeData( formatter, data );
}
