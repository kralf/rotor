#ifndef ROTOR_LOCK_H
#define ROTOR_LOCK_H


namespace Rotor {

class Mutex;

class Lock
{
public:
  Lock( Mutex & mutex );
  ~Lock();

private:
  Lock( const Lock & );
  void operator=( const Lock & );
  
  Mutex & _mutex;
};


}


#endif //ROTOR_LOCK_H
