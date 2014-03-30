/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2007 Justin Olbrantz. All Rights Reserved.
*/

#include <QResource.h>
#include <assert.h>

BOOL AddTempFileToList(LPCSTR lpszFileName);

// The stride of the loader's attempts to locate an EFS file inside another file
#define SECTOR_SIZE 512
// The size we should pad the file out to at the end of the EFS file
#define FILE_GRANULARITY 4096

// NOTE: All values in EFS files are in little endian byte order

// The magic number of an EFS file header
#define EFS_SIGNATURE 0x20534645

#include <pshpack1.h>
struct EFSFILEHEADER
{
	DWORD dwSignature; // Must be "EFS ": 0x20534645
	union
	{
		// The version of the EFS format. So far there is only one.
		DWORD dwVersion;
		BYTE byVersion[4]; // Should be 1, 0, 0, 0
	};
	// Total size of the EFS file
	DWORD dwFileSize;
	// The offset of the file list in the EFS file, relative to the beginning of the EFS header
	DWORD dwDirectoryOffset;
	// The number of files in the EFS file
	DWORD dwNumDirectoryEntries;
	// Unused, for now
	DWORD dwUnused14;
	DWORD dwUnused18;
	DWORD dwUnused1C;
};

// An file entry in the EFS file directory table
struct EFSDIRECTORYENTRY
{
	// The major file ID
	DWORD dwComponentID;
	// The minor file ID
	DWORD dwFileID;
	// The user-specified data value of the file
	DWORD dwData;
	// The offset of the file's data from the start of the EFS header
	DWORD dwOffset;
	// The file's size
	DWORD dwSize;
	// File flags. Unused, for now.
	DWORD dwFlags;
};
#include <poppack.h>

// All the data required to modify an EFS file
struct EFSFILEHANDLEFORWRITE
{
	// The Windows file handle
	HANDLE hFile;
	// Whether the EFS file has been modified
	BOOL bModified;
	// The offset to the EFS header in the directory
	DWORD dwHeaderOffset;
	// The EFS file list, stored in memory
	EFSDIRECTORYENTRY *pDirectory;
	// The number of files currently in the EFS file
	DWORD nNumDirectoryEntries;
	// The maximum number of file that will fit in the directory
	DWORD nMaxDirectoryEntries;
	// The offset in the EFS file where new files will be added
	DWORD dwInsertPoint;
};

// An entry in the extracted file list. This list stores all the temporary files that have been extracted, so that they can be freed by DeleteTemporaryFiles.
struct EXTRACTEDFILE
{
	EXTRACTEDFILE *pNext;
	char szFileName[MAX_PATH + 1];
};

// The temporary file list. Protected by csExtrListLock
EXTRACTEDFILE *pFirstExtractedFile = NULL, *pLastExtractedFile = NULL;

CRITICAL_SECTION csExtrListLock;

// Adds a file to the temporary file list
BOOL AddTempFileToList(IN LPCSTR lpszFileName)
{
	assert(lpszFileName);

	// Create the list entry for the file
	EXTRACTEDFILE *pExtrFile = (EXTRACTEDFILE *)malloc(sizeof(EXTRACTEDFILE));
	if (!pExtrFile)
		return FALSE;

	pExtrFile->pNext = NULL;

	strncpy(pExtrFile->szFileName, lpszFileName, sizeof(pExtrFile->szFileName));
	pExtrFile->szFileName[sizeof(pExtrFile->szFileName) - 1] = '\0';

	// Add it to the list
	EnterCriticalSection(&csExtrListLock);
	if (pFirstExtractedFile)
	{
		pLastExtractedFile->pNext = pExtrFile;
		pLastExtractedFile = pExtrFile;
	}
	else
		pFirstExtractedFile = pLastExtractedFile = pExtrFile;
	LeaveCriticalSection(&csExtrListLock);

	return TRUE;
}

// Initialize whatever is necessary
void WINAPI QResourceInitialize()
{
	// Initialize the temporary file list lock
	InitializeCriticalSection(&csExtrListLock);
}

// Clean up everything
void WINAPI QResourceDestroy()
{
	// Free the temporary files list. Don't delete the files themselves.
	EXTRACTEDFILE *pExtrFile = pFirstExtractedFile;
	while (pExtrFile)
	{
		EXTRACTEDFILE *pNextExtrFile = pExtrFile->pNext;

		free(pExtrFile);

		pExtrFile = pNextExtrFile;
	}

	pFirstExtractedFile = pLastExtractedFile = NULL;

	// Destroy the temporary file list lock
	DeleteCriticalSection(&csExtrListLock);
}

