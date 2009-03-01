#include "BaseRegistry.h"
#include "BasicTypes.h"
#include "Exceptions.h"
#include "Options.h"
#include "StructureParser.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

BaseRegistry::BaseRegistry( const std::string & name, Options & options )
  : Registry( name, options )
{
  _registries[ "default" ] = this;
}

//------------------------------------------------------------------------------

BaseRegistry::~BaseRegistry()
{
  for ( Types::iterator i = _types.begin(); 
        i != _types.end();
        i++ )
  {
    delete i->second;
  }
  for ( Registries::iterator i = _registries.begin(); 
        i != _registries.end(); 
        i++ ) 
  {
    if ( i->first != "default" ) {
      delete i->second;
    }
  }
}

//------------------------------------------------------------------------------

const Type & 
BaseRegistry::registerType( const string & definitionString )
{
  Rotor::StructureParser parser;
  if ( ! parser.parse( definitionString ) ) {
    throw ParsingError( "Problem parsing type: " + parser.error() );
  }
  Types::const_iterator iType = _types.find( parser.name() );
  if ( iType != _types.end() ) {
    if ( ! equal( parser.members().begin(), parser.members().end(), iType->second->members().begin() ) ) {
      throw TypeError( "Type definitions mismatch" );
    }
    return *( iType->second );
  } else {
    Type * type = new Type( *this, parser.name(), parser.members() );
    _types[type->name()] = type;
    return * type;
  }
}

//------------------------------------------------------------------------------

const Type & 
BaseRegistry::operator[]( const std::string & typeName ) const
{
  Types::const_iterator iType = _types.find( typeName );
  if ( iType == _types.end() ) {
    throw TypeError( "No type definition for type: " + typeName );
  }
  return *( iType->second );
}

//------------------------------------------------------------------------------

void 
BaseRegistry::registerMessage( 
  const string & messageName, 
  const string & typeName )
{
  StringMap::const_iterator iName = _messages.find( messageName );
  if ( iName != _messages.end() && iName->second != typeName ) {
    throw TypeError( "Message is registered with a different type: " + iName->second );
  }
  
  Types::const_iterator iType = _types.find( typeName );
  if ( iType == _types.end() ) {
    throw TypeError( "No type definition for type: " + typeName );
  }
  _messages[messageName] = typeName;
}

//------------------------------------------------------------------------------

void 
BaseRegistry::subscribeToMessage( const std::string & messageName )
{
}

//------------------------------------------------------------------------------

const Type & 
BaseRegistry::messageType( const string & messageName ) const
{
  StringMap::const_iterator iName = _messages.find( messageName );
  if ( iName == _messages.end() ) {
    throw MessagingError( "Message is is not defined: " + messageName );
  }
  Types::const_iterator iType = _types.find( iName->second );
  return *( iType->second );
}

//------------------------------------------------------------------------------

void 
BaseRegistry::sendMessage( const Message & message )
{
}

//------------------------------------------------------------------------------

Message 
BaseRegistry::receiveMessage( double timeout )  throw( MessagingTimeout )
{
}

//------------------------------------------------------------------------------

Message 
BaseRegistry::query( const Message & message, double timeout )  
throw( MessagingTimeout )
{
}
//------------------------------------------------------------------------------

void
BaseRegistry::reply( const Message & message )  
{
}
