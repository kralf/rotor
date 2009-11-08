#ifndef ROTOR_CONDITION_H
#define ROTOR_CONDITION_H


namespace Rotor {


class Mutex;
class ConditionImpl;

class Condition
{
public:
  Condition();
  virtual ~Condition();

  bool wait( Mutex & mutex, double seconds );
  void notify();
  void notifyAll();

private:
  Condition( const Condition & );
  void operator=( const Condition & );

  ConditionImpl * _condition;

};


}


#endif // ROTOR_CONDITION_H
