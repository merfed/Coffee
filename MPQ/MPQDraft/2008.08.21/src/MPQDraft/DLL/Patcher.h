/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#if !defined(PATCHER_H)
#define PATCHER_H

#include <windows.h>
#include <MPQDraftPlugin.h>

// The MPQDraft patcher forms an additional layer of patching outside of the patcher in QInjectDLL. It adds on all the features necessary to do the patching MPQDraft requires, as opposed to merely providing basic patching services.

// These constants define the size of arrays used in the patching process. It should be possible to remove these limitations by dynamically allocating the memory for the arrays; I just haven't (though I'm generally wary of allocating memory in target processes).
#define MAX_PATCH_MPQS 8
#define MAX_MPQDRAFT_PLUGINS 8
#define MAX_AUXILIARY_MODULES 32

// Data and functions related to the patch context

// One thing that's worth noting is the meaning of the target path and the shunt count. Both the target path and shunting system are parts of the solution to one problem: how to determine which process is the one we want to patch, so that we don't load MPQs and plugins in every single process we happen to get injected into. There are two reasons this is not straightforward: first, games like Diablo II and Warcraft III have SecuROM wrapper executables which are executed every time the game is run. The purpose of these is to display a splash screen, wrap creation of the actual game executable, and then display an error message if the game fails to decrypt from CD-ROM; if this fails, the game executable will not be able to execute, and simply fails to start. As the stub is a different file than the game executable, the target path tells us which is the one we want to patch.
// However, I observed even stranger behavior from one game: Diablo. For reasons unknown (I never did investigate, but I wouldn't be surprised if it was also related to DRM), when the Diablo executable is first run, it would spawn a second copy of the same executable, and then exit. This is what the shunt system is for: it tells the patcher how many times the target path should be executed before the patcher executes. E.g. 0 means it should execute the first time, 1 means it should skip the first execution, etc.

// The structure that holds all the configuration information to perform an MPQDraft patch operation. One context will be created for each patching operation, and shared by all processes participating in the patch. This structure is the constant part of the patch context. After the constant part come the MPQ and module names, which are char(MPQDRAFT_MAX_PATH) and sizeof(MPQDRAFTPLUGINMODULE) sized, respectively. The patch context has an associated mutex used to protect writable members. However, at present there are no writable members. The shunt count remaining is not contained here, as it is something specific to each process, and cannot be shared.
struct MPQDRAFTPATCHCONTEXT
{
	DWORD dwSize;	// Size of this structure (as it's now variable)
	DWORD dwFlags;		// Patcher flags, constant (maybe)

	char szTargetPath[MPQDRAFT_MAX_PATH];	// Constant
	DWORD nShuntCount;	// Constant

	char szMPQDraftDir[MPQDRAFT_MAX_PATH];	// Constant
	char szStartupDir[MPQDRAFT_MAX_PATH];	// Constant

	// To save size, we won't hard-code the numbers here. We'll let the 
	// initial patcher allocate memory for them, and just give us the offsets
	// to the arrays. Each entry in any of the arrays will be MPQDRAFT_MAX_PATH
	// chars.
	DWORD nPatchMPQs;	// Constant
	DWORD nModules;	// Constant

	DWORD dwMPQNamesOffset;	// MPQs to load, constant
	DWORD dwModulesOffset;	// Plugin module array, constant

	// Modules must have been already extracted to files on disk by the caller, so we need only to pass the paths to the modules to the patch target.
};

// Claims ownership of the shared patch context simply by entering the mutex
BOOL ClaimContext(HANDLE hContextMutex);
// Release ownership of the patch context by leaving the mutex
BOOL ReleaseContext(HANDLE hContextMutex);

// Creates a patch context for a new patching session. This function is not to be used when creating a new process in an existing patching session (e.g. from inside a patched process).
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
);

// This is a little bit misleading of a name. All it does is inject the patch context and mutex handles into the child process, so that they can access them.
BOOL DuplicatePatchContextInChild(
	// The child process to inject the handles into
	IN HANDLE hChildProcess,
	// The patch context in this process
	IN HANDLE hCurrentContext,
	// The patch context mutex in this process
	IN HANDLE hCurrentContextMutex,
	// The patch context HANDLE in the child process
	OUT HANDLE &rhChildContext,
	// The patch context mutex HANDLE in the child process
	OUT HANDLE &rhChildContextMutex
);

// Opens the patch context by mapping the injected HANDLE into the address space. This is used inside patched processes. Returns NULL on failure.
MPQDRAFTPATCHCONTEXT *OpenContextHandle(IN HANDLE hContext);
// Closes everything associated with the patch context - the HANDLE to the context mapping, the mapping itself, and the context mutex HANDLE.
void CloseContext(IN HANDLE hContext, IN MPQDRAFTPATCHCONTEXT *lpContext, IN HANDLE hContextMutex);
// Wait for all processes that began with us to close, so that we can return to the caller (MPQDraft itself)
void WaitForContextClose();

// Data and functions relating to the patching operation itself

// The data passed to a patched process. Contains the information needed by the patcher to determine what to do with the process - how to access the patch context, and the number of shunts remaining for this level of the process tree.
struct MPQDRAFTPATCHERDATA
{
	HANDLE hPatchContext;
	HANDLE hContextMutex;

	DWORD nShuntRemaining;
};

// The common function called to patch a process by either of the two paths. As the two methods only differ by how the patch context is created, creation of the patch context must be done before this function is called to actually do the patching, either through CreatePatchContext or the patch context for the current process.
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
);

#endif