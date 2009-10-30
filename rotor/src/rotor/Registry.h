#ifndef ROTOR_REGISTRY_H
#define ROTOR_REGISTRY_H


#include "Exceptions.h"
#include "Type.h"
#include "Message.h"
#include "QueuePolicy.h"
#include "SharedPointers.h"
#include "Structure.h"
#include <string>


namespace Rotor {

class Options;
class Structure;

/**
 * Main interface to the communicacions API.
 *
 * The registry class is the main interaction point for the user. It permits
 * to register types, define and subscribe to messages, etc. It is the only 
 * class that needs to be subclassed in order to add a new communications
 * framework/middleware.
 */
class Registry
{
public:

  Registry();
  virtual ~Registry();
  
  /**
   * Standard low level constructor.
   *
   * @param name    The name of the module this registry is associated with.
   * @param options Options to be used for registry initialization.
   */
  Registry( const std::string & name, Options & options );


  /**
   * Returns the name of the module associated with this registry.
   */
  virtual const std::string & name() const = 0;
  
  /**
   * Returns the Options associated with this registry.
   */
  virtual Options & options() const = 0;
  
  /**
   * Registers a new type from a definition string.
   *
   * @param definition The definition string is a stripped down version of a 
   *                   standard c++ struct statement. See the user's guide for
   *                   more info on this format.
   * @return The Type object.
   */
  virtual const Type & 
  registerType( const std::string & definition ) = 0;

  
  /**
   * Gets the Type object corresponding to a type name.
   *
   * @param typeName The name of the type to retrieve.
   */
  virtual const Type & 
  operator[]( const std::string & typeName ) const = 0;
    
  /**
   * Registers a new message name and type.
   *
   * @param messageName The name of the message.
   * @param typeName    The message's type name.
   */
  virtual void 
  registerMessage( 
    const std::string & messageName, 
    const std::string & typeName ) = 0;

  /**
   * Registers a new message name and and type, registering the type from a
   * definition string.
   *
   * @param messageName      The name of the message.
   * @param definitionString The type definition string.
   */
  virtual void 
  registerMessageType( 
    const std::string & messageName, 
    const std::string & definitionString );
    
  /**
   * Subscribes to a given message name.
   *
   * Instructs the registry to accept the given message name. It is necessary
   * to subscribe to a message before using receiveMessage. At this moment it is
   * mandatory to register the message and type before subscribing to it, 
   * however this may change for higher-level registries like RemoteRegistry.
   * @param messageName   The name of the message to subscribe to.
   * @param queueOwner    Indicates if this message should have its own queue
   *                      and use receiveMessage with a message name parameter.
   * @param queueCapacity The capacity for this message's queue or 0 for no
   *                      limit.
   * @param queuePolicy   The QueuePolicy for this message's queue.
   */
  virtual void subscribeToMessage( 
    const std::string & messageName,
    bool queueOwner = false,
    size_t queueCapacity = 0,
    QueuePolicy queuePolicy = DISCARD_OLDEST ) = 0;  
  
  /**
   * Convenience method that subscribes to a message name, declaring and 
   * registering the corresponding type.
   *
   * @param messageName      The name of the message to subscribe to.
   * @param definitionString The type definition string.
   */
  virtual void subscribeToMessageType( 
    const std::string & messageName, 
    const std::string & definitionString );  

  /**
   * Subscribes to a query.
   *
   * Instructs the registry that this module is able to reply to synchronous 
   * queries. As in the case of messages, it is required at this moment to 
   * register the query message name and type.
   * @param messageName The name of the query.
   */
  virtual void subscribeToQuery( const std::string & messageName ) = 0;  
  
  
  /**
   * Convenience method that subscribes to a query, declaring and registering 
   * the corresponding type.
   *
   * @param messageName      The name of the query.
   * @param definitionString The type definition string.
   */
  virtual void subscribeToQueryType( 
    const std::string & messageName, 
    const std::string & definitionString );  

  /**
   * Returns the type object of a given message.
   *
   * @param messageName The name of the message.
   */
  virtual const Type & 
  messageType( const std::string & messageName ) const = 0;

  /**
   * Returns the frequency of a given message in [Hz].
   *
   * @param messageName The name of the message.
   */
  virtual double
  messageFrequency( const std::string & messageName ) const;

  /**
   * Sends the given Message.
   *
   * @param message The message to send.
   */
  virtual void sendMessage( const Message & message ) = 0;

  /**
   * Convenience function that makes a Message object and sends it.
   *
   * @param messageName The message name.
   * @param structure   The message content.
   */
  virtual void sendStructure( const std::string & messageName, Structure & structure );

  /**
   * Waits for a message to arrive, with an optional timeout.
   * 
   * @param timeout The maximum waiting time in seconds or 0 for no timeout.
   * @return The next message in the reception queue.
   * @throw MessagingTimeout If the timeout is exceeded.
   */
  virtual Message receiveMessage( double timeout = 0 ) throw( MessagingTimeout ) = 0;

  /**
   * Waits for a message having a specific name to arrive, with an optional 
   * timeout.
   * 
   * @param messageName The message name.
   * @param timeout     The maximum waiting time in seconds or 0 for no timeout.
   * @return The next message in the reception queue.
   * @throw MessagingTimeout If the timeout is exceeded.
   */
  virtual Message receiveMessage( 
    const std::string & messageName, 
    double timeout = 0 ) 
  throw( MessagingTimeout ) = 0;

  /**
   * Sends a query and waits for the corresponding reply, with an optional 
   * timeout.
   * 
   * @param message The query message.
   * @param timeout The maximum waiting time in seconds or 0 for no timeout.
   * @return The next query in the reception queue.
   * @throw MessagingTimeout If the timeout is exceeded.
   */
  virtual Structure query( const Message & message, double timeout = 0 ) throw( MessagingTimeout ) = 0;

  /**
   * Convenience method that makes a query message from the given message name 
   * and structure.
   * 
   * @param messageName The query message name.
   * @param structure   The message's data.
   * @param timeout     The maximum waiting time in seconds or 0 for no timeout.
   * @return The next query in the reception queue.
   * @throw MessagingTimeout If the timeout is exceeded.
   */
  virtual Structure queryStructure( const std::string & messageName, Structure & structure, double timeout = 0 ) throw( MessagingTimeout );

  /**
   * Waits for a query to arrive, with an optional timeout.
   * 
   * @param timeout The maximum waiting time in seconds or 0 for no timeout.
   * @return The next query in the reception queue.
   * @throw MessagingTimeout If the timeout is exceeded.
   */
  virtual Message receiveQuery( double timeout = 0 ) throw( MessagingTimeout ) = 0;

  /**
   * Replies a query.
   *
   * NOTE: Queries should be replied in the same order they were received i.e.
   * in FIFO order. 
   */
  virtual void reply( const Message & message ) = 0;
  
  /**
   * Creates a new Structure object of the given type name.
   *
   * @param typeName The name of the structure's type.
   * @param address  The address of the c++ object this Structure points to or
   *                 0 if it should own its memory.
   */
  virtual Structure newStructure( const std::string & typeName, void * address = NULL ) const;
    
};


}


#endif //ROTOR_REGISTRY_H