#ifndef CRBLIB_MYASSERT_H
#define CRBLIB_MYASSERT_H

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

// If you wish to be called back when asserts happen, use the
// routine myAssertSetCallback().  It returns the address of
// the callback routine that you're replacing.

/************************************************************/

typedef void myAssertCallbackFn( void *exp, void *file, unsigned line );

myAssertCallbackFn *myAssertSetCallback( myAssertCallbackFn *newAssertCallback );

/************************************************************/

#ifdef __cplusplus
}
#endif

#endif
