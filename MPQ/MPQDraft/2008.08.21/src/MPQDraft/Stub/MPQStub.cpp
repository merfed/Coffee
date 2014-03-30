/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#include "stdafx.h"
#include "resource.h"
#include "..\\common.h"

// Get the stub data in fully usable in-memory form
BOOL FindStubData(OUT STUBDATA *lpStubData, OUT LPDWORD lpdwDataSize)
{
	assert(lpStubData);
	assert(lpdwDataSize);

	STUBDATA *lpStubDataPtr;
	if (!LookupResource(NULL, "STUBDATA", "BIN", (LPCVOID *)&lpStubDataPtr, lpdwDataSize))
		return FALSE;
	
	// Quick sanity check
	if (*lpdwDataSize != STUBDATASIZE ||
		lpStubDataPtr->cbSize < sizeof(STUBDATA))
		return FALSE;

	memcpy(lpStubData, lpStubDataPtr, *lpdwDataSize);

	// Adjust PATCHTARGETEX pointers
	lpStubData->patchTarget.lpszRegistryKey += (UINT_PTR)&lpStubData->patchTarget;
	lpStubData->patchTarget.lpszRegistryValue += (UINT_PTR)&lpStubData->patchTarget;

	lpStubData->patchTarget.lpszTargetPath += (UINT_PTR)&lpStubData->patchTarget;
	lpStubData->patchTarget.lpszTargetFileName += (UINT_PTR)&lpStubData->patchTarget;
	lpStubData->patchTarget.lpszSpawnFileName += (UINT_PTR)&lpStubData->patchTarget;

	lpStubData->patchTarget.lpszArguments += (UINT_PTR)&lpStubData->patchTarget;

	return TRUE;
}

// Locates the actual patch target path and spawn path based on the info in the PATCHTARGETEX
BOOL LocatePatchTarget(IN PATCHTARGETEX &patchTarget, OUT LPSTR lpszSpawnPath, OUT LPSTR lpszTargetPath)
{
	char szSpawnPath[MAX_PATH + 1];
	char szTargetPath[MAX_PATH + 1];

	// First off, see if it uses the registry or drive paths
	if (patchTarget.bUseRegistry)
	{
		BOOL bFoundRegKey = FALSE;

		HKEY hKey;
		DWORD dwValueType, dwValueSize = MAX_PATH + 1;
		char szValue[MAX_PATH + 1];

		// First try under HKEY_CURRENT_USER
		if (RegOpenKeyEx(HKEY_CURRENT_USER, patchTarget.lpszRegistryKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, patchTarget.lpszRegistryValue, NULL, &dwValueType, (LPBYTE)&szValue, &dwValueSize) == ERROR_SUCCESS)
			{
				// The key exists. Verify that it actually exists, and means what we expect it to.
				if (patchTarget.bValueIsFileName && PathFileExists(szValue) && !PathIsDirectory(szValue))
					bFoundRegKey = TRUE;
				else if (!patchTarget.bValueIsFileName && PathIsDirectory(szValue))
					bFoundRegKey = TRUE;
			}

			RegCloseKey(hKey);
		}

		// Second shot: try HKEY_LOCAL_MACHINE
		if (!bFoundRegKey && RegOpenKeyEx(HKEY_LOCAL_MACHINE, patchTarget.lpszRegistryKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, patchTarget.lpszRegistryValue, NULL, &dwValueType, (LPBYTE)&szValue, &dwValueSize) == ERROR_SUCCESS)
			{
				if (patchTarget.bValueIsFileName && PathFileExists(szValue) && !PathIsDirectory(szValue))
					bFoundRegKey = TRUE;
				else if (!patchTarget.bValueIsFileName && PathIsDirectory(szValue))
					bFoundRegKey = TRUE;
			}

			RegCloseKey(hKey);
		}

		if (!bFoundRegKey)
			return FALSE;

		// Okay, we found the registry key. Now let's find the file we're looking for
		strcpy(szSpawnPath, szValue);
		strcpy(szTargetPath, szValue);

		if (!patchTarget.bValueIsFileName)
		{
			// If the key isn't the full path, we need to append the filenames
			PathAddBackslash(szSpawnPath);
			strcat(szSpawnPath, patchTarget.lpszSpawnFileName);

			PathAddBackslash(szTargetPath);
			if (patchTarget.lpszTargetFileName)	// If it's NULL, it's same as spawn
				strcat(szTargetPath, patchTarget.lpszTargetFileName);
			else
				strcat(szTargetPath, patchTarget.lpszSpawnFileName);
		}
	}
	else
	{
		// It doesn't use the registry. All we can do is take the patch target and spawn path as relative paths.
		strcpy(szSpawnPath, patchTarget.lpszTargetPath);
		strcpy(szTargetPath, patchTarget.lpszTargetPath);

		PathAddBackslash(szSpawnPath);
		strcat(szSpawnPath, patchTarget.lpszSpawnFileName);

		PathAddBackslash(szTargetPath);
		if (patchTarget.lpszTargetFileName)	// If it's NULL, it's same as spawn
			strcat(szTargetPath, patchTarget.lpszTargetFileName);
		else
			strcat(szTargetPath, patchTarget.lpszSpawnFileName);
	}

	// Verify the paths are correct
	if (!PathFileExists(szTargetPath) || !PathFileExists(szSpawnPath))
		return FALSE;

	// Return whatever information is requested
	if (lpszSpawnPath)
		strcpy(lpszSpawnPath, szSpawnPath);
	if (lpszTargetPath)
		strcpy(lpszTargetPath, szTargetPath);

	return TRUE;
}

