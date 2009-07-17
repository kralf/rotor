%module rotorc
%{
#include <rotor/NetUtils.h>
#include <rotor/BaseRegistry.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/BaseOptions.h>
#include <rotor/BasicTypes.h>
#include <rotor/Structure.h>
#include <rotor/StructureArray.h>
#include <rotor/Exceptions.h>
#include <rotor/FileUtils.h>
#include <rotor/Time.h>
#include <rotor/Logger.h>
#include <rotor/Serialization.h>
#include <rotor/Thread.h>
#include <rotor/QueuePolicy.h>
%}

//------------------------------------------------------------------------------

%include std_string.i
%include std_vector.i
%include <rotor/Exceptions.h>

//------------------------------------------------------------------------------

%include exception.i
%exception {
  try {
    $action
  } SWIG_CATCH_STDEXCEPT
}

//------------------------------------------------------------------------------


namespace boost {
  template<class T> class shared_ptr
  {
  public:
    T * operator-> () const;
    T & operator* () const;
  };
}


namespace Rotor {
  class LightweightStructure;
}

%include <rotor/SharedPointers.h>

%template( StructurePtr ) boost::shared_ptr<Rotor::Structure>;
%template( RegistryPtr ) boost::shared_ptr<Rotor::Registry>;
//------------------------------------------------------------------------------

%include <rotor/QueuePolicy.h>
%include <rotor/Time.h>
%include <rotor/Logger.h>
%include <rotor/Options.h>
%include <rotor/BaseOptions.h>
%include <rotor/MemberDefinition.h>
%include <rotor/BasicTypes.h>
%include <rotor/Message.h>
%include <rotor/Type.h>
%include <rotor/Registry.h>
%include <rotor/RemoteRegistry.h>
%include <rotor/Serialization.h>
%include <rotor/BaseRegistry.h>
%include <rotor/AbstractVariable.h>
%include <rotor/Structure.h>
%include <rotor/StructureArray.h>
%include <rotor/LightweightStructure.h>
%include <rotor/Thread.h>
%include <rotor/FileUtils.h>

//------------------------------------------------------------------------------

namespace std {
    %template(MemberDefinitionVector) vector<Rotor::MemberDefinition>;
}

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  AbstractVariable & _member( const char * fieldName )
  {
    return (*$self)[fieldName];
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  AbstractVariable & _member( int index )
  {
    return (*$self)[index];
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  AbstractVariable & _assign( int value )
  {
    return (*$self) = value;
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  AbstractVariable & _assign( double value )
  {
    return (*$self) = value;
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  AbstractVariable & _assign( const char * value )
  {
    return (*$self) = value;
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  double asDouble()
  {
    double result = *$self;
    return result;
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  int asInteger()
  {
    int result = *$self;
    return result;
  }
};

//------------------------------------------------------------------------------

%extend Rotor::AbstractVariable {
  const char * asString()
  {
    const char * result = *$self;
    return result;
  }
};

//------------------------------------------------------------------------------

%pythoncode%{
import socket
import sys
import dl
sys.setdlopenflags( dl.RTLD_NOW | dl.RTLD_GLOBAL )
def hostName():
  s = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
  try:
    s.connect( ( '1.2.3.4', 56 ) )
    return s.getsockname()[0]
  except socket.error:
    return "127.0.0.1"


def _GETITEM( self, index ): 
  result = self._member( index )
  if result.type() == DoubleType:
    return result.asDouble()
  elif result.type() == IntType:
    return result.asInteger()
  elif result.type() == StringType:
    return result.asString()
  elif result.type() == ArrayType or result.type() == StructureType:
    return result

def _SETITEM( self, index, value ): 
  self._member( index )._assign( value )
    
def _GETATTR( self, name ): 
  result = self._member( name )
  if result.type() == DoubleType:
    return result.asDouble()
  elif result.type() == IntType:
    return result.asInteger()
  elif result.type() == StringType:
    return result.asString()
  elif result.type() == ArrayType or result.type() == StructureType:
    return result

def _SETATTR( self, name, value ): 
  try: 
    self._member( name )._assign( value )
  except:
    self.__dict__[name] = value

Structure.__getattr__ = _GETATTR
Structure.__setattr__ = _SETATTR
LightweightStructure.__getattr__ = _GETATTR
LightweightStructure.__setattr__ = _SETATTR
AbstractVariable.__getitem__ = _GETITEM
AbstractVariable.__setitem__ = _SETITEM
AbstractVariable.__getattr__ = _GETATTR
AbstractVariable.__setattr__ = _SETATTR
%}