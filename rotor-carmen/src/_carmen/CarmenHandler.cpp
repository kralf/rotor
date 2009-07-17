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
CarmenHandler::reply( const Message & message )
{
  MSG_INSTANCE msgInstance = _instanceQueue.front();
  _instanceQueue.pop();
  _registry._ipcMutex.lock();
  if (  IPC_respondData( 
          msgInstance,
          message.name().c_str(),
          message.data().buffer() ) == IPC_Error ) 
  {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
  _registry._ipcMutex.unlock();
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

CarmenHandler::QueryInfo
CarmenHandler::dequeueQuery( double timeout )
{
  QueryInfo result = _queryQueue.next( timeout );
  _instanceQueue.push( result.second );
  _queryQueue.pop();
  return result;
}

//------------------------------------------------------------------------------

LightweightStructure
CarmenHandler::dequeueReply( double timeout )
{
  Message result = _replyQueue.next( timeout );
  _replyQueue.pop();
  return result.data();
}

//------------------------------------------------------------------------------

void * 
CarmenHandler::dispatcher( void * data )
{
  CarmenRegistry * registry = reinterpret_cast<CarmenRegistry *>( data );
  while ( true ) {
    while( ! registry->_outputQueue.empty() ) {
      Lock lock( registry->_ipcMutex ); 
      Message message = registry->_outputQueue.popNext();
      if (  IPC_publishData( 
              message.name().c_str(), 
              message.data().buffer() ) == IPC_Error ) 
      {
        fprintf( stderr, "Problem sending message\n" );
        exit( 1 );
      } 
      Logger::spam( "Carmen Sent:" + message.name() );      
    }
    registry->_ipcMutex.lock(); 
    IPC_listen( 10 );
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
//   Lock lock( handler->_registry._ipcMutex );
  string name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( name ).name();
  Structure tmp( typeName, data, handler->registry() );
  StructurePtr newData( StructurePtr( new Structure( typeName, 0, handler->registry() ) ) ); 
  (*newData) = tmp;
  LightweightStructure s( newData );
  Message message( name, s );

  handler->_registry._queueHandler.enqueueMessage( message );
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
//   Lock lock( handler->_registry._ipcMutex );
  Logger::spam( "Handling query" );
  string name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( name ).name();
  Structure tmp( typeName, data, handler->registry() );
  StructurePtr newData( StructurePtr( new Structure( typeName, 0, handler->registry() ) ) ); 
  (*newData) = tmp;
  LightweightStructure s( newData );
  Message message( name, s );
 
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
  CarmenHandler * handler = reinterpret_cast<CarmenHandler *>( handlerPtr );
//   Lock lock( handler->_registry._ipcMutex );
  Logger::spam( "Handling reply" );
  void * data;
  FORMATTER_PTR formatter = IPC_msgInstanceFormatter( msgInstance );
  
  IPC_unmarshall( formatter, byteArray, &data );
  IPC_freeByteArray(byteArray);
  
  string name = IPC_msgInstanceName( msgInstance );

  string typeName    = handler->registry().messageType( name ).name();
  Structure tmp( typeName, data, handler->registry() );
  StructurePtr newData( StructurePtr( new Structure( typeName, 0, handler->registry() ) ) ); 
  (*newData) = tmp;
  LightweightStructure s( newData );
  Message message( name, s );
  
  handler->enqueueReply( message );
  IPC_freeData( formatter, data );
}
