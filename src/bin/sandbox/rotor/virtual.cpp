#include <cstdio>

class  Base 
{
public:
  virtual operator int() const { 
    return 5; 
  }
};

class Derived : public Base
{
public:
  Derived( int value ) : _value( value ) {}
  virtual operator int()  const {
    return _value;
  }

private:
  int _value;
};


int main() {
  Derived derived( 10 );
  Base & base = derived;
  int iderived = derived;
  int ibase = base;
  printf( "Derived %i\n", iderived );
  printf( "Base %i\n", ibase );
}