BOOL WINAPI DeleteTemporaryFiles()
{
// Delete all entries in the temporary file list, and free the list
	EnterCriticalSection(&csExtrListLock);
	EXTRACTEDFILE *pExtrFile = pFirstExtractedFile;
	while (pExtrFile)
	{
		EXTRACTEDFILE *pNextExtrFile = pExtrFile->pNext;

		DeleteFile(pExtrFile->szFileName);
		free(pExtrFile);

		pExtrFile = pNextExtrFile;
	}
	pFirstExtractedFile = pLastExtractedFile = NULL;
	LeaveCriticalSection(&csExtrListLock);

	return TRUE;
}

BOOL WINAPI LookupResource(IN HMODULE hModule, IN LPCSTR lpszResName, IN LPCSTR lpszResType, OUT LPCVOID *lpvResData, OUT LPDWORD lpdwResSize)
{
	assert(lpszResName);
	assert(lpszResType);
	assert(lpvResData);
	assert(lpdwResSize);

	// This is the basic 3-part Windows resource loading that was basically deprecated in Win32. First you find the resource in the resource directory, then you load it as a memory block, and finally you map the memory block into memory. There was a reason for all of this in Win16, but now it's just annoying.
	// Note that these functions all handle hModule being NULL by using the current process' EXE.
	HRSRC hResInfo = FindResource(hModule, lpszResName, lpszResType);
	if (!hResInfo)
		return FALSE;

	HGLOBAL hResData = LoadResource(hModule, hResInfo);
	if (!hResData)
		return FALSE;

	*lpvResData = LockResource(hResData);
	if (!(*lpvResData))
		return FALSE;

	*lpdwResSize = SizeofResource(hModule, hResInfo);

	return TRUE;
}

BOOL WINAPI ExtractResource(IN HMODULE hModule, IN LPCSTR lpszResName, IN LPCSTR lpszResType, IN LPCSTR lpszFileName)
{
	assert(lpszResName);
	assert(lpszResType);
	assert(lpszFileName);

	// Load the resource from the specified module
	LPCVOID lpvResData;
	DWORD dwResSize, dwBytesWritten;
	if (!LookupResource(hModule, lpszResName, lpszResType, &lpvResData, &dwResSize))
		return FALSE;

	// Open the target file for writing
	BOOL bRetVal = FALSE;
	HANDLE hOutFile = CreateFile(lpszFileName, GENERIC_READ 
		| GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hOutFile != INVALID_HANDLE_VALUE)
	{
		// Write the resource to the file. Because the entire resource is memory mapped, we can do this in a single call.
		if (WriteFile(hOutFile, lpvResData, dwResSize, &dwBytesWritten, NULL)
			&& (dwBytesWritten == dwResSize))
		{
			SetEndOfFile(hOutFile);

			bRetVal = TRUE;
		}

		// Clean up. Delete the file if extraction failed.
		CloseHandle(hOutFile);

		if (!bRetVal)
			DeleteFile(lpszFileName);
	}

	return bRetVal;
}

BOOL WINAPI ExtractTempResource(IN HMODULE hModule, IN LPCSTR lpszResName, IN LPCSTR lpszResType, OUT LPSTR lpszOutFileName)
{
	assert(lpszResName);
	assert(lpszResType);
	assert(lpszOutFileName);

	// Obtain a unique file name for the temporary file in the Windows temporary files directory using the standard Windows functions.
	char szTempDir[MAX_PATH + 1], szTempFileName[MAX_PATH + 1];
	if (!GetTempPath(MAX_PATH, szTempDir) ||
		!GetTempFileName(szTempDir, "Resource", 0, szTempFileName))
		return FALSE;
	
	// Extract the resource using the functions we already have
	if (!ExtractResource(hModule, lpszResName, lpszResType, szTempFileName))
		return FALSE;

	// Add the file to the temporary file list
	if (!AddTempFileToList(szTempFileName))
	{
		DeleteFile(szTempFileName);

		return FALSE;
	}

	strcpy(lpszOutFileName, szTempFileName);

	return TRUE;
}

// Locates (if possible) an EFS file header in the specified file on disk
BOOL FindEFSHeader(
	// Handle of the file on disk to be searched
	IN HANDLE hEFSFile,
	// The offset of the header in the EFS file
	OUT LPDWORD lpdwHeaderOffset
)
{
	assert(hEFSFile != INVALID_HANDLE_VALUE);
	assert(lpdwHeaderOffset);

	EFSFILEHEADER header;
	DWORD dwFileOffset = 0, dwFileSize = GetFileSize(hEFSFile, NULL),
		dwBytesRead;

	// Scan the file from beginning to end, checking for an EFS header every SECTOR_SIZE bytes
	while ((dwFileOffset + sizeof(EFSFILEHEADER)) <= dwFileSize)
	{
		SetFilePointer(hEFSFile, dwFileOffset, NULL, FILE_BEGIN);
		if (!ReadFile(hEFSFile, &header, sizeof(EFSFILEHEADER), &dwBytesRead, NULL) ||
			(dwBytesRead < sizeof(EFSFILEHEADER)))
			return FALSE;

		// Must have the magic number and correct version to be recognized as an EFS file. It must also have a valid file size, directory offset, and directory size.
		if ((header.dwSignature == EFS_SIGNATURE) &&

			(header.dwVersion == 0x00000001) &&

			((dwFileOffset + header.dwFileSize) <= dwFileSize) &&
			((dwFileOffset + header.dwDirectoryOffset + (header.dwNumDirectoryEntries * sizeof(EFSDIRECTORYENTRY))) <= dwFileSize))
		{
			*lpdwHeaderOffset = dwFileOffset;

			return TRUE;
		}

		dwFileOffset += SECTOR_SIZE;
	}

	return FALSE;
}

