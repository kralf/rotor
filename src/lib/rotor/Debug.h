#ifndef ROTOR_DEBUG_H
#define ROTOR_DEBUG_H


#include <string>
#include <iostream>

#define ROTOR_DEBUG( STUFF ) \
{Rotor::Debug d( #STUFF ); STUFF;}

#define ROTOR_DEBUG_METHOD \
Rotor::Debug d( std::string( __FILE__ ) + " " + std::string( __FUNCTION__ ) );

namespace Rotor {


class Debug
{
public:
  Debug( const std::string & message );
  virtual ~Debug();

private:
  static int  _classDepth;
  int         _depth;
  std::string _message;
};


}


#endif //ROTOR_DEBUG_H
