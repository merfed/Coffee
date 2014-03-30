/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2007 Justin Olbrantz. All Rights Reserved.
*/

// Prevent this header from being included multiple times
#ifndef QRESOURCE_H
#define QRESOURCE_H

#include <windows.h>

typedef HANDLE EFSHANDLEFORWRITE;
typedef HANDLE EFSHANDLEFORREAD;

/*
	* QResourceInitialize *
	Performs initialization of the QResource module. Must be called before any other QResource functions are used.
*/
void WINAPI QResourceInitialize();

/*
	* QResourceDestroy *
	Frees resources allocated by QResourceInitialize and other QResource functions. Must be called before the program closes. Does not delete temporary files that have been extracted; DeleteTemporaryFiles must be used for this purpose, before QResourceDestroy.
*/
void WINAPI QResourceDestroy();

/*
	* DeleteTemporaryFiles *
	Deletes all embedded files and resources which have been extracted using either ExtractTempResource or ExtractTempEFSFile. Useful as a one-step cleanup on program termination.
*/
BOOL WINAPI DeleteTemporaryFiles();

/*
	* LookupResource *
	Loads a resource into memory from any module that is already loaded, and returns a pointer to the resource and the resource's size. The resource is memory mapped, and so does not consume additional memory; the memory the resource is loaded into does not need to be explicitly freed; however, the resource will disappear if the module containing it is unloaded with FreeLibrary.
	If the resource cannot be found or cannot be loaded, LookupResource will return FALSE.
*/
BOOL WINAPI LookupResource(
	// Handle of the module containing the desired resource. If NULL, the EXE of the current process will be used.
	IN OPTIONAL HMODULE hModule, 
	// The name or ID number of the resource
	IN LPCSTR lpszResourceName, 
	// The name or ID number of the type of the desired resource
	IN LPCSTR lpszResoureType, 
	// The pointer to the resource data
	OUT LPCVOID *lplpvResourceData, 
	// The size of the resource data
	OUT LPDWORD lpnResourceSize
);

/*
	* ExtractResource *
	Extracts the specified resource to a file on disk. If the file already exists, it will be overwritten. Functions otherwise like LookupResource.
*/

BOOL WINAPI ExtractResource(
	// Handle of the module containing the desired resource. If NULL, the EXE of the current process will be used.
	IN OPTIONAL HMODULE hModule,
	// The name or ID number of the resource
	IN LPCSTR lpszResourceName, 
	// The name or ID number of the type of the desired resource
	IN LPCSTR lpszResourceType, 
	// Path of the file the resource is to be extracted to
	IN LPCSTR lpszFileName
);

/*
	* ExtractTempResource *
	Extracts a resource from an already loaded module to disk, giving it a unique, temporary name. On successful extraction, ExtractTempResource will return the file's unique name. The extracted file will be automatically deleted from the disk when DeleteTemporaryFiles is called. Functions otherwise like LookupResource.
*/
BOOL WINAPI ExtractTempResource(
	// Handle of the module containing the desired resource. If NULL, the EXE of the current process will be used.
	IN OPTIONAL HMODULE hModule,
	// The name or ID number of the resource
	IN LPCSTR lpszResourceName, 
	// The name or ID number of the type of the desired resource
	IN LPCSTR lpszResourceType, 
	// The unique path of the extracted file. This buffer must be at least MAX_PATH characters large.
	OUT LPSTR lpszOutFileName
);

/*
	* MapFileIntoMemoryForRead *
	Maps the specified file on disk entirely into memory, and returns the size and location of the file data. The file is memory mapped, and so does not consume memory; it will, however, obviously consume virtual address space. The file can be unmapped using UnmapViewOfFile. Fails if the file cannot be opened or cannot be mapped for whatever reason (such as there not being enough address space to hold the file).
*/
BOOL WINAPI MapFileIntoMemoryForRead(
	// The path of the file to be mapped into memory
	IN LPCSTR lpszFileName,
	// The file data after mapping
	OUT LPCVOID *lplpvFileData,
	// The size of the file
	OUT LPDWORD lpnFileSize
);

/*
	The Embedded File System (EFS) is a minimalistic archive format for storing the plugins and their data files in an SEMPQ. While the MPQDraft program itself uses module file resources to store the SEMPQ stub and the patcher DLL, resources were impractical for SEMPQ files, because the format is more complicated, and it's troublesome to modify resources after a module has been compiled and linked.
	Files are stored end-to-end, uncompressed, and unencrypted. Files are identified by a component ID number (major ID) and a file ID number (minor ID); the naming reflects the creation for use in MPQDraft, where there could be multiple plugins, each with its own set of data files. In retrospect, it might have been better to use something like TAR, instead.
*/

/*
	* OpenEFSFileForWrite *
	Opens an Embedded File System (EFS) file for modifications, and returns an handle EFSHANDLEFORWRITE which can be used in calls to modify the EFS file, including CloseEFSFileForWrite, AddToEFSFile,	and DeleteFromEFSFile. The handle cannot be used in calls to LookupEFSFile or ExtractEFSFile. On failure, returns NULL; if the file does not have an EFS file embedded in it, OpenEFSFileForWrite will embed a new EFS file.

*/
EFSHANDLEFORWRITE WINAPI OpenEFSFileForWrite(
	// The path of the file to be opened for writing
	IN LPCSTR lpszFileName, 
	// Unused. Must be 0.
	DWORD dwUnused
);