// Create an EFS file from an existing file on disk by appending an EFS header to it, and returns an EFS write handle. On success, ownership of the handle to the file on disk is transferred to the EFS file handle. If CreateEFSFile fails, the file should be considered corrupt, and should be deleted.
BOOL CreateEFSFile(
	// Handle of the file on disk to append an EFS header to
	IN HANDLE hEFSFile,
	// The handle to the new EFS file. This is allocated by the caller.
	OUT EFSFILEHANDLEFORWRITE *pEFSFile
)
{
	assert(hEFSFile != INVALID_HANDLE_VALUE);
	assert(pEFSFile);

	// Set up the header values
	EFSFILEHEADER header;

	ZeroMemory(&header, sizeof(EFSFILEHEADER));

	header.dwSignature = EFS_SIGNATURE;
	header.dwVersion = 0x00000001;
	header.dwFileSize = sizeof(EFSFILEHEADER);
	header.dwDirectoryOffset = 0;
	header.dwNumDirectoryEntries = 0;

	// Place the header at the end of the file, aligned to a SECTOR_SIZE boundary
	DWORD dwHeaderOffset, dwBytesWritten;
	dwHeaderOffset = SetFilePointer(hEFSFile, 0, NULL, FILE_END);
	dwHeaderOffset = (dwHeaderOffset + SECTOR_SIZE - 1) & ~(SECTOR_SIZE - 1);

	// Write the header out
	SetFilePointer(hEFSFile, dwHeaderOffset, NULL, FILE_BEGIN);
	if (!WriteFile(hEFSFile, &header, sizeof(EFSFILEHEADER), &dwBytesWritten, NULL)
		|| (dwBytesWritten != sizeof(EFSFILEHEADER)))
		return FALSE;

	SetEndOfFile(hEFSFile);

	// Allocate a directory in memory for the new EFS file
	DWORD nNumBytesToAlloc = 32 * sizeof(EFSDIRECTORYENTRY);
	EFSDIRECTORYENTRY *pDirEntries = 
		(EFSDIRECTORYENTRY *)malloc(nNumBytesToAlloc);
	if (!pDirEntries)
		return FALSE;

	// Clear the EFS directory, and set up the EFS handle
	ZeroMemory(pDirEntries, nNumBytesToAlloc);

	pEFSFile->hFile = hEFSFile;
	pEFSFile->bModified = FALSE;

	pEFSFile->dwHeaderOffset = dwHeaderOffset;
	pEFSFile->dwInsertPoint = sizeof(EFSFILEHEADER);

	pEFSFile->pDirectory = pDirEntries;
	pEFSFile->nNumDirectoryEntries = 0;
	pEFSFile->nMaxDirectoryEntries = 32;

	return TRUE;
}

// Checks the directory of an EFS file to see if there are any entries that appear to be invalid. Most importantly, we need to check if there are any that point to data outside the EFS file. Returns FALSE if the check failed.
BOOL CheckEFSDirectoryAndFindInsertPoint(
	// The directory to check
	IN const EFSDIRECTORYENTRY *pDirectory,
	// The number of entries in the directory
	IN DWORD nNumDirEntries,
	// The total size of the EFS file, not including the part of the disk file before the EFS header
	IN DWORD nFileSize,
	// The largest offset of the end of a file
	OUT LPDWORD lpnInsertPoint
)
{
	assert(pDirectory);
	assert(nFileSize);
	assert(lpnInsertPoint);

	*lpnInsertPoint = sizeof(EFSFILEHEADER);

	// Loop through the list to do two things
	for (DWORD iCurDirEntry = 0; iCurDirEntry < nNumDirEntries; iCurDirEntry++)
	{
		// Skip files that are 0 bytes, since the file offset isn't used, and shouldn't be checked
		if (pDirectory[iCurDirEntry].dwSize == 0)
			continue;

		// Find the insert point for the EFS file by finding the largest offset after all files in the EFS file
		*lpnInsertPoint = max(*lpnInsertPoint,
			pDirectory[iCurDirEntry].dwOffset +
			pDirectory[iCurDirEntry].dwSize);
	}

	// Trivial integrity check: check to make sure the insert point is within the file on disk. If this is not the case, it means at least one of the directory entries is corrupted, and crashes will likely ensue.
	if (*lpnInsertPoint <= nFileSize)
		return TRUE;

	return FALSE;
}

