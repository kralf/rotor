%module rotorc
%{
#include <rotor/BaseRegistry.h>
#include <rotor/BaseOptions.h>
#include <rotor/Structure.h>
#include <rotor/StructureArray.h>
#include <rotor/Exceptions.h>
#include <rotor/Time.h>
#include <rotor/Serialization.h>
%}


%include std_string.i
%include std_vector.i
%include exception.i
%include <rotor/Exceptions.h>

//------------------------------------------------------------------------------

%exception Rotor::Registry::receiveMessage {
  try {
    $action
  } catch( Rotor::MessagingTimeout & e ) {
    SWIG_exception( SWIG_RuntimeError, e.what() );
  } 
  SWIG_CATCH_STDEXCEPT
  catch (...) {
    SWIG_exception( SWIG_UnknownError, "Unknown exception" );
  }
}

//------------------------------------------------------------------------------

%exception Rotor::Registry::sendReceiveMessage {
  try {
    $action
  } catch( Rotor::MessagingTimeout & e ) {
    SWIG_exception( SWIG_RuntimeError, e.what() );
  } 
  SWIG_CATCH_STDEXCEPT
  catch (...) {
    SWIG_exception( SWIG_UnknownError, "Unknown exception" );
  }
}

//------------------------------------------------------------------------------

%include <rotor/Time.h>
%include <rotor/Options.h>
%include <rotor/BaseOptions.h>
%include <rotor/Message.h>
%include <rotor/MemberDefinition.h>
%include <rotor/Type.h>
%include <rotor/BasicTypes.h>
%include <rotor/Registry.h>
%include <rotor/Serialization.h>
%include <rotor/BaseRegistry.h>
%include <rotor/AbstractVariable.h>
%include <rotor/Structure.h>
%include <rotor/StructureArray.h>

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
AbstractVariable.__getitem__ = _GETITEM
AbstractVariable.__setitem__ = _SETITEM
AbstractVariable.__getattr__ = _GETATTR
AbstractVariable.__setattr__ = _SETATTR
%}