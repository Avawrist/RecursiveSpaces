// ====================================================================================
// Title: utility.hpp
// Description: File containing preprocessor directives & typedefs
// ====================================================================================

#ifndef UTILITY_H
#define UTILITY_H

/*
  ASSERTIONS:
  1 - Assertions are enabled
  0 - Assertions are disabled
*/

#if ASSERTIONS
#include <assert.h>
#define _assert(Expression) assert(Expression);
#else
#define _assert(Expression)
#endif

// Const
typedef const void      c_void;
typedef const char      c_char;
typedef const int       c_int;
typedef const float     c_float;
typedef const double    c_double;
typedef const short int c_shint;
typedef const long int  c_lint;

// Const unsigned
typedef const unsigned char c_uchar;
typedef const unsigned int  c_uint;

// Unsigned
typedef unsigned char uchar;
typedef unsigned int  uint;

// Signed
typedef short int shint;
typedef long int  lint;

#endif
