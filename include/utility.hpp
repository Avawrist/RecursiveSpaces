// ====================================================================================
// Title: utility.cpp
// Description: File declaring preprocessor directives & typedefs,
//              plus other utility functions/structs
// ====================================================================================

#ifndef UTILITY_H
#define UTILITY_H

// TO-DO: Remove from release ver:
#include <stdio.h>  
#include <windows.h>

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

// Struct Profiler //

c_uint PROFILER_MSG_LENGTH = 144;
typedef struct Profiler
{
    double start_time = 0.0f;
    double end_time   = 0.0f;
    char   msg[PROFILER_MSG_LENGTH];
} Profiler;

inline void
profilerGetTime(Profiler& p)
{
    double time = p.end_time - p.start_time;
    sprintf_s(p.msg, PROFILER_MSG_LENGTH, "Time (Seconds): %f\n", time);
    OutputDebugStringA(p.msg);
}

// Utility Functions //


#endif
