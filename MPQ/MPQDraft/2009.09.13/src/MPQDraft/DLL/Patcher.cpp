/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#include "stdafx.h"
#include "Patcher.h"
#include "..\\Common.h"
#include <QInjectDLL.h>

// The maximum length of a patch context name
#define CONTEXT_NAME_LENGTH 32

// The template used to create the name of the patch context based on MPQDraft's process ID
const char szSectionName[] = "MPQDRAFT_%08x_SECTION";

BOOL ClaimContext(IN HANDLE hContextMutex)
{
	assert(hContextMutex);	// NULL if invalid

	WaitForSingleObject(hContextMutex, INFINITE);

	return TRUE;
}

BOOL ReleaseContext(IN HANDLE hContextMutex)
{
	assert(hContextMutex);

	return ReleaseMutex(hContextMutex);
}

BOOL CreatePatchContext(
	// Various patching flags
	IN DWORD dwFlags,
	// The directory that "MPQDraft" (the program that began the patching session) is running in
	IN LPCSTR lpszMPQDraftDir,
	// The startup directory (initial current directory) of the file process to be patched in this patching session.
	IN LPCSTR lpszStartupDir,
	// The path of the patch target. If we're only patching a single program, this will be the program we want to load the MPQs and plugins in. We have to inject into all processes, but only this one will have the custom stuff loaded.
	IN LPCSTR lpszTargetPath,
	// The shunt count for the target, or 0 if shunting is not to be used
	IN OPTIONAL DWORD nShuntCount,
	// The number of patch MPQs to load in the target
	IN OPTIONAL DWORD nPatchMPQs,
	// The number of auxiliary modules to register with the plugin server, and possibly load as plugins
	IN OPTIONAL DWORD nAuxModules,
	// The array of MPQ names to load
	IN OPTIONAL LPCSTR *lplpszMPQNames, 
	// The auxiliary modules to load. Modules that are flagged as executable will be loaded as plugins. Earlier ones have a lower priority.
	IN OPTIONAL const MPQDRAFTPLUGINMODULE *lpAuxModules,

	// The memory mapped file HANDLE for the patch context
	OUT HANDLE &rhContext,
	// The patch context itself
	OUT MPQDRAFTPATCHCONTEXT **ppContext,
	// The mutex protecting the patch context
	OUT HANDLE &rhContextMutex
)
{
	// Assert that we're having fun
	assert(lpszMPQDraftDir);
	assert(lpszStartupDir);

	assert(lpszTargetPath);

	assert(ppContext);

	// If there are patch MPQs specified, verify the list is valid, and log them
	if (nPatchMPQs)
	{
		assert(lplpszMPQNames);

		for (DWORD i = 0; i < nPatchMPQs; i++)
		{
			assert(lplpszMPQNames[i]);

			QDebugWriteEntry("CreatePatchContext() : MPQ #%d: %s", i, lplpszMPQNames[i]);
		}
	}	

	if (nAuxModules)
		assert(lpAuxModules);

	QDebugWriteEntry("CreatePatchContext() : Patch target: %s", lpszTargetPath);

	// We'll do the easy stuff first: compute the size of the context
	DWORD dwContextSize = sizeof(MPQDRAFTPATCHCONTEXT) 
		+ nPatchMPQs * MPQDRAFT_MAX_PATH
		+ nAuxModules * sizeof(MPQDRAFTPLUGINMODULE);

	// Create the mutex to protect the context
	rhContextMutex = CreateMutex(NULL, FALSE, NULL);
	if (!rhContextMutex)	// NULL on failure
		return FALSE;

	// Create the context name. This is a bit of a hack to allow waiting for completion to occur
	char szContextName[CONTEXT_NAME_LENGTH] = "";
	sprintf(szContextName, szSectionName, GetCurrentProcessId());

	// Create the context itself
	rhContext = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE | SEC_COMMIT, 0, dwContextSize, szContextName);
	if (rhContext)
	{
		// Map the context so that we can fill it in
		MPQDRAFTPATCHCONTEXT *pContext = (MPQDRAFTPATCHCONTEXT *)MapViewOfFile(rhContext, FILE_MAP_WRITE, 0, 0, 0);

		if (pContext)
		{
			// Fresh start
			ZeroMemory(pContext, dwContextSize);

			// Set the basic members of the context
			pContext->dwSize = dwContextSize;
			pContext->dwFlags = dwFlags;

			pContext->nPatchMPQs = nPatchMPQs;
			pContext->nModules = nAuxModules;

			strncpy(pContext->szMPQDraftDir, lpszMPQDraftDir, MPQDRAFT_MAX_PATH);
			strncpy(pContext->szStartupDir, lpszStartupDir, MPQDRAFT_MAX_PATH);
			pContext->szMPQDraftDir[MPQDRAFT_MAX_PATH - 1] =
				pContext->szMPQDraftDir[MPQDRAFT_MAX_PATH - 1] = '\0';

			strncpy(pContext->szTargetPath, lpszTargetPath, MPQDRAFT_MAX_PATH);
			pContext->szTargetPath[MPQDRAFT_MAX_PATH - 1] = '\0';
			pContext->nShuntCount = /*pContext->nShuntRemaining =*/ nShuntCount;

			// Compute offsets for the MPQs and modules from array sizes
			pContext->dwMPQNamesOffset = sizeof(MPQDRAFTPATCHCONTEXT);
			pContext->dwModulesOffset = pContext->dwMPQNamesOffset
				+ nPatchMPQs * MPQDRAFT_MAX_PATH;
	
			// Copy the MPQ paths
			LPSTR lpszMPQPath = (LPSTR)pContext + pContext->dwMPQNamesOffset;
			for (DWORD iMPQ = 0; iMPQ < nPatchMPQs; iMPQ++, lpszMPQPath += MPQDRAFT_MAX_PATH)
			{
				strncpy(lpszMPQPath, lplpszMPQNames[iMPQ], MPQDRAFT_MAX_PATH);
				lpszMPQPath[MPQDRAFT_MAX_PATH - 1] = '\0';
			}

			// Copy the modules
			MPQDRAFTPLUGINMODULE *lpModules = (MPQDRAFTPLUGINMODULE *)((UINT_PTR)pContext 
				+ pContext->dwModulesOffset);
			for (DWORD iModule = 0; iModule < nAuxModules; iModule++)
				memcpy(&lpModules[iModule], &lpAuxModules[iModule], sizeof(MPQDRAFTPLUGINMODULE));

			// Done! (I think)
			*ppContext = pContext;

			return TRUE;

		}

		// Clean up on failure
		CloseHandle(rhContext);
	}

	// Clean up on failure
	CloseHandle(rhContextMutex);

	return FALSE;
}