/*
	* CloseEFSFileForWrite *
	Closes an EFS file previously opened with OpenEFSFileForWrite. Should always be performed when you are done using an EFS file. A return value of FALSE indicates that the file has been closed, but it has probably been corrupted, and should be deleted.
*/
BOOL WINAPI CloseEFSFileForWrite(
	EFSHANDLEFORWRITE hEFSFile
);

/*
	* AddToEFSFile *
	Adds a file to the specified EFS file. The individual file will be identified by two numbers - a component ID and a file ID. Together, these numbers should be unique in an EFS file. Fails if a file with that ID already exists.
*/
BOOL WINAPI AddToEFSFile(
	// The handle of the EFS file the new file is to be added to
	IN EFSHANDLEFORWRITE hEFSFile,
	// The name of the file on disk that is to be added to the EFS file
	IN LPCSTR lpszFileName,
	// The major ID of the file
	IN DWORD dwComponentID,
	// The minor ID of the file
	IN DWORD dwFileID,
	// A user-defined value that is associated with the file, and may be retrieved
	IN DWORD dwData,
	// Flags for the add operation. For now, must be 0.
	IN DWORD dwFlags
);

/*
	* GetEFSHandleFromMappedFile *
	GetEFSHandleFromMappedFile creates an EFSHANDLEFORREAD handle from any EFS file which has been loaded ENTIRELY into memory, preferrably in the form of a memory-mapped file. This handle can be used with either of the EFS file reading functions. If the mapped file does not contain an EFS file, or some other failure occurs, GetEFSHandleFromMappedFile will return NULL.
	Note that, like a resource found with LookupResource, this EFSHANDLEFORREAD does not need to be explicitely closed. However, this handle will be invalid if the mapped file is unmapped.*/
EFSHANDLEFORREAD WINAPI GetEFSHandleFromMappedFile(
	// The mapped data containing the EFS file
	IN const BYTE *lpbyFileData, 
	// The size of the data in memory which is to be loaded as an EFS file
	IN DWORD dwFileSize
);

/*
	* LookupEFSFile *
	Loads into memory a file in an EFS file, and returns a pointer to the file, the file's size, and the file's data attribute. The memory the file is loaded into does not need to be explicitely freed; however, the memory will become invalid if the EFS file is freed, such as by the module containing it is unloaded with FreeLibrary.
*/
BOOL WINAPI LookupEFSFile(
	// The handle of the EFS file, obtained previously with one of the preceding functions
	IN EFSHANDLEFORREAD hEFSFile,
	// The major ID of the specified file
	IN DWORD dwComponentID,
	// The minor ID of the specified file
	IN DWORD dwFileID,
	// The pointer to the specified file's data
	OUT LPCVOID *lplpvFileData,
	// The specified file's size
	OUT LPDWORD lpdwFileSize,
	// The user-data value of the specified file. If this parameter is NULL, this value will not be returned.
	OUT OPTIONAL LPDWORD lpdwFileData
);

/*
	* ExtractEFSFile *
	Extracts a file inside the EFS file to a specified file on disk, overwriting that file if necessary. Otherwise operates like LookupEFSFile.
*/
BOOL WINAPI ExtractEFSFile(
	// The handle of the EFS file, obtained previously with one of the preceding functions
	IN EFSHANDLEFORREAD hEFSFile,
	// The major ID of the specified file
	IN DWORD dwComponentID,
	// The minor ID of the specified file
	IN DWORD dwFileID,
	// The file name the specified file should be extracted to
	IN LPCSTR lpszFileName
);

/*
	* ExtractTempEFSFile *
	Extracts a file inside an EFS file to a temporary file on disk having a unique filename. The file will be automatically deleted if DeleteTemporaryFiles is called.
*/
BOOL WINAPI ExtractTempEFSFile(
	// The handle to the EFS file containing the specified file
	IN EFSHANDLEFORREAD hEFSFile,
	// The major ID of the specified file
	IN DWORD dwComponentID,
	// The minor ID of the specified file
	IN DWORD dwFileID,
	// The unique filename of the extracted file
	OUT LPSTR lpszOutFileName
);

/*
	* GetNumEFSFiles *
	Retrieves the number of files embedded in the specified EFS file.
*/
DWORD WINAPI GetNumEFSFiles(
	// The EFS file whose number of embedded files is to be obtained
	IN EFSHANDLEFORREAD hEFSFile
);

/*
	* EnumEFSFiles *
	Retrieves information about the specified file contained in an EFS file. The file is specified by ordinal (number in the archive), and the file IDs, size, and user-defined data value are returned. To enumerate all files in an EFS file. this function should be called with indices from 0 to GetNumEFSFiles - 1. Fails if an index outside this range is specified.
*/
BOOL WINAPI EnumEFSFiles(
	// The handle of the EFS file about whose files info is to be obtained
	IN EFSHANDLEFORREAD hEFSFile,
	// The index of the file to retrieve the info of
	IN DWORD dwEFSFileIndex,
	// The major ID of the specified file
	OUT LPDWORD lpdwComponentID,
	// The minor ID of the specified file
	OUT LPDWORD lpdwFileID,
	// The user-defined data value of the specified file
	OUT OPTIONAL LPDWORD lpdwData,
	// The size of the specified file
	OUT OPTIONAL LPDWORD lpdwFileSize
);

#endif // #ifndef QRESOURCE.H