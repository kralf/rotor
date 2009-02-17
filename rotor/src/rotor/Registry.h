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
  Registry( const std::string & name, Options & options );
  
  virtual const std::string & name() const;
  
  virtual Options & options() const;
  
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

  virtual const Type & 
  messageType( const std::string & messageName ) const = 0;
  
  virtual void sendMessage( const Message & message ) = 0;

  virtual void sendMessage( const std::string & messageName, Structure & structure );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout ) = 0;

  virtual Message query( const Message & message, double timeout = 0 ) throw( MessagingTimeout ) = 0;
  
  virtual void reply( const Message & message ) = 0;

  static Registry * load( 
    const std::string & className,
    const std::string & registryName,
    Options & options,
    const std::string & searchPath = "" );

private:
  typedef Registry * (* RegistryFactory)( const std::string &, Options & );
  std::string _name;
  Options &   _options;
};


}


#endif //ROTOR_REGISTRY_H