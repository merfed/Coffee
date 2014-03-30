/*
BLPConverter.cpp - Converts BLP files to TGA and back.
See the Usage() function for info on using this application.

///////////////////////////////////////////////////////////////////////////////

	(C) 2008 Patrick Cyr (kobuster@yahoo.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

///////////////////////////////////////////////////////////////////////////////

This program requires quite a few things in order to compile correctly:

- Windows 2000 and up.  I'm unsure about the various libraries but at the very 
  least this file calls win32 functions to handle input wildcards.
- palbmp: A pallete library (C) Charles Bloom.  http://www.cbloom.co/m
	I'm opting to comply with section 0 of the Bloom Public License: I'm 
	distributing this program using the GPL.  I've also made some very small 
	changes to it: I commented out the stderr spam.
- crblib: Another library (C) Charles Bloom, which is a dependency of palbmp.
- libpng: The offical PNG reference library. http://www.libpng.org/
- squish: A DXT compression library.  http://www.sjbrown.co.uk/?code=squish

///////////////////////////////////////////////////////////////////////////////

Versions:
* 8.1 * 
- Added support for BLP_PAL_A4 format.

* 8 *
- Incorporated David Holland's (david.w.holland@gmail.com) changes no allow for Linux-compatible builds.
- Using squish instead of the ATIlib for DXT compression, as it is cross platform.

* 7 * 
- Textures that have 256 or fewer unique colors now use exactly those colors when palettized.
- Made -r option default.  -s turns it off.

* 6 *
- Support for decoding DXT5 BLPs.
- Improved the way mip level are created for images with alpha.
- Fixed bug which could cause palettized BLPs with alpha to not have their alpha converted correctly. (Occured when the first transparent pixel uses palette entry zero.)

* 5 *
- Fixed bug which was causing -g value to be applied twice when using -r and converting a RGBA PNG to a BLP.
- -g will now work when when converting a Palettized PNG that has no alpha.
- Added -h, which forces cHaracter format BLPs. (palettized, no alpha).

* 4 * 
- Added -e and -n options.
- -g option now works when doing a rgb png -> p8 blp.
- -g option will automatically create an alpha channel for RGB source PNGs.
- Removed palettization spam.
- -m option now supports wildcards, ie. *.blp.

* 3 *
- Removed dependency on DirectX.
- Added -m, -i, and -c options.

* 2 * 
- Fixed bug preventing files from being processed if there was a '.' in their path.
- Now setting device format to current display format, in hopes of fixing an error
that people are reporting when the program tries to call CreateDevice().

* 1 * 
- Initial release


///////////////////////////////////////////////////////////////////////////////

Personal Notes:

*** PNG -> BLP ***
- P8 -> uncompressed, 0 bit alpha
- P8 with transparency -> uncompressed, 8bit alpha  (Note: Don't know how to make PNGs of this type, except for transparency count = 1)
- RGB -> compressed, DXT1
- RGBA	-> compressed, DXT3
	*OR*
		-> P8 with transparency [Use this option to preserve BLP format ]

*** BLP -> PNG ***
- uncompressed, 0 bit alpha -> P8
	Ex. Character Skins.
- uncompressed, 1 bit alpha -> P8 with alpha palette (1 entry)
	Ex. Some clothing (Deckhand's shirt for example)
- uncompressed, 8 bit alpha -> P8 with alpha palette (1 entry) [This is what WowImage does: retains palette, destroys alpha detail.]
	*OR*
							-> RGBA [Destroys palette, retains alpha detail.]
	Ex. Most clothing.  Item\TextureComponents\TorsoUpperTexture\Cloth_Horde_C_01Purple_Chest_TU_U.png for example.

  - compressed, 0 bit alpha -> DXT1
	Ex. Sansam root
- compressed, 1 bit alpha -> DXT1
	Ex. peaceflower
- compressed, 8 bit alpha -> DXT3
	Ex. sungrass


uncomp 1-bit:
- Deckhand's shirt (Cloth_A_03Crewman_Chest_TU_F.blp).  WowImage doesn't give an error when trying to decode this blp
but doesn't create a png, either.

Notes:
09-12-24: Added support for BLP_PAL_A4.
08-02-10: Removed SQUISH_USE_SSE=2 predefine from squish project.

Test:			| Source		| -			| -s
______________________________________________________________
BLP_U_A0		| char skin		| OK/OK					HumanMaleSkin00_00.blp
BLP_U_A1		| some clothes	| OK/OK					?
BLP_U_A4		| some clothes	| OK/OK					Plate_RaidWarrior_H_01_Chest_TL_F.blp (completely opaque, though)
BLP_U_A8		| clothes		| OK/OK					Leather_A_05Yellow_Chest_TU_M.blp (human male war starting upper shirt)
BLP_C_A0		| earthroot		| OK					Sword_1H_Short_A_02Rusty.blp (human male war starting sword)
BLP_C_A1		| peacebloom	| OK					Hair01_05 (Human golden blonde hair)
BLP_DXT3		| 				| OK					NorthshireValley1.blp | sungrass
BLP_C_A8_7		| see end ->	|						Ability_Druid_Cyclone.blp
					
PNG_RGB			|				| BLP_U_A0	| BLP_C_A0 (DXT1)
PNG_RGBA		|				| BLP_U_A8	| BLP_C_A8 (DXT3)
PNG_P8			|				| BLP_U_A0	| -
PNG_P8_1trans	|				| BLP_U_A8


PNG_RGB			| ok, igok (pig)
PNG_RGBA			| ok, igok (horde armory sign)
PNG_RGBA	-> P A8	| ok, igok (fang top)
PNG_P8			| ok, igok (character skin)
PNG_P8_1trans	| ok, igok (fang top)
	
 */