// Loads an EFS file from a file on disk at the specified offset, and returns an EFS write handle for it. On success, the hEFSFile ownership is transferred to the EFS write handle.
BOOL LoadEFSFile(
	// Handle of the file on disk to load from
	IN HANDLE hEFSFile,
	// Offset of the EFS header in the file on disk
	IN DWORD dwHeaderOffset,
	// The returned EFS write handle. This is allocated by the caller.
	OUT EFSFILEHANDLEFORWRITE *pEFSFile
)
{
	assert(hEFSFile != INVALID_HANDLE_VALUE);
	assert(dwHeaderOffset);
	assert(pEFSFile);

	// Read the header
	EFSFILEHEADER header;
	DWORD dwBytesRead;

	SetFilePointer(hEFSFile, dwHeaderOffset, NULL, FILE_BEGIN);
	if (!ReadFile(hEFSFile, &header, sizeof(EFSFILEHEADER), &dwBytesRead, NULL)
		|| (dwBytesRead == sizeof(EFSFILEHEADER)))
		return FALSE;

	// Allocate the directory for the EFS file
	DWORD nNumDirEntriesToAlloc = header.dwNumDirectoryEntries + 32,
		nNumBytesToAlloc = nNumDirEntriesToAlloc * sizeof(EFSDIRECTORYENTRY);
	EFSDIRECTORYENTRY *pDirEntry;

	pDirEntry = (EFSDIRECTORYENTRY *)malloc(nNumBytesToAlloc);
	if (!pDirEntry)
		return FALSE;

	// Set up the directory and the EFS handle (most of it)
	ZeroMemory(pDirEntry, nNumBytesToAlloc);

	pEFSFile->hFile = hEFSFile;
	pEFSFile->bModified = FALSE;

	pEFSFile->dwHeaderOffset = dwHeaderOffset;
	pEFSFile->dwInsertPoint = sizeof(EFSFILEHEADER);

	pEFSFile->nNumDirectoryEntries = header.dwNumDirectoryEntries;
	pEFSFile->nMaxDirectoryEntries = nNumDirEntriesToAlloc;

	pEFSFile->pDirectory = pDirEntry;

	// If there are no directory entries to read from the file, we're done
	if (!header.dwNumDirectoryEntries)
		return TRUE;

	// Read the directory entries from the file
	DWORD nNumBytesToRead = header.dwNumDirectoryEntries * sizeof(EFSDIRECTORYENTRY);
	SetFilePointer(hEFSFile, dwHeaderOffset + header.dwDirectoryOffset, NULL, FILE_BEGIN);
	if (ReadFile(hEFSFile, pDirEntry, nNumBytesToRead, &dwBytesRead, NULL)
		&& (dwBytesRead == nNumBytesToRead))
	{
		if (CheckEFSDirectoryAndFindInsertPoint(pDirEntry, header.dwNumDirectoryEntries, GetFileSize(hEFSFile, NULL) - dwHeaderOffset, &pEFSFile->dwInsertPoint))
			return TRUE;
	}

	// Failed. Clean up.
	free(pDirEntry);
	pEFSFile->pDirectory = NULL;

	return FALSE;
}

BOOL WINAPI MapFileIntoMemoryForRead(
	IN LPCSTR lpszFileName,
	OUT LPCVOID *lplpvFileData,
	OUT LPDWORD lpnFileSize
)
{
	assert(lpszFileName);
	assert(lplpvFileData);
	assert(lpnFileSize);

	// First, open the file for reading
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// Get the file size now, while we can
	*lpnFileSize = GetFileSize(hFile, NULL);

	// Create the file mapping object for it. Note that we will close the file immediately afterwards, as the existence of the file mapping object will keep the file open.
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	CloseHandle(hFile);
	if (!hFileMap)
		return FALSE;

	// Map the file into memory
	*lplpvFileData = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(hFileMap);

	// Right now we either have the file mapped or we don't. In either case, we've already closed the handles, so we don't need to clean them up.
	return (*lplpvFileData != NULL);
}

