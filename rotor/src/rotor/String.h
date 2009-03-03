#ifndef ROTOR_STRING_H
#define ROTOR_STRING_H


#include "Exceptions.h"
#include "AbstractVariable.h"


namespace Rotor {


class String : public AbstractVariable
{
public:
  String();
  String( const std::string & s );
  String( const char * address );
  explicit String( void * ownerAddress );
  virtual AbstractVariable & operator=( const AbstractVariable & variable );
  virtual AbstractVariable & operator=( const char * address );
  virtual operator const char*() const;
  virtual size_t size() const;
  virtual GenericType type() const;
  
private:
  void ** _ownerAddress;
  int     _size;
};


}


#endif //ROTOR_STRING_H