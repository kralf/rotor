#ifndef ROTOR_OPERATOR_MACROS_H
#define ROTOR_OPERATOR_MACROS_H

#define DECLARE_BOOL_OPERATOR( OP ) \
bool operator OP( const AbstractVariable & v2, int v1 ); \
bool operator OP( int v1, const AbstractVariable & v2 ); \
bool operator OP( const AbstractVariable & v2, double v1 ); \
bool operator OP( double v1, const AbstractVariable & v2 ); \
bool operator OP( const AbstractVariable & v2, const char * v1 ); \
bool operator OP( const char * v1, const AbstractVariable & v2 ); \
bool operator OP( const AbstractVariable & v1, const AbstractVariable & v2 );

#define IMPLEMENT_BOOL_OPERATOR( OP ) \
bool \
Rotor::operator OP( const AbstractVariable & v2, int v1 ) { \
  if ( v2.type() == IntType ) { \
    return v2.operator int() OP v1; \
  } else if ( v2.type() == DoubleType ) { \
    return v2.operator double() OP v1; \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
bool \
Rotor::operator OP( int v1, const AbstractVariable & v2 ) { \
  if ( v2.type() == IntType ) { \
    return v1 OP v2.operator int(); \
  } else if ( v2.type() == DoubleType ) { \
    return v1 OP v2.operator double(); \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
bool \
Rotor::operator OP( const AbstractVariable & v2, double v1 ) { \
  if ( v2.type() == IntType ) { \
    return v2.operator int() OP v1; \
  } else if ( v2.type() == DoubleType ) { \
    return v2.operator double() OP v1; \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
bool \
Rotor::operator OP( double v1, const AbstractVariable & v2 ) { \
  if ( v2.type() == IntType ) { \
    return v1 OP v2.operator int(); \
  } else if ( v2.type() == DoubleType ) { \
    return v1 OP v2.operator double(); \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
bool \
Rotor::operator OP( const AbstractVariable & v2, const char * v1 ) { \
  if ( v2.type() == StringType ) { \
    return std::string( v2.operator const char *() ) OP std::string( v1 ); \
  } \
  throw TypeError( "Operands should be strings" ); \
} \
 \
bool \
Rotor::operator OP( const char * v1, const AbstractVariable & v2 ) { \
  if ( v2.type() == StringType ) { \
    return std::string( v1 ) OP std::string( v2.operator const char *() ); \
  } \
  throw TypeError( "Operands should be strings" ); \
} \
 \
bool \
Rotor::operator OP( const AbstractVariable & v1, const AbstractVariable & v2 ) { \
  if ( v1.type() == IntType && v2.type() == IntType ) { \
    return v1.operator int() OP v2.operator int(); \
  } else if ( v1.type() == IntType && v2.type() == DoubleType ) { \
    return v1.operator int() OP v2.operator double(); \
  } else if ( v1.type() == DoubleType && v2.type() == IntType ) { \
    return v1.operator double() OP v2.operator int(); \
  } else if ( v1.type() == DoubleType && v2.type() == DoubleType ) { \
    return v1.operator double() OP v2.operator double(); \
  } \
  throw TypeError( "Operands should be numbers" ); \
}

#define DECLARE_NUMERIC_OPERATOR( OP ) \
int operator OP( const AbstractVariable & v2, int v1 ); \
double operator OP( const AbstractVariable & v2, double v1 ); \
int operator OP( int v1, const AbstractVariable & v2 ); \
double operator OP( double v1, const AbstractVariable & v2 ); \
double operator OP( const AbstractVariable & v1, const AbstractVariable & v2 );

#define IMPLEMENT_NUMERIC_OPERATOR( OP ) \
int \
Rotor::operator OP( const AbstractVariable & v2, int v1 ) { \
  if ( v2.type() == IntType ) { \
    return v2.operator int() OP v1; \
  } else if ( v2.type() == DoubleType ) { \
    return v2.operator double() OP v1; \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
int \
Rotor::operator OP( int v1, const AbstractVariable & v2 ) { \
  if ( v2.type() == IntType ) { \
    return v1 OP v2.operator int(); \
  } else if ( v2.type() == DoubleType ) { \
    return v1 OP v2.operator double(); \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
double \
Rotor::operator OP( const AbstractVariable & v2, double v1 ) { \
  if ( v2.type() == IntType ) { \
    return v2.operator int() OP v1; \
  } else if ( v2.type() == DoubleType ) { \
    return v2.operator double() OP v1; \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
double \
Rotor::operator OP( double v1, const AbstractVariable & v2 ) { \
  if ( v2.type() == IntType ) { \
    return v1 OP v2.operator int(); \
  } else if ( v2.type() == DoubleType ) { \
    return v1 OP v2.operator double(); \
  } \
  throw TypeError( "Operands should be numbers" ); \
} \
 \
double \
Rotor::operator OP( const AbstractVariable & v1, const AbstractVariable & v2 ) { \
  if ( v1.type() == IntType && v2.type() == IntType ) { \
    return v1.operator int() OP v2.operator int(); \
  } else if ( v1.type() == IntType && v2.type() == DoubleType ) { \
    return v1.operator int() OP v2.operator double(); \
  } else if ( v1.type() == DoubleType && v2.type() == IntType ) { \
    return v1.operator double() OP v2.operator int(); \
  } else if ( v1.type() == DoubleType && v2.type() == DoubleType ) { \
    return v1.operator double() OP v2.operator double(); \
  } \
  throw TypeError( "Operands should be numbers" ); \
}

#endif //ROTOR_OPERATOR_MACROS_H