BOOL DuplicatePatchContextInChild(
	IN HANDLE hChildProcess,
	IN HANDLE hCurrentContext,
	IN HANDLE hCurrentContextMutex,
	OUT HANDLE &rhChildContext,
	OUT HANDLE &rhChildContextMutex
)
{
	assert(hChildProcess);	// NULL if invalid
	assert(hCurrentContext);	// NULL if invalid
	assert(hCurrentContextMutex);	// NULL if invalid

	// Don't need the mutex, because we're not actually touching the patch context
	HANDLE hCurProcess = GetCurrentProcess();

	// Inject the patch context
	if (!DuplicateHandle(hCurProcess, hCurrentContext, hChildProcess, 
		&rhChildContext, 0, FALSE, DUPLICATE_SAME_ACCESS))
		return FALSE;

	// Inject the patch context mutex
	if (DuplicateHandle(hCurProcess, hCurrentContextMutex, hChildProcess, 
		&rhChildContextMutex, 0, FALSE, DUPLICATE_SAME_ACCESS))
		return TRUE;	// Success

	// Failed to inject the mutex. Clean up.
	CloseHandle(rhChildContext);

	return FALSE;
}

MPQDRAFTPATCHCONTEXT *OpenContextHandle(IN HANDLE hContext)
{
	// Don't need the mutex
	assert(hContext);	// NULL if invalid

	return (MPQDRAFTPATCHCONTEXT *)MapViewOfFile(hContext, FILE_MAP_WRITE, 0, 0, 0);
}

