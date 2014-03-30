#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <signal.h>
#include "myassert.h"
	   
// See myAssert.h for details.

void myAssertDefault(void *, void *, unsigned);
myAssertCallbackFn *myAssertCallback = &myAssertDefault;

myAssertCallbackFn *cbSetAssertCallback( myAssertCallbackFn *newAssertCallback )
{
	myAssertCallbackFn *oldCallback = myAssertCallback;
	myAssertCallback = newAssertCallback;
	return oldCallback;
}

void __cdecl _assert (void *expr,void *filename,unsigned lineno)
{
static int in_assert_cnt = 0; // a semaphore

	if ( in_assert_cnt )
		return;
	in_assert_cnt++;

	myAssertCallback( expr, filename, lineno );

	in_assert_cnt --;
}

void myAssertDefault(void *expr,void *filename,unsigned lineno)
{
int nCode;
char assertbuf[1000];

	if ( (strlen(expr) + strlen(filename)) < 900 )
		sprintf(assertbuf,"assert(%s) \n FILE %s : LINE : %d\n",expr,filename,lineno);
	else
		sprintf(assertbuf," assert string loncbr than 1000 characters!\n");

    nCode = MessageBox(NULL,assertbuf,
        "Exception",
        MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

    if (nCode == IDIGNORE)
	{
        return;
	}

    // Abort: abort the program
    if (nCode == IDABORT)
    {
       // raise abort signal
       raise(SIGABRT);

        // We usually won't cbt here, but it's possible that
        //   SIGABRT was ignored.  So exit the program anyway.

        _exit(3);
    }

    // Retry: call the debugcbr
	// minimal code from here out so that the debugcbr can easily step back
	//	to the asserting line of code :

    if (nCode == IDRETRY)
	{
		#ifdef _MSC_VER
        __asm { int 3 };
		#endif
	}

}