EFSHANDLEFORWRITE WINAPI OpenEFSFileForWrite(
	IN LPCSTR lpszFileName, 
	IN DWORD dwUnused
)
{
	assert(lpszFileName);
	assert(!dwUnused);

	// First, open the file on disk
	HANDLE hEFSFile;

	hEFSFile = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hEFSFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// Allocate the data structure for the EFS file
	EFSFILEHANDLEFORWRITE *pFile = (EFSFILEHANDLEFORWRITE *)malloc(sizeof(EFSFILEHANDLEFORWRITE));
	if (pFile)
	{
		ZeroMemory(pFile, sizeof(EFSFILEHANDLEFORWRITE));

		// Find out if there's already an EFS archive in the file
		DWORD dwHeaderOffset;
		if (FindEFSHeader(hEFSFile, &dwHeaderOffset))
		{
			// There is. Try to load it.
			if (LoadEFSFile(hEFSFile, dwHeaderOffset, pFile))
				return (EFSHANDLEFORWRITE)pFile;
		}
		else
		{
			// There isn't. Append one to the file.
			if (CreateEFSFile(hEFSFile, pFile))
				return (EFSHANDLEFORWRITE)pFile;
		}

		free(pFile);
	}

	CloseHandle(hEFSFile);

	return NULL;
}

// Saves the EFS header and directory table to the file, in preparation for close. If there are no modifications, does nothing and returns success.
BOOL SaveEFSFile(
	// The EFS archive to save
	IN const EFSFILEHANDLEFORWRITE *pEFSFile
)
{
	assert(pEFSFile);
	assert(pEFSFile->hFile != INVALID_HANDLE_VALUE);
	assert(pEFSFile->pDirectory);

	if (!pEFSFile->bModified)
		return TRUE;	// Nothing to save

	// The EFS archive has been modified, so we need to save it
	// Create the EFS header
	EFSFILEHEADER header;
	DWORD dwDirectorySize = 
		pEFSFile->nNumDirectoryEntries * sizeof(EFSDIRECTORYENTRY), dwBytesWritten;

	ZeroMemory(&header, sizeof(EFSDIRECTORYENTRY));

	header.dwSignature = EFS_SIGNATURE;
	header.dwVersion = 0x00000001;

	header.dwFileSize = pEFSFile->dwInsertPoint + dwDirectorySize;

	header.dwDirectoryOffset = pEFSFile->dwInsertPoint;
	header.dwNumDirectoryEntries = pEFSFile->nNumDirectoryEntries;

	// Save the header
	SetFilePointer(pEFSFile->hFile, pEFSFile->dwHeaderOffset, NULL, FILE_BEGIN);
	if (!WriteFile(pEFSFile->hFile, &header, sizeof(EFSFILEHEADER), &dwBytesWritten, NULL)
		|| (dwBytesWritten != sizeof(EFSFILEHEADER)))
		return FALSE;

	// Now we need to write the end of the file, and possibly...
	DWORD dwEndOfArchive;
	if (pEFSFile->nNumDirectoryEntries)
	{
		// Write the EFS directory
		SetFilePointer(pEFSFile->hFile, pEFSFile->dwInsertPoint + pEFSFile->dwHeaderOffset, NULL, FILE_BEGIN);
		if (!WriteFile(pEFSFile->hFile, pEFSFile->pDirectory, dwDirectorySize, &dwBytesWritten, NULL)
			|| (dwBytesWritten != dwDirectorySize))
			return FALSE;

		dwEndOfArchive = pEFSFile->dwInsertPoint + 
			pEFSFile->dwHeaderOffset + dwDirectorySize;
	}
	else
		dwEndOfArchive = pEFSFile->dwInsertPoint + 
			pEFSFile->dwHeaderOffset;

	// Set the file size, padded out to the nearest FILE_GRANULARITY
	SetFilePointer(pEFSFile->hFile, (dwEndOfArchive + FILE_GRANULARITY - 1) & ~(FILE_GRANULARITY - 1), NULL, FILE_BEGIN);
	SetEndOfFile(pEFSFile->hFile);

	return TRUE;
}

BOOL WINAPI CloseEFSFileForWrite(
	IN EFSHANDLEFORWRITE hEFSFile
)
{
	assert(hEFSFile);

	// Dereference the handle structure
	EFSFILEHANDLEFORWRITE *pFile = (EFSFILEHANDLEFORWRITE *)hEFSFile;

	assert(pFile->hFile != INVALID_HANDLE_VALUE);

	// Save the file, if necessary
	BOOL bRetVal = SaveEFSFile(pFile);

	// Close the file
	CloseHandle(pFile->hFile);

	// Free the data structures
	free(pFile->pDirectory);
	free(pFile);

	return bRetVal;
}

// Checks the directory to see if the file exists in the archive. If it exists, the index of the file is returned. If the file doesn't exist, (DWORD)-1 is returned
DWORD FindFileInEFSFile(
	// The directory of the EFS archive
	IN const EFSDIRECTORYENTRY *pDirectory,
	// The number of files in the EFS archive
	IN DWORD dwNumDirectoryEntries,
	// The major ID of the file to find
	IN DWORD dwComponentID,
	// The minor ID of the file to find
	IN DWORD dwFileID
)
{
	assert(pDirectory);

	// Check each entry in the directory
	for (DWORD iCurDirEntry = 0; iCurDirEntry < dwNumDirectoryEntries; iCurDirEntry++)
	{
		if (pDirectory[iCurDirEntry].dwComponentID == dwComponentID &&
			pDirectory[iCurDirEntry].dwFileID == dwFileID)
			return iCurDirEntry;	// Found it
	}

	return (DWORD)-1;	// Doesn't exist
}

// Adds a new file to an EFS archive, in standard uncompressed, undecorated form. The file is read from disk, and written to the file containing the EFS archive, and the slot provided in the EFS directory is filled with the file's size and offset.
BOOL AddUncompressedToEFSFile(
	// The EFS archive structure to add to
	EFSFILEHANDLEFORWRITE *pEFSFile,
	// Handle of the file to be added
	HANDLE hFile,
	// The directory entry the new file will go in
	EFSDIRECTORYENTRY *pDirEntry
)
{
	assert(pEFSFile);
	assert(hFile != INVALID_HANDLE_VALUE);
	assert(pDirEntry);

	// If the file size is 0, we don't need to add anything before we're done. We only need to update the size and offset of the file in the directory.
	DWORD dwFileSize = GetFileSize(hFile, NULL);

	if (!dwFileSize)
	{
		pDirEntry->dwOffset = 0;
		pDirEntry->dwSize = 0;

		return TRUE;
	}

	// The file size isn't 0, so we have to add it
	// Allocate a read buffer
	DWORD dwBufferSize = min(dwFileSize, 128 << 10);
	LPBYTE lpbyReadBuffer = (LPBYTE)malloc(dwBufferSize);

	if (!lpbyReadBuffer)
		return FALSE;

	// Simple read and write loop
	BOOL bRetVal = FALSE;
	DWORD dwReadPtr = 0, dwWritePtr = pEFSFile->dwInsertPoint + pEFSFile->dwHeaderOffset, 
		dwRemaining = dwFileSize, dwBlockSize, dwBytesRead;

	while (dwRemaining)
	{
		// Get the size of the next read to make
		dwBlockSize = min(dwRemaining, dwBufferSize);

		// Read it...
		SetFilePointer(hFile, dwReadPtr, NULL, FILE_BEGIN);
		if (!ReadFile(hFile, lpbyReadBuffer, dwBlockSize, &dwBytesRead, NULL)
			|| (dwBytesRead < dwBlockSize))
			break;

		// And write it
		SetFilePointer(pEFSFile->hFile, dwWritePtr, NULL, FILE_BEGIN);
		if (!WriteFile(pEFSFile->hFile, lpbyReadBuffer, dwBlockSize, &dwBytesRead, NULL)
			|| (dwBytesRead < dwBlockSize))
			break;

		dwReadPtr += dwBlockSize;
		dwWritePtr += dwBlockSize;

		dwRemaining -= dwBlockSize;
	}

	// If we've reached the end of the file, addition was successful. Otherwise, a read or write failed.
	if (!dwRemaining)
	{
		// Success. Update the directory entry.
		pDirEntry->dwOffset = pEFSFile->dwInsertPoint;
		pDirEntry->dwSize = dwFileSize;

		bRetVal = TRUE;
	}
	else
	{
		// Addition failed. Roll back the changes by reverting the insert point to what it was before we started writing.
		SetFilePointer(pEFSFile->hFile, pEFSFile->dwInsertPoint, NULL, FILE_BEGIN);
		SetEndOfFile(pEFSFile->hFile);

		bRetVal = FALSE;
	}

	free(lpbyReadBuffer);

	return bRetVal;
}

BOOL WINAPI AddToEFSFile(
	IN EFSHANDLEFORWRITE hEFSFile,
	IN LPCSTR lpszFileName,
	IN DWORD dwComponentID,
	IN DWORD dwFileID,
	IN DWORD dwData,
	IN DWORD dwFlags
)
{
	assert(hEFSFile);
	assert(lpszFileName);

	// Check for unsupported flags. Right now all flags are unsupported.
	if (dwFlags)
		return FALSE;

	// Extract the EFS archive structure
	EFSFILEHANDLEFORWRITE *pEFSFile = (EFSFILEHANDLEFORWRITE *)hEFSFile;

	assert(pEFSFile->hFile != INVALID_HANDLE_VALUE);
	assert(pEFSFile->pDirectory);

	// If we've exceeded the number of entries in the directory table, we need to allocate a bigger one
	if (pEFSFile->nNumDirectoryEntries >= pEFSFile->nMaxDirectoryEntries)
	{
		DWORD nNumDirEntriesToAlloc = pEFSFile->nMaxDirectoryEntries * 2,
			nNumBytesToAlloc = nNumDirEntriesToAlloc * sizeof(EFSDIRECTORYENTRY);
		EFSDIRECTORYENTRY *pNewDirectory = (EFSDIRECTORYENTRY *)malloc(nNumBytesToAlloc);

		if (!pNewDirectory)
			return FALSE;

		// Copy the entrees over
		DWORD nNumBytesToCopy = pEFSFile->nMaxDirectoryEntries * sizeof(EFSDIRECTORYENTRY);
		memcpy(pNewDirectory, pEFSFile->pDirectory, nNumBytesToCopy);
		ZeroMemory((LPBYTE)pNewDirectory + nNumBytesToCopy, nNumBytesToAlloc - nNumBytesToCopy);

		// Replace the old directory entirely
		free(pEFSFile->pDirectory);

		pEFSFile->pDirectory = pNewDirectory;
		pEFSFile->nMaxDirectoryEntries = nNumDirEntriesToAlloc;
	}

	// Open the file to add to the EFS archive
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// Add the file using AddUncompressedToEFSFile
	BOOL bRetVal = FALSE;
	// AddUncompressedToEFSFile may trash the directory even if the addition fails, so we need to make sure we always write it again (mark the archive as modified)
	pEFSFile->bModified = TRUE;

	EFSDIRECTORYENTRY *pDirEntry = &pEFSFile->pDirectory[pEFSFile->nNumDirectoryEntries];
	if (AddUncompressedToEFSFile(pEFSFile, hFile, pDirEntry))
	{
		// Success. Fill in the file data not filled in by AddUncompressedToEFSFile.
		pDirEntry->dwComponentID = dwComponentID;
		pDirEntry->dwFileID = dwFileID;
		pDirEntry->dwData = dwData;
		pDirEntry->dwFlags = 0;

		// Update the archive state
		pEFSFile->dwInsertPoint += pDirEntry->dwSize;
		pEFSFile->nNumDirectoryEntries++;

		bRetVal = TRUE;
	}

	CloseHandle(hFile);

	return bRetVal;
}

/*BOOL WINAPI DeleteFromEFSFile(
	IN EFSHANDLEFORWRITE hEFSFile
	IN DWORD dwComponentID,
	IN DWORD dwFileID
);*/

EFSHANDLEFORREAD WINAPI GetEFSHandleFromMappedFile(
	IN const BYTE *lpbyFileData, 
	IN DWORD dwFileSize
)
{
	assert(lpbyFileData);
	assert(dwFileSize);

	// This is an inherently dangerous operation. We're accessing a block the caller says is good and says that the file size is accurate, but we can't be sure that's correct. You could put a structured exception handling block here to catch any access violations, but personally I'd rather just let the program blow up and let the caller deal with it (fix it), as it's their fault to begin with.

	// Basically the exact same thing we did in FindEFSHeader: scan the file every SECTOR_SIZE bytes for an EFS header. If we can't find one, fail.
	DWORD dwFileOffset = 0;
	EFSFILEHEADER *pHeader;
	while (dwFileOffset + sizeof(EFSFILEHEADER) < dwFileSize)
	{
		pHeader = (EFSFILEHEADER *)(lpbyFileData + dwFileOffset);

		// Does it look like an EFS header?
		if ((pHeader->dwSignature == EFS_SIGNATURE) &&

			(pHeader->dwVersion == 0x00000001) &&

			((dwFileOffset + pHeader->dwFileSize) <= dwFileSize) &&
			((dwFileOffset + pHeader->dwDirectoryOffset + (pHeader->dwNumDirectoryEntries * sizeof(EFSDIRECTORYENTRY))) <= dwFileSize))
		{
			// Yes. Check its directory for validity.
			EFSDIRECTORYENTRY *pDirectory = (EFSDIRECTORYENTRY *)(lpbyFileData + dwFileOffset + pHeader->dwDirectoryOffset);
			DWORD dwInsertPoint;

			if (CheckEFSDirectoryAndFindInsertPoint(pDirectory, pHeader->dwNumDirectoryEntries, dwFileSize, &dwInsertPoint))
				return (EFSHANDLEFORREAD)(pHeader);	// Directory looks good
		}

		dwFileOffset += SECTOR_SIZE;
	}

	return NULL;
}