DWORD GetNumAuxFiles(IN EFSHANDLEFORREAD hEFSFile)
{
	assert(hEFSFile);

	return GetNumEFSFiles(hEFSFile);
}

// The patcher DLL expects all modules to already exist in files on the hard drive. While this may seem wasteful, there is reason to it; while plugins may not require this for all of their modules, Windows will definitely require at least the DLLs to be loaded to be extracted, so we might as well just do it all now.
BOOL UnpackAuxFiles(IN EFSHANDLEFORREAD hEFSFile, IN MPQDRAFTPLUGINMODULE *pAuxModules, IN DWORD dwNumAuxFiles, OUT LPSTR lpszDLLFileName)
{
	assert(hEFSFile);
	assert(pAuxModules);
	assert(lpszDLLFileName);

	BOOL bFoundDLL = FALSE;

	for (DWORD iCurModule = 0; iCurModule < dwNumAuxFiles; iCurModule++)
	{
		DWORD dwComponentID, dwFileID, dwFileData;
		char szFileName[MAX_PATH + 1];

		// Extract the module
		if (!EnumEFSFiles(hEFSFile, iCurModule, &dwComponentID, &dwFileID, &dwFileData, NULL)
			|| !ExtractTempEFSFile(hEFSFile, dwComponentID, dwFileID, szFileName))
			return FALSE;

		// Pass the module info back to the caller
		pAuxModules[iCurModule].dwComponentID = dwComponentID;
		pAuxModules[iCurModule].dwModuleID = dwFileID;
		strcpy(pAuxModules[iCurModule].szModuleFileName, szFileName);
		pAuxModules[iCurModule].bExecute = (dwFileData != 0) ? TRUE : FALSE;

		// Check if it's the MPQDraft DLL
		if ((dwComponentID == MPQDRAFT_COMPONENT) &&
			(dwFileID == MPQDRAFTDLL_MODULE))
		{
			strcpy(lpszDLLFileName, szFileName);

			bFoundDLL = TRUE;
		}
	}

	if (bFoundDLL)
		return TRUE;
	else
		return FALSE;
}

