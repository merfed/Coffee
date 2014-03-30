/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#include "stdafx.h"
#include "MPQDraftDLL.h"

// Typedefs for the Storm function pointers we're going to use in this DLL
typedef BOOL (WINAPI *SFileOpenArchivePtr)(IN LPCSTR lpFileName, IN DWORD dwPriority, IN DWORD dwFlags, OUT HANDLE *lphMPQ);
typedef BOOL (WINAPI *SFileCloseArchivePtr)(IN HANDLE hMPQ);

typedef BOOL (WINAPI *SFileOpenFileExPtr)(IN HANDLE hMPQ, IN LPCSTR lpFileName, IN DWORD dwSearchScope, OUT HANDLE *lphFile);

typedef BOOL (WINAPI *SFileLoadFileExPtr)(IN HANDLE hMPQ, IN LPCSTR lpFileName, OUT LPVOID *lplpFileData, OUT LPDWORD lpFileSize, IN DWORD dwParam5, IN DWORD dwSearchScope, DWORD dwParam7);
typedef BOOL (WINAPI *SFileLoadFileEx2Ptr)(IN HANDLE hMPQ, IN LPCSTR lpFileName, OUT LPVOID *lplpFileData, OUT LPDWORD lpFileSize, IN DWORD dwParam5, IN DWORD dwSearchScope, DWORD dwParam7, DWORD dwParam8);

typedef BOOL (WINAPI *SFileOpenFileAsArchivePtr)(IN HANDLE hMPQ, IN LPCSTR lpszFileName, IN DWORD dwPriority, IN DWORD dwFlags, OUT HANDLE *lphMPQ);
typedef BOOL (WINAPI *SFileOpenPathAsArchivePtr)(IN HANDLE hParentMPQ, IN LPCSTR lpszPath, IN DWORD nArchivePriority, IN DWORD dwUnused, OUT HANDLE *lphChildArchive);

// Skywing's function to gather debug information when a crash occurs
int HandleException(IN DWORD dwExceptionCode, IN LPEXCEPTION_POINTERS lpExceptionPointers, IN LPVOID lpCallerAddress, IN bool bFatal);

// Bootstrap variables
HMODULE hMPQDraftDLL = NULL; // The MPQDraft DLL itself, constant

CRITICAL_SECTION g_csDataLock;	// Protects writable global data

// Global MPQDraft variables
// The patch context and mutex, constant
HANDLE hPatchContext = NULL;
MPQDRAFTPATCHCONTEXT *lpPatchContext = NULL;
HANDLE hPatchMutex = NULL;

DWORD nShuntRemaining;	// *Constant

// What operations should be done in this process
BOOL bLoadMPQs = FALSE; // *Constant
BOOL bLoadPlugins = FALSE; // *Constant

BOOL bHideFailureMessage = FALSE; // *Constant

// MPQ-loading variables

// The number of MPQs the host process has opened. This is used to determine when we should active and load our MPQs, and later unload them. Our MPQs are not actually loaded until the host tries to open the first MPQ, for historical reasons relating to Windows 9x. The MPQs are then unloaded when the host unloads its last MPQ.
DWORD nOpenHostMPQs = 0; // Protected by g_csDataLock
// Whether we've loaded our patch MPQs yet
BOOL bMPQsLoaded = FALSE; // *Protected by g_csDataLock
DWORD nOpenPatchMPQs = 0; // Protected by g_csDataLock
HANDLE hOpenPatchMPQs[MAX_PATCH_MPQS]; // Protected by g_csDataLock

// Plugin-related variables
CPluginServer PluginServer; // Constant

// The number of plugins, their IMPQDraftPlugin interfaces, and their DLL handles
DWORD nNumPlugins = 0; // Constant
IMPQDraftPlugin *pPlugins[MAX_MPQDRAFT_PLUGINS]; // Constant
HMODULE hPluginModules[MAX_MPQDRAFT_PLUGINS]; // Constant

// Function pointers and other Storm stuff - constant
// Storm goes by a few different names. Normally Storm.dll, but it has also been observed to be StormAE.dll (asserts enabled) and StormBE.dll in some beta builds.
LPCSTR lpszStormNames[] = {"Storm.dll", "StormAE.dll", "StormBE.dll", NULL};
// The name and handle of Storm in this porcess
LPCSTR lpszStormName = NULL; // Constant
HMODULE hStormDLL = NULL; // Constant

// Required functions - will always be patched if MPQs are to be loaded
// As Storm has function names stripped, we have to identify the functions we want to use by ordinal. These are the ones that are present in every version of Storm and, so far, have been observed to have invariant ordinals.
const DWORD nOpenArchiveOrd = 0x10A;
const DWORD nCloseArchiveOrd = 0xFC;

const DWORD nOpenFileExOrd = 0x10C;

// The functions themselves
SFileOpenArchivePtr SFileOpenArchive = NULL; // Constant
SFileCloseArchivePtr SFileCloseArchive = NULL; // Constant

SFileOpenFileExPtr SFileOpenFileEx = NULL; // Constant

HMODULE hKernel32 = NULL;	// Constant

// Non-required functions - if we can't get pointers, don't patch them!
// These functions are ones which are not present in every version of Storm. Thus we patch them if they're there, and ignore them if they're not.
const DWORD nLoadFileExOrd = 0x119;
// Blizzard did something really evil in Diablo II and Warcraft III: the ordinals of new functions in Storm got out of sync. Thus we have two sets of ordinals for this one - the initial value is the one for Warcraft III.
DWORD nLoadFileEx2Ord = 0x124;	// 0x122 for Diablo 2

const DWORD nOpenFileAsArchiveOrd = 0x125;
const DWORD nOpenPathAsArchiveOrd = 0x12C;

SFileLoadFileExPtr SFileLoadFileEx = NULL; // Constant
SFileLoadFileEx2Ptr SFileLoadFileEx2 = NULL; // Constant

SFileOpenFileAsArchivePtr SFileOpenFileAsArchive = NULL; // Constant
SFileOpenPathAsArchivePtr SFileOpenPathAsArchive = NULL; // Constant

// This is just a stub to call Skywing's exception dump code
LONG WINAPI MyUnhandledExceptionFilter(IN _EXCEPTION_POINTERS *ExceptionInfo)
{
	return HandleException(ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo, MyUnhandledExceptionFilter, TRUE);
}

BOOL APIENTRY DllMain(IN HANDLE hInstDLL, IN DWORD fdwReason, IN LPVOID lpvReserved)
{
	QDebugWriteEntry("DllMain(0x%X, 0x%X, %d)", hInstDLL, fdwReason, lpvReserved);

	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			hMPQDraftDLL = (HMODULE)hInstDLL;

			// Because of the method we use for patching, we're going to be called before the process start function establishes the default unhandled exception filter. Thus we need to set up our own frame, or things would break if an exception got thrown.
			__try
			{
				//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);

				InitializeCriticalSection(&g_csDataLock);

			}
			// Use our custom handler because it's more effective than the Windows one
			__except(MyUnhandledExceptionFilter(GetExceptionInformation()))
			{
			}

			break;
		}

		case DLL_PROCESS_DETACH:
			QDebugWriteEntry("DllMain : DLL_PROCESS_DETACH, process %d", GetCurrentProcessId());

			MPQDraftTerminate();

			QDebugCloseLogFile();

			DeleteCriticalSection(&g_csDataLock);

			break;

		case DLL_THREAD_ATTACH:
			QDebugWriteEntry("DllMain() : DLL_THREAD_ATTACH, thread %d", GetCurrentThreadId());
			break;
		case DLL_THREAD_DETACH:
			QDebugWriteEntry("DllMain() : DLL_THREAD_DETACH, thread %d", GetCurrentThreadId());
			break;
    }

    return TRUE;
}

// SFileOpenArchive: the first of the Storm MPQ functions we need to patch. Really, we're not interested in changing the way this function operates, as opening MPQs (or closing them, for that matter), doesn't affect how MPQDraft works. We hook it, however, because we need to know when the first MPQ is opened, so that we can open our MPQs. The reason for this is historical. Using the injection method MPQDraft does, on Windows 9x, it's possible that the patching function will get called before any of the DllMains get called. If Storm's DllMain hasn't been called and we call SFileOpenArchive, that would be bad. That one was a major pain to debug (in fact, as I couldn't get it to reproduce on any computer I had access to, I had to have the one person who had this problem debug it themselves), and it wasn't until years later that I learned the exact mechanism of this occurring (or rather, of DllMain not occurring); I think I posted this information on my blog at one point.
BOOL WINAPI PatchOpenArchive(IN LPCSTR lpFileName, IN DWORD dwMPQPriority, IN DWORD dwFlags, OUT HANDLE *lphMPQ)
{
	QDebugWriteEntry("PatchOpenArchive(\"%s\", %d, 0x%X, x)", lpFileName, dwMPQPriority, dwFlags);

	BOOL bRetVal;

	// As this touches writable patcher data, we need to synchronize access
	EnterCriticalSection(&g_csDataLock);

	// If this is the first MPQ and we need to load MPQs, do so now
	if (!nOpenHostMPQs && !bMPQsLoaded)
	{
		bRetVal = LoadPatchMPQs();
		QDebugWriteEntry("PatchOpenArchive : LoadPatchMPQs returned %d", bRetVal);

		if (!bRetVal)
		{
			MessageBox(NULL, "MPQDraft has encountered a fatal error: the patch MPQs could not be loaded. The game will now close.", "MPQDraft Error", MB_OK | MB_ICONSTOP);

			MPQDraftAbort();
		}
	}

	bRetVal = SFileOpenArchive(lpFileName, dwMPQPriority, dwFlags, lphMPQ);
	
	if (bRetVal)
		nOpenHostMPQs++;
	QDebugWriteEntry("PatchOpenArchive : SFileOpenArchive returned %d, handle 0x%X; %d MPQs open", bRetVal, *lphMPQ, nOpenHostMPQs);

	LeaveCriticalSection(&g_csDataLock);

	return bRetVal;
}

// SFileCloseArchive. This is hooked for exactly the same reason as SFileOpenArchive: so that we know when to close our MPQs.
BOOL WINAPI PatchCloseArchive(IN HANDLE hMPQ)
{
	QDebugWriteEntry("PatchCloseArchive(0x%X)", hMPQ);

	BOOL bRetVal = SFileCloseArchive(hMPQ);

	EnterCriticalSection(&g_csDataLock);

	if (bRetVal)
		nOpenHostMPQs--;
	QDebugWriteEntry("PatchCloseArchive : SFileCloseArchive returned %d; %d open MPQs", bRetVal, nOpenHostMPQs);

	if (!nOpenHostMPQs && bMPQsLoaded)
		UnloadPatchMPQs();

	LeaveCriticalSection(&g_csDataLock);

	return bRetVal;
}

// NOTE: Always redirects (Redirect SFileOpenFileEx Calls)
// Our SFileOpenArchiveEx hook. As most of the work in MPQDraft is handled by Storm itself (we're kind of viral that way), we don't exactly need to do much here. However, this function optionally takes a handle to an MPQ; if there is one, it will only load the file from that archive, which would bypass our patch MPQs. Thus we need to make sure it doesn't do that.
BOOL WINAPI PatchOpenFileEx(IN OPTIONAL HANDLE hMPQ, IN LPCSTR lpFileName, IN DWORD dwSearchScope, OUT HANDLE *lphFile)
{
	QDebugWriteEntry("PatchOpenFileEx(0x%X, \"%s\", 0x%X, x)", hMPQ, lpFileName, dwSearchScope);

	// Force it to use the priority chain
	BOOL bRetVal = SFileOpenFileEx(NULL, lpFileName, dwSearchScope, lphFile);

	QDebugWriteEntry("PatchOpenFileEx : SFileOpenFileEx returned %d, handle at 0x%X", bRetVal, *lphFile);

	return bRetVal;
}

// SFileLoadFileEx hook. This is exactly the same thing we saw in SFileOpenFileEx.
BOOL WINAPI PatchLoadFileEx(IN OPTIONAL HANDLE hMPQ, IN LPCSTR lpFileName, OUT LPVOID *lplpFileData, OUT LPDWORD lpFileSize, DWORD dwParam5, IN DWORD dwSearchScope, DWORD dwParam7)
{
	QDebugWriteEntry("PatchLoadFileEx(0x%X, \"%s\", 0x%X, 0x%X, 0x%X, %d, 0x%X)", hMPQ, lpFileName, lplpFileData, lpFileSize, dwParam5, dwSearchScope, dwParam7);

	BOOL bRetVal = SFileLoadFileEx(NULL, lpFileName, lplpFileData, lpFileSize, dwParam5, dwSearchScope, dwParam7);

	QDebugWriteEntry("PatchLoadFileEx : SFileLoadFileEx returned %d", bRetVal);
	if (bRetVal)
		QDebugWriteEntry("PatchLoadFileEx : File loaded at 0x%X, file size %d", *lplpFileData, *lpFileSize);

	return bRetVal;
}

// And one more time, in SFileLoadFileEx2
BOOL WINAPI PatchLoadFileEx2(IN OPTIONAL HANDLE hMPQ, IN LPCSTR lpFileName, OUT LPVOID *lplpFileData, OUT LPDWORD lpFileSize, DWORD dwParam5, IN DWORD dwSearchScope, DWORD dwParam7, DWORD dwParam8)
{
	QDebugWriteEntry("PatchLoadFileEx2(0x%X, \"%s\", 0x%X, 0x%X, 0x%X, %d, 0x%X, 0x%X)", hMPQ, lpFileName, lplpFileData, lpFileSize, dwParam5, dwSearchScope, dwParam7, dwParam8);

	BOOL bRetVal = SFileLoadFileEx2(NULL, lpFileName, lplpFileData, lpFileSize, dwParam5, dwSearchScope, dwParam7, dwParam8);

	QDebugWriteEntry("PatchLoadFileEx2 : SFileLoadFileEx2 returned %d", bRetVal);
	if (bRetVal)
		QDebugWriteEntry("PatchLoadFileEx2 : File loaded at 0x%X, file size %d", *lplpFileData, *lpFileSize);

	return bRetVal;
}

// SFileOpenFileAsArchive. This one is like a cross between SFileOpenArchive and SFileOpenFileEx. If we're in MPQD_EXTENDED_REDIR mode, we need to make sure it uses the priority chain. But we also need to check if we're opening the first archive, so that we can open our patch archives.
BOOL WINAPI PatchOpenFileAsArchive(IN OPTIONAL HANDLE hMPQ, IN LPCSTR lpszFileName, IN DWORD dwPriority, IN DWORD dwFlags, OUT HANDLE *lphMPQ)
{
	QDebugWriteEntry("PatchOpenFileAsArchive(0x%X, \"%s\", %d, 0x%X, x)", hMPQ, lpszFileName, dwPriority, dwFlags);

	BOOL bRetVal;

	EnterCriticalSection(&g_csDataLock);

	if (!nOpenHostMPQs && !bMPQsLoaded)
	{
		bRetVal = LoadPatchMPQs();
		QDebugWriteEntry("PatchOpenArchive : LoadPatchMPQs returned %d", bRetVal);

		if (!bRetVal)
		{
			MessageBox(NULL, "MPQDraft has encountered a fatal error: the patch MPQs could not be loaded. The game will now close.", "MPQDraft Error", MB_OK | MB_ICONSTOP);

			MPQDraftAbort();
		}
	}

	if (lpPatchContext->dwFlags & MPQD_EXTENDED_REDIR)
		hMPQ = NULL;	// Search open MPQs
	bRetVal = SFileOpenFileAsArchive(hMPQ, lpszFileName, dwPriority, dwFlags, lphMPQ);
	
	if (bRetVal)
		nOpenHostMPQs++;
	QDebugWriteEntry("PatchOpenFileAsArchive : SFileOpenFileAsArchive returned %d, handle 0x%X; %d MPQs open", bRetVal, *lphMPQ, nOpenHostMPQs);

	LeaveCriticalSection(&g_csDataLock);

	return bRetVal;
}

// SFileOpenPathAsArchive hook. While we need to redirect it to use the priority chain, we don't need to deal with loading our MPQs, because this can't be the first archive open in a process - an archive has to be opened with one of the other two functions first.
BOOL WINAPI PatchOpenPathAsArchive(IN HANDLE hParentMPQ, IN LPCSTR lpszPath, IN DWORD nArchivePriority, DWORD dwUnused, OUT HANDLE *lphChildArchive)
{
	QDebugWriteEntry("PatchOpenPathAsArchive(0x%X, \"%s\", %d, 0x%X, x)", hParentMPQ, lpszPath, nArchivePriority, dwUnused);

	BOOL bRetVal = SFileOpenPathAsArchive(NULL, lpszPath, nArchivePriority, dwUnused, lphChildArchive);

	EnterCriticalSection(&g_csDataLock);

	if (bRetVal)
		nOpenHostMPQs++;
	QDebugWriteEntry("PatchOpenPathAsArchive : SFileOpenPathAsArchive returned %d, handle 0x%X; %d MPQs open", bRetVal, *lphChildArchive, nOpenHostMPQs);

	LeaveCriticalSection(&g_csDataLock);

	return bRetVal;
}

// Patch function which hooks the CreateProcessA call. This is necessary because we will usually want to inject the MPQDraft DLL into the child process as well.
BOOL WINAPI PatchCreateProcessA(
  LPCSTR lpApplicationName,                 // name of executable module
  LPSTR lpszCommandLine,                      // command line string
  LPSECURITY_ATTRIBUTES lpProcessAttributes, // SD
  LPSECURITY_ATTRIBUTES lpThreadAttributes,  // SD
  BOOL bInheritHandles,                      // handle inheritance option
  DWORD dwCreationFlags,                     // creation flags
  LPVOID lpEnvironment,                      // new environment block
  LPCSTR lpCurrentDirectory,                // current directory name
  LPSTARTUPINFO lpStartupInfo,               // startup information
  LPPROCESS_INFORMATION lpProcessInformation // process information
)
{
	QDebugWriteEntry("PatchCreateProcessA(\"%s\", \"%s\", x, x, %d, x, 0x%X, \"%s\", x, x)",
		(lpApplicationName ? lpApplicationName : "NULL"),
		(lpszCommandLine ? lpszCommandLine : "NULL"),
		bInheritHandles, dwCreationFlags,
		(lpCurrentDirectory ? lpCurrentDirectory : "NULL"));

	BOOL bRetVal = CreateAndPatchProcess(lpApplicationName, lpszCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, hPatchContext, hPatchMutex, (nShuntRemaining > 0 ? nShuntRemaining - 1 : 0), (LPPATCHERENTRYPOINT)MPQDraftInitialize);

	QDebugWriteEntry("PatchCreateProcessA : CreateAndPatchProcess returned %d, process ID %d, thread ID %d", bRetVal, lpProcessInformation->dwProcessId, lpProcessInformation->dwThreadId);

	if (bRetVal)
	{
		if (!(dwCreationFlags & CREATE_SUSPENDED))
			ResumeThread(lpProcessInformation->hThread);
	}

	return bRetVal;
}

// We need to patch ExitProcess to correct a bug where the real ExitProcess would unload the DLLs in a different order than they were loaded, and when MPQDraft tried to unload its plugins, it would crash. So we'll intercept the ExitProcess call and unload the plugins before the real ExitProcess ever gets called.
void WINAPI PatchExitProcess(IN DWORD nExitCode)
{
	QDebugWriteEntry("PatchExitProcess(%d)", nExitCode);

	// Now there may be other threads still running, so we have to execute the shutdown atomically
	EnterCriticalSection(&g_csDataLock);

	/* Unloading plugins turned out to be a huge can of poorly-thought-out worms, and there still isn't really a good solution to the problem. There are several problems contributing to a huge mess, here. The way MPQDraft plugins were originally intended to work was that MPQDraft would call TerminatePlugin and unload the plugins when it received DLL_PROCESS_DETACH. This turned out to be a very poor idea for a couple reasons. First, there's no way we can ensure that DLL_PROCESS_DETACH will be sent to the MPQDraft DLL before it reaches plugin DLLs, meaning we might try to unload some plugin DLLs twice; that's bad. Second, it's always a bad idea to do too much in DllMain, due to the loader lock.
	However, that's just the start of the problem, which turned out to be much larger. The problem is that we're patching things in other modules: hooking functions, etc. That means that there's always the possibility that functions in plugins could get called after the plugins have terminated.
	While in theory we could avoid this problem by having TerminatePlugin unpatch the modules (as I initially envisioned), this idea is also flawed due to the possibility of multithreading: it's always possible that another thread could read our hook's address, get preempted, then get later call the hooked function, after the plugin has "removed" the hook. 
	This can be mitigated by some careful behavior: once TerminatePlugin is called, all hook functions must behave as if they were the original functions that are hooked. They will also need to be able to handle calls arriving AFTER the DLL has been "unloaded" by DLL_PROCESS_DETACH, as that will eventually get called in an indeterminate order. The plugin DLLs themselves, however, can never be unloaded, as an instant crash would result if a hooked function that is no longer in memory gets called. 
	As ANY code in a DLL executing after DLL_PROCESS_DETACH has been called is a bad thing, this is clearly not an ideal solution. It's merely the best of the available options. The problem with it is that it requires very delicate behavior by both the MPQDraft DLL and the plugins themselves, and I have no idea how many plugins would survive such a process (ThunderGraft sure doesn't ATM). Consequently, we currently take the ostrich approach and ignore the problem and hope nothing bad happens.
	// Unload the plugins and modules in reverse order
	UnloadPlugins(); */

	UnloadPatchMPQs();

	LeaveCriticalSection(&g_csDataLock);

	// Goodnight, world
	ExitProcess(nExitCode);
}

// Function that replaces LoadLibraryA. We need to patch this to make sure than any DLLs loaded by the game or one of the plugins gets patched to go through our hooked MPQ functions.
HMODULE WINAPI PatchLoadLibraryA(IN LPCSTR lpFileName)
{
	QDebugWriteEntry("PatchLoadLibrary(\"%s\")", lpFileName);

	HMODULE hModule = LoadLibraryA(lpFileName);
	QDebugWriteEntry("PatchLoadLibrary : LoadLibrary returned 0x%X", hModule);

	if (!hModule)
		return NULL;

	BOOL bRetVal = PatchModuleFunctions(hModule);
	QDebugWriteEntry("PatchLoadLibrary : PatchModuleFunctions returned %d", bRetVal);

	return hModule;
}

// Hook for GetProcAddress. This is yet another vertex of our polygon of "sandboxing" of the host process. We've already patched the import tables of all the modules we've found in the process. Now we need to make sure that calls to GetProcAddress (the other way of getting the address of a function) suffer the same fate for the functions we need to hook.
FARPROC WINAPI PatchGetProcAddress(IN HMODULE hModule, IN LPCSTR lpProcName)
{
	// Recall that lpProcName can be either a function name or a function ordinal. These are easy to differentiate because ordinals go from 0-65535, meaning they would be invalid pointers (specifically, falling in the NULL pointer sentinel region of the address space).
	/*QDebugWriteEntry("PatchGetProcAddress(0x%X, \"%s\"/0x%X)", hModule,
		((DWORD)lpProcName > 64 << 10) ? lpProcName : "NaN", lpProcName);*/

	DWORD dwOrdinal = (DWORD)lpProcName;

	// If we're not supposed to load MPQs, we don't really care if they use our MPQ functions or not
	if (bLoadMPQs)
	{
		// Ensure it's a Storm ordinal
		if ((hModule == hStormDLL) && (dwOrdinal < 0x10000))
		{
			if (dwOrdinal == nOpenArchiveOrd)
				return (FARPROC)PatchOpenArchive;
			if (dwOrdinal == nCloseArchiveOrd)
				return (FARPROC)PatchCloseArchive;

			if (dwOrdinal == nOpenFileAsArchiveOrd)
				return (FARPROC)PatchOpenFileAsArchive;
			if (dwOrdinal == nOpenPathAsArchiveOrd)
				return (FARPROC)PatchOpenPathAsArchive;

			// Catch the extended ones if necessary
			if (lpPatchContext->dwFlags & MPQD_EXTENDED_REDIR)
			{
				if (dwOrdinal == nOpenFileExOrd)
					return (FARPROC)PatchOpenFileEx;

				if (dwOrdinal == nLoadFileExOrd)
					return (FARPROC)PatchLoadFileEx;
				if (dwOrdinal == nLoadFileEx2Ord)
					return (FARPROC)PatchLoadFileEx2;
			}
		}
	}

	// Finally, check if it's one of our Kernel32 hooks
	if (hModule == hKernel32)
	{
		if (!_stricmp(lpProcName, "CreateProcessA"))
			return (FARPROC)PatchCreateProcessA;
		if (!_stricmp(lpProcName, "ExitProcess"))
			return (FARPROC)PatchExitProcess;
		if (!_stricmp(lpProcName, "LoadLibraryA"))
			return (FARPROC)PatchLoadLibraryA;
		if (!_stricmp(lpProcName, "GetProcAddress"))
			return (FARPROC)PatchGetProcAddress;
	}
	
	// Answer E: none of the above
	FARPROC pfnProc = GetProcAddress(hModule, lpProcName);
	//QDebugWriteEntry("PatchLoadLibrary : GetProcAddress returned 0x%X", pfnProc);

	return pfnProc;
}

// Initializes the MPQDraft patch for this process. This function will be called directly from the injection code after DLL_PROCESS_ATTACH has already been called.
extern "C" BOOL WINAPI MPQDraftInitialize(IN MPQDRAFTPATCHERDATA *pData, IN DWORD nDataSize)
{
	assert(pData);
	assert(nDataSize == sizeof(*pData));

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)MyUnhandledExceptionFilter);

	// Open and write the header for the debug log
	assert(QDebugOpenLogFile("MPQDraft Patcher", NULL, 0));

	QDebugWriteEntry("MPQDraftInitialize(0x%X (0x%X, 0x%X), %d)", pData, pData->hPatchContext, pData->hContextMutex, nDataSize);

#ifdef _DEBUG
	// This is a feature I've used plenty to debug the patching system. If you hold down shift and control while this process is starting up, it will ask you if you want to break into the debugger. If you attach to the process with a debugger then, it will throw a breakpoint exception before most of the patching code gets executed.
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
	{
		if (MessageBox(NULL, "MPQDraft debugging feature enabled. Do you wish to break for debugger?", "MPQDraft", MB_YESNO) == IDYES)
			DebugBreak();
	}
#endif

	// Initialize the data that couldn't be assigned in declaration
	ZeroMemory(hOpenPatchMPQs, MAX_PATCH_MPQS * sizeof(HANDLE));

	ZeroMemory(pPlugins, MAX_MPQDRAFT_PLUGINS * sizeof(IMPQDraftPlugin));
	ZeroMemory(hPluginModules, MAX_MPQDRAFT_PLUGINS * sizeof(HMODULE));

	// Map the patch context so that we can begin patching
	lpPatchContext = OpenContextHandle(pData->hPatchContext);
	QDebugWriteEntry("MPQDraftInitialize : OpenContextHandle returned 0x%X", lpPatchContext);

	if (!lpPatchContext)
		return FALSE;

	hPatchContext = pData->hPatchContext;
	hPatchMutex = pData->hContextMutex;

	nShuntRemaining = pData->nShuntRemaining;

	// Validate and load the data in the patch context, if necessary
	BOOL bRetVal = LoadContext();
	QDebugWriteEntry("MPQDraftInitialize : LoadContext returned %d", bRetVal);

	if (!bRetVal)
		return FALSE;

	// Patch any functions we need to intercept, including the Storm functions and some Windows function
	bRetVal = PatchFunctions();
	QDebugWriteEntry("MPQDraftInitialize : PatchFunctions returned %d", bRetVal);

	if (!bRetVal)
	{
		MessageBox(NULL, "MPQDraft has encountered a fatal error: the game does not appear to use MPQs. The game will now close.", "MPQDraft Error", MB_OK | MB_ICONSTOP);
		bHideFailureMessage = TRUE;

		return FALSE;
	}

	if (bLoadPlugins)
	{
		// If we're supposed to load modules in this process, notify the plugin server and load the plugins
		MPQDRAFTPLUGINMODULE *lpModules = (MPQDRAFTPLUGINMODULE *)((DWORD)lpPatchContext + lpPatchContext->dwModulesOffset);

		PluginServer.SetModules(lpModules, lpPatchContext->nModules);

		bRetVal = LoadPlugins();
		QDebugWriteEntry("MPQDraftInitialize : LoadPlugins returned %d", bRetVal);

		if (!bRetVal)
		{
			MessageBox(NULL, "MPQDraft has encountered a fatal error: the plugins could not be loaded. The game will now close.", "MPQDraft Error", MB_OK | MB_ICONSTOP);
			bHideFailureMessage = TRUE;

			return FALSE;
		}
	}

	return TRUE;
}

// Validates and loads the data in the patch context, as well as initializes some variables based on the data in the context
BOOL LoadContext()
{
	QDebugWriteEntry("LoadContext()");

	// Log the general stuff
	QDebugWriteEntry("LoadContext : Context flags 0x%X, %d MPQs, %d modules", 
		lpPatchContext->dwFlags, lpPatchContext->nPatchMPQs, lpPatchContext->nModules);
	QDebugWriteEntry("LoadContext : Patch target path \"%s\", MPQDraft directory \"%s\"", lpPatchContext->szTargetPath, lpPatchContext->szMPQDraftDir);

	// Log the MPQs to load
	for (DWORD iCurMPQ = 0; iCurMPQ < lpPatchContext->nPatchMPQs; iCurMPQ++)
	{
		LPCSTR lpszPatchMPQ = (LPCSTR)lpPatchContext + 
			lpPatchContext->dwMPQNamesOffset + iCurMPQ * MPQDRAFT_MAX_PATH;
		QDebugWriteEntry("LoadContext : MPQ slot %d path \"%s\"", iCurMPQ, lpszPatchMPQ);
	}

	// Log the plugins to load
	MPQDRAFTPLUGINMODULE *pModules = (MPQDRAFTPLUGINMODULE *)
		((DWORD)lpPatchContext + lpPatchContext->dwModulesOffset);
	for (DWORD iCurModule = 0; iCurModule < lpPatchContext->nModules; iCurModule++)
	{
		QDebugWriteEntry("LoadContext : Module slot %d plugin ID 0x%X, module ID 0x%X, path \"%s\", execute %d", 
			iCurModule, pModules[iCurModule].dwComponentID, 
			pModules[iCurModule].dwModuleID, 
			pModules[iCurModule].szModuleFileName,
			pModules[iCurModule].bExecute);
	}

	// Determine whether this is the desired patch target, as well as whether we should load MPQs or plugins in this process
	BOOL bIsPatchTarget = IsPatchTarget();
	QDebugWriteEntry("LoadContext : IsPatchTarget returned %d", bIsPatchTarget);

	if (bIsPatchTarget && lpPatchContext->nPatchMPQs)
		bLoadMPQs = TRUE;

	if (bIsPatchTarget && lpPatchContext->nModules)
		bLoadPlugins = TRUE;

	return TRUE;
}

// Determines whether this is the process we want to patch, based on several different settings
BOOL IsPatchTarget()
{
	QDebugWriteEntry("IsPatchTarget()");

	BOOL bIsPatchTarget = FALSE;

	// If we don't support spawning, we don't need to worry about patch targets
	if (lpPatchContext->dwFlags & MPQD_NO_SPAWNING)
		return TRUE;

	// Get the handle of the executable module and compare it to the module of the one we want. Usually if they're not the same the target module won't even be loaded, but perhaps in some rare circumstances it will be, so it's best to get both and compare the two.
	HMODULE hHostModule = GetModuleHandle(NULL),
		hTargetModule = GetModuleHandle(lpPatchContext->szTargetPath);

	if (hHostModule != hTargetModule)
		return FALSE;

	// This is the correct executable. However, we still don't want to activate if we still need to shunt. Only activate if shunt count or shunts remaining is 0
	QDebugWriteEntry("IsPatchTarget : Shunt count: %d, shunts remaining: %d", 
		lpPatchContext->nShuntCount, nShuntRemaining);

	return (lpPatchContext->nShuntCount == 0 || nShuntRemaining == 0);
}

// Does everything relating to patching the functions, from locating the modules and functions to patch, then patching all modules imported by the host process
BOOL PatchFunctions()
{
	QDebugWriteEntry("PatchFunctions()");

	BOOL bRetVal;

	// If we're supposed to load our MPQs, locate storm and get the addresses of the functions we want to patch
	if (bLoadMPQs)
	{
		bRetVal = LocateStorm();
		QDebugWriteEntry("PatchFunctions : LocateStorm returned %d", bRetVal);

		if (!bRetVal)
			return FALSE;

		bRetVal = GetFunctionPointers();
		QDebugWriteEntry("PatchFunctions : GetFunctionPointers returned %d", bRetVal);

		if (!bRetVal)
			return FALSE;
	}

	// Get the module handle for the executable and patch the imports of everything beneath it recursively
	HMODULE hHostProcess = GetModuleHandle(NULL);
	QDebugWriteEntry("PatchFunctions : GetModuleHandle returned 0x%X", hHostProcess);

	bRetVal = PatchModuleFunctions(hHostProcess);
	QDebugWriteEntry("PatchFunctions : PatchModuleFunctions returned %d", bRetVal);

	return bRetVal;
}

// Determines which module is Storm, including which file name it has
BOOL LocateStorm()
{
	QDebugWriteEntry("LocateStorm()");

	for (DWORD iCurName = 0; lpszStormNames[iCurName]; iCurName++)
	{
		hStormDLL = GetModuleHandle(lpszStormNames[iCurName]);
		QDebugWriteEntry("LocateStorm : GetModuleHandle returned 0x%X for \"%s\"", hStormDLL, lpszStormNames[iCurName]);

		if (hStormDLL)
		{
			lpszStormName = lpszStormNames[iCurName];

			return TRUE;
		}
	}

	return FALSE;
}

// Determine whether we have a Warcraft III or Diablo II Storm.dll. These two can be differentiated because all observed versions of Storm other than in Diablo II load at the base address 0x15000000, while the Diablo II loads at a higher address I don't recall off the top of my head.
BOOL GetStormVersion()
{
	QDebugWriteEntry("GetStormVersion()");

	assert(hStormDLL);

	// Sort through the Portable Executable header to determine the preferred load address
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hStormDLL;
	// Pass up the DOS header to get to the Win32 (PE) header
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)
		((LPBYTE)pDosHeader + pDosHeader->e_lfanew);

	assert(pNTHeader);

	QDebugWriteEntry("GetStormVersion : Storm base address 0x%X", pNTHeader->OptionalHeader.ImageBase);

	if (pNTHeader->OptionalHeader.ImageBase != 0x15000000)
	{
		// It's a crappy D2 Storm
		nLoadFileEx2Ord = 0x122;
	}

	return TRUE;
}

// Get the pointers for all of the Storm functions we will or might want to patch
BOOL GetFunctionPointers()
{
	QDebugWriteEntry("GetFunctionPointers()");

	SFileOpenArchive = (SFileOpenArchivePtr)GetProcAddress(hStormDLL, (LPCSTR)nOpenArchiveOrd);
	SFileCloseArchive = (SFileCloseArchivePtr)GetProcAddress(hStormDLL, (LPCSTR)nCloseArchiveOrd);

	SFileOpenFileEx = (SFileOpenFileExPtr)GetProcAddress(hStormDLL, (LPCSTR)nOpenFileExOrd);

	SFileLoadFileEx = (SFileLoadFileExPtr)GetProcAddress(hStormDLL, (LPCSTR)nLoadFileExOrd);
	SFileLoadFileEx2 = (SFileLoadFileEx2Ptr)GetProcAddress(hStormDLL, (LPCSTR)nLoadFileEx2Ord);

	SFileOpenFileAsArchive = (SFileOpenFileAsArchivePtr)GetProcAddress(hStormDLL, (LPCSTR)nOpenFileAsArchiveOrd);
	SFileOpenPathAsArchive = (SFileOpenPathAsArchivePtr)GetProcAddress(hStormDLL, (LPCSTR)nOpenPathAsArchiveOrd);

	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileOpenArchive);
	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileCloseArchive);

	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileOpenFileEx);

	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileLoadFileEx);
	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileLoadFileEx2);

	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileOpenFileAsArchive);
	QDebugWriteEntry("GetFunctionPointers : GetProcAddress returned 0x%X", SFileOpenPathAsArchive);

	// Fail if we couldn't get the mandatory functions
	if (!SFileOpenArchive || !SFileCloseArchive || !SFileOpenFileEx)
		return FALSE;

	return TRUE;
}

// Patches all the functions we want in a given module and modules it imports
BOOL PatchModuleFunctions(IN HMODULE hModule)
{
	QDebugWriteEntry("PatchModuleFunctions(0x%X)", hModule);

	assert(hModule);

	// Get the addresses of the Kernel32 functions we're going to patch. We have to use GetProcAddress because simply referencing the functions would give us the address of the functions in our own import table, not the true addresses of the functions, which is what we need.
	hKernel32 = GetModuleHandle("Kernel32");
	QDebugWriteEntry("PatchModuleFunctions : GetModuleHandle returned 0x%X", hKernel32);
	if (hKernel32 == NULL)
		return FALSE;

	FARPROC fnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA"),
		fnGetProcAddress = GetProcAddress(hKernel32, "GetProcAddress"),
		fnCreateProcessA = GetProcAddress(hKernel32, "CreateProcessA"),
		fnExitProcess = GetProcAddress(hKernel32, "ExitProcess");
	
	if (!fnLoadLibraryA || !fnGetProcAddress || !fnCreateProcessA || !fnExitProcess)
		return FALSE;

	// We'll always patch the functions in Kernel32, because we need to handle those functions regardless of whether we are loading any MPQs or plugins in this context; e.g. we need to patch CreateProcess so that we can inject into the next process in the shunting chain.
	DWORD dwPatchCount = PatchImportEntry(hModule, "Kernel32.dll", (FARPROC)fnLoadLibraryA, (FARPROC)PatchLoadLibraryA, TRUE);
	QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);
	dwPatchCount = PatchImportEntry(hModule, "Kernel32.dll", (FARPROC)fnGetProcAddress, (FARPROC)PatchGetProcAddress, TRUE);
	QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);
	dwPatchCount = PatchImportEntry(hModule, "Kernel32.dll", (FARPROC)fnExitProcess, (FARPROC)PatchExitProcess, TRUE);
	QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);

	if (!(lpPatchContext->dwFlags & MPQD_NO_SPAWNING))
	{
		dwPatchCount = PatchImportEntry(hModule, "Kernel32.dll", (FARPROC)fnCreateProcessA, (FARPROC)PatchCreateProcessA, TRUE);
		QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);
	}

	if (bLoadMPQs)
	{
		// If we're loading MPQs in this process, patch the Storm functions
		dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileOpenArchive, (FARPROC)PatchOpenArchive, TRUE);
		QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);
		dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileCloseArchive, (FARPROC)PatchCloseArchive, TRUE);
		QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);

		// Patch extended redirect functions if that's what the caller wants to do
		if (lpPatchContext->dwFlags & MPQD_EXTENDED_REDIR)
		{
			dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileOpenFileEx, (FARPROC)PatchOpenFileEx, TRUE);
			QDebugWriteEntry("PatchModuleFunctions : PatchImportEntry returned 0x%X", dwPatchCount);

			if (SFileLoadFileEx)
				dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileLoadFileEx, (FARPROC)PatchLoadFileEx, TRUE);
			if (SFileLoadFileEx2)
				dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileLoadFileEx2, (FARPROC)PatchLoadFileEx2, TRUE);

			if (SFileOpenPathAsArchive)
				dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileOpenPathAsArchive, (FARPROC)PatchOpenPathAsArchive, TRUE);
		}

		if (SFileOpenFileAsArchive)
			dwPatchCount = PatchImportEntry(hModule, "Storm.dll", (FARPROC)SFileOpenFileAsArchive, (FARPROC)PatchOpenFileAsArchive, TRUE);
	}

	return TRUE;
}

// Loads the patch MPQs and saves their handles in the global variables
BOOL LoadPatchMPQs()
{
	QDebugWriteEntry("LoadPatchMPQs()");

	assert(!bMPQsLoaded);
	assert(!nOpenPatchMPQs);
	assert(SFileOpenArchive);

	LPCSTR lpszMPQName = (LPCSTR)lpPatchContext + lpPatchContext->dwMPQNamesOffset;
	for (DWORD dwCurMPQ = 0; dwCurMPQ < lpPatchContext->nPatchMPQs; dwCurMPQ++)
	{
		HANDLE hMPQ;

		// Load the MPQ. What makes MPQDraft so elegant is the fact that we can preempt other MPQs simply by setting a high priority for the MPQ. That way, Storm will automatically read from our MPQs before looking at the game's own MPQs. Notice that we're giving highest priority to the ones last in the list of MPQs to load; this is more or less arbitrary. The 2 flag tells Storm that asynchronous (overlapped) I/O should be allowed for this archive. This can potentially slow things down for archives that don't need to support asynchronous reads, but we can't really tell how our MPQs will be used, so we have to set if for all of our MPQs.
		BOOL bRetVal = SFileOpenArchive(lpszMPQName, 10000 + dwCurMPQ, 2, &hMPQ);
		QDebugWriteEntry("GetFunctionPointers : SFileOpenArchive returned %d for \"%s\", handle 0x%X", bRetVal, lpszMPQName, hMPQ);

		if (!bRetVal)
			return FALSE;

		hOpenPatchMPQs[nOpenPatchMPQs] = hMPQ;
		nOpenPatchMPQs++;

		lpszMPQName += MPQDRAFT_MAX_PATH;
	}

	bMPQsLoaded = TRUE;

	return TRUE;
}

// Loads and initializes all plugins in the current process
BOOL LoadPlugins()
{
	QDebugWriteEntry("LoadPlugins()");

	// Plugins are treated exactly the same as any other auxilliary module, save that they're marked for execute. Only load modules having that flag
	MPQDRAFTPLUGINMODULE *lpModules = (MPQDRAFTPLUGINMODULE *)((DWORD)lpPatchContext + lpPatchContext->dwModulesOffset);
	for (DWORD iCurAuxModule = 0; iCurAuxModule < lpPatchContext->nModules; iCurAuxModule++)
	{
		QDebugWriteEntry("LoadPlugins : Module slot %d, execute %d", iCurAuxModule, lpModules[iCurAuxModule].bExecute);

		if (lpModules[iCurAuxModule].bExecute)
		{
			// First of all, load the DLL containing the plugin
			HMODULE hDLLModule = LoadLibrary(lpModules[iCurAuxModule].szModuleFileName);
			QDebugWriteEntry("LoadPlugins : LoadLibrary returned 0x%X on \"%s\"", hDLLModule, lpModules[iCurAuxModule].szModuleFileName);

			if (!hDLLModule)
				return FALSE;

			// Next, patch the modules to make sure that calls to Storm functions by plugins behave as expected based on the patch MPQs. Remember that we only patched modules imported by the host executable earlier, so we need to patch the modules manually.
			BOOL bRetVal = PatchModuleFunctions(hDLLModule);
			QDebugWriteEntry("LoadPlugins : PatchModuleFunctions returned %d", bRetVal);

			// Get the exported plugin function and get the IMPQDraftPlugin for the plugin
			GetMPQDraftPluginPtr pGetMPQDraftPlugin = (GetMPQDraftPluginPtr)GetProcAddress(hDLLModule, "GetMPQDraftPlugin");
			QDebugWriteEntry("LoadPlugins : GetProcAddress returned 0x%X", pGetMPQDraftPlugin);

			IMPQDraftPlugin *pPlugin;

			if (!pGetMPQDraftPlugin)
			{
				FreeLibrary(hDLLModule);
				return FALSE;
			}
			
			bRetVal = pGetMPQDraftPlugin(&pPlugin);
			QDebugWriteEntry("LoadPlugins : GetMPQDraftPlugin returned %d", bRetVal);

			if (!bRetVal)
			{
				FreeLibrary(hDLLModule);
				return FALSE;
			}
			
			// Finally, initialize the plugin to allow it to do any patching of the host it needs to, then add it to our list of plugins
			bRetVal = pPlugin->InitializePlugin(&PluginServer);
			QDebugWriteEntry("LoadPlugins : InitializePlugin returned %d", bRetVal);

			if (!bRetVal)
			{
				FreeLibrary(hDLLModule);
				return FALSE;
			}

			pPlugins[nNumPlugins] = pPlugin;
			hPluginModules[nNumPlugins] = hDLLModule;

			nNumPlugins++;
		}
	}

	return TRUE;
}

// Close everything down so that the process may close cleanly. Note that this is called by DLL_PROCESS_DETACH, so this is the last call before lights out.
BOOL MPQDraftTerminate()
{
	QDebugWriteEntry("MPQDraftTerminate()");

	// If PatchExitProcess hasn't been called, clean up the MPQs here. We can't clean up the plugins here due to various evilness involving DLL unload order.
	UnloadPatchMPQs();

	// Close the patch context
	if (hPatchContext && lpPatchContext && hPatchMutex)
	{
		CloseContext(hPatchContext, lpPatchContext, hPatchMutex);

		hPatchContext = NULL;
		lpPatchContext = NULL;
		hPatchMutex = NULL;
	}

	return TRUE;
}

// Tells plugins to unpatch the host process, and then unload them
BOOL UnloadPlugins()
{
	QDebugWriteEntry("UnloadPlugins()");

	// Unload plugins in the opposite order they were loaded, to attempt to roll back changes
	for (long iCurPlugin = nNumPlugins - 1; iCurPlugin >= 0; iCurPlugin--)
	{
		assert(pPlugins[iCurPlugin]);
		assert(hPluginModules[iCurPlugin]);

		QDebugWriteEntry("UnloadPlugins : Terminating plugin slot %d, IMPQDraftPlugin 0x%X, HMODULE 0x%X", iCurPlugin, pPlugins[iCurPlugin], hPluginModules[iCurPlugin]);

		// Tell it to terminate
		pPlugins[iCurPlugin]->TerminatePlugin();

		// Unload the DLL
		FreeLibrary(hPluginModules[iCurPlugin]);

		pPlugins[iCurPlugin] = NULL;
		hPluginModules[iCurPlugin] = NULL;
	}

	nNumPlugins = 0;

	return TRUE;
}

// Unload the patch MPQs previously loaded
BOOL UnloadPatchMPQs()
{
	QDebugWriteEntry("UnloadPatchMPQs()");

	// Unload the MPQs in reverse order, in case there's any need for that symmetry
	for (DWORD dwCurMPQ = 0; dwCurMPQ < nOpenPatchMPQs; dwCurMPQ++)
	{
		assert(hOpenPatchMPQs[dwCurMPQ]);

		QDebugWriteEntry("UnloadPatchMPQs : Unloading MPQ slot %d, HANDLE 0x%X", dwCurMPQ, hOpenPatchMPQs[dwCurMPQ]);

		SFileCloseArchive(hOpenPatchMPQs[dwCurMPQ]);
		hOpenPatchMPQs[dwCurMPQ] = NULL;
	}

	nOpenPatchMPQs = 0;

	bMPQsLoaded = FALSE;

	return TRUE;
}

// Terminates the current process after some major error that preempts further running of the process
void MPQDraftAbort()
{
	QDebugWriteEntry("MPQDraftAbort()");

	MPQDraftTerminate();

	// Random number for the error code
	ExitProcess(0x93);
}

// Finally, the patcher function that is the border between MPQDraft and the MPQDraftDLL, the latter performing all patching itself
extern "C" BOOL WINAPI MPQDraftPatcher(
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

	IN DWORD dwFlags,

	IN LPCSTR lpszMPQDraftDir,

	IN LPCSTR lpszTargetPath,
	IN OPTIONAL DWORD nShuntCount,

	IN OPTIONAL DWORD nPatchMPQs,
	IN OPTIONAL DWORD nAuxModules,

	IN OPTIONAL LPCSTR *lplpszMPQNames, 
	IN OPTIONAL const MPQDRAFTPLUGINMODULE *lpAuxModules
)
{
	// Create a new context from parameters. Let the function assert if any of the parameters are invalid (so we don't have to).
	MPQDRAFTPATCHCONTEXT *pPatchContext;
	HANDLE hPatchContext, hPatchMutex;

	if (!CreatePatchContext(dwFlags, lpszMPQDraftDir, lpszCurrentDirectory, lpszTargetPath, nShuntCount, nPatchMPQs, nAuxModules, lplpszMPQNames, lpAuxModules, hPatchContext, &pPatchContext, hPatchMutex))
		return FALSE;

	// Now we have a context. Everything else that's part of the patching process will be done by CreateAndPatchProcess.
	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(processInfo));

	DWORD dwInitialTick = GetTickCount();

	BOOL bRetVal = CreateAndPatchProcess(lpszApplicationName, lpszCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpszEnvironment, lpszCurrentDirectory, lpStartupInfo, &processInfo, hPatchContext, hPatchMutex, nShuntCount, (LPPATCHERENTRYPOINT)MPQDraftInitialize);

	// We're going to close the handles here because if injecting it into the process succeeded, that process will now have a reference to the context, so it will stay put. If injection failed, we need to close the context anyway, as part of clean-up.
	CloseContext(hPatchContext, pPatchContext, hPatchMutex);
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	if (!bRetVal)
		return FALSE;	// Injection failed

	// All done injecting. Now we wait for it to finish. We time the execution because a very quick exit probably indicates injection failure, rather than the program closing normally.
	WaitForContextClose();

	if (GetTickCount() - dwInitialTick < 2000)
		return FALSE;	// Doesn't look good

	return TRUE;	// V
}