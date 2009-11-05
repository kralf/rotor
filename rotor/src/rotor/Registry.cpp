#include "Registry.h"
#include "BasicTypes.h"
#include "Exceptions.h"
#include "FileUtils.h"
#include "Logger.h"
#include "Options.h"
#include "Structure.h"
#include "NullDeleter.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

Registry::Registry()
{
}

//------------------------------------------------------------------------------

Registry::~Registry()
{
}

//------------------------------------------------------------------------------

Registry::Registry( const std::string & name, Options & options )
{
  Logger::setLevel(
    static_cast<Logger::Level>( options.getInt( name, "loggingLevel",  3 ) ),
    name
  );
}

//------------------------------------------------------------------------------

void 
Registry::registerMessageType( 
  const std::string & messageName, 
  const std::string & definitionString )
{
  const Type & type = registerType( definitionString );
  registerMessage( messageName, type.name() );
}

//------------------------------------------------------------------------------

void 
Registry::subscribeToMessageType( 
  const std::string & messageName, 
  const std::string & definitionString )
{
  registerMessageType( messageName, definitionString );
  subscribeToMessage( messageName );
}

//------------------------------------------------------------------------------

void 
Registry::subscribeToQueryType( 
  const std::string & messageName, 
  const std::string & definitionString )
{
  registerMessageType( messageName, definitionString );
  subscribeToQuery( messageName );
}

//------------------------------------------------------------------------------

double
Registry::messageFrequency(
  const std::string & /* messageName */ ) const
{
  return 0;
}

//------------------------------------------------------------------------------
  
void 
Registry::sendStructure( 
  const std::string & messageName, 
  Structure & structure )
{
  sendMessage( Message( messageName, structure ) );
}

//------------------------------------------------------------------------------

Structure
Registry::queryStructure( 
  const std::string & messageName, 
  Structure & structure,
  double timeout )
 throw( MessagingTimeout )  
{
  return query( Message( messageName, structure ), timeout );
}

//------------------------------------------------------------------------------

Structure
Registry::newStructure( const std::string & type, void * address ) const
{
  return Structure( type, address, *this );
}
