/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2007 Justin Olbrantz. All Rights Reserved.
*/

#include <windows.h>

// Prevent this header from being included multiple times
#ifndef QINJECTDLL_H
#define QINJECTDLL_H

// The definition of the patcher function that will be called in the target process. A return value of FALSE indicates a fatal error, and that the loader should terminate the process.
typedef BOOL (WINAPI *LPPATCHERENTRYPOINT)(
	// User-specified data to be passed to the patcher function
	LPCVOID lpvData, 
	// The size in bytes of the patcher data. Nothing outside this range will be injected into the target process, so this buffer should not contain any pointers.
	DWORD nDataSize
);

/*
	* InjectDLLIntoProcessV3 *
	This is the third revision of the DLL injector, originally used in MPQDraft. This function loads a DLL in the target process, and then (optionally) calls a function in the DLL from inside the specified thread, passing it some amount of data specified by the calling function. When the DLL's function returns, the specified thread resumes execution where it was previously. If injection fails, it's likely that the target process is in an unstable state, and should be terminated. In theory, it should be possible to inject multiple DLLs into the same thread; however, this has never been tested.
	Injection requires that the target thread be created suspended, as the thread cannot execute during this process, and it's very risky to suspend a thread in the middle of execution for something like this. In these circumstances, the injection will occur after all DLLs have been initialized, but before the first line of program wrapper code is executed; this means that the Structured Exception Handling unhandled exception filter will NOT be established.
	Note that I will likely move this into a Win32 tools wing of LibQ
*/

BOOL WINAPI InjectDLLIntoProcessV3(
	// Handle to the process to inject the DLL into
	IN HANDLE hTargetProcess,
	// Handle to the thread the loader and patcher will be called from
	IN HANDLE hTargetThread,
	// ANSI path of the DLL to be injected. This parameter is only used when a DLL is being injected without a patcher function being called, and is ignored if lpfnPatcher is specified.
	IN OPTIONAL LPCSTR lpDLLFileName,
	// The function to be called in the DLL to be injected. This must either be NULL, indicating that no patcher function should be called, or be a pointer into a DLL loaded with LoadLibrary.
	IN OPTIONAL LPPATCHERENTRYPOINT lpfnPatcher,
	// Data to be passed to the patcher function in the DLL
	IN OPTIONAL LPCVOID lpPatcherData,
	// Size of the patcher data
	IN OPTIONAL DWORD nPatcherDataLen
);

#endif // #ifndef QINJECTDLL_H