BOOL WINAPI LookupEFSFile(
	IN EFSHANDLEFORREAD hEFSFile,
	IN DWORD dwComponentID,
	IN DWORD dwFileID,
	OUT LPCVOID *lplpvFileData,
	OUT LPDWORD lpdwFileSize,
	OUT OPTIONAL LPDWORD lpdwFileData
)
{
	assert(hEFSFile);
	assert(lplpvFileData);
	assert(lpdwFileSize);

	// Convert the read handle to the header pointer
	EFSFILEHEADER *pEFSHeader = (EFSFILEHEADER *)hEFSFile;

	// If there are no files in the archive, obviously the file doesn't exist
	if (!pEFSHeader->dwNumDirectoryEntries)
		return FALSE;

	// See if the file exists in the EFS archive
	EFSDIRECTORYENTRY *pDirectory = (EFSDIRECTORYENTRY *)((LPBYTE)pEFSHeader + pEFSHeader->dwDirectoryOffset);

	DWORD iDirEntry = FindFileInEFSFile(pDirectory, pEFSHeader->dwNumDirectoryEntries, dwComponentID, dwFileID);
	if (iDirEntry == (DWORD)-1)
		return FALSE;	// It doesn't

	// It does. Get the requested info.
	*lplpvFileData = (LPVOID)((LPBYTE)pEFSHeader + pDirectory[iDirEntry].dwOffset);
	*lpdwFileSize = pDirectory[iDirEntry].dwSize;

	if (lpdwFileData)
		*lpdwFileData = pDirectory[iDirEntry].dwData;

	return TRUE;
}

BOOL WINAPI ExtractEFSFile(
	IN EFSHANDLEFORREAD hEFSFile,
	IN DWORD dwComponentID,
	IN DWORD dwFileID,
	IN LPCSTR lpszFileName
)
{
	assert(lpszFileName);

	// This is exactly the same procedure for ExtractResource
	// Find the file in the EFS archive (if it exists)
	LPCVOID lpvFileData;
	DWORD dwFileSize;
	if (!LookupEFSFile(hEFSFile, dwComponentID, dwFileID, &lpvFileData, &dwFileSize, NULL))
		return FALSE;

	// Open the output file
	HANDLE hOutFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL bRetVal = FALSE;

	// Write the file in one shot, as it's all mapped into memory
	DWORD dwBytesRead;
	if (WriteFile(hOutFile, lpvFileData, dwFileSize, &dwBytesRead, NULL)
		&& (dwBytesRead == dwFileSize))
	{
		SetEndOfFile(hOutFile);

		bRetVal = TRUE;
	}

	CloseHandle(hOutFile);

	// Delete the file if extraction failed
	if (!bRetVal)
		DeleteFile(lpszFileName);

	return bRetVal;
}

BOOL WINAPI ExtractTempEFSFile(
	IN EFSHANDLEFORREAD hEFSFile,
	IN DWORD dwComponentID,
	IN DWORD dwFileID,
	OUT LPSTR lpszOutFileName
)
{
	assert(hEFSFile);
	assert(lpszOutFileName);

	// Once more, as before
	// Get a unique file name in the Windows temporary file path
	char szTempDir[MAX_PATH + 1], szTempFileName[MAX_PATH + 1];
	if (!GetTempPath(MAX_PATH, szTempDir) ||
		!GetTempFileName(szTempDir, "Embedded", 0, szTempFileName))
		return FALSE;

	// Do the extraction
	if (!ExtractEFSFile(hEFSFile, dwComponentID, dwFileID, szTempFileName))
		return FALSE;

	// Add the extracted file to the temporary file list
	if (!AddTempFileToList(szTempFileName))
	{
		DeleteFile(szTempFileName);

		return FALSE;
	}

	strcpy(lpszOutFileName, szTempFileName);

	return TRUE;
}

DWORD WINAPI GetNumEFSFiles(
	IN EFSHANDLEFORREAD hEFSFile
)
{
	assert(hEFSFile);

	// Nothing simpler than this
	EFSFILEHEADER *pEFSHeader = (EFSFILEHEADER *)hEFSFile;

	return pEFSHeader->dwNumDirectoryEntries;
}

BOOL WINAPI EnumEFSFiles(
	IN EFSHANDLEFORREAD hEFSFile,
	IN DWORD dwEFSFileIndex,
	OUT LPDWORD lpdwComponentID,
	OUT LPDWORD lpdwFileID,
	OUT OPTIONAL LPDWORD lpdwData,
	OUT OPTIONAL LPDWORD lpdwFileSize
)
{
	assert(hEFSFile);
	assert(lpdwComponentID);
	assert(lpdwFileID);

	// Get the EFS header
	EFSFILEHEADER *pEFSHeader = (EFSFILEHEADER *)hEFSFile;

	// Check that the index is within the bounds of the directory
	if (dwEFSFileIndex >= pEFSHeader->dwNumDirectoryEntries)
		return FALSE;

	// Get the directory and then find the file at the specified index
	EFSDIRECTORYENTRY *pDirectory = (EFSDIRECTORYENTRY *)((LPBYTE)pEFSHeader + pEFSHeader->dwDirectoryOffset),
		*pDirEntry = &pDirectory[dwEFSFileIndex];

	// Grab the file info
	*lpdwComponentID = pDirEntry->dwComponentID;
	*lpdwFileID = pDirEntry->dwFileID;

	if (lpdwData)
		*lpdwData = pDirEntry->dwData;

	if (lpdwFileSize)
		*lpdwFileSize = pDirEntry->dwSize;

	return TRUE;
}