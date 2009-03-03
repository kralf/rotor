#ifndef ROTOR_EXCEPTIONS_H
#define ROTOR_EXCEPTIONS_H


#include <stdexcept>
#include <string>


namespace Rotor {
  class MemoryAllocationError : public std::logic_error {
  public:
    MemoryAllocationError( const std::string & message ) : logic_error( message ) {}
  };
  
  class InvalidAssignmentError: public std::logic_error {
  public:
    InvalidAssignmentError( const std::string & message ) : logic_error( message ) {}
  };
  
  class InvalidCastError: public std::logic_error {
  public:
    InvalidCastError( const std::string & message ) : logic_error( message ) {}
  };

  class InvalidOperatorError: public std::logic_error {
  public:
    InvalidOperatorError( const std::string & message ) : logic_error( message ) {}
  };

  class TypeError: public std::logic_error {
  public:
    TypeError( const std::string & message ) : logic_error( message ) {}
  };
  
  class ParsingError: public std::runtime_error {
  public:
    ParsingError( const std::string & message ) : runtime_error( message ) {}
  };
  
  class MessagingError : public std::logic_error {
  public:
    MessagingError( const std::string & message ) : logic_error( message ) {}
  };

  class MessagingTimeout : public std::runtime_error {
  public:
    MessagingTimeout( const std::string & message ) : runtime_error( message ) {}
  };

  class ClassLoadingError : public std::logic_error {
  public:
    ClassLoadingError( const std::string & message ) : logic_error( message ) {}
  };
  
  class OptionError : public std::runtime_error {
  public:
    OptionError( const std::string & message ) : runtime_error( message ) {}
  };
  
  class TimeoutException : public std::runtime_error {
  public:
    TimeoutException( const std::string & message ) : runtime_error( message ) {}
  };
}


#endif //ROTOR_EXCEPTIONS_H
