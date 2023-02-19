// ====================================================================================
// Title: preprocessor.hpp
// Description: File that includes any preprocessor directives
// ====================================================================================

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
