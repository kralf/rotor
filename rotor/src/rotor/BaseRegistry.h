#ifndef ROTOR_BASE_REGISTRY_H
#define ROTOR_BASE_REGISTRY_H


#include "Exceptions.h"
#include "Registry.h"
#include <map>


namespace Rotor {


class Options;
class Type;


class BaseRegistry : public Registry
{
public:
  BaseRegistry( const std::string & name, Options & options );
  virtual ~BaseRegistry();
  
  virtual const Type & 
  registerType( const std::string & definitionString );
  
  virtual const Type & 
  operator[]( const std::string & typeName ) const;

  virtual void 
  registerMessage( 
    const std::string & messageName, 
    const std::string & typeName );
    
  virtual void subscribeToMessage( const std::string & messageName );

  virtual const Type & 
  messageType( const std::string & messageName ) const;

  virtual void sendMessage( const Message & message );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout );

  virtual Message query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );
  
  virtual void reply( const Message & message );

private:
  typedef std::map<std::string, Registry*> Registries;
  typedef std::map<std::string, std::string>   StringMap;
  typedef std::map<std::string, Type* > Types;

  BaseRegistry( const BaseRegistry & );

  Registries              _registries;
  StringMap               _definitionStrings;
  Types                   _types;
  StringMap               _messages;
};

}


#endif //ROTOR_BASE_REGISTRY_H
