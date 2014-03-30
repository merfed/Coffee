/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2007 Justin Olbrantz. All Rights Reserved.
*/

#include <QInjectDLL.h>
#include <assert.h>

/*
	The process is very straightforward: the injector writes an assembly loader function and the various patcher data to a block of memory in the target process allocated with VirtualAllocEx, and emulates a function call to the loader function in the target thread by setting up a stack frame and setting the instruction pointer. This loader function first calls LoadLibrary to load the DLL, then calls the target DLL function, if there is one. Finally, it returns to the target thread, which has no idea any of this has happened.
*/

// The size of the buffer for the assembly loader function that is injected into the target process
#define LOADER_MAX_SIZE 256
// Alignment of the data passed to the target DLL function
#define PATCHER_DATA_ALIGNMENT 16
// Rounds an offset up to the nearest PATCHER_DATA_ALIGNMENT boundary (power of 2)
#define ALIGN_PATCHER_DATA(x) (((UINT_PTR)x + PATCHER_DATA_ALIGNMENT - 1) & ~(PATCHER_DATA_ALIGNMENT - 1))

// All of the data the loader function and target DLL function will need to execute
struct LOADERFUNCTIONPARAMS
{
	FARPROC lpfnLoadLibraryA;	// Functions that the loader will call
	FARPROC lpfnGetLastError;
	FARPROC lpfnExitProcess;

	DWORD nPatcherOffset;	// RVA of patcher entry point in DLL
	DWORD nPatcherDataLen;	// Length of data to be passed to patcher

	char szDLLFilePath[MAX_PATH];	// Name of patcher DLL

	BYTE fnLoaderFunction[LOADER_MAX_SIZE];	// Loader function code

	BYTE byPatcherData[PATCHER_DATA_ALIGNMENT];	// Patcher data of variable length
};

// The stack frame for the call to the loader function
struct LOADERSTACKFRAME
{
	LPCVOID lpReturnAddress;	// Address the function will return to
	LOADERFUNCTIONPARAMS *lpParams;	// Address of the parameters
};

// The loader function that gets injected into the target process. This function cannot call any functions outside itself and the ones pointers are stored to in the loader data, as any external references will not be copied into the target process. The problem of dependencies is also the reason we must use hand-coded assembly for this function.
// ** NOT 64-BIT COMPATIBLE **
void __declspec(naked) __stdcall LoaderFunction(IN const LOADERFUNCTIONPARAMS *lpParams)
{
	__asm {
		push ebp
		mov ebp, esp
		pushad

		// Call LoadLibraryA to load DLL
		//int 3	// Uncomment this for debugging of the loader function
		mov ebx, lpParams
		lea edx, [ebx]LOADERFUNCTIONPARAMS.szDLLFilePath
		push edx
		call [ebx]LOADERFUNCTIONPARAMS.lpfnLoadLibraryA

		test eax, eax
		jne LibraryLoaded

KillProcess:
		// Couldn't load library. Call ExitProcess with GetLastError value
		call [ebx]LOADERFUNCTIONPARAMS.lpfnGetLastError
		push eax
		call [ebx]LOADERFUNCTIONPARAMS.lpfnExitProcess

LibraryLoaded:	// Now call the patcher entry point, if applicable
		cmp [ebx]LOADERFUNCTIONPARAMS.nPatcherOffset, 0
		je Done
		lea ecx, [ebx]LOADERFUNCTIONPARAMS.byPatcherData
		add ecx, (PATCHER_DATA_ALIGNMENT - 1)	// Align the data on a 16 byte boundary
		and ecx, ~(PATCHER_DATA_ALIGNMENT - 1)
		mov edx, [ebx]LOADERFUNCTIONPARAMS.nPatcherDataLen
		add eax, [ebx]LOADERFUNCTIONPARAMS.nPatcherOffset
		push edx
		push ecx
		call eax

		test eax, eax
		je KillProcess

Done:
		// Return to the target thread
		popad
		mov esp, ebp
		pop ebp
		ret size LPCVOID	// Pop the parameter
	};
}

// This helper function attempts to find the true location of a given function. This is necessary because compilers will, in various circumstances, generate call stubs in place of the actual functions, and attempts to get a function address will actually get the address of the stub function. This can be easily demonstrated with incremental linking in VC++: the address returned by LoaderFunction will actually be to a JMP rel32 stub. Since we're injecting the loader function into a foreign process, we have to get the function, and not its call stub.
// ** NOT 64-BIT COMPATIBLE **
LPCVOID GetTrueFunctionAddress(IN LPCVOID lpFunction)
{
	assert(lpFunction);

	const BYTE *lpCurFunc = (const BYTE *)lpFunction, *lpNextFunc = lpCurFunc;

	// Loop until we find something that doesn't appear to be a jump stub
	do
	{
		lpCurFunc = lpNextFunc;

		BYTE byOpcode = lpCurFunc[0];

		if (byOpcode == 0xE9)	// Is it a JMP rel32 instruction?
			// Parameter is target address relative to the next instruction (5 bytes)
			lpNextFunc = lpCurFunc + 5 + *(UINT_PTR *)(lpCurFunc + 1);
		else if (byOpcode == 0xFF)	// Does it have an extended opcode?
		{
			BYTE byModRM = lpCurFunc[1];
			if (byModRM == 0x25)	// Is it a JMP [address] instruction?
				// Parameter is the absolute address of the pointer to the target address
				lpNextFunc = **(LPBYTE **)(lpCurFunc + 2);
		}
		// More options can be added here if I missed anything
	} while (lpCurFunc != lpNextFunc);

	return (LPCVOID)lpCurFunc;	// Seems to be the function we're looking for
}