void CloseContext(IN HANDLE hContext, IN MPQDRAFTPATCHCONTEXT *lpContext, IN HANDLE hContextMutex)
{
	assert(hContext);
	assert(lpContext);
	assert(hContextMutex);

	UnmapViewOfFile(lpContext);

	CloseHandle(hContext);
	CloseHandle(hContextMutex);
}

void WaitForContextClose()
{
	// This is an evil but effective hack to wait for patching completion. Note that we inject a HANDLE to the section and mutex into every single process, and these are closed when the process closes. Internally, the objects that HANDLEs refer to are reference-counted, based on the number of open HANDLEs. So we're going to sit here and wait until the context reference count reaches 0, at which point we won't be able to open it anymore. This implies that this will be called from the MPQDraft (master patching) process, and that the context must be closed in this process before calling this function.
	// The name of the context we created earlier
	char szContextName[CONTEXT_NAME_LENGTH] = "";
	sprintf(szContextName, szSectionName, GetCurrentProcessId());

	HANDLE hContext = OpenFileMapping(FILE_MAP_READ, FALSE, szContextName);
	while (hContext)
	{
		// It's still there. The patch is still going. Close the handle and wait a bit before trying again.
		CloseHandle(hContext);

		Sleep(1000);

		hContext = OpenFileMapping(FILE_MAP_READ, FALSE, szContextName);
	}

	// It's not there. The patching process is finished.
}

BOOL CreateAndPatchProcess(
	// These first 10 parameters are nothing other than the parameters passed to CreateProcess, and used for just that.
	IN LPCSTR lpszApplicationName,
	IN OUT LPSTR lpszCommandLine,

	IN OPTIONAL LPSECURITY_ATTRIBUTES lpProcessAttributes,
	IN OPTIONAL LPSECURITY_ATTRIBUTES lpThreadAttributes,

	IN BOOL bInheritHandles,
	IN DWORD dwCreationFlags,
	IN OPTIONAL LPVOID lpszEnvironment,
	IN OPTIONAL LPCSTR lpszCurrentDirectory,

	IN LPSTARTUPINFO lpStartupInfo,
	OUT LPPROCESS_INFORMATION lpProcessInformation,

	// The patch context and its mutex to use
	IN HANDLE hPatchContext,
	IN HANDLE hPatchMutex,

	// Shunt counting remaining, or 0 if not using shunting
	IN OPTIONAL DWORD nShuntRemaining,

	// The MPQDraftDLL patching function - don't make this CPP include the MPQDraftDLL header
	IN LPPATCHERENTRYPOINT pfnEntryPoint
)
{
	assert(hPatchContext);
	assert(hPatchMutex);
	assert(pfnEntryPoint);

	memset(lpProcessInformation, 0, sizeof(*lpProcessInformation));

	// First, create the target process. Use the supplied parameters, but create it suspended so that we can inject into it. If the caller doesn't want it suspended, we'll resume it later.
	if (!CreateProcess(lpszApplicationName, 
		lpszCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED, lpszEnvironment, lpszCurrentDirectory, lpStartupInfo, lpProcessInformation))
		return FALSE;

	// Next, inject the context into the process
	HANDLE hChildContext, hChildMutex;

	if (DuplicatePatchContextInChild(lpProcessInformation->hProcess, hPatchContext, hPatchMutex, hChildContext, hChildMutex))
	{
		// Set up the data to pass to the patcher
		MPQDRAFTPATCHERDATA patcherData;

		patcherData.hPatchContext = hChildContext;
		patcherData.hContextMutex = hChildMutex;
		patcherData.nShuntRemaining = nShuntRemaining;

		// Then inject the DLL
		if (InjectDLLIntoProcessV3(lpProcessInformation->hProcess, lpProcessInformation->hThread, NULL, pfnEntryPoint, &patcherData, sizeof(patcherData)))
		{
			// Patching is done. What we do next depends on what the caller wants - specifically, whether the process is to be created suspended
			if (!(dwCreationFlags & CREATE_SUSPENDED))
				ResumeThread(lpProcessInformation->hThread);	// Let 'er rip

			return TRUE;
		}
	}

	// Failure. Clean up the process.
	TerminateProcess(lpProcessInformation->hProcess, 0);

	CloseHandle(lpProcessInformation->hProcess);
	CloseHandle(lpProcessInformation->hThread);

	return FALSE;
}