// Get the EFS file in the SEMPQ
BOOL FindEFSFile(IN LPCSTR lpSEMPQPath, OUT LPVOID *lplpvSEMPQMapping, OUT EFSHANDLEFORREAD &hEFSFile)
{
	assert(lpSEMPQPath);

	// Open the SEMPQ. We can't pull it directly out of memory for various reasons.
	HANDLE hSEMPQ = CreateFile(lpSEMPQPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hSEMPQ == INVALID_HANDLE_VALUE)
		return FALSE;

	// Map the SEMPQ into memory (yes, it is kinda strange to have two copies in memory)
	DWORD dwSEMPQSize = GetFileSize(hSEMPQ, NULL);
	HANDLE hSEMPQMapping = CreateFileMapping(hSEMPQ, NULL, PAGE_READONLY, 0, 0, NULL);

	CloseHandle(hSEMPQ);
	if (hSEMPQMapping == INVALID_HANDLE_VALUE)
		return FALSE;

	LPVOID lpvSEMPQMapping = MapViewOfFile(hSEMPQMapping, FILE_MAP_READ, 0, 0, 0);

	CloseHandle(hSEMPQMapping);
	if (!lpvSEMPQMapping)
		return FALSE;

	// Finally, find the EFS file
	hEFSFile = GetEFSHandleFromMappedFile((LPBYTE)lpvSEMPQMapping, dwSEMPQSize);
	if (hEFSFile)
	{
		*lplpvSEMPQMapping = lpvSEMPQMapping;
		return TRUE;
	}
	else
	{
		UnmapViewOfFile(lpvSEMPQMapping);
		return FALSE;
	}
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	QResourceInitialize();

	// First of all, fish the stub data out
	char stubDataBuffer[STUBDATASIZE];
	STUBDATA *lpStubData = (STUBDATA *)&stubDataBuffer;
	DWORD dwStubDataSize;

	if (!FindStubData(lpStubData, &dwStubDataSize))
	{
		MessageBox(NULL, "Unable to perform the patch. This SEMPQ is corrupted.", 
			"Self-Executing MPQ", MB_OK | MB_ICONSTOP);

		return 1;
	}

	// Get the myriad of paths we're going to need to do this
	char szMPQPath[MAX_PATH + 1], szDLLPath[MAX_PATH + 1], 
		szTargetPath[MAX_PATH + 1], szSpawnPath[MAX_PATH + 1],
		szMessage[MAX_PATH + 1], szCurrentDir[MAX_PATH + 1],
		szSpawnDir[MAX_PATH + 1];
	LPCSTR lpszMPQNames = szMPQPath;

	// Get the name of the SEMPQ
	GetModuleFileName(NULL, szMPQPath, MAX_PATH);

	// Get the "MPQDraft" directory, which is really the directory we're in now
	GetModuleFileName(NULL, szCurrentDir, MAX_PATH);
	PathRemoveFileSpec(szCurrentDir);

	// Find the patch target and its paths
	if (!LocatePatchTarget(lpStubData->patchTarget, szSpawnPath, szTargetPath))
	{
		sprintf(szMessage, "Unable to locate the target to patch.");
		MessageBox(NULL, szMessage, lpStubData->szCustomName, MB_OK | MB_ICONSTOP);

		return 1;
	}

	// Get the startup directory - the directory of the patch target
	strcpy(szSpawnDir, szSpawnPath);
	PathRemoveFileSpec(szSpawnDir);

	char szCommandLine[MAX_PATH + 1];

	sprintf(szCommandLine, "\"%s\" %s", szTargetPath, lpStubData->patchTarget.lpszArguments);

	BOOL bCorrupted = TRUE;

	// Find the EFS file
	LPVOID lpvSEMPQMapping = NULL;
	EFSHANDLEFORREAD hEFSFile = NULL;
	if (FindEFSFile(szMPQPath, &lpvSEMPQMapping, hEFSFile))
	{
		// Get the number of modules, allocate the array for them
		DWORD nNumAuxFiles = GetNumEFSFiles(hEFSFile);
		MPQDRAFTPLUGINMODULE *pAuxModules = NULL;
		if (nNumAuxFiles && (pAuxModules = new MPQDRAFTPLUGINMODULE[nNumAuxFiles]))
		{
			// Unpack the modules
			if (UnpackAuxFiles(hEFSFile, pAuxModules, nNumAuxFiles, szDLLPath))
			{
				// Finally, do the patch
				STARTUPINFO si;
				GetStartupInfo(&si);

				HMODULE hDLL = LoadLibrary(szDLLPath);
				MPQDraftPatcherPtr MPQDraftPatcher = (MPQDraftPatcherPtr)GetProcAddress(hDLL, "MPQDraftPatcher");

				if (!MPQDraftPatcher(szSpawnPath, szCommandLine, NULL, 
					NULL, FALSE, 0, NULL, szCurrentDir, &si, 
					lpStubData->patchTarget.grfFlags, szCurrentDir, 
					szTargetPath,lpStubData->patchTarget.nShuntCount, 1, 
					nNumAuxFiles, &lpszMPQNames, pAuxModules))
					MessageBox(NULL, "The patch was unsuccessful.", lpStubData->szCustomName, MB_OK | MB_ICONEXCLAMATION);

				bCorrupted = FALSE;
			}

			delete [] pAuxModules;
		}

		UnmapViewOfFile(lpvSEMPQMapping);
	}

	// We're all done with the patch, now we need to clean up after ourselves. Nobody (especially not me) likes it when you forget to remove your temp files from their computer.
	DeleteTemporaryFiles();

	QResourceDestroy();

	if (bCorrupted)
	{
		MessageBox(NULL, "Unable to perform the patch. This SEMPQ is corrupted.", 
			lpStubData->szCustomName, MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}

	return 0;
}