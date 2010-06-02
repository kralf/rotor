#include "Point.h"
#include <rotor/BaseOptions.h>
#include <rotor/Rotor.h>
#include <rotor/Structure.h>
#include <broadcast/BroadcastRegistry.h>
#include <rotor/Serialization.h>
#include <iostream>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

ROTOR_DEFINE_TYPE( RegistryRequest,
  struct RegistryRequest {
    char * moduleName;
  };
)

ROTOR_DEFINE_TYPE( RegistryResponse,
  struct RegistryResponse {
    uint8_t nameOk;
    char *  registryClass;
  };
)

//------------------------------------------------------------------------------

int main( int argc, char * argv[] ) {
  BaseOptions options;
  options.setInt( argv[1], "listenPort", 60710 );
  BroadcastRegistry registry( argv[1], options );
  
  registry.registerType( ROTOR_DEFINITION_STRING( RegistryRequest ) );
  registry.registerType( ROTOR_DEFINITION_STRING( RegistryResponse ) );
  registry.registerMessage( "REGISTRY_RESPONSE", "RegistryResponse" );

  Message request( "REGISTRY_REQUEST", new Structure( "RegistryRequest", 0, registry ) );
  try {
    Structure * response  = registry.query( request );
    RegistryResponse rep;
    rep << response;
    
    printf( "nameOk          = %i\n", rep.nameOk );
    printf( "registryClass = %s\n", rep.registryClass );
  
    delete response;
  } catch ( MessagingTimeout ) {
    fprintf( stderr, "Failed to contact central registry\n" );
  }
  delete request.data;
}
