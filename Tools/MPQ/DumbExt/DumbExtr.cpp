/*****************************************************************************/
/* DumbExtr.cpp                      Copyright Justin Olbrantz(Quantam) 2000 */
/*                                                                           */
/* DumbExtractor                                                             */
/*                                                                           */
/* Author : Justin Olbrantz(Quantam)                                         */
/* E-mail : omega@dragonfire.net                                             */
/* WWW    : www.campaigncreations.com/starcraft/mpq2k/inside_mopaq/          */
/*---------------------------------------------------------------------------*/
/*                Sample program for chapter 3 of Inside MoPaQ               */
/*****************************************************************************/

#include <iostream.h>
#include <conio.h>

//Prototypes for the Storm functions we will be using
#include "storm.h"

int main()
{
	const LCID lcDefaultLocale = 0x409;	//My language is English, 0x409
	const DWORD dwBufferSize = 0x400000;	//Read buffer size of 4 MB
	BOOL bDeleteFile = FALSE;	//If this is true, we'll delete the output file
	HANDLE hMPQ, hFile, hOutFile;	//Handles for the MPQ, file, and HD file
	DWORD dwFileSize, dwFileLeft, dwBytesToRead, dwBytesRead, dwBytesWritten, i;	//Misc counters
	PVOID pvReadBuffer;	//Read buffer
	char szArchiveName[MAX_PATH + 1] = "patch_rt.mpq", szFileName[MAX_PATH + 1] = "arr\\units.dat";

	//Display program fanfare
	cout << "DumbExtractor, (c) 2000 Justin Olbrantz(Quantam)\r\n"
		"This simple console application demonstrates how to extract a file from an\r\n"
		"MPQ archive using the Storm.dll API.\r\n\r\n";

	//Allocate the read buffer
	pvReadBuffer = new BYTE[dwBufferSize];
	if (pvReadBuffer)
	{
		//Get the archive and file names
		cin.width(MAX_PATH);
		cout << "Enter name of MPQ archive to extract file from: ";
		do {
			cin >> szArchiveName;
		} while (!strlen(szArchiveName));

		cout << "\r\nEnter name of file to extract from MPQ: ";
		do {
			cin >> szFileName;
		} while (!strlen(szFileName));

		//We got the archive and file names.
		//Now we just have to use Storm

		//Set the language so we'll get the right file
		SFileSetLocale(lcDefaultLocale);

		cout << "\r\nSetting language code to English...\r\n";

		//First we open the archive
		if (SFileOpenArchive(szArchiveName, 0, 0, &hMPQ))
		{
			cout << "Opening archive " << szArchiveName << "...\r\n";

			//Got the archive open, now we need to open the file
			if (SFileOpenFileEx(hMPQ, szFileName, 0, &hFile))
			{
				cout << "Opening file " << szFileName << " in archive...\r\n";

				//Make a file name for the output file on HD
				char szOutFileName[MAX_PATH + 1];
				strncpy(szOutFileName, szFileName, MAX_PATH);

				//Convert backslashes in the file name to underscores
				for (i = 0; i < strlen(szOutFileName); i++)
				{
					if (szOutFileName[i] == '\\')
						szOutFileName[i] = '_';
				}

				//Got the name, now we open the file
				hOutFile = CreateFile(szOutFileName, GENERIC_WRITE, 
					NULL, NULL, CREATE_ALWAYS, NULL, NULL);
				if (hOutFile != INVALID_HANDLE_VALUE)
				{
					cout << "Opening output file " << szOutFileName << "...\r\n";

					//Now we need the file size...
					dwFileLeft = dwFileSize = SFileGetFileSize(hFile, NULL);
					if (dwFileSize != 0xffffffff)
					{
						cout << "File " << szFileName << " is " << dwFileSize << " bytes...\r\n";

						//Everything's ready. Now we extract the file
						while (dwFileLeft && !bDeleteFile)
						{
							//Get the size of the block to read
							dwBytesToRead = min(dwFileLeft, dwBufferSize);
							//Read the block...
							if (SFileReadFile(hFile, pvReadBuffer, dwBytesToRead, &dwBytesRead, NULL) && dwBytesRead)
							{
								//...and write it
								if (!WriteFile(hOutFile, pvReadBuffer, dwBytesRead, &dwBytesWritten, NULL))
								{
									cout << "Error: An error occurred while writing to file " << szOutFileName << "\r\n";

									bDeleteFile = TRUE;
								}
							}
							else
							{
								cout << "Error: An error occurred while reading from the archive\r\n";

								bDeleteFile = TRUE;
							}
							
							cout << "Reading/writing " << dwBytesRead << " bytes...\r\n";

							//Onto the next block
							dwFileLeft -= dwBytesRead;
						}
					}
					else
						cout << "Error: Cannot retrieve size of file " << szFileName << ". The archive is probably corrupted\r\n";
					
					//Done with the output file. Close it
					CloseHandle(hOutFile);

					cout << "Closing output file " << szOutFileName << "...\r\n";

					if (bDeleteFile)
					{
						//An error occured, so the output file's invalid; delete it
						DeleteFile(szOutFileName);

						cout << "Deleting output file " << szOutFileName << "...\r\n";
					}
				}
				else
					cout << "Error: Cannot open file " << szOutFileName << " on hard drive\r\n";

				//Done with the file; close it
				SFileCloseFile(hFile);

				cout << "Closing file " << szFileName << " in archive...\r\n";
			}
			else
			{
				cout << "Error: File " << szFileName 
					<< " does not exist in archive " << szArchiveName << "!\r\n";
			}

			//Done with the MPQ, so we'll close it
			SFileCloseArchive(hMPQ);

			cout << "Closing archive " << szArchiveName << "...\r\n";
		}
		else
			cout << "Error: Cannot open archive " << szArchiveName << "!\r\n";

		//We're done with the read buffer, so deallocate it
		delete [] pvReadBuffer;
	}
	else
		//We can't allocate the read buffer
		cout << "Fatal Error: Cannot allocate a read buffer of " << dwBufferSize << " bytes!\r\n";

	//Pause for those people who ran the program by double-clicking it in explorer
	cout << "\r\nPress any key to continue...\r\n";
	cout.flush();

	getch();

	return 0;
}
