#ifndef ROTOR_REGISTRY_H
#define ROTOR_REGISTRY_H


#include "Exceptions.h"
#include "Type.h"
#include "Message.h"
#include <string>


namespace Rotor {


class Options;
class Structure;


class Registry
{
public:

  virtual const std::string & name() const = 0;
  
  virtual Options & options() const = 0;
  
  virtual const Type & 
  registerType( const std::string & definition ) = 0;

  virtual const Type & 
  operator[]( const std::string & typeName ) const = 0;
    
  virtual void 
  registerMessage( 
    const std::string & messageName, 
    const std::string & typeName ) = 0;

  virtual void 
  registerMessageType( 
    const std::string & messageName, 
    const std::string & definitionString );
    
  virtual void subscribeToMessage( const std::string & messageName ) = 0;  
  
  virtual void subscribeToMessageType( 
    const std::string & messageName, 
    const std::string & definitionString );  

  virtual const Type & 
  messageType( const std::string & messageName ) const = 0;
  
  virtual void sendMessage( const Message & message ) = 0;

  virtual void sendStructure( const std::string & messageName, Structure & structure );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout ) = 0;

  virtual Structure * query( const Message & message, double timeout = 0 ) throw( MessagingTimeout ) = 0;

  virtual Structure * queryStructure( const std::string & messageName, Structure & structure, double timeout = 0 ) throw( MessagingTimeout );

  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout ) = 0;

  virtual void reply( const Message & message ) = 0;
  
  virtual Structure newStructure( const std::string & string, void * address = NULL ) const;

  static Registry * load( 
    const std::string & className,
    const std::string & registryName,
    Options & options,
    const std::string & searchPath = "" );
    
private:
  typedef Registry * (* RegistryFactory)( const std::string &, Options & );
};


}


#endif //ROTOR_REGISTRY_H