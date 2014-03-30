#ifndef CRBLIB_LOG_H
#define CRBLIB_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/*****

the Log protos :

void Log_Puts(	const char * string);
void Log_Printf(const char * string, ...);
void Log_TeeFile( FILE * FP );	// defaults to stdout

note : Log also writes out to ThreadLog !
use ThreadLog_Report() to dump data

*****/

#ifndef	NO_LOG

#include <stdio.h>

void Log_Puts(	const char * string);
void Log_Printf(const char * string, ...);
void Log_TeeFile( FILE * FP );

#else	// NO_LOG

#pragma warning (disable:4100)
static _inline void Log_Printf(const char * str, ...) { }
#pragma warning (default:4100)

#define Log_Puts(string)
#define Log_TeeFile(f)

#endif	// NO_LOG

#ifdef __cplusplus
}
#endif

#endif // LOG_H
