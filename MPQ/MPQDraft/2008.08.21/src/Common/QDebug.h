/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2007 Justin Olbrantz. All Rights Reserved.
*/

// Prevent this header from being included multiple times
#ifndef QDEBUG_H
#define QDEBUG_H

#include <windows.h>

// Functions used internally. Ignore these.
BOOL WINAPI QDebugOpenLogFileFn(LPCSTR lpszAppName, LPSTR lpszFileName, DWORD dwBufferLen);
BOOL WINAPI QDebugCloseLogFileFn();

BOOL WINAPI QDebugCloseDeleteLogFileFn();

BOOL _cdecl QDebugWriteEntryFn(LPCSTR lpszFormat, va_list params);

/*
	* QDebugWriteEntry *
	Writes a log entry to the currently open log. Entry will be on a line of its own, and will have a time and date stamp. Log entries use sprintf, so any sprintf format specification is valid. Function fails if a log file is not open, but does not assert.
	This function is inlined to make the function be optimized out for release build.
*/
inline void QDebugWriteEntry(LPCSTR lpszFormat, ...)
{
#ifdef _DEBUG
	// Simply call the real writing function with the parameter list
	va_list params;
	va_start(params, lpszFormat);

	QDebugWriteEntryFn(lpszFormat, params);
#endif
}

#ifdef _DEBUG
/*
	* QDebugOpenLogFile *
	Opens a log file to be used throughout the program to log various events throughout execution. The file name will be chosen by the function, and will be of the form "[app name] log - [date].[time].log", in the Windows temporary file directory. Only one log file may be open at a time, and QDebugOpenLogFile will assert if this is not the case.
	If the file cannot be opened, QDebugOpenLogFile returns FALSE. The other QDebug functions will fail, but these can be ignored; the program simply will not perform logging.
*/
inline BOOL QDebugOpenLogFile(
	// The program name to use in the log filename. Should not exceed MAX_PATH chars.
	IN LPCTSTR lpszAppName,
	// A buffer to receive the path of the log file created. Should be at least MAX_PATH chars.
	OUT OPTIONAL LPSTR lpszFileName,
	// The length of lpszFileName
	IN OPTIONAL DWORD dwBufferLen
)
{ return QDebugOpenLogFileFn(lpszAppName, lpszFileName, dwBufferLen); }

/*
	* QDebugCloseLogFile *
	Closes the log file opened previously with QDebugOpenLogFile. Fails if there is no log file open.
*/
#define QDebugCloseLogFile() QDebugCloseLogFileFn()

/*
	* QDebugCloseDeleteLogFile *
	Closes the open log file and deletes it. This function could be used when log files should only be left when a problem occurs. Fails if there is no log file open.
*/
#define QDebugCloseDeleteLogFile() QDebugCloseDeleteLogFileFn()
#else
// Stubs to omit the log function calls in release builds
#define QDebugOpenLogFile(p1, p2, p3) (TRUE)
#define QDebugCloseLogFile() (TRUE)

#define QDebugCloseDeleteLogFile() (TRUE)
#endif // #ifndef _DEBUG

#endif // #ifndef QDEBUG_H