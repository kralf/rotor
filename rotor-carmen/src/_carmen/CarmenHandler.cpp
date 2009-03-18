#include "CarmenHandler.h"
#include <carmen/CarmenRegistry.h>
#include <rotor/Logger.h>
#include <rotor/Message.h>
#include <rotor/Structure.h>
#include <rotor/TypedThread.h>
#include <cstdlib>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

CarmenHandler::CarmenHandler( CarmenRegistry & registry )
  : _registry( registry ),
    _messageQueue( 1, Queue<Message>::DISCARD_OLDEST )
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
CarmenHandler::reply( const Message & message )
{
  MSG_INSTANCE msgInstance = _instanceQueue.front();
  _instanceQueue.pop();
  _registry._ipcMutex.lock();
  if (  IPC_respondData( 
          msgInstance,
          message.name.c_str(),
          message.data->buffer() ) == IPC_Error ) 
  {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
  _registry._ipcMutex.unlock();
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

CarmenHandler::QueryInfo
CarmenHandler::dequeueQuery( double timeout )
{
  QueryInfo result = _queryQueue.next( timeout );
  _instanceQueue.push( result.second );
  _queryQueue.pop();
  return result;
}

//------------------------------------------------------------------------------

Structure * 
CarmenHandler::dequeueReply( double timeout )
{
  Message result = _replyQueue.next( timeout );
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
    IPC_listenClear( 10 );
    registry->_ipcMutex.unlock(); 
    Thread::yield();
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
  BYTE_ARRAY byteArray, 
  void * handlerPtr )
{
  void * data;
  FORMATTER_PTR formatter = IPC_msgInstanceFormatter( msgInstance );
  
  IPC_unmarshall( formatter, byteArray, &data );
  IPC_freeByteArray(byteArray);
  
  CarmenHandler * handler = reinterpret_cast<CarmenHandler *>( handlerPtr );
  Message message;
  message.name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( message.name ).name();
  Structure tmp( typeName, data, handler->registry() );
  message.data = new Structure( typeName, 0, handler->registry() );
  *(message.data) = tmp;
  
  handler->enqueueReply( message );
  IPC_freeData( formatter, data );
}