// This helper function attemts to find the file name and base address (HMODULE) of the module containing the specified function. This is accomplished by walking backwards in the address space from the function till what appears to be a module is found, using VirtualQuery. Once we've found something that looks like a module, we'll call GetModuleFileName to get the filename.
BOOL GetContainingModule(IN LPCVOID lpFunc, OUT HMODULE *lphModule, OUT LPSTR lpszFileName, IN DWORD nBufferSize)
{
	assert(lpFunc);
	assert(lphModule);
	assert(lpszFileName);
	assert(nBufferSize);

	// Get the info for what should be the module containing the function
	MEMORY_BASIC_INFORMATION memInfo;
	if (!VirtualQuery(lpFunc, &memInfo, sizeof(memInfo)) 
		|| memInfo.Type != MEM_IMAGE)
		return FALSE;

	*lphModule = (HMODULE)memInfo.AllocationBase;

	// It looks like a module. Try to get its file name.
	if (!GetModuleFileName(*lphModule, lpszFileName, nBufferSize))
		return FALSE;

	// We got the file name, so it must be a module
	return TRUE;
}

// ** NOT 64-BIT COMPATIBLE **
BOOL WINAPI InjectDLLIntoProcessV3(IN HANDLE hTargetProcess, IN HANDLE hTargetThread,
	IN OPTIONAL LPCSTR lpDLLFileName, IN OPTIONAL LPPATCHERENTRYPOINT lpfnPatcher, 
	IN OPTIONAL LPCVOID lpPatcherData, IN OPTIONAL DWORD nPatcherDataLen)
{
	// We're going to assert if the caller gives us any crap
	assert(hTargetProcess);
	assert(hTargetThread);
	assert(lpDLLFileName || lpfnPatcher);
	if (nPatcherDataLen)
		assert(lpPatcherData);

	DWORD nPatcherRVA = 0;	// The patcher function RVA
	// A buffer to hold the file name, if a patcher function is specified
	char szDLLFileName[MAX_PATH] = "";

	if (lpfnPatcher)
	{
		// Get the DLL file name and RVA from the patcher function pointer
		HMODULE hDLL;
		if (!GetContainingModule((LPCVOID)lpfnPatcher, &hDLL, szDLLFileName, sizeof(szDLLFileName)))
			return FALSE;

		lpDLLFileName = szDLLFileName;
		nPatcherRVA = (DWORD)(((UINT_PTR)lpfnPatcher - (UINT_PTR)hDLL) & 0xFFFFFFFF);
	}

	CONTEXT ctxTarget; // The target thread's CPU registers
	LOADERSTACKFRAME LoaderStack;	// Local copy of the stack frame
	LOADERFUNCTIONPARAMS LoaderParams; // Local copy of the parameters

	ZeroMemory(&ctxTarget, sizeof(ctxTarget));
	ZeroMemory(&LoaderStack, sizeof(LoaderStack));
	ZeroMemory(&LoaderParams, sizeof(LoaderParams));

	// Retrieve the CPU registers of the target thread
	ctxTarget.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
	GetThreadContext(hTargetThread, &ctxTarget);

	// Set up the return address
	LoaderStack.lpReturnAddress = (LPCVOID)ctxTarget.Eip;

	// Get the function pointers we'll need in the loader function
	HMODULE hKernel = GetModuleHandle("kernel32");

	LoaderParams.lpfnLoadLibraryA = GetProcAddress(hKernel, "LoadLibraryA");
	LoaderParams.lpfnGetLastError = GetProcAddress(hKernel, "GetProcAddress");
	LoaderParams.lpfnExitProcess = GetProcAddress(hKernel, "ExitProcess");

	// Set the patcher entry point and param 
	LoaderParams.nPatcherOffset = nPatcherRVA;
	LoaderParams.nPatcherDataLen = nPatcherDataLen;

	// Set up the DLL filename
	strncpy(LoaderParams.szDLLFilePath, lpDLLFileName, MAX_PATH - 1);
	LoaderParams.szDLLFilePath[MAX_PATH - 1] = '\0';

	// Copy the loader function
	LPCVOID lpvLoaderCode = GetTrueFunctionAddress(LoaderFunction);
	memcpy(&LoaderParams.fnLoaderFunction, lpvLoaderCode, LOADER_MAX_SIZE);

	// Allocate the memory for the loader and patcher parameters
	LOADERFUNCTIONPARAMS *lpLoaderParams = (LOADERFUNCTIONPARAMS *)VirtualAllocEx(hTargetProcess, NULL, sizeof(LOADERFUNCTIONPARAMS) + nPatcherDataLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpLoaderParams)
		return FALSE;

	LoaderStack.lpParams = lpLoaderParams;

	// Reserve space on the target's stack for the stack frame, and set the new instruction pointer so it executes the loader function
	ctxTarget.Esp -= sizeof(LOADERSTACKFRAME);
	ctxTarget.Eip = (DWORD)LoaderStack.lpParams->fnLoaderFunction;

	// Write the CPU registers
	if (!SetThreadContext(hTargetThread, &ctxTarget))
		return FALSE;

	// Calculate the start of the the patcher data
	LPVOID lpLoaderData = (LPVOID)ALIGN_PATCHER_DATA(&lpLoaderParams->byPatcherData);
	// Write the patcher parameters
	if (!WriteProcessMemory(hTargetProcess, lpLoaderParams, &LoaderParams, sizeof(LoaderParams), NULL))
		return FALSE;
	if (nPatcherDataLen && !WriteProcessMemory(hTargetProcess, lpLoaderData, lpPatcherData, nPatcherDataLen, NULL))
		return FALSE;

	// Write the stack frame
	if (!WriteProcessMemory(hTargetProcess, (LPVOID)ctxTarget.Esp, 
		&LoaderStack, sizeof(LoaderStack), NULL))
	return FALSE;
	
	return TRUE;
}