#ifndef LINUX
#include <stdio.h>
#include <conio.h>
#else
#include "port.h"
#endif


///////////////////////////////////////////////////////////////////////////////
// Windows Stuff

#ifndef LINUX
#include <windows.h>
#include "png.h"
#else
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include <png.h>
#include <errno.h>
#include "palbmp/palcreate.h"
#include "palbmp/palettize.h"
#endif
#include "squish/squish.h"
#include "MemImage.h"
#include "BLP.h"

///////////////////////////////////////////////////////////////////////////////
// User options.
bool g_bInfoMode = false;
FORMATID g_targetFormatID = FORMAT_UNSPECIFIED;


///////////////////////////////////////////////////////////////////////////////
// Testing code.

const char* pngTestList[] = 
{
	"PNG_RGB",
	"PNG_RGBA",
	"PNG_P8",
	"PNG_P8_1Trans",
};

const char* blpTestList[] = 
{
	"BLP_U_A0",
	"BLP_U_A1",
	"BLP_U_A8",
	"BLP_DXT1",
	"BLP_DXT1_A1",
	"BLP_DXT3",
	"BLP_DXT5"
};

bool RunTest()
{
	MemImage aMemImage;

	/*
	// PNG->BLP test.
	for (int iFile = 0; iFile < 4; ++iFile)
	{
		char inputFilename[BLPCONV_MAX_PATH];
		::sprintf(inputFilename, "%s.png",  pngTestList[iFile]);
		if (!aMemImage.LoadFromPNG(inputFilename))
			return false;
		
		for (int iFormat = 1; iFormat < BLPTYPE_COUNT; ++iFormat)
		{
			char outputFilename[BLPCONV_MAX_PATH];
			::sprintf(outputFilename, "%s__%s.blp", pngTestList[iFile], FORMATIDNames[iFormat]);

			if (!aMemImage.SaveToBLP(outputFilename, BLPType(iFormat)))
				return false;
		}
	}

	// BLP->PNG test.
	for (int iFile = 0; iFile < 4; ++iFile)
	{
		char inputFilename[BLPCONV_MAX_PATH];
		::sprintf(inputFilename, "%s.png",  pngTestList[iFile]);
		if (!aMemImage.LoadFromPNG(inputFilename))
			return false;
		
		for (int iFormat = 1; iFormat < BLPTYPE_COUNT; ++iFormat)
		{
			char outputFilename[BLPCONV_MAX_PATH];
			::sprintf(outputFilename, "%s__%s.blp", pngTestList[iFile], FORMATIDNames[iFormat]);

			if (!aMemImage.SaveToBLP(outputFilename, BLPType(iFormat)))
				return false;
		}
	}
	*/

	return true;
}

int ProcessFile(const char* pszFilenameArgument, const char* pszDestinationFilename)
{
	char pszFilenameBuffer[MAX_PATH];
	strcpy(pszFilenameBuffer, pszFilenameArgument);

	char* pszPeriod = ::strrchr(pszFilenameBuffer, '.');
	if (NULL == pszPeriod)
	{
		printf("Invalid filename '%s' (no . in it).\n", pszFilenameArgument);
		return - 1;
	}
	pszPeriod[0] = 0;
	char* pszExtension = &pszPeriod[1];
	
	// Load the file.
	FileType inputFileType;
	FORMATID inputFormatID;
	MemImage aMemImage;
	if (0 == ::_stricmp(pszExtension, "blp"))
	{
		inputFileType = FILETYPE_BLP;
		if (!aMemImage.LoadFromBLP(pszFilenameArgument, &inputFormatID))
			return -1;
	}
	else if (0 == ::_stricmp(pszExtension, "png"))
	{
		inputFileType = FILETYPE_PNG;
		if (!aMemImage.LoadFromPNG(pszFilenameArgument, &inputFormatID))
			return -1;
	}
	else
	{
		printf("ERROR: Input file '%s' not a png or blp.\n", pszFilenameArgument);
		return -1;
	}

	if (g_bInfoMode)
		return 0;

	// Look up the target format.
	FORMATID targetFormatID = g_targetFormatID;
	if (FORMAT_UNSPECIFIED == targetFormatID)
		targetFormatID = MemImage::s_ruleTable[inputFormatID];

	// Generate the target filename.
	char pszTargetFilename[MAX_PATH];
	if (NULL == pszDestinationFilename)
	{
		bool bSameFiletype =	(FILETYPE_BLP == inputFileType && ISBLP(targetFormatID)) || 
								(FILETYPE_PNG == inputFileType && ISPNG(targetFormatID));

		::sprintf(pszTargetFilename, "%s%s.%s", 
				(char*) pszFilenameBuffer,
				bSameFiletype ? "_" : "",
				ISBLP(targetFormatID) ? "blp" : "png");
	}
	else
		::sprintf(pszTargetFilename, pszDestinationFilename);

	// Save the file with the given format.
	LOG("Converting: %s (%s) -> %s (%s)\n", 
			pszFilenameArgument, 
			FORMATIDNames[inputFormatID],
			pszTargetFilename, 
			FORMATIDNames[targetFormatID]);
	bool result = aMemImage.Save(pszTargetFilename, targetFormatID);

	return result ? 0 : -1;
}

FORMATID GetFormatFromString(const char* string)
{
	for (int iType = 1; iType < FORMAT_COUNT; ++iType)
	{
		if (0 == ::_stricmp(string, FORMATIDNames[iType]))
		{
			return (FORMATID) iType;
		}
	}

	LOG("ERROR: \"%s\" not a valid format string.\n", string);
	return FORMAT_UNSPECIFIED;
}

void ListFormats()
{
	LOG("****************\n");
	LOG("* File Formats *\n");
	LOG("****************\n");
	LOG("Format Name\tDescription\n");
	LOG("________________________________________________\n");
	for (int ii = 1; ii < FORMAT_COUNT; ++ii)
		LOG("%s \t%s\n", FORMATIDNames[ii], FORMATIDDescriptions[ii]);

	LOG("\n**************************\n");
	LOG("* Conversion Rules Table *\n");
	LOG("**************************\n");
	LOG("Source Format\t  Target Format\n");
	LOG("________________________________________________\n");
	for (int ii = 1; ii < FORMAT_COUNT; ++ii)
		LOG("%s \t> %s\n", FORMATIDNames[ii], FORMATIDNames[MemImage::s_ruleTable[ii]]);

	LOG("\nThe Conversion Rules table shows the format given to the destination file\nwhen the source file has a given format.  ");
	LOG("You can change a rule with the /U\noption, or you can force the destination file into a given format with /F.\n");
	LOG("/U can be specified multiple times.  BLP->BLP and PNG->PNG is OK.\n");
	LOG("\nExamples:\n");
	LOG("  blpconverter /FPNG_RGB myfile.blp\n");
	LOG("  blpconverter /UBLP_PAL_A0=PNG_RGB /UPNG_PAL=PNG_RGB myfile.blp\n\n");
}

#define INDENT "\n           "

void Usage()
{
	printf("\nBLPCONVERTER: Converts BLP files to PNGs and vice versa.\n");
	printf("Version 8.1 (C) 2009 Patrick Cyr (kobuster@yahoo.com)\n");
	printf("This program is free software under the GNU General Public License.\n\n");
	printf("BLPCONVERTER [options] sourceFile [targetFile | sourceFile [...]]\n");
	printf("\n");
	printf("sourceFile The file to convert.\n");
	printf("targetFile Optionally, the name of the converted file.  If omitted, target"INDENT"file is given the same name as sourceFile but with the opposite"INDENT"extension.\n");
	printf("\n");
	printf("/A(value)  Sets the Alpha threshold when converting from palettized, 8-bit"INDENT"BLPs to palettized PNGs.  Value is a number between 0 and 255."INDENT"Source alpha values below the threshold are fully transparent, above"INDENT"are fully opaque.  Default is %u.\n", MemImage::s_byAlphaThreshold);
	printf("/C         Create mip test image.  Outputs an image which contains all of the"INDENT"generated mip levels.\n");
	printf("/E         Pause on Error.  (Handy for drag-and-drop use.)\n");
	printf("/F(format) Forces target Format.  Overrides all other settings, including"INDENT"targetFile extension.\n");
	//printf("-g(factor) : (PNG->BLP only) Applies a Gamma factor to the entire image.\n   (factor) is a number between 0.0 and 1.0.  If source texture is PNG RGB,\n   an alpha channel will be added.\n");
	printf("/H         Force WoW cHaracter texture format (palettized, no alpha) when"INDENT"making BLPs.\n");
	printf("/I         Info mode.  Only outputs file information.  This option"INDENT"automatically sets the /V and /M options.\n");
	printf("/L         Lists formats and conversion rules.\n");
	printf("/M         Multi-file mode.  In this mode, multiple files can be input after"INDENT"options.  It is not possible to specify custom output names for them"INDENT"in this mode.\n");
	printf("/N         No mips.  Disables creation of mip levels when saving BLPs.\n");
	printf("/P         Pause upon completion. (Handy for drag-and-drop use.)\n");
	printf("/R         Force WoW clothing texture formats.  All created BLPs are palettized"INDENT"and all PNGs are RGB/RGBA.\n");
	//printf("-s : Don't preserve alpha.  With this mode, palettized BLPs will always make\n     palettized PNGs (and vice versa).  Note: Alpha channels can be lost with this option.\n");
	printf("/U(format)=(format)"INDENT"Change conversion rUle.  See /L.\n");
	printf("/V         Verbose mode.  Outputs additional information.\n");
}

int main(int argc, char* argv[])
{
	int result = 0;

	const char* pszSourceFilename = NULL;
	const char* pszDestFilename = NULL;
	bool bMultiFileMode = false;
	bool bPauseAtEnd = false;
	bool bPauseOnError = false;
	bool bRunTest = false;
	bool bClothingOptionSet = false;
	bool bCharacterOptionSet = false;

	if (argc == 1)
	{
		Usage();
		result = -1;
		goto Finish;
	}
	
	int iArg;
	for (iArg = 1; iArg < argc; ++iArg)
	{
		if ('-' == argv[iArg][0] || '/' == argv[iArg][0])
		{
			switch (argv[iArg][1])
			{
			case '?':
				Usage();
				result = 0;
				goto Finish;
				break;
			case 'a':
			case 'A':
			{
				int threshold = atoi(&argv[iArg][2]);
				if (threshold < 0 || threshold > 255)
				{
					printf("ERROR: Alpha threshold must be between 0 and 255.\n");
					result = -1;
					goto Finish;
				}
				MemImage::s_byAlphaThreshold = threshold;
				break;
			}
			case 'c':
			case 'C':
				MemImage::s_bCreateMipTestImage = true;
				break;
			case 'e':
			case 'E':
				bPauseOnError = true;
				break;
			case 'f':
			case 'F':
			{
				const char* arg = &argv[iArg][2];
				g_targetFormatID = FORMAT_UNSPECIFIED;

				g_targetFormatID = GetFormatFromString(arg);
				if (FORMAT_UNSPECIFIED == g_targetFormatID)
				{
					ListFormats();
					result = -1;
					goto Finish;
				}

				break;
			}
			/*
			case 'g':
			case 'G':
				MemImage::s_fGammaFactor = (float) atof(&argv[iArg][2]);
				if (MemImage::s_fGammaFactor < 0.0f || MemImage::s_fGammaFactor > 1.0f)
				{
					printf("ERROR: Gamma factor must between 0.0 and 1.0.\n");
					result = -1;
					goto Finish;
				}
				break;
			*/
			case 'h':
			case 'H':
				bCharacterOptionSet = true;
				MemImage::s_ruleTable[PNGTYPE_PALETTIZED_ALPHAMASK] = BLPTYPE_PAL_ALPHA0;
				MemImage::s_ruleTable[PNGTYPE_RGB] = BLPTYPE_PAL_ALPHA0;
				MemImage::s_ruleTable[PNGTYPE_RGBA] = BLPTYPE_PAL_ALPHA0;
				break;
			case 'i':
			case 'I':
				printf("File Info Mode.\n");
				g_bInfoMode = true;
				MemImage::s_bVerbose = true;
				bMultiFileMode = true;
				break;
			case 'l':
			case 'L':
				ListFormats();
				goto Finish;
				break;
			case 'm':
			case 'M':
				bMultiFileMode = true;
				break;
			case 'n':
			case 'N':
				MemImage::s_bNoMips = true;
				break;
			case 'p':
			case 'P':
				bPauseAtEnd = true;
				break;
			case 'r':
			case 'R':
				bClothingOptionSet = true;
				MemImage::s_ruleTable[PNGTYPE_RGB] = BLPTYPE_PAL_ALPHA0;
				MemImage::s_ruleTable[PNGTYPE_RGBA] = BLPTYPE_PAL_ALPHA8;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA0] = PNGTYPE_RGB;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA1] = PNGTYPE_RGBA;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA4] = PNGTYPE_RGBA;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA8] = PNGTYPE_RGBA;
				break;
			case 't':
			case 'T':
				bRunTest = true;
				break;
			case 'u':
			case 'U':
			{
				char arg[128];
				::strncpy(arg, &argv[iArg][2], 127);
				char* pszGT = ::strchr(arg, '=');
				if (NULL == pszGT)
				{
					LOG("ERROR: No \"=\" character found in /U option.\n");
					result = -1;
					goto Finish;
				}
				pszGT[0] = 0;
				char* arg2 = &pszGT[1];
				FORMATID srcID = GetFormatFromString(arg);
				FORMATID destID = GetFormatFromString(arg2);

				if (FORMAT_UNSPECIFIED == srcID || FORMAT_UNSPECIFIED == destID)
				{
					ListFormats();
					result = -1;
					goto Finish;
				}
				
				MemImage::s_ruleTable[srcID] = destID;
				break;
			}
			case 'v':
			case 'V':
				MemImage::s_bVerbose = true;
				break;
			default:
				printf("ERROR; %c is not a valid option.\n", argv[iArg][1]);
				result = -1;
				goto Finish;
			}
		}
		else
		{
			break;
		}
	}

	if (bRunTest)
	{
		if (!RunTest())
			result = -1;
		goto Finish;
	}

	if (bClothingOptionSet && bCharacterOptionSet)
	{
		printf("ERROR: -r and -h are exclusive.\n");
		result = -1;
		goto Finish;
	}

	if (iArg == argc)
	{
		printf("ERROR: Filename argument missing.\n");
		result = -1;
		goto Finish;
	}
	else if (bMultiFileMode)
	{
		do
		{
#ifndef LINUX
			char filepath[BLPCONV_MAX_PATH] = "";
			::strcpy(filepath, argv[iArg]);
			char* slash = ::strrchr(filepath, '\\');
			if (NULL == slash)
				slash = ::strrchr(filepath, '/');
			if (NULL != slash)
				slash[1] = 0;
			else
				filepath[0] = 0;

			//LOG("File to search for = %s.\n", argv[iArg]);

			WIN32_FIND_DATA aFindData;
			HANDLE hFindFile = ::FindFirstFile(argv[iArg], &aFindData);
			if (INVALID_HANDLE_VALUE == hFindFile)
			{
				printf("ERROR: Couldn't find file '%s'.\n", argv[iArg]);
				result = -1;
				break;
			}


			char foundFile[BLPCONV_MAX_PATH];
			::sprintf(foundFile, "%s%s", filepath, aFindData.cFileName);
			//LOG("FindFirstFile found %s\n", foundFile);

			do
			{
				result = ProcessFile(foundFile, NULL);
				if (result != 0)
					break;
			}
			while (FindNextFile(hFindFile, &aFindData));

			FindClose(hFindFile);
#else 
			DIR *dp; 
			struct dirent *de; 

			dp = opendir( dirname( argv[iArg] )); 
			if( dp == NULL ) { 
			    printf("ERROR: Couldn't find file '%s' (%d).\n", argv[iArg], errno);
			    result = -1; 
			} else { 

			while ( (de = readdir(dp)) != NULL ) {
			    if( fnmatch( basename(argv[iArg]), de->d_name, FNM_PATHNAME ) == 0 ) { 
				result = ProcessFile(de->d_name, NULL);
				if( result != 0 ) { 
				    break; 
				}
			    }
			}
			closedir(dp); 

			}
#endif

			++iArg;
		}
		while ((result == 0) && (iArg < argc));
	}
	else
	{
		pszSourceFilename = argv[iArg++];
		if (!bMultiFileMode && iArg < argc)
		{
			pszDestFilename = argv[iArg];
		}

		result = ProcessFile(pszSourceFilename, pszDestFilename);
	}

Finish:
	if (bPauseAtEnd || (bPauseOnError && result != 0))
	{
		printf("Press any key to continue...\n");
#ifndef LINUX
		_getch();
#else
		getchar(); 
#endif
	}

	return result;
}

