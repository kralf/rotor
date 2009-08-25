#ifndef ROTOR_MEMORY_H
#define ROTOR_MEMORY_H


#include <cstring>


namespace Rotor {


class Memory 
{
public:
  Memory( void * address, size_t size );
  virtual ~Memory();
  
  virtual size_t bufferSize() const;
  virtual void * buffer() const;
  virtual bool owner() const;
   
protected:
  void resizeBuffer( size_t newSize );
  void copyBuffer( const Memory & memory );

private:
  void * _buffer;
  bool   _owner;
  size_t _bufferSize;
};


}


#endif //ROTOR_MEMORY_H
