#include <rotor/Queue.h>
#include <rotor/Exceptions.h>
#include <rotor/Thread.h>
#include <cstdlib>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

void * producer( void * value )
{
  Queue<int> * q = static_cast< Queue<int> * >( value );
  int i = 0;
  while ( true ) {
    Thread::sleep( 1.0 * rand() / RAND_MAX );
    q->push( i++ );
  }
}

//------------------------------------------------------------------------------

void * consumer( void * value )
{
  Queue<int> * q = static_cast< Queue<int> * >( value );
  int i = 0;
  while ( true ) {
    try {
      i = q->next( 0.1 );
      q->pop();
      fprintf( stderr, "%i\n", i );
    } catch ( TimeoutException ) {
    }
  }
}

//------------------------------------------------------------------------------


int main()
{
  Queue<int> q1;
  Queue<int> q2;
  Thread t1;
  t1.start( producer, &q1 );
  Thread t2;
  t2.start( consumer, &q1 );
  Thread t3;
  t3.start( producer, &q2 );
  Thread t4;
  t4.start( consumer, &q2 );
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}