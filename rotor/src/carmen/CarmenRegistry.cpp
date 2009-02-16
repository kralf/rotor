#include "CarmenRegistry.h"
#include <rotor/Lock.h>
#include <rotor/Message.h>
#include <rotor/Options.h>
#include <rotor/Rotor.h>
#include <rotor/Structure.h>
#include <rotor/Time.h>
#include <rotor/TypedThread.h>
#include <carmen/ipc.h>
#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

ROTOR_REGISTRY_FACTORY( CarmenRegistry )

//------------------------------------------------------------------------------

void 
handle( MSG_INSTANCE msgInstance, void * data, void * registryPtr )
{
  CarmenRegistry * registry = reinterpret_cast<CarmenRegistry *>( registryPtr );
  Message message;
  message.name = IPC_msgInstanceName( msgInstance );

  string typeName    = registry->messageType( message.name ).name();
  Structure tmp( typeName, data, *registry );
  
  message.data = new Structure( typeName, 0, *registry );
  *(message.data) = tmp;
  registry->setMessage( message );
  FORMATTER_PTR formatter = IPC_msgInstanceFormatter( msgInstance );
  IPC_freeData( formatter, data );
}

//------------------------------------------------------------------------------

CarmenRegistry::CarmenRegistry( const string & name, Options & options)
  : Registry( name, options ),
    _registry( name, options ),
    _hasMessage( false )
{
  _message.data = 0;
  stringstream tmpName;
  tmpName << setprecision( 10 );
  tmpName << name << "_" << seconds();

  if ( IPC_isModuleConnected( name.c_str() ) == 1 ) {
    fprintf( stderr, "Module '%s' is already connected\n", name.c_str() );
    exit( 1 );
  }
  
  if ( IPC_connectModule( tmpName.str().c_str(), options.getString( name, "serverName" ).c_str() ) == IPC_Error ) {
    fprintf( stderr, "Could not connect IPC\n" );
    exit( 1 );
  }
  
  IPC_setCapacity( 4 );
  
  _dispatchThread = typedThread( *this );
  _dispatchThread->start();
}

//------------------------------------------------------------------------------

CarmenRegistry::~CarmenRegistry()
{
  delete _dispatchThread;
  if ( IPC_disconnect() == IPC_Error ) {
    fprintf( stderr, "Could not disconnect IPC\n" );
    exit( 1 );
  }
}

//------------------------------------------------------------------------------

const Type & 
CarmenRegistry::registerType( const string & definitionString )
{
  return _registry.registerType( definitionString );
}

//------------------------------------------------------------------------------

const Type & 
CarmenRegistry::operator[]( const std::string & typeName ) const
{
  return _registry[typeName];
}

//------------------------------------------------------------------------------

void 
CarmenRegistry::registerMessage( 
  const string & messageName, 
  const string & typeName )
{
  Lock lock( _ipcMutex );
  _registry.registerMessage( messageName, typeName );
  if ( ! IPC_isMsgDefined( messageName.c_str() ) ) {
    if (  IPC_defineMsg( 
            messageName.c_str(), 
            IPC_VARIABLE_LENGTH, 
            formatString( typeName ).c_str() ) 
          == IPC_Error ) 
    {
      fprintf( stderr, "Could not define message\n" );
      exit( 1 );
    }
  }
}

//------------------------------------------------------------------------------

void
CarmenRegistry::subscribeToMessage( const string & messageName )
{
  Lock lock( _ipcMutex );
  if ( IPC_subscribeData(  messageName.c_str(), handle, this ) == IPC_Error ) {
    fprintf( stderr, "Could not define message\n" );
    exit( 1 );
  }
  IPC_setMsgQueueLength( messageName.c_str(), 1 );
}

//------------------------------------------------------------------------------

const Type & 
CarmenRegistry::messageType( const string & messageName ) const
{
  return _registry.messageType( messageName );
}

//------------------------------------------------------------------------------

void 
CarmenRegistry::sendMessage( const Message & message )
{
  if (  IPC_publishData( 
          message.name.c_str(), 
          message.data->buffer() ) == IPC_Error ) 
  {
    fprintf( stderr, "Problem sending message\n" );
    exit( 1 );
  }
}

//------------------------------------------------------------------------------

Message 
CarmenRegistry::receiveMessage( double timeout ) throw( MessagingTimeout )
{
  Lock lock( _mutex );
  while ( ! _hasMessage ) {
    if ( ! _messageAvailable.wait( _mutex, timeout ) ) {
      throw MessagingTimeout( "No message was received" );
    }
  }
  _hasMessage = false;
  return _message;
}

//------------------------------------------------------------------------------

Message 
CarmenRegistry::sendReceiveMessage( const Message & message, double timeout ) 
throw( MessagingTimeout )
{
  throw MessagingTimeout( "Not implemented" );
}

//------------------------------------------------------------------------------
  
void 
CarmenRegistry::setMessage( Message & message )
{
  _mutex.lock();
  if ( _hasMessage ) {
    delete _message.data;
  }
  _message = message;
  _hasMessage = true;
  _mutex.unlock();
  _messageAvailable.notify();
}

//------------------------------------------------------------------------------

string 
CarmenRegistry::formatString( const string & typeName  ) const
{
  const MemberDefinitions & definitions = operator[]( typeName ).members();
  string result = "{ ";
  for ( int i = 0; i < definitions.size(); i++ ) {
    const MemberDefinition & definition = definitions[i];
    if ( i > 0 ) {
      result += ", ";
    }
    if ( definition.cardinality == ONE ) {
      if ( definition.type == "int8_t" ) {
        result += "byte";
      } else if ( definition.type == "uint8_t" ) {
        result += "ubyte";
      } else if ( definition.type == "int16_t" ) {
        result += "short";
      } else if ( definition.type == "uint16_t" ) {
        result += "ushort";
      } else if ( definition.type == "int32_t" ) {
        result += "int";
      } else if ( definition.type == "uint32_t" ) {
        result += "uint";
      } else if ( definition.type == "float" ) {
        result += "float";
      } else if ( definition.type == "double" ) {
        result += "double";
      } else {
        result += formatString( definition.type );
      }
    } else if ( definition.cardinality == VARIABLE ) {
      if ( definition.type == "int8_t" ) {
        result += "byte *";
      } else if ( definition.type == "uint8_t" ) {
        result += "ubyte *";
      } else if ( definition.type == "int16_t" ) {
        result += "short *";
      } else if ( definition.type == "uint16_t" ) {
        result += "ushort *";
      } else if ( definition.type == "int32_t" ) {
        result += "int *";
      } else if ( definition.type == "uint32_t" ) {
        result += "uint *";
      } else if ( definition.type == "float" ) {
        result += "float *";
      } else if ( definition.type == "double" ) {
        result += "double *";
      } else if ( definition.type == "char" ) {
        result += "string";
      }      
    }
  }
  result += " }";
  return result;
}

//------------------------------------------------------------------------------

int
CarmenRegistry::operator ()()
{
  while ( true ) {
    Lock lock( _ipcMutex ); 
    IPC_listenClear( 1 );
    Thread::sleep( 1 );
  }
  return 0;
}