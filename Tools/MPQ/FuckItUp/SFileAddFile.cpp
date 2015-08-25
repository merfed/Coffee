/*****************************************************************************/
/* SFileAddFile.cpp                       Copyright (c) Ladislav Zezula 2010 */
/*---------------------------------------------------------------------------*/
/* MPQ Editing functions                                                     */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 27.03.10  1.00  Lad  Splitted from SFileCreateArchiveEx.cpp               */
/*****************************************************************************/

#define __STORMLIB_SELF__
#define __INCLUDE_COMPRESSION__
#define __INCLUDE_CRYPTOGRAPHY__
#include "StormLib.h"
#include "StormCommon.h"

//-----------------------------------------------------------------------------
// Local variables

// Data compression for SFileAddFile
// Kept here for compatibility with code that was created with StormLib version < 6.50
static DWORD DefaultDataCompression = MPQ_COMPRESSION_PKWARE;

static SFILE_ADDFILE_CALLBACK AddFileCB = NULL;
static void * pvUserData = NULL;

//-----------------------------------------------------------------------------
// MPQ write data functions

#define LOSSY_COMPRESSION_MASK (MPQ_COMPRESSION_ADPCM_MONO | MPQ_COMPRESSION_ADPCM_STEREO | MPQ_COMPRESSION_HUFFMANN)

static int WriteDataToMpqFile(
    TMPQArchive * ha,
    TMPQFile * hf,
    LPBYTE pbFileData,
    DWORD dwDataSize,
    DWORD dwCompression)
{
    TFileEntry * pFileEntry = hf->pFileEntry;
    ULONGLONG ByteOffset;
    LPBYTE pbCompressed = NULL;         // Compressed (target) data
    LPBYTE pbToWrite = NULL;            // Data to write to the file
    int nCompressionLevel = -1;         // ADPCM compression level (only used for wave files)
    int nError = ERROR_SUCCESS;

    // If the caller wants ADPCM compression, we will set wave compression level to 4,
    // which corresponds to medium quality
    if(dwCompression & LOSSY_COMPRESSION_MASK)
        nCompressionLevel = 4;

    // Make sure that the caller won't overrun the previously initiated file size
    assert(hf->dwFilePos + dwDataSize <= pFileEntry->dwFileSize);
    assert(hf->dwSectorCount != 0);
    assert(hf->pbFileSector != NULL);
    if((hf->dwFilePos + dwDataSize) > pFileEntry->dwFileSize)
        return ERROR_DISK_FULL;
    pbToWrite = hf->pbFileSector;

    // Now write all data to the file sector buffer
    if(nError == ERROR_SUCCESS)
    {
        DWORD dwBytesInSector = hf->dwFilePos % hf->dwSectorSize;
        DWORD dwSectorIndex = hf->dwFilePos / hf->dwSectorSize;
        DWORD dwBytesToCopy;

        // Process all data. 
        while(dwDataSize != 0)
        {
            dwBytesToCopy = dwDataSize;
                
            // Check for sector overflow
            if(dwBytesToCopy > (hf->dwSectorSize - dwBytesInSector))
                dwBytesToCopy = (hf->dwSectorSize - dwBytesInSector);

            // Copy the data to the file sector
            memcpy(hf->pbFileSector + dwBytesInSector, pbFileData, dwBytesToCopy);
            dwBytesInSector += dwBytesToCopy;
            pbFileData += dwBytesToCopy;
            dwDataSize -= dwBytesToCopy;

            // Update the file position
            hf->dwFilePos += dwBytesToCopy;

            // If the current sector is full, or if the file is already full,
            // then write the data to the MPQ
            if(dwBytesInSector >= hf->dwSectorSize || hf->dwFilePos >= pFileEntry->dwFileSize)
            {
                // Set the position in the file
                ByteOffset = hf->RawFilePos + pFileEntry->dwCmpSize;

                // If the file is compressed, allocate buffer for the compressed data.
                // Note that we allocate buffer that is a bit longer than sector size,
                // for case if the compression method performs a buffer overrun
                if((pFileEntry->dwFlags & MPQ_FILE_COMPRESSED) && pbCompressed == NULL)
                {
                    pbToWrite = pbCompressed = ALLOCMEM(BYTE, hf->dwSectorSize + 0x100);
                    if(pbCompressed == NULL)
                        nError = ERROR_NOT_ENOUGH_MEMORY;
                }

                // Update CRC32 and MD5 of the file
                md5_process((hash_state *)hf->hctx, hf->pbFileSector, dwBytesInSector);
                hf->dwCrc32 = crc32(hf->dwCrc32, hf->pbFileSector, dwBytesInSector);

                // Compress the file sector, if needed
                if(pFileEntry->dwFlags & MPQ_FILE_COMPRESSED)
                {
                    int nOutBuffer = (int)dwBytesInSector;
                    int nInBuffer = (int)dwBytesInSector;

                    assert(pbCompressed != NULL);

                    //
                    // Note that both SCompImplode and SCompCompress give original buffer,
                    // if they are unable to comperss the data.
                    //

                    if(pFileEntry->dwFlags & MPQ_FILE_IMPLODE)
                    {
                        SCompImplode((char *)pbCompressed,
                                            &nOutBuffer,
                                     (char *)hf->pbFileSector,
                                             nInBuffer);
                    }

                    if(pFileEntry->dwFlags & MPQ_FILE_COMPRESS)
                    {
                        SCompCompress((char *)pbCompressed,
                                             &nOutBuffer,
                                      (char *)hf->pbFileSector,
                                              nInBuffer,
                                    (unsigned)dwCompression,
                                              0,
                                              nCompressionLevel);
                    }

                    // Update sector positions
                    dwBytesInSector = nOutBuffer;
                    if(hf->SectorOffsets != NULL)
                        hf->SectorOffsets[dwSectorIndex+1] = hf->SectorOffsets[dwSectorIndex] + dwBytesInSector;

                    // We have to calculate sector CRC, if enabled
                    if(hf->SectorChksums != NULL)
                        hf->SectorChksums[dwSectorIndex] = adler32(0, pbCompressed, nOutBuffer);
                }                 

                // Encrypt the sector, if necessary
                if(pFileEntry->dwFlags & MPQ_FILE_ENCRYPTED)
                {
                    BSWAP_ARRAY32_UNSIGNED(pbToWrite, dwBytesInSector);
                    EncryptMpqBlock(pbToWrite, dwBytesInSector, hf->dwFileKey + dwSectorIndex);
                    BSWAP_ARRAY32_UNSIGNED(pbToWrite, dwBytesInSector);
                }

                // Write the file sector
                if(!FileStream_Write(ha->pStream, &ByteOffset, pbToWrite, dwBytesInSector))
                {
                    nError = GetLastError();
                    break;
                }

                // Call the compact callback, if any
                if(AddFileCB != NULL)
                    AddFileCB(pvUserData, hf->dwFilePos, hf->dwDataSize, false);

                // Update the compressed file size
                pFileEntry->dwCmpSize += dwBytesInSector;
                dwBytesInSector = 0;
                dwSectorIndex++;
            }
        }
    }

    // Cleanup
    if(pbCompressed != NULL)
        FREEMEM(pbCompressed);
    return nError;
}

