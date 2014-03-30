#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// PC: Took advice of comment.  This line wouldn't compile (stdout not constant).
//static FILE * TeeFP = stdout; // should default to null ?
static FILE * TeeFP = NULL;

void Log_Out(const char * string)
{
	OutputDebugString(string);
	if ( TeeFP )
		fprintf(TeeFP,string);
}


void Log_Puts(const char * string)
{
	Log_Out(string);
	Log_Out("\n");
}

void Log_Printf(const char * String, ...)
{
	va_list			ArgPtr;
    char			TempStr[4096];

	va_start(ArgPtr, String);
    vsprintf(TempStr, String, ArgPtr);
	va_end(ArgPtr);

	Log_Out(TempStr);
}

void Log_TeeFile( FILE * FP )
{
	TeeFP = FP;
}

#ifndef NO_LOG
#pragma message("LOG on")
#endif