#ifndef ROTOR_REMOTE_REGISTRY_H
#define ROTOR_REMOTE_REGISTRY_H


#include "BaseOptions.h"
#include "Registry.h"

namespace Rotor {


class RemoteRegistry : public Registry
{
public:
  RemoteRegistry( 
    const std::string & className, 
    const std::string & name, 
    Options & options, 
    const std::string & path );
    
  explicit RemoteRegistry( const std::string & name );
    
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

  virtual void subscribeToMessage( 
    const std::string & messageName,
    bool queueOwner = false,
    size_t queueCapacity = 0,
    QueuePolicy queuePolicy = DISCARD_OLDEST );  
  
  virtual void subscribeToQuery( const std::string & messageName );

  virtual const Type & 
  messageType( const std::string & messageName ) const;
  
  virtual void sendMessage( const Message & message );

  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout );

  virtual Message receiveMessage( 
    const std::string & messageName, 
    double timeout = 0 ) 
  throw( MessagingTimeout );

  virtual Structure query( const Message & message, double timeout = 0 ) throw( MessagingTimeout );

  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout );

  virtual void reply( const Message & message ) ;
  
protected:
  static RegistryPtr load( 
    const std::string & className,
    const std::string & registryName,
    Options & options,
    const std::string & searchPath = "" );
    
private:
  typedef RegistryPtr (* RegistryFactory)( const std::string &, Options & );

  RegistryPtr _registry;
  BaseOptions _options;
};


}


#endif //ROTOR_REMOTE_REGISTRY_H