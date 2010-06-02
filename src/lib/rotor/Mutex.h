#ifndef ROTOR_MUTEX_H
#define ROTOR_MUTEX_H


namespace Rotor {


class MutexImpl;


class Mutex
{
public:
  Mutex();
  ~Mutex();

  void lock();
  void unlock();

  friend class Condition;

private:
  Mutex( const Mutex & );
  void operator=( const Mutex & );


  MutexImpl * _mutex;
};


}


#endif //ROTOR_MUTEX_H
