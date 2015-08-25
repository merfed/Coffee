/*****************************************************************************/
/* SListFile.cpp                          Copyright (c) Ladislav Zezula 2004 */
/*---------------------------------------------------------------------------*/
/* Description:                                                              */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 12.06.04  1.00  Lad  The first version of SListFile.cpp                   */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormCommon.h"
#include <assert.h>

//-----------------------------------------------------------------------------
// Listfile entry structure

#define CACHE_BUFFER_SIZE  0x1000       // Size of the cache buffer

struct TListFileCache
{
    HANDLE  hFile;                      // Stormlib file handle
    char  * szMask;                     // File mask
    DWORD   dwFileSize;                 // Total size of the cached file
    DWORD   dwFilePos;                  // Position of the cache in the file
    BYTE  * pBegin;                     // The begin of the listfile cache
    BYTE  * pPos;
    BYTE  * pEnd;                       // The last character in the file cache

    BYTE Buffer[CACHE_BUFFER_SIZE];     // Listfile cache itself
};

//-----------------------------------------------------------------------------
// Local functions (cache)

static TListFileCache * CreateListFileCache(HANDLE hMpq, const char * szListFile)
{
    TListFileCache * pCache = NULL;
    HANDLE hListFile = NULL;
    DWORD dwSearchScope = SFILE_OPEN_LOCAL_FILE;
    DWORD dwBytesRead = 0;
    int nError = ERROR_SUCCESS;

    // If the szListFile is NULL, it means we have to open internal listfile
    if(szListFile == NULL)
    {
        // Use SFILE_OPEN_ANY_LOCALE for listfile. This will allow us to load
        // the listfile even if there is only non-neutral version of the listfile in the MPQ
        dwSearchScope = SFILE_OPEN_ANY_LOCALE;
        szListFile = LISTFILE_NAME;
    }

    // Open the local/internal listfile
    if(!SFileOpenFileEx(hMpq, szListFile, dwSearchScope, &hListFile))
        nError = GetLastError();

    // Allocate cache for one file block
    if(nError == ERROR_SUCCESS)
    {
        pCache = (TListFileCache *)ALLOCMEM(TListFileCache, 1);
        if(pCache == NULL)
            nError = ERROR_NOT_ENOUGH_MEMORY;
    }

    if(nError == ERROR_SUCCESS)
    {
        // Initialize the file cache
        memset(pCache, 0, sizeof(TListFileCache));
        pCache->dwFileSize = SFileGetFileSize(hListFile, NULL);
        pCache->hFile = hListFile;

        // Fill the cache
        SFileReadFile(hListFile, pCache->Buffer, CACHE_BUFFER_SIZE, &dwBytesRead, NULL);
        if(dwBytesRead == 0)
            nError = GetLastError();
    }

    // Initialize the pointers
    if(nError == ERROR_SUCCESS)
    {
        pCache->pBegin =
        pCache->pPos = &pCache->Buffer[0];
        pCache->pEnd = pCache->pBegin + dwBytesRead;
    }
    else
    {
        SListFileFindClose((HANDLE)pCache);
        SetLastError(nError);
        pCache = NULL;
    }

    // Return the cache
    return pCache;
}

// Reloads the cache. Returns number of characters
// that has been loaded into the cache.
static DWORD ReloadListFileCache(TListFileCache * pCache)
{
    DWORD dwBytesToRead;
    DWORD dwBytesRead = 0;

    // Only do something if the cache is empty
    if(pCache->pPos >= pCache->pEnd)
    {
        __TryReadBlock:

        // Move the file position forward
        pCache->dwFilePos += CACHE_BUFFER_SIZE;
        if(pCache->dwFilePos >= pCache->dwFileSize)
            return 0;

        // Get the number of bytes remaining
        dwBytesToRead = pCache->dwFileSize - pCache->dwFilePos;
        if(dwBytesToRead > CACHE_BUFFER_SIZE)
            dwBytesToRead = CACHE_BUFFER_SIZE;

        // Load the next data chunk to the cache
        SFileSetFilePointer(pCache->hFile, pCache->dwFilePos, NULL, FILE_BEGIN);
        SFileReadFile(pCache->hFile, pCache->Buffer, CACHE_BUFFER_SIZE, &dwBytesRead, NULL);

        // If we didn't read anything, it might mean that the block
        // of the file is not available (in case of partial MPQs).
        // We sill skip it and try to read next block, until we reach
        // the end of the file.
        if(dwBytesRead == 0)
            goto __TryReadBlock;

        // Set the buffer pointers
        pCache->pBegin =
        pCache->pPos = &pCache->Buffer[0];
        pCache->pEnd = pCache->pBegin + dwBytesRead;
    }

    return dwBytesRead;
}

static size_t ReadListFileLine(TListFileCache * pCache, char * szLine, int nMaxChars)
{
    char * szLineBegin = szLine;
    char * szLineEnd = szLine + nMaxChars - 1;
    char * szExtraString = NULL;
    
    // Skip newlines, spaces, tabs and another non-printable stuff
    for(;;)
    {
        // If we need to reload the cache, do it
        if(pCache->pPos == pCache->pEnd)
        {
            if(ReloadListFileCache(pCache) == 0)
                break;
        }

        // If we found a non-whitespace character, stop
        if(*pCache->pPos > 0x20)
            break;

        // Skip the character
        pCache->pPos++;
    }

    // Copy the remaining characters
    while(szLine < szLineEnd)
    {
        // If we need to reload the cache, do it now and resume copying
        if(pCache->pPos == pCache->pEnd)
        {
            if(ReloadListFileCache(pCache) == 0)
                break;
        }

        // If we have found a newline, stop loading
        if(*pCache->pPos == 0x0D || *pCache->pPos == 0x0A)
            break;

        // Blizzard listfiles can also contain information about patch:
        // Pass1\Files\MacOS\unconditional\user\Background Downloader.app\Contents\Info.plist~Patch(Data#frFR#base-frFR,1326)
        if(*pCache->pPos == '~')
            szExtraString = szLine;

        // Copy the character
        *szLine++ = *pCache->pPos++;
    }

    // Terminate line with zero
    *szLine = 0;

    // If there was extra string after the file name, clear it
    if(szExtraString != NULL)
    {
        if(szExtraString[0] == '~' && szExtraString[1] == 'P')
        {
            szLine = szExtraString;
            *szExtraString = 0;
        }
    }

    // Return the length of the line
    return (szLine - szLineBegin);
}

static int CompareFileNodes(const void * p1, const void * p2) 
{
    char * szFileName1 = *(char **)p1;
    char * szFileName2 = *(char **)p2;

    return _stricmp(szFileName1, szFileName2);
}

static int WriteListFileLine(
    TMPQFile * hf,
    const char * szLine)
{
    char szNewLine[2] = {0x0D, 0x0A};
    size_t nLength = strlen(szLine);
    int nError;

    nError = SFileAddFile_Write(hf, szLine, (DWORD)nLength, MPQ_COMPRESSION_ZLIB);
    if(nError != ERROR_SUCCESS)
        return nError;

    return SFileAddFile_Write(hf, szNewLine, sizeof(szNewLine), MPQ_COMPRESSION_ZLIB);
}

//-----------------------------------------------------------------------------
// Local functions (listfile nodes)

// Adds a name into the list of all names. For each locale in the MPQ,
// one entry will be created
// If the file name is already there, does nothing.
int SListFileCreateNodeForAllLocales(TMPQArchive * ha, const char * szFileName)
{
    TMPQHeader * pHeader = ha->pHeader;
    TFileEntry * pFileEntry;
    TMPQHash * pFirstHash;
    TMPQHash * pHash;

    // Look for the first hash table entry for the file
    pFirstHash = pHash = GetFirstHashEntry(ha, szFileName);

    // Go while we found something
    while(pHash != NULL)
    {
        // Is it a valid file table index ?
        if(pHash->dwBlockIndex < pHeader->dwBlockTableSize)
        {
            // If the file name is already there, don't bother.
            pFileEntry = ha->pFileTable + pHash->dwBlockIndex;
            if(pFileEntry->szFileName == NULL)
            {
                pFileEntry->szFileName = ALLOCMEM(char, strlen(szFileName) + 1);
                if(pFileEntry->szFileName != NULL)
                {
                    strcpy(pFileEntry->szFileName, szFileName);
                }
            }
        }

        // Now find the next language version of the file
        pHash = GetNextHashEntry(ha, pFirstHash, pHash);
    }

    return ERROR_SUCCESS;
}

//-----------------------------------------------------------------------------
// File functions

// Adds a listfile into the MPQ archive.
// Note that the function does not remove the 
int WINAPI SFileAddListFile(HANDLE hMpq, const char * szListFile)
{
    TListFileCache * pCache = NULL;
    TMPQArchive * ha = (TMPQArchive *)hMpq;
    char  szFileName[MAX_PATH];
    size_t nLength = 0;
    int nError = ERROR_SUCCESS;

    // Add the listfile for each MPQ in the patch chain
    while(ha != NULL)
    {
        // Load the listfile to cache
        pCache = CreateListFileCache(hMpq, szListFile);
        if(pCache == NULL)
        {
            nError = GetLastError();
            break;
        }

        // Load the node list. Add the node for every locale in the archive
        while((nLength = ReadListFileLine(pCache, szFileName, sizeof(szFileName))) > 0)
            SListFileCreateNodeForAllLocales(ha, szFileName);

        // Also, add three special files to the listfile:
        // (listfile) itself, (attributes) and (signature)
        SListFileCreateNodeForAllLocales(ha, LISTFILE_NAME);
        SListFileCreateNodeForAllLocales(ha, SIGNATURE_NAME);
        SListFileCreateNodeForAllLocales(ha, ATTRIBUTES_NAME);

        // Delete the cache
        SListFileFindClose((HANDLE)pCache);

        // Move to the next archive in the chain
        ha = ha->haPatch;
    }

    return nError;
}

bool WINAPI SListFileFindClose(HANDLE hFind)
{
    TListFileCache * pCache = (TListFileCache *)hFind;

    if(pCache != NULL)
    {
        if(pCache->hFile != NULL)
            SFileCloseFile(pCache->hFile);
        if(pCache->szMask != NULL)
            FREEMEM(pCache->szMask);

        FREEMEM(pCache);
        return true;
    }

    return false;
}

