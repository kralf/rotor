#include "Exceptions.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

MemoryAllocationError::MemoryAllocationError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

InvalidAssignmentError::InvalidAssignmentError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

InvalidCastError::InvalidCastError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

InvalidOperatorError::InvalidOperatorError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

TypeError::TypeError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

ParsingError::ParsingError( const std::string & message ) 
: runtime_error( message ) {}

//------------------------------------------------------------------------------

MessagingError::MessagingError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

MessagingTimeout::MessagingTimeout( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

ClassLoadingError::ClassLoadingError( const std::string & message ) 
: logic_error( message ) {}

//------------------------------------------------------------------------------

OptionError::OptionError( const std::string & message ) 
: runtime_error( message ) {}

//------------------------------------------------------------------------------

TimeoutException::TimeoutException( const std::string & message ) 
: runtime_error( message ) {}
