#ifndef ROTOR_REMOTE_REGISTRY_H
#define ROTOR_REMOTE_REGISTRY_H


#include "Registry.h"


namespace Rotor {


class RemoteRegistry : public Registry
{
public:
  RemoteRegistry( const std::string & name );
  
  virtual const std::string & name() const;
  
  virtual Options & options() const;
  
  virtual const Type & 
  registerType( const std::string & definition );

  virtual const Type & 
  operator[]( const std::string & typeName ) const;
    
  virtual void 
  registerMessage( 
    const std::string & messageName, 
    const std::string & typeName );

  virtual void subscribeToMessage( const std::string & messageName );  
  
  virtual void subscribeToQuery( const std::string & messageName );

  virtual const Type & 
  messageType( const std::string & messageName ) const;
  
  virtual void sendMessage( const Message & message );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout );

  virtual Structure * query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );

  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout );

  virtual void reply( const Message & message ) ;
  
private:
  Registry * _registry;
};


}


#endif //ROTOR_REMOTE_REGISTRY_H