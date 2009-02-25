#include "Registry.h"
#include "BasicTypes.h"
#include "Exceptions.h"
#include "FileUtils.h"
#include "Options.h"
#include "Structure.h"
#include <dlfcn.h>

using namespace Rotor;
using namespace std;


//------------------------------------------------------------------------------
  
Registry::Registry( const std::string & name, Options & options )
  : _name( name ),
    _options( options )
{
}
  
//------------------------------------------------------------------------------
  
const std::string & 
Registry::name() const
{
  return _name;
}
  
//------------------------------------------------------------------------------
  
Options & 
Registry::options() const
{
  return _options;
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
Registry::sendMessage( 
  const std::string & messageName, 
  Structure & structure )
{
  sendMessage( Message( messageName, &structure ) );
}

//------------------------------------------------------------------------------

Registry * 
Registry::load( 
  const string & className, 
  const string & registryName, 
  Options & options,
  const string & searchPath )
{
  void * handle = dlopen( findFile( "lib" + className + ".so", searchPath ).c_str() , RTLD_NOW | RTLD_GLOBAL );  
  if( handle == 0 ) {
    fprintf( stderr, "Error: %s\n", dlerror() );
    handle = dlopen( findFile( className + ".so", searchPath ).c_str() , RTLD_NOW | RTLD_GLOBAL );  
    if ( handle == 0 ) {
      throw ClassLoadingError( "Unable to open class lib for registry: " + className + "\nError:" + dlerror() );
    }
  }
  
  std::string factoryName = className + "Factory";
  RegistryFactory factory = reinterpret_cast<RegistryFactory>( dlsym( handle, factoryName.c_str() ) );

  if( factory == 0 ) {
    dlclose( handle );
    throw ClassLoadingError( "Factory function not found for registry: " + className );
  }
  return factory( registryName, options );
}
