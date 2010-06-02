
//------------------------------------------------------------------------------

unsigned int 
Rotor::microseconds()
{
  struct timeval tv;
  struct timezone tz;
  gettimeofday( &tv, &tz );
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

//------------------------------------------------------------------------------

double 
Rotor::seconds()
{
  struct timeval tv;
  struct timezone tz;
  gettimeofday( &tv, &tz );
  return 0.0 + tv.tv_sec + tv.tv_usec / 1000000.0 ;
}