//-----------------------------------------------------------------------------
// Recrypts file data for file renaming

static int RecryptFileData(
    TMPQArchive * ha,
    TMPQFile * hf,
    const char * szFileName,
    const char * szNewFileName)
{
    ULONGLONG RawFilePos;
    TFileEntry * pFileEntry = hf->pFileEntry;
    DWORD dwBytesToRecrypt = pFileEntry->dwCmpSize;
    DWORD dwOldKey;
    DWORD dwNewKey;
    int nError = ERROR_SUCCESS;

    // The file must be encrypted
    assert(pFileEntry->dwFlags & MPQ_FILE_ENCRYPTED);

    // File decryption key is calculated from the plain name
    szNewFileName = GetPlainFileName(szNewFileName);
    szFileName = GetPlainFileName(szFileName);

    // Calculate both file keys
    dwOldKey = DecryptFileKey(szFileName,    pFileEntry->ByteOffset, pFileEntry->dwFileSize, pFileEntry->dwFlags);
    dwNewKey = DecryptFileKey(szNewFileName, pFileEntry->ByteOffset, pFileEntry->dwFileSize, pFileEntry->dwFlags);

    // Incase the keys are equal, don't recrypt the file
    if(dwNewKey == dwOldKey)
        return ERROR_SUCCESS;
    hf->dwFileKey = dwOldKey;

    // Calculate the raw position of the file in the archive
    hf->MpqFilePos = pFileEntry->ByteOffset;
    hf->RawFilePos = ha->MpqPos + hf->MpqFilePos;

    // Allocate buffer for file transfer
    nError = AllocateSectorBuffer(hf);
    if(nError != ERROR_SUCCESS)
        return nError;

    // Also allocate buffer for sector offsets
    // Note: Don't load sector checksums, we don't need to recrypt them
    nError = AllocateSectorOffsets(hf, true);
    if(nError != ERROR_SUCCESS)
        return nError;

    // If we have sector offsets, recrypt these as well
    if(hf->SectorOffsets != NULL)
    {
        // Allocate secondary buffer for sectors copy
        LPDWORD SectorOffsetsCopy = ALLOCMEM(DWORD, hf->dwSectorCount);
        DWORD dwArraySize = hf->dwSectorCount * sizeof(DWORD);

        if(SectorOffsetsCopy == NULL)
            return ERROR_NOT_ENOUGH_MEMORY;

        // Recrypt the array of sector offsets
        memcpy(SectorOffsetsCopy, hf->SectorOffsets, dwArraySize);
        EncryptMpqBlock(SectorOffsetsCopy, dwArraySize, dwNewKey - 1);
        BSWAP_ARRAY32_UNSIGNED(SectorOffsetsCopy, dwArraySize);

        // Write the recrypted array back
        if(!FileStream_Write(ha->pStream, &hf->RawFilePos, SectorOffsetsCopy, dwArraySize))
            nError = GetLastError();
        FREEMEM(SectorOffsetsCopy);
    }

    // Now we have to recrypt all file sectors. We do it without
    // recompression, because recompression is not necessary in this case
    if(nError == ERROR_SUCCESS)
    {
        for(DWORD dwSector = 0; dwSector < hf->dwDataSectors; dwSector++)
        {
            DWORD dwRawDataInSector = hf->dwSectorSize;
            DWORD dwRawByteOffset = dwSector * hf->dwSectorSize;

            // Last sector: If there is not enough bytes remaining in the file, cut the raw size
            if(dwRawDataInSector > dwBytesToRecrypt)
                dwRawDataInSector = dwBytesToRecrypt;

            // Fix the raw data length if the file is compressed
            if(hf->SectorOffsets != NULL)
            {
                dwRawDataInSector = hf->SectorOffsets[dwSector+1] - hf->SectorOffsets[dwSector];
                dwRawByteOffset = hf->SectorOffsets[dwSector];
            }

            // Calculate the raw file offset of the file sector
            CalculateRawSectorOffset(RawFilePos, hf, dwRawByteOffset);

            // Read the file sector
            if(!FileStream_Read(ha->pStream, &RawFilePos, hf->pbFileSector, dwRawDataInSector))
            {
                nError = GetLastError();
                break;
            }

            // If necessary, re-encrypt the sector
            // Note: Recompression is not necessary here. Unlike encryption, 
            // the compression does not depend on the position of the file in MPQ.
            BSWAP_ARRAY32_UNSIGNED(hf->pbFileSector, dwRawDataInSector);
            DecryptMpqBlock(hf->pbFileSector, dwRawDataInSector, dwOldKey + dwSector);
            EncryptMpqBlock(hf->pbFileSector, dwRawDataInSector, dwNewKey + dwSector);
            BSWAP_ARRAY32_UNSIGNED(hf->pbFileSector, dwRawDataInSector);

            // Write the sector back
            if(!FileStream_Write(ha->pStream, &RawFilePos, hf->pbFileSector, dwRawDataInSector))
            {
                nError = GetLastError();
                break;
            }

            // Decrement number of bytes remaining
            dwBytesToRecrypt -= hf->dwSectorSize;
        }
    }

    return nError;
}

//-----------------------------------------------------------------------------
// Support functions for adding files to the MPQ

int SFileAddFile_Init(
    TMPQArchive * ha,
    const char * szFileName,
    ULONGLONG FileTime,
    DWORD dwFileSize,
    LCID lcLocale,
    DWORD dwFlags,
    TMPQFile ** phf)
{
    TFileEntry * pFileEntry = NULL;
    ULONGLONG TempPos;                  // For various file offset calculations
    TMPQFile * hf = NULL;               // File structure for newly added file
    int nError = ERROR_SUCCESS;

    //
    // Note: This is an internal function so no validity checks are done.
    // It is the caller's responsibility to make sure that no invalid
    // flags get to this point
    //

    // Adjust file flags for too-small files
    if(dwFileSize < 0x04)
        dwFlags &= ~(MPQ_FILE_ENCRYPTED | MPQ_FILE_FIX_KEY);
    if(dwFileSize < 0x20)
        dwFlags &= ~(MPQ_FILE_COMPRESSED | MPQ_FILE_SECTOR_CRC);

    // Allocate the TMPQFile entry for newly added file
    hf = CreateMpqFile(ha);
    if(hf == NULL)
        nError = ERROR_NOT_ENOUGH_MEMORY;

    // If the MPQ header has not yet been written, do it now
    if(nError == ERROR_SUCCESS && (ha->dwFlags & MPQ_FLAG_NO_HEADER))
    {
        // Remember the header size before swapping
        DWORD dwBytesToWrite = ha->pHeader->dwHeaderSize;

        BSWAP_TMPQHEADER(ha->pHeader);
        if(FileStream_Write(ha->pStream, &ha->MpqPos, ha->pHeader, dwBytesToWrite))
            ha->dwFlags &= ~MPQ_FLAG_NO_HEADER;
        else
            nError = GetLastError();
        BSWAP_TMPQHEADER(ha->pHeader);
    }

    // Find a free space in the MPQ, as well as free block table entry
    if(nError == ERROR_SUCCESS)
    {
        // Find the position where the file will be stored
        FindFreeMpqSpace(ha, &hf->MpqFilePos);
        hf->RawFilePos = ha->MpqPos + hf->MpqFilePos;
        hf->bIsWriteHandle = true;

        // When format V1, the size of the archive cannot exceed 4 GB
        if(ha->pHeader->wFormatVersion == MPQ_FORMAT_VERSION_1)
        {
            TempPos  = hf->MpqFilePos + dwFileSize;
            TempPos += ha->pHeader->dwHashTableSize * sizeof(TMPQHash);
            TempPos += ha->pHeader->dwBlockTableSize * sizeof(TMPQBlock);
            TempPos += ha->pHeader->dwBlockTableSize * sizeof(USHORT);
            if((TempPos >> 32) != 0)
                nError = ERROR_DISK_FULL;
        }
    }

    // Allocate file entry in the MPQ
    if(nError == ERROR_SUCCESS)
    {
        // Check if the file already exists in the archive
        pFileEntry = GetFileEntryExact(ha, szFileName, lcLocale);
        if(pFileEntry == NULL)
        {
            pFileEntry = AllocateFileEntry(ha, szFileName, lcLocale);
            if(pFileEntry == NULL)
                nError = ERROR_DISK_FULL;
        }
        else
        {
            if((dwFlags & MPQ_FILE_REPLACEEXISTING) == 0)
                nError = ERROR_ALREADY_EXISTS;
        }
    }

    // Allocate space for the file name
    if(nError == ERROR_SUCCESS && pFileEntry->szFileName == NULL)
    {
        pFileEntry->szFileName = ALLOCMEM(char, strlen(szFileName) + 1);
        if(pFileEntry != NULL)
            strcpy(pFileEntry->szFileName, szFileName);
        else
            nError = ERROR_NOT_ENOUGH_MEMORY;
    }

    // Create key for file encryption
    if(nError == ERROR_SUCCESS && (dwFlags & MPQ_FILE_ENCRYPTED))
    {
        hf->dwFileKey = DecryptFileKey(szFileName, hf->MpqFilePos, dwFileSize, dwFlags);
    }

    if(nError == ERROR_SUCCESS)
    {
        // Initialize the hash entry for the file
        hf->pFileEntry = pFileEntry;
        hf->dwDataSize = dwFileSize;

        // Initialize the block table entry for the file
        pFileEntry->ByteOffset = hf->MpqFilePos;
        pFileEntry->dwFileSize = dwFileSize;
        pFileEntry->dwCmpSize = 0;
        pFileEntry->dwFlags  = dwFlags | MPQ_FILE_EXISTS;
        pFileEntry->lcLocale = (USHORT)lcLocale;

        // Initialize the file time, CRC32 and MD5
        assert(sizeof(hf->hctx) >= sizeof(hash_state));
        memset(pFileEntry->md5, 0, MD5_DIGEST_SIZE);
        md5_init((hash_state *)hf->hctx);
        pFileEntry->dwCrc32 = crc32(0, Z_NULL, 0);

        // If the caller gave us a file time, use it.
        pFileEntry->FileTime = FileTime;

        // Call the callback, if needed
        if(AddFileCB != NULL)
            AddFileCB(pvUserData, 0, hf->dwDataSize, false);
    }

    // If an error occured, remember it
    if(nError != ERROR_SUCCESS)
        hf->bErrorOccured = true;
    *phf = hf;
    return nError;
}

int SFileAddFile_Write(TMPQFile * hf, const void * pvData, DWORD dwSize, DWORD dwCompression)
{
    TMPQArchive * ha;
    TFileEntry * pFileEntry;
    DWORD dwSectorPosLen = 0;
    int nError = ERROR_SUCCESS;

    // Don't bother if the caller gave us zero size
    if(pvData == NULL || dwSize == 0)
        return ERROR_SUCCESS;

    // Get pointer to the MPQ archive
    pFileEntry = hf->pFileEntry;
    ha = hf->ha;

    // Allocate file buffers
    if(hf->pbFileSector == NULL)
    {
        ULONGLONG RawFilePos = hf->RawFilePos;

        // Allocate buffer for file sector
        nError = AllocateSectorBuffer(hf);
        if(nError != ERROR_SUCCESS)
        {
            hf->bErrorOccured = true;
            return nError;
        }

        // Allocate sector offsets
        if(hf->SectorOffsets == NULL)
        {
            nError = AllocateSectorOffsets(hf, false);
            if(nError != ERROR_SUCCESS)
            {
                hf->bErrorOccured = true;
                return nError;
            }
        }

        // Create array of sector checksums
        if(hf->SectorChksums == NULL && (pFileEntry->dwFlags & MPQ_FILE_SECTOR_CRC))
        {
            nError = AllocateSectorChecksums(hf, false);
            if(nError != ERROR_SUCCESS)
            {
                hf->bErrorOccured = true;
                return nError;
            }
        }

        // Pre-save the patch info, if any
        if(hf->PatchInfo != NULL)
        {
            if(!FileStream_Write(ha->pStream, &RawFilePos, hf->PatchInfo, hf->PatchInfo->dwLength))
                nError = GetLastError();
  
            pFileEntry->dwCmpSize += hf->PatchInfo->dwLength;
            RawFilePos += hf->PatchInfo->dwLength;
        }

        // Pre-save the sector offset table, just to reserve space in the file.
        // Note that we dont need to swap the sector positions, nor encrypt the table
        // at the moment, as it will be written again after writing all file sectors.
        if(hf->SectorOffsets != NULL)
        {
            dwSectorPosLen = hf->dwSectorCount * sizeof(DWORD);
            if(!FileStream_Write(ha->pStream, &RawFilePos, hf->SectorOffsets, dwSectorPosLen))
                nError = GetLastError();

            pFileEntry->dwCmpSize += dwSectorPosLen;
            RawFilePos += dwSectorPosLen;
        }
    }

    // Write the MPQ data to the file
    if(nError == ERROR_SUCCESS)
        nError = WriteDataToMpqFile(ha, hf, (LPBYTE)pvData, dwSize, dwCompression);

    // If it succeeded and we wrote all the file data,
    // we need to re-save sector offset table
    if(nError == ERROR_SUCCESS)
    {
        if(hf->dwFilePos >= pFileEntry->dwFileSize)
        {
            // Finish calculating CRC32
            hf->pFileEntry->dwCrc32 = hf->dwCrc32;

            // Finish calculating MD5
            md5_done((hash_state *)hf->hctx, hf->pFileEntry->md5);

            // If we also have sector checksums, write them to the file
            if(hf->SectorChksums != NULL)
            {
                nError = WriteSectorChecksums(hf);
                if(nError != ERROR_SUCCESS)
                    hf->bErrorOccured = true;
            }

            // Now write sector offsets to the file
            if(hf->PatchInfo != NULL)
            {
                memcpy(hf->PatchInfo->md5, hf->pFileEntry->md5, MD5_DIGEST_SIZE);
                hf->PatchInfo->dwDataSize  = hf->pFileEntry->dwFileSize;
                hf->pFileEntry->dwFileSize = hf->dwPatchedFileSize;
                nError = WritePatchInfo(hf);
                if(nError != ERROR_SUCCESS)
                    hf->bErrorOccured = true;
            }

            // Now write sector offsets to the file
            if(hf->SectorOffsets != NULL)
            {
                nError = WriteSectorOffsets(hf);
                if(nError != ERROR_SUCCESS)
                    hf->bErrorOccured = true;
            }
        }
    }
    else
    {
        hf->bErrorOccured = true;
    }

    return nError;
}

int SFileAddFile_Finish(TMPQFile * hf)
{
    TMPQArchive * ha = hf->ha;
    TFileEntry * pFileEntry = hf->pFileEntry;
    int nError = ERROR_SUCCESS;

    // If all previous operations succeeded, we can update the MPQ
    if(!hf->bErrorOccured)
    {
        // Verify if the caller wrote the file properly
        if(hf->PatchInfo == NULL)
        {
            assert(pFileEntry != NULL);
            if(hf->dwFilePos != pFileEntry->dwFileSize)
            {
                nError = ERROR_CAN_NOT_COMPLETE;
                hf->bErrorOccured = true;
            }
        }
        else
        {
            if(hf->dwFilePos != hf->PatchInfo->dwDataSize)
            {
                nError = ERROR_CAN_NOT_COMPLETE;
                hf->bErrorOccured = true;
            }
        }
    }

    if(!hf->bErrorOccured)
    {
        // Call the user callback, if any
        if(AddFileCB != NULL)
            AddFileCB(pvUserData, hf->dwDataSize, hf->dwDataSize, true);

        // Update the size of the block table
        ha->pHeader->dwBlockTableSize = ha->dwFileTableSize;
    }
    else
    {
        // Free the file entry in MPQ tables
        if(pFileEntry != NULL)
            FreeFileEntry(ha, pFileEntry);
    }

    // Schedule to saving MPQ tables regardless of success or error
    ha->dwFlags |= MPQ_FLAG_CHANGED;

    // Clear the add file callback
    FreeMPQFile(hf);
    pvUserData = NULL;
    AddFileCB = NULL;
    return nError;
}

//-----------------------------------------------------------------------------
// bool SFileRemoveFile(HANDLE hMpq, char * szFileName)
//
// This function removes a file from the archive. The file content
// remains there, only the entries in the hash table and in the block
// table are updated. 

bool WINAPI SFileRemoveFile(HANDLE hMpq, const char * szFileName, DWORD dwSearchScope)
{
    TMPQArchive * ha = (TMPQArchive *)hMpq;
    TFileEntry * pFileEntry = NULL; // File entry of the file to be deleted
    int nError = ERROR_SUCCESS;

    // Check the parameters
    if(nError == ERROR_SUCCESS)
    {
        if(!IsValidMpqHandle(ha))
            nError = ERROR_INVALID_HANDLE;
        if(dwSearchScope != SFILE_OPEN_BY_INDEX && *szFileName == 0)
            nError = ERROR_INVALID_PARAMETER;
    }

    if(nError == ERROR_SUCCESS)
    {
        // Do not allow to remove files from MPQ open for read only
        if(ha->dwFlags & MPQ_FLAG_READ_ONLY)
            nError = ERROR_ACCESS_DENIED;
    }

    // Get hash entry belonging to this file
    if(nError == ERROR_SUCCESS)
    {
        if(dwSearchScope == SFILE_OPEN_FROM_MPQ)
        {
            if((pFileEntry = GetFileEntryExact(ha, (char *)szFileName, lcFileLocale)) == NULL)
                nError = ERROR_FILE_NOT_FOUND;
        }
        else
        {
            if((pFileEntry = GetFileEntryByIndex(ha, (DWORD)(DWORD_PTR)szFileName)) == NULL)
                nError = ERROR_FILE_NOT_FOUND;
        }
    }

    // Test if the file is not already deleted
    if(nError == ERROR_SUCCESS)
    {
        if((pFileEntry->dwFlags & MPQ_FILE_EXISTS) == 0)
            nError = ERROR_FILE_NOT_FOUND;
    }

    if(nError == ERROR_SUCCESS)
    {
        // Mark the file entry as free
        FreeFileEntry(ha, pFileEntry);

        // Update MPQ archive
        ha->dwFlags |= MPQ_FLAG_CHANGED;
    }

    // Resolve error and exit
    if(nError != ERROR_SUCCESS)
        SetLastError(nError);
    return (nError == ERROR_SUCCESS);
}