/* MemImage.cpp : (C) 2008 Patrick Cyr

Implements MemImage class.  There are 4 types of MemImages, palettized/RGB & alpha/no-alpha.
MemImage has built-in functions to convert between any of it's types.  MemImage also
implements Save/Load functions for various file types.  Currently, just PNG and BLP.
Because all types of PNG and BLP load into the same 4 types of MemImages, you can convert
any of the supported file types into any of the others.

TODO: Break the specific loading and saving functions out and keep them in arrays to
make it easier to add support for other file types.
*/

//#define SQUISH_USE_SSE 1

#ifndef LINUX
#include <stdio.h>
#else
#include "port.h"
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#endif

#include <memory.h>
#include <math.h>

#include <set>
#include <list>

#include <png.h>
#include "MemImage.h"
#include "squish/squish.h"
#include "BLP.h"
#include "palbmp/palettize.h"
#include "palbmp/palcreate.h"

const char* FORMATIDNames[] = {
	"[UNSPECIFIED]",
	"BLP_PAL_A0",
	"BLP_PAL_A1",
	"BLP_PAL_A4",
	"BLP_PAL_A8",
	"BLP_DXT1_A0",
	"BLP_DXT1_A1",
	"BLP_DXT3",
	"BLP_DXT5",
	"PNG_PAL",
	"PNG_PAL_MASK",
	"PNG_RGB",
	"PNG_RGBA",
};

const char* FORMATIDDescriptions[] = {
	"[INVALID]",
	"Palettized (no alpha)",
	"Palettized (1-bit alpha)",
	"Palettized (4-bit alpha)",
	"Palettized (full alpha)",
	"DXT1 (no alpha)",
	"DXT1 (alpha)",
	"DXT3",
	"DXT5",
	"Palettized (no alpha)",
	"Palettized (with transparency)",
	"RGB",
	"RGBA",
};

FORMATID MemImage::s_ruleTable[FORMAT_COUNT] =
{
	FORMAT_UNSPECIFIED,

	PNGTYPE_PALETTIZED,				// < BLPTYPE_PAL_ALPHA0,		
	PNGTYPE_PALETTIZED_ALPHAMASK,	// < BLPTYPE_PAL_ALPHA1,		
		PNGTYPE_RGBA,					// < BLPTYPE_PAL_ALPHA4,		
		PNGTYPE_RGBA,					// < BLPTYPE_PAL_ALPHA8,		
	PNGTYPE_RGB,					// < BLPTYPE_DXT1_ALPHA0,	
		PNGTYPE_RGBA,					// < BLPTYPE_DXT1_ALPHA1,	
	PNGTYPE_RGBA,					// < BLPTYPE_DXT3,			
		PNGTYPE_RGBA,					// < BLPTYPE_DXT5,			

	BLPTYPE_PAL_ALPHA0,				// < PNGTYPE_PALETTIZED,
	BLPTYPE_PAL_ALPHA1,				// < PNGTYPE_PALETTIZED_ALPHAMASK,
	BLPTYPE_DXT1_ALPHA0,			// < PNGTYPE_RGB,
	BLPTYPE_DXT3,					// < PNGTYPE_RGBA,
};

///////////////////////////////////////////////////////////////////////////////
// Mip Stuff
// http://number-none.com/product/Mipmapping,%20Part%201/index.html

#define RO 0
#define GO 1
#define BO 2

/*
w = width of image, used to keep things within the borders.  this kicks in when converting from a 2x1->1x1 mip, etc.
h = height of image
c = "color", a byte offset.  for example, for an rgb image, specify 1 if you want the green component.
b = bytes per pixel, ie 1 = palettized, 3 = rgb, 4 = rgba
*/
int OFFSET_RGB(int x, int y, int w, int h, int c, int b)
{
	if (x >= w) 
		x = w - 1;
	if (y >= h) 
		y = h - 1;
	return ((y * w * b) + (x * b) + c);
}

const BYTE SO[4][2] =
{
	{0, 0},
	{0, 1},
	{1, 0},
	{1, 1},
};


///////////////////////////////////////////////////////////////////////////////
// Palette sorting code.

struct HSB
{
	BYTE index;

	DWORD h;
	DWORD s;
	DWORD b;

	bool operator<(HSB& rhs)
	{
		if (h < rhs.h) return true;
		else if (h > rhs.h) return false;

		if (s < rhs.s) return true;
		else if (s > rhs.s) return false;

		if (b < rhs.b) return true;
		else if (b > rhs.b) return false;

		return false;
	}
};

HSB RGBToHSB(int r, int g, int b)
{
	HSB hsb;

	int BMax = 0, BMin = 0, grey = 0, hue = 0, sat = 0;
	float  bright = 0, diff = 0, TempHue = 0;

	if (r > g)
		if (r > b)
			BMax = r;
		else
			BMax = b;
	else
		if (g > b)
			BMax = g;
		else
			BMax = b;

	if (r < g)
		if (r < b)
			BMin = r;
		else
			BMin = b;
	else
		if (g < b)
			BMin = g;
		else
			BMin = b;
	
	diff = float(BMax - BMin);

	if (BMin > 0)
	{
		bright = ((float)BMin/255) + (diff/255);
		grey = int(((float)BMin / bright) + 0.5f);
		sat = int((100 - (((float)grey/255) * 100)) + 0.5f);
		bright = (float(bright * 100) + 0.5f);
		
		if (r == BMax)
		{
			TempHue = (float)(g - b) / diff;
		}
		else if (g == BMax)
		{
			TempHue = 2 + (float)(b - r) / diff;
		}
		else if (b == BMax)
		{
			TempHue = 4 + (float)(r - g) / diff;
		}
		hue = int(TempHue * 60.0f);
		
		if (hue < 0)
			hue = hue + 360;
	}
	
	hsb.b = DWORD(bright);
	hsb.h = hue;
	hsb.s = sat;

	return hsb;
}

typedef std::list<HSB> THSBList;

void SortPalette(BYTE* palette, int numEntries)
{
	THSBList hsbList;
	
	HSB hsb;
	int ii;
	for (ii = 0; ii < numEntries; ++ii)
	{
		hsb = RGBToHSB(palette[ii*3 + 0], palette[ii*3 + 1], palette[ii*3 + 2]);
		hsb.index = ii;

		hsbList.insert(hsbList.end(), hsb);
	}

	hsbList.sort();

	BYTE tempPalette[MEMIMAGE_PALETTEBYTES];
	THSBList::iterator it;
	for (it = hsbList.begin(), ii = 0; it != hsbList.end(); ++it, ++ii) 
	{
		HSB* pHSB = &(*it);
		int ix = pHSB->index;
		tempPalette[ii*3 + 0] = palette[ix*3 + 0];
		tempPalette[ii*3 + 1] = palette[ix*3 + 1];
		tempPalette[ii*3 + 2] = palette[ix*3 + 2];
	}

	memcpy(palette, tempPalette, numEntries*3);
}

///////////////////////////////////////////////////////////////////////////////
// BitArray struct--used to simplify access to the 1-bit alpha array.

struct BitArray
{
	BYTE* m_buffer;
	DWORD m_bytes;
	DWORD m_length;

	BitArray();
	~BitArray();
	void Clear();
	void SetLength(DWORD length);
	// Returns either 0x00 or 0x01, or 0xFF if error
	BYTE Get(DWORD index) const;
	// bit needs to be either 0x00 or 0x01.
	bool Set(DWORD index, BYTE bit);
};

BitArray::BitArray()
{
	m_buffer = NULL;
	Clear();
}

BitArray::~BitArray()
{
	Clear();
}

void BitArray::Clear()
{
	delete[] m_buffer;
	m_buffer = NULL;
	m_bytes = 0;
	m_length = 0;
}

void BitArray::SetLength(DWORD length)
{
	Clear();
	m_bytes = length/8 + (length%8 == 0 ? 0 : 1);
	m_length = length;
	m_buffer = new BYTE[m_bytes];
	::memset(m_buffer, 0, m_bytes);
}

BYTE BitArray::Get(DWORD index) const
{
	if (index >= m_length)
		return 0xFF;

	return m_buffer[index/8] & (0x1 << index%8);
}

// bit needs to be either 0x00 or 0x01.
bool BitArray::Set(DWORD index, BYTE bit)
{
	if (index >= m_length)
		return false;
	if (0x00 != bit && 0x01 != bit)
		return false;

	BYTE& hostByte = m_buffer[index/8];
	BYTE mask = (bit << index%8);
	if (bit)
		hostByte = m_buffer[index/8] | mask;
	else if (hostByte & mask)
		hostByte -= mask;
	//else both are already zero.

	return true;
}

///////////////////////////////////////////////////////////////////////////////

// This class is used for SAVING BLP files.
class BLPMemFile
{
public:
	BLPHeader aHeader;
	BYTE aPalette[4 * 256];
	BYTE* pMips[16];

public:
	BLPMemFile()
	{
		memcpy(aHeader.id, BLPID, 4);
		aHeader.version = 1;
		for (int iMip = 0; iMip < 16; ++iMip)
		{
			aHeader.mipSizes[iMip] = 0;
			aHeader.mipOffsets[iMip] = 0;
		}

		memset(&aPalette, 0, sizeof(BYTE) * 4 * 256);

		int mipIndex;
		for (mipIndex = 0; mipIndex < 16; ++mipIndex)
		{
			pMips[mipIndex] = NULL;
		}
	}

	~BLPMemFile()
	{
		Unload();
	}

	void Unload()
	{
		int mipIndex;
		for (mipIndex = 0; mipIndex < 16; ++mipIndex)
		{
			delete[] pMips[mipIndex];
			pMips[mipIndex] = NULL;
		}
	}

	void SetPalette(const png_color* pPaletteEntries)
	{
		int ii;
		for (ii = 0; ii < 256; ++ii)
		{
			// BLP palettes are BGRA
			aPalette[ii * 4 + 2] = pPaletteEntries[ii].red;
			aPalette[ii * 4 + 1] = pPaletteEntries[ii].green;
			aPalette[ii * 4 + 0] = pPaletteEntries[ii].blue;
			aPalette[ii * 4 + 3] = 0; // guessing.  Seems to have no affect.
		}
	}

	bool Save(const char* pszFilename)
	{
		// Calculate the mip offsets.
		DWORD offset = sizeof(BLPHeader) + (sizeof(BYTE) * 4 * 256);
		int mipIndex = 0;
		int mipCount = 0;
		for (mipIndex = 0; mipIndex < 16; ++mipIndex)
		{
			if (aHeader.mipSizes[mipIndex] > 0)
			{
				++mipCount;
				aHeader.mipOffsets[mipIndex] = offset;
				offset += aHeader.mipSizes[mipIndex];
			}
			else
			{
				break;
			}
		}

		// Open the file for writing.
		FILE* hOutputFile = ::fopen(pszFilename, "wb");
		if (NULL == hOutputFile)
		{
			printf("ERROR: Couldn't open %s for writing.", pszFilename);
			return false;
		}

		////////////////

		if (MemImage::s_bVerbose)
		{
			LOG("\t%dx%d\n", aHeader.xResolution, aHeader.yResolution);
			LOG("\tencoding = %d (%s)\n", aHeader.encoding, (aHeader.encoding < BLP_ENCODING_COUNT ? encodingStrings[aHeader.encoding]: "UNRECOGNIZED"));
			LOG("\talphaBitDepth = %d\n", aHeader.alphaBitDepth);
			LOG("\talphaEncoding = %d\n", aHeader.alphaEncoding);
			if (aHeader.hasMips)
				LOG("\t%d mips\n", mipCount);
			else
				LOG("\tno mips\n");
		}

		/////////////////

		// Write the header.
		::fwrite(&aHeader, sizeof(BLPHeader), 1, hOutputFile);
		
		// Write the palette.
		::fwrite(&aPalette, sizeof(BYTE) * 4, 256, hOutputFile);

		// Output each mip level.
		for (mipIndex = 0; mipIndex < 16; ++mipIndex)
		{
			if (aHeader.mipSizes[mipIndex] > 0)
			{
				::fwrite(pMips[mipIndex], sizeof(BYTE), aHeader.mipSizes[mipIndex], hOutputFile);
			}
		}

		/////////////////

		// Close file.
		::fclose(hOutputFile);
		hOutputFile = NULL;

		return true;
	}
};


///////////////////////////////////////////////////////////////////////////////
// MemImage class

// Public settings.
bool MemImage::s_bCreateMipTestImage = false;
float MemImage::s_fGammaFactor = 1.0;
BYTE MemImage::s_byAlphaThreshold = 0x80;
bool MemImage::s_bVerbose = false;
bool MemImage::s_bNoMips = false;

MemImage::MemImage()
{
	m_buffer = NULL;
	m_bufferBytes = 0;
	Clear();
}

MemImage::~MemImage()
{
	Clear();
}

MemImage::MemImage(const MemImage &rhs)
{
	m_buffer = NULL;
	m_bufferBytes = 0;
	Clear();
	Copy(rhs);
}

MemImage& MemImage::operator=(const MemImage& rhs)
{
	Clear();
	Copy(rhs);

	return *this;
}

void MemImage::Copy(const MemImage& rhs)
{
	m_bHasAlpha = rhs.m_bHasAlpha;
	m_bPalettized = rhs.m_bPalettized;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	::memcpy(m_palette, rhs.m_palette, MEMIMAGE_PALETTEBYTES);
	m_buffer = new BYTE[rhs.m_bufferBytes];
	m_bufferBytes = rhs.m_bufferBytes;
	::memcpy(m_buffer, rhs.m_buffer, rhs.m_bufferBytes);
}

void MemImage::Clear()
{
	m_bPalettized = false;
	m_bHasAlpha = false;
	m_width = 0;
	m_height = 0;
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = NULL;
		m_bufferBytes = 0;
	}
}

bool MemImage::AllocateBuffer(DWORD bytes)
{
	if (0 == bytes)
		bytes = CalculateBufferBytes(m_width, m_height, m_bHasAlpha, m_bPalettized);

	if (m_buffer)
	{
		delete[] m_buffer;
		m_bufferBytes = 0;
	}
	m_buffer = new BYTE[bytes];
	if (NULL == m_buffer)
	{
		LOG("ERROR: Failed to allocate buffer (%d bytes).\n", bytes);
		return false;
	}
	m_bufferBytes = bytes;

	return true;
}

bool MemImage::LoadFromBLP(const char* filename, FORMATID* blpTypeRet)
{
	Clear();

	FILE* fileInput = ::fopen(filename, "rb");
	if (NULL == fileInput)
	{
		errno_t err;
		_get_errno( &err );
		LOG("ERROR opening %s: %s.\n", filename, ::strerror(err));
		return false;
	}
	if (s_bVerbose)
		LOG("%s:\n", filename);

	// Load the entire file into a buffer.
	::fseek(fileInput, 0, SEEK_END);
	DWORD dwFileBytes = ::ftell(fileInput);
	BYTE* fileBuffer = new BYTE[dwFileBytes];
	::fseek(fileInput, 0, SEEK_SET);
	::fread(fileBuffer, dwFileBytes, 1, fileInput);
	::fclose(fileInput);

	BLPHeader* pHeader = (BLPHeader*) fileBuffer;
	BYTE* pPalette = (BYTE*) &(fileBuffer[sizeof(BLPHeader)]);

	/////////////////////
	m_width = pHeader->xResolution;
	m_height = pHeader->yResolution;
	DWORD pixelCount = m_width*m_height;

	if (s_bVerbose)
	{
		LOG("\t%dx%d\n", pHeader->xResolution, pHeader->yResolution);
		LOG("\tencoding = %d (%s)\n", pHeader->encoding, (pHeader->encoding < BLP_ENCODING_COUNT ? encodingStrings[pHeader->encoding]: "UNRECOGNIZED"));
		LOG("\talphaBitDepth = %d\n", pHeader->alphaBitDepth);
		LOG("\talphaEncoding = %d\n", pHeader->alphaEncoding);
		if (pHeader->hasMips)
		{
			int mipCount = 0;
			while(NULL != pHeader->mipOffsets[mipCount])
				++mipCount;
			LOG("\t%d mips\n", mipCount);
		}
		else
			LOG("\tno mips\n");
	}

	// Determine the BLP type.
	FORMATID blpType = FORMAT_UNSPECIFIED;
	if (BLP_ENCODING_PALETTIZED == pHeader->encoding)
	{
		m_bPalettized = true;

		// Determine alpha type.
		m_bHasAlpha = false;
		DWORD bpp = 1;
		switch (pHeader->alphaBitDepth)
		{
		case 0: blpType = BLPTYPE_PAL_ALPHA0; break;
		case 1: blpType = BLPTYPE_PAL_ALPHA1; m_bHasAlpha = true; bpp = 2; break;
		case 4: blpType = BLPTYPE_PAL_ALPHA4; m_bHasAlpha = true; bpp = 2; break;
		case 8: blpType = BLPTYPE_PAL_ALPHA8; m_bHasAlpha = true; bpp = 2; break;
		default: LOG("ERROR: BLP 'alphaBitDepth' field an unrecognized value (%d).\n", pHeader->alphaBitDepth); break;
		}

		// Save the palette.
		// Note that our palette is RGB, but BLP palettes are BGRx.
		for (int ii = 0; ii < 256; ++ii)
		{
			m_palette[ii*3 + 0] = pPalette[ii*4 + 2];
			m_palette[ii*3 + 1] = pPalette[ii*4 + 1];
			m_palette[ii*3 + 2] = pPalette[ii*4 + 0];
		}

		// Save the image data.
		if (!AllocateBuffer(m_width * m_height * bpp))
			return false;

		// Save image data.
		BYTE* pImageData = &(fileBuffer[pHeader->mipOffsets[0]]);
		switch (blpType)
		{
		case BLPTYPE_PAL_ALPHA0:
		case BLPTYPE_PAL_ALPHA8:
			{
				::memcpy(m_buffer, pImageData, m_bufferBytes);
				break;
			}
		case BLPTYPE_PAL_ALPHA1:
			{
				::memcpy(m_buffer, pImageData, pixelCount);

				// Convert the 1-bit alpha channel to 8-bit.
				BYTE* pAlphaData = &(pImageData[pixelCount]);
				for (DWORD ii = 0; ii < pixelCount; ++ii)
				{
					m_buffer[pixelCount + ii] = (pAlphaData[ii / 8] & (0x1 << (ii % 8))) ? 0xFF : 0;
				}

				break;
			}
		case BLPTYPE_PAL_ALPHA4:
			{
				::memcpy(m_buffer, pImageData, pixelCount);

				// Convert the 4-bit alpha channel to 8-bit.
				BYTE* pAlphaData = &(pImageData[pixelCount]);
				for (DWORD ii = 0; ii < pixelCount; ++ii)
				{
					// Two alpha values per byte.
					// Note: This could obviously be made faster but I wanted it to be clear as possible.
					BYTE by = pAlphaData[ii / 2];		// The byte
					BYTE mask = 0xF << (ii % 2)*4;		// The mask to get which of the two we want.
					BYTE maskedBy = by & mask;			// The masked byte.
					DWORD shift = (ii % 2)*4;			// The amount of shift needed.
					BYTE alpha4 = maskedBy >> shift;	// The 4-bit alpha value.
					BYTE alpha = BYTE(float(0xFF) * (float(alpha4) / float(0xF)));	// Translated into 8-bit.

					m_buffer[pixelCount + ii] = alpha;
				}

				break;
			}
		}
	}
	else if (BLP_ENCODING_COMPRESSED == pHeader->encoding)
	{
		m_bPalettized = false;
		int squishFlags = squish::kDxt1;

		m_bHasAlpha = true;
		switch (pHeader->alphaBitDepth)
		{
		case 0: blpType = BLPTYPE_DXT1_ALPHA0; m_bHasAlpha = false; break;
		case 1: blpType = BLPTYPE_DXT1_ALPHA1; break;
		case 8: 
			{
				switch (pHeader->alphaEncoding)
				{
				case 1: blpType = BLPTYPE_DXT3; squishFlags = squish::kDxt3; break;
				case 7:	blpType = BLPTYPE_DXT5; squishFlags = squish::kDxt5; break;
				default: LOG("ERROR: BLP 'alphaEncoding' field an unrecognized value (%d).\n", pHeader->alphaEncoding); break;
				}
				break;
			}
		default: LOG("ERROR: BLP 'alphaBitDepth' field an unrecognized value (%d).\n", pHeader->alphaBitDepth); break;
		}

		////////////

		void* Source = &(fileBuffer[pHeader->mipOffsets[0]]); 
		squish::u8* Dest = new squish::u8[ m_width * m_height * 4 ]; 

		// Do the conversion.
		squish::DecompressImage( Dest, 
				 m_width, 
				 m_height, 
				 Source, 
				 squishFlags); 

		// Create a buffer for the data.
		DWORD bpp = (0 == pHeader->alphaBitDepth) ? 3 : 4;
		if (!AllocateBuffer(m_width * m_height * bpp))
			return false;

		// Copy into our own buffer.
		if (0 == pHeader->alphaBitDepth)
		{
			for (DWORD ii = 0; ii < pixelCount; ++ii)
			{
				::memcpy(&m_buffer[ii*3], &Dest[ii*4], 3);
			}
		}
		else
		{
			::memcpy(m_buffer, Dest, m_width*m_height*bpp);
		}

		// Cleanup.
		delete[] Dest;
		Dest = NULL;
	}
	if (FORMAT_UNSPECIFIED == blpType)
		return false;

	if (blpTypeRet)
		*blpTypeRet = blpType;
	if (s_bVerbose)
		LOG("\tFormat = %s (%s).\n", FORMATIDNames[blpType], FORMATIDDescriptions[blpType]);

	// Cleanup.
	delete[] fileBuffer;

	return true;
}

bool MemImage::LoadFromPNG(const char* filename, FORMATID* pngTypeRet)
{
	bool retVal = true;

	// Clear any existing content.
	Clear();

	// Open the PNG.
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	FILE *fp;
	if ((fp = ::fopen(filename, "rb")) == NULL)
	{
		errno_t err;
		_get_errno( &err );
		LOG("ERROR opening %s: %s.\n", filename, ::strerror(err));
		return false;
	}
	if (s_bVerbose)
		LOG("%s:\n", filename);

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also supply the
	* the compiler header file version, so that we know if the application
	* was compiled with a compatible version of the library.  REQUIRED
	*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,  NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
	  fclose(fp);
	  return false;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
	  fclose(fp);
	  png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
	  return false;
	}

	/* Set error handling if you are using the setjmp/longjmp method (this is
	* the normal method of doing things with libpng).  REQUIRED unless you
	* set up your own error handlers in the png_create_read_struct() earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
	  /* Free all of the memory associated with the png_ptr and info_ptr */
	  png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	  fclose(fp);
	  /* If we get here, we had a problem reading the file */
	  return false;
	}

	/* Set up the input control if you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* If we have already read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);

	/*
	* If you have enough memory to read in the entire image at once,
	* and you need to specify only transforms that can be controlled
	* with one of the PNG_TRANSFORM_* bits (this presently excludes
	* dithering, filling, setting background, and doing gamma
	* adjustment), then you can read the entire image (including
	* pixels) into the info structure with this call:
	*/
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, png_voidp_NULL);

	/* At this point you have read the entire image */

	///////////////////////////////////////////////////////////////////////////

	m_width = info_ptr->width;
	m_height = info_ptr->height;
	DWORD pixelCount = m_width * m_height;

	if (s_bVerbose)
		LOG("\t%dx%d\n", m_width, m_height);

	// Create a buffer for the image data.
	DWORD imageBytes = info_ptr->rowbytes * info_ptr->height * (info_ptr->num_trans == 0 ? 1 : 2);
	if (!AllocateBuffer(imageBytes))
		return false;

	// Copy the data in row by row.
	if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA || 
		info_ptr->color_type == PNG_COLOR_TYPE_RGB || 
		(info_ptr->color_type == PNG_COLOR_TYPE_PALETTE && info_ptr->num_trans == 0))
	{
		for (DWORD row = 0; row < info_ptr->height; ++row)
		{
			memcpy(&m_buffer[row*info_ptr->rowbytes], info_ptr->row_pointers[row], info_ptr->rowbytes);
		}
	}
	else if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE && info_ptr->num_trans > 0)
	{
		for (DWORD row = 0; row < info_ptr->height; ++row)
		{
			for (DWORD ii = 0; ii < info_ptr->width; ++ii)
			{
				BYTE palIx = info_ptr->row_pointers[row][ii];
				m_buffer[row*info_ptr->rowbytes + ii] = palIx;

				BYTE alpha = 255;
				if (palIx < info_ptr->num_trans)
					alpha = info_ptr->trans[palIx];
				m_buffer[pixelCount + row*info_ptr->rowbytes + ii] = alpha;
			}
		}
	}
	else 
	{
		printf("ERROR: PNG format unsupported.  Format = %d", info_ptr->color_type);
		return false;
	}

	// Copy the palette.
	if (info_ptr->palette)
	{
		// Assuming here that png_color is 3 bytes (it is at time of writing).
		memcpy(m_palette, info_ptr->palette, info_ptr->num_palette*sizeof(png_color));
	}

	///////////////////////////////////////////////////////////////////////////
	// Print image info.

	FORMATID type = FORMAT_UNSPECIFIED;
	m_bPalettized = false;
	m_bHasAlpha = false;
	if (PNG_COLOR_TYPE_PALETTE == info_ptr->color_type)
	{
		m_bPalettized = true;

		if (s_bVerbose)
			LOG("\t%d palette entries\n", info_ptr->num_palette);

		if (info_ptr->num_trans == 0)
		{
			type = PNGTYPE_PALETTIZED;
		}
		else
		{
			type = PNGTYPE_PALETTIZED_ALPHAMASK;
			if (s_bVerbose)
				LOG("\t%d transparency values\n", info_ptr->num_trans);
			m_bHasAlpha = true;
		}
	}
	else if (PNG_COLOR_TYPE_RGB_ALPHA == info_ptr->color_type)
	{
		type = PNGTYPE_RGBA;
		m_bHasAlpha = true;
	}
	else if (PNG_COLOR_TYPE_RGB == info_ptr->color_type)
	{
		type = PNGTYPE_RGB;
	}
	else
	{
		printf("ERROR: Unsupported PNG format.\n");
		retVal = false;
	}
	if (pngTypeRet)
		*pngTypeRet = type;
	if (s_bVerbose)
		LOG("\tFormat = %s (%s).\n", FORMATIDNames[type], FORMATIDDescriptions[type]);

	///////////////////////////////////////////////////////////////////////////

	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	/* close the file */
	fclose(fp);

	/* that's it */
	return retVal;
}

bool MemImage::Save(const char* filename, FORMATID type) const
{
	if (BLPTYPE_FIRST <= type && type <= BLPTYPE_LAST)
		return SaveToBLP(filename, type);
	else if (PNGTYPE_FIRST <= type && type <= PNGTYPE_LAST)
		return SaveToPNG(filename, type);
	
	LOG("ERROR: Save called with invalid type %d.\n", type);
	return false;
}

bool MemImage::SaveToPNG(const char* filename, FORMATID type) const
{
	// Determine default png type if unspecified.
	if (FORMAT_UNSPECIFIED == type)
	{
		if (m_bPalettized)
		{
			type = m_bHasAlpha ? PNGTYPE_PALETTIZED_ALPHAMASK : PNGTYPE_PALETTIZED;
		}
		else
		{
			type = m_bHasAlpha ? PNGTYPE_RGBA : PNGTYPE_RGB;
		}
	}
	else
	{
		// See if we need to make any conversions.
		bool bPalettize = false;
		bool bDepalettize = false;
		bool bRemoveAlpha = false;
		bool bAddAlpha = false;

		if (PNGTYPE_PALETTIZED <= type && type <= PNGTYPE_PALETTIZED_ALPHAMASK)
		{
			if (!m_bPalettized)
				bPalettize = true;
		}
		else
		{
			if (m_bPalettized)
				bDepalettize = true;
		}

		if (PNGTYPE_PALETTIZED == type || PNGTYPE_RGB == type)
		{
			if (m_bHasAlpha)
				bRemoveAlpha = true;
		}
		else
		{
			if (!m_bHasAlpha)
				bAddAlpha = true;
		}

		if (bPalettize || bDepalettize || bRemoveAlpha || bAddAlpha)
		{
			// Create a image to convert.
			// NOTE: The point of doing this is to make the code which actually saves the BLP
			// data simpler.  Since we do this, it can assume that when it is asked to save 
			// to a given format the MemImage will be in the nearest corresponding format.
			// For example, if the code below is saving to pal 1-bit alpha, the MemImage will
			// always be palettized w/ alpha.
			MemImage convertedImage(*this);
			if (bPalettize)
			{
				if (!convertedImage.Palettize())
					return false;
			}
			else if (bDepalettize)
			{
				convertedImage.Depalettize();
			}

			if (bRemoveAlpha)
			{
				if (!convertedImage.RemoveAlpha())
					return false;
			}
			else if (bAddAlpha)
			{
				if (!convertedImage.AddAlpha())
					return false;
			}

			return convertedImage.SaveToPNG(filename, type);
		}
	}
	//LOG("Saving \"%s\" (%s)...\n", filename, FORMATIDNames[type]);

	//////////////////
	// PNG initialization:

	// Allocate the png structures.
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		return false;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return false;
	}

	/* Set error handling if you are using the setjmp/longjmp method (this is
	* the normal method of doing things with libpng).  REQUIRED unless you
	* set up your own error handlers in the png_create_read_struct() earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		/* If we get here, we had a problem reading the file */
		return false;
	}

	/* Set up the input control if you are using standard C streams */
	FILE* fp = ::fopen(filename, "wb");
	if (!fp)
	{
		LOG("ERROR: Couldn't open %s for writing.\n", filename);
		return false;
	}	
	png_init_io(png_ptr, fp);

	// Allocate row pointers.
	BYTE** pRowPointers = new BYTE*[m_height];

	bool bDeleteRows = false;
	int transform = PNG_TRANSFORM_IDENTITY;

	///////////////////////////////////
	BYTE* tempBuffer = NULL;
	DWORD pixelCount = m_width*m_height;

	switch (type)
	{
	case PNGTYPE_PALETTIZED: 
	case PNGTYPE_PALETTIZED_ALPHAMASK: 
		{
			// Create the PNG palette and set it.
			png_color pngPalette[256];
			for (int ii = 0; ii < 256; ++ii)
			{
				pngPalette[ii].red		= m_palette[ii * 3 + 0];
				pngPalette[ii].green	= m_palette[ii * 3 + 1];
				pngPalette[ii].blue		= m_palette[ii * 3 + 2];
			}

			// Set the image type.
			png_set_IHDR(png_ptr, info_ptr, 
					m_width,
					m_height,
					8,	// Bit depth.
					PNG_COLOR_TYPE_PALETTE,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);

			// PC: What if type is alpha mask but we have none?  I'm guessing its irrelevant.
			if ((PNGTYPE_PALETTIZED_ALPHAMASK == type) && m_bHasAlpha)
			{
				DWORD pixelCount = m_width * m_height;

				// We need to juggle around the palette indexes to do the trans-alpha thing, so
				// make a temp copy of the image.
				tempBuffer = new BYTE[pixelCount];
				::memcpy(tempBuffer, m_buffer, pixelCount);

				// Assign row pointers.
				for (WORD row = 0; row < m_height; ++row)
				{
					pRowPointers[row] = &tempBuffer[row * m_width];
				}

				// Scan the image.  The palette entry of the first pixel with an alpha below a threshold
				// is chosen to be the 0 alpha entry.  The actual 0 entry is moved into the found palette entry.
				// All pixels that are below that alpha are set to the new palette entry 0.

				// Not necessarily lossy if the source was 1-bit alpha, too.
				printf("\t1-bit Alpha\n");
				
				// This needs to come first, its the way the file format is defined.
				const BYTE ALPHAPALETTEINDEX = 0;

				bool transFound = false;
				bool transIsZeroAlready = false;
				BYTE transparentAlphaIndex;
				BYTE* alphaBuffer = &m_buffer[pixelCount];
				for (DWORD ii = 0; ii < pixelCount; ++ii)
				{
					BYTE alpha = alphaBuffer[ii];

					if (alpha < s_byAlphaThreshold)
					{
						if (!transFound)
						{
							transparentAlphaIndex = tempBuffer[ii];

							// Dont use 0--thats the one we want.
							// I'm not really 100% sure about this, but I don't care a whole lot 
							// because this is a lossy conversion.  RGBA is a much better option.
							if (transparentAlphaIndex != 0)
							{
								// We want palette entry 0 to be our transparent one, so swap out
								// this color with 0 in the palette.^		
								pngPalette[transparentAlphaIndex].red = pngPalette[ALPHAPALETTEINDEX].red;
								pngPalette[transparentAlphaIndex].green = pngPalette[ALPHAPALETTEINDEX].green;
								pngPalette[transparentAlphaIndex].blue = pngPalette[ALPHAPALETTEINDEX].blue;

								// Set to an annoying color for testing.
								pngPalette[ALPHAPALETTEINDEX].red = 0;
								pngPalette[ALPHAPALETTEINDEX].green = 0xFF;
								pngPalette[ALPHAPALETTEINDEX].blue = 0;

								transFound = true;
								break;
							}
							else
							{
								// Never tested case where this is true but transFound is false.
								transIsZeroAlready = true;
							}
						}
					}
				}

				if (transFound)
				{
					for (DWORD ii = 0; ii < pixelCount; ++ii)
					{
						// If this pixel was an original pallete 0, change it to the entry that 0 was moved to.
						if (tempBuffer[ii] == ALPHAPALETTEINDEX)
							tempBuffer[ii] = transparentAlphaIndex;

						// If this pixel is below the alpha threshold, set it to the 0 alpha palette entry (0).
						BYTE alpha = alphaBuffer[ii];
						if (alpha < s_byAlphaThreshold)
							tempBuffer[ii] = ALPHAPALETTEINDEX;
					}
				}

				if (transFound || transIsZeroAlready)
				{
					png_byte trans[1];
					// The [0] means that "alpha for pallete entry 0", and the = 0 means "is zero".
					trans[0] = 0;
					png_set_tRNS(png_ptr, info_ptr, trans, 1, NULL);

					// Note: I don't understand why, but the resulting image doesn't have the
					// green zero entry created.  It looks correct, but instead of the alpha entry
					// being 0 it is 255.
				}
			} 
			else // no alpha
			{
				// Assign row pointers.
				for (WORD row = 0; row < m_height; ++row)
				{
					pRowPointers[row] = &m_buffer[row * m_width];
				}
			}

			// Set the palette.
			png_set_PLTE(png_ptr, info_ptr, pngPalette, 256);

			break;
		}

	case PNGTYPE_RGB: 
	case PNGTYPE_RGBA: 
		{
			// Set the image type.
			png_set_IHDR(png_ptr, info_ptr, 
					m_width,
					m_height,
					8,	// Bit depth.
					(PNGTYPE_RGBA == type) ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);

			// Assign row pointers.
			if (PNGTYPE_RGB == type && m_bHasAlpha)
			{
				// Create a RGB buffer.
				tempBuffer = new BYTE[3*pixelCount];
				for (DWORD ii = 0; ii < pixelCount; ++ii)
				{
					::memcpy(&tempBuffer[ii*3], &m_buffer[ii*4], 3);
				}

				for (WORD row = 0; row < m_height; ++row)
				{
					pRowPointers[row] = &tempBuffer[row * m_width * 3];
				}
			}
			else if (PNGTYPE_RGBA == type && !m_bHasAlpha)
			{
				// Create a RGBA buffer.
				tempBuffer = new BYTE[4*pixelCount];
				for (DWORD ii = 0; ii < pixelCount; ++ii)
				{
					::memcpy(&tempBuffer[ii*4], &m_buffer[ii*3], 3);
					tempBuffer[ii*4+3] = 0xFF;
				}

				for (WORD row = 0; row < m_height; ++row)
				{
					pRowPointers[row] = &tempBuffer[row * m_width * 4];
				}
			}
			else
			{
				DWORD bpp = (PNGTYPE_RGBA == type) ? 4 : 3;
				for (WORD row = 0; row < m_height; ++row)
				{
					pRowPointers[row] = &m_buffer[row * m_width * bpp];
				}
			}

			break;
		}

	default:
		{
			LOG("ERROR: Unexpected PNG type.\n");
			return false;
			break;
		}
	}

	// Put our image data into the PNG.
	png_set_rows(png_ptr, info_ptr, pRowPointers);

	// *** Do the write.
	png_write_png(png_ptr, info_ptr, transform, NULL);

	// Clean up.
	if (bDeleteRows)
	{
		for (DWORD ii = 0; ii < m_height; ++ii)
			delete[] pRowPointers[ii];
	}
	delete[] pRowPointers;
	delete[] tempBuffer;

	LOG("...done!\n");

	return true;
}

bool MemImage::SaveToBLP(const char* filename, FORMATID type) const
{
	// Determine the destination format.
	FORMATID destFormat = type;
	if (FORMAT_UNSPECIFIED == destFormat)
	{
		// By default, do 8-bit or 0-bit alpha only.  We could theoretically 
		// check the actual alpha levels in the image and try to guess whether 1-bit
		// is appropriate, but it seems pretty rare that people will need 1-bit alpha
		// and they can always manually specify it when/if they do.
		if (m_bPalettized)
			destFormat = m_bHasAlpha ? BLPTYPE_PAL_ALPHA8 : BLPTYPE_PAL_ALPHA0;
		else
			destFormat = m_bHasAlpha ? BLPTYPE_DXT3 : BLPTYPE_DXT1_ALPHA0;
	}
	else
	{
		// See if we need to make any conversions.
		bool bPalettize = false;
		bool bDepalettize = false;
		bool bRemoveAlpha = false;
		bool bAddAlpha = false;

		if (BLPTYPE_PAL_ALPHA0 <= destFormat && destFormat <= BLPTYPE_PAL_ALPHA8)
		{
			if (!m_bPalettized)
				bPalettize = true;
		}
		else
		{
			if (m_bPalettized)
				bDepalettize = true;
		}

		if (BLPTYPE_PAL_ALPHA0 == destFormat || BLPTYPE_DXT1_ALPHA0 == destFormat)
		{
			if (m_bHasAlpha)
				bRemoveAlpha = true;
		}
		else
		{
			if (!m_bHasAlpha)
				bAddAlpha = true;
		}

		if (bPalettize || bDepalettize || bRemoveAlpha || bAddAlpha)
		{
			// Create a image to convert.
			// NOTE: The point of doing this is to make the code which actually saves the BLP
			// data simpler.  Since we do this, it can assume that when it is asked to save 
			// to a given format the MemImage will be in the nearest corresponding format.
			// For example, if the code below is saving to pal 1-bit alpha, the MemImage will
			// always be palettized w/ alpha.
			MemImage convertedImage(*this);
			if (bPalettize)
			{
				if (!convertedImage.Palettize())
					return false;
			}
			else if (bDepalettize)
			{
				convertedImage.Depalettize();
			}

			if (bRemoveAlpha)
			{
				if (!convertedImage.RemoveAlpha())
					return false;
			}
			else if (bAddAlpha)
			{
				if (!convertedImage.AddAlpha())
					return false;
			}

			return convertedImage.SaveToBLP(filename, destFormat);
		}
	}
	//LOG("Saving \"%s\" (%s)...\n", filename, FORMATIDNames[type]);

	///////////////////////

	// Create structures to hold the BLP data.
	BLPMemFile aBLPFile;

	// Initialize its header.
	aBLPFile.aHeader.xResolution = m_width;
	aBLPFile.aHeader.yResolution = m_height;
	aBLPFile.aHeader.hasMips = s_bNoMips ? 0 : 1;

	DWORD pixelCount = m_width * m_height;
	switch (destFormat)
	{
	case BLPTYPE_PAL_ALPHA0:
		{
			aBLPFile.aHeader.alphaBitDepth = 0;		// No alpha
			aBLPFile.aHeader.alphaEncoding = 8;		// 8 value taken from example
			aBLPFile.aHeader.encoding = BLP_ENCODING_PALETTIZED;		
			break;
		}
	case BLPTYPE_PAL_ALPHA1:
		{
			aBLPFile.aHeader.alphaBitDepth = 1;		// 1-bit alpha
			aBLPFile.aHeader.alphaEncoding = 1;		// 1 value taken from example
			aBLPFile.aHeader.encoding = BLP_ENCODING_PALETTIZED;		
			break;
		}
	case BLPTYPE_PAL_ALPHA4:
		{
			aBLPFile.aHeader.alphaBitDepth = 4;		// 1-bit alpha
			aBLPFile.aHeader.alphaEncoding = 8;		// 8 value taken from example
			aBLPFile.aHeader.encoding = BLP_ENCODING_PALETTIZED;		
			break;
		}
	case BLPTYPE_PAL_ALPHA8:
		{
			aBLPFile.aHeader.alphaBitDepth = 8;		// Regular alpha
			aBLPFile.aHeader.alphaEncoding = 8;		// 8 value taken from example
			aBLPFile.aHeader.encoding = BLP_ENCODING_PALETTIZED;		
			break;
		}
	case BLPTYPE_DXT1_ALPHA0:
		{
			aBLPFile.aHeader.alphaBitDepth = 0;		// No alpha
			aBLPFile.aHeader.alphaEncoding = 0;		// 1 = DXT1
			aBLPFile.aHeader.encoding = BLP_ENCODING_COMPRESSED;
			break;
		}
	case BLPTYPE_DXT1_ALPHA1:
		{
			aBLPFile.aHeader.alphaBitDepth = 1;		// No alpha
			aBLPFile.aHeader.alphaEncoding = 0;		// 0 = DXT1 
			aBLPFile.aHeader.encoding = BLP_ENCODING_COMPRESSED;
			break;
		}
	case BLPTYPE_DXT3:
		{
			aBLPFile.aHeader.alphaBitDepth = 8;		// 
			aBLPFile.aHeader.alphaEncoding = 1;		// 1 = DXT3
			aBLPFile.aHeader.encoding = BLP_ENCODING_COMPRESSED;
			break;
		}
	case BLPTYPE_DXT5:
		{
			aBLPFile.aHeader.alphaBitDepth = 8;		// 
			aBLPFile.aHeader.alphaEncoding = 7;		// 7 = DXT5
			aBLPFile.aHeader.encoding = BLP_ENCODING_COMPRESSED;
			break;
		}
	default:
		{
			LOG("ERROR: Currently unable to save BLPs with format %d.\n", destFormat);
			return false;
		}
	}

	///////////////////////
	// MIP Generation:

	// [0] represents the original image.
	int mipInfoCount = 1;
	MemImage mips[16];
	mips[0] = *this;

	if (!s_bNoMips)
	{
		// Generate each level of mip using the format of the file itself.
		do
		{
			mips[mipInfoCount].BuildMipmap(mips[mipInfoCount-1]);
			++mipInfoCount;
		}
		while (mips[mipInfoCount-1].m_width != 1 && mips[mipInfoCount-1].m_height != 1);

		if (s_bCreateMipTestImage)
		{
			png_color* palette = m_bPalettized ? (png_color*) m_palette : NULL;
			MemImage debugImage;
			debugImage.SaveMipDebugImage(filename, mips, mipInfoCount);
		}
	}

	// Go through each mip level and do the conversion to the target format.
	// This might seem kind of backwards but we have to do it this way for the DXT textures.
	for (int iLevel = 0; iLevel < mipInfoCount; ++iLevel)
	{
		DWORD mipWidth = mips[iLevel].m_width;
		DWORD mipHeight = mips[iLevel].m_height;
		DWORD mipPixelCount = mipWidth * mipHeight;
		BYTE* srcMipBuffer = mips[iLevel].m_buffer;

		// Figure out how big the converted mip level needs to be.
		DWORD convertedMipBytes;
		switch (destFormat)
		{
			case BLPTYPE_PAL_ALPHA0:
			{
				convertedMipBytes = mipWidth * mipHeight;
				break;
			}
			case BLPTYPE_PAL_ALPHA1:
			{
				convertedMipBytes = (mipWidth * mipHeight);
				convertedMipBytes += (convertedMipBytes / 8) + (convertedMipBytes % 8 ? 1 : 0);
				break;
			}
			case BLPTYPE_PAL_ALPHA4:
			{
				convertedMipBytes = (mipWidth * mipHeight);
				convertedMipBytes += (convertedMipBytes / 2);	// Images are always a power of 2 so a modulo like above would always be false.
				break;
			}
			case BLPTYPE_PAL_ALPHA8:
			{
				convertedMipBytes = mipWidth * mipHeight * 2;
				break;
			}
			case BLPTYPE_DXT1_ALPHA0:
			{
				int blksize = 8; // 16 for dxt3
				int dx = (mipWidth + 3) >> 2;
				int dy = (mipHeight + 3) >> 2;
				convertedMipBytes = dx * dy * blksize;
				break;
			}
			case BLPTYPE_DXT1_ALPHA1:
			{
				int blksize = 8; // 16 for dxt3
				int dx = (mipWidth + 3) >> 2;
				int dy = (mipHeight + 3) >> 2;
				convertedMipBytes = dx * dy * blksize;
				break;
			}
			case BLPTYPE_DXT3:
			{
				int blksize = 16; // 16 for dxt3
				int dx = (mipWidth  + 3) >> 2;
				int dy = (mipHeight + 3) >> 2;
				convertedMipBytes = dx * dy * blksize;
				break;
			}
			case BLPTYPE_DXT5:
			{
				int blksize = 16; // 16 for dxt5 (i think)
				int dx = (mipWidth  + 3) >> 2;
				int dy = (mipHeight + 3) >> 2;
				convertedMipBytes = dx * dy * blksize;
				break;
			}
			default:
			{
				LOG("ERROR: Destination format not supported, internal error.\n");
				return false;
			}
		}
				
		// Allocate a buffer for the converted mip level.
		aBLPFile.aHeader.mipSizes[iLevel] = convertedMipBytes;
		aBLPFile.pMips[iLevel] = new BYTE[convertedMipBytes];

		// Do the conversion.
		if (BLPTYPE_PAL_ALPHA0 == destFormat)
		{
			// Copy the data into the file buffer directly.
			::memcpy(aBLPFile.pMips[iLevel], srcMipBuffer, convertedMipBytes);
		}
		else if (BLPTYPE_PAL_ALPHA1 == destFormat)
		{
			// Copy the indexd data into the file buffer directly.
			::memcpy(aBLPFile.pMips[iLevel], srcMipBuffer, mipPixelCount);

			// Create the alpha section.
			BitArray alphaBits;
			alphaBits.SetLength(mipPixelCount);
			BYTE* pMipAlphaBuffer = &srcMipBuffer[mipPixelCount];
			for (DWORD iMipPixel = 0; iMipPixel < mipPixelCount; ++iMipPixel)
			{
				alphaBits.Set(iMipPixel, pMipAlphaBuffer[iMipPixel] >= s_byAlphaThreshold ? 0x1 : 0x0);
			}

			BYTE* pAlphaBuffer = &aBLPFile.pMips[iLevel][mipPixelCount];
			::memcpy(pAlphaBuffer, alphaBits.m_buffer, alphaBits.m_bytes);
		}
		else if (BLPTYPE_PAL_ALPHA4 == destFormat)
		{
			// Copy the indexd data into the file buffer directly.
			::memcpy(aBLPFile.pMips[iLevel], srcMipBuffer, mipPixelCount);

			DWORD alphaOffset = mipPixelCount;

			// Copy the alpha data into the file buffer directly.
			for (DWORD pixelIx = 0; pixelIx < mipWidth * mipHeight; pixelIx += 2)
			{
				// Get alpha values.
				BYTE pixelAlpha0 = BYTE(float(srcMipBuffer[alphaOffset + pixelIx]) * s_fGammaFactor);
				BYTE pixelAlpha1 = BYTE(float(srcMipBuffer[alphaOffset + pixelIx + 1]) * s_fGammaFactor);

				// Convert to 4-bit.
				pixelAlpha0 = pixelAlpha0 >> 4;
				pixelAlpha1 = pixelAlpha1 >> 4;

				// Pack into same byte.
				aBLPFile.pMips[iLevel][alphaOffset + pixelIx / 2] = pixelAlpha0 + (pixelAlpha1 << 4);
			}
		}
		else if (BLPTYPE_PAL_ALPHA8 == destFormat)
		{
			DWORD alphaOffset = convertedMipBytes / 2;

			// Copy the data into the file buffer directly.
			for (DWORD pixelIx = 0; pixelIx < mipWidth * mipHeight; ++pixelIx)
			{
				aBLPFile.pMips[iLevel][pixelIx]					= srcMipBuffer[pixelIx];

				BYTE pixelAlpha = srcMipBuffer[alphaOffset + pixelIx];
				aBLPFile.pMips[iLevel][alphaOffset + pixelIx]	= BYTE(float(pixelAlpha) * s_fGammaFactor);
			}
		}
		else if (BLPTYPE_DXT1_ALPHA0 <= destFormat && destFormat <= BLPTYPE_DXT5)
		{
			// Setup the source texture
			squish::u8 *Src;
			if (BLPTYPE_DXT1_ALPHA0 == destFormat)
			{
				// squish::CompressImage expects an rgba buffer.
				Src = new squish::u8[mipPixelCount*4];
				for (DWORD iPixel = 0; iPixel < mipPixelCount; ++iPixel)
				{
					Src[iPixel*4+0] = srcMipBuffer[iPixel*3+0];
					Src[iPixel*4+1] = srcMipBuffer[iPixel*3+1];
					Src[iPixel*4+2] = srcMipBuffer[iPixel*3+2];
					Src[iPixel*4+3] = 0xFF;
				}
			}
			else
			{
				Src = srcMipBuffer;
			}

			// Set up the destination texture.
			squish::u8	*Dest; 
			Dest = new squish::u8[convertedMipBytes]; 

			int dxtType = squish::kDxt1;
			if (BLPTYPE_DXT3 == destFormat)
				dxtType = squish::kDxt3;
			else if (BLPTYPE_DXT5 == destFormat)
				dxtType = squish::kDxt5;

			// Do the conversion.
			squish::CompressImage( Src, mipWidth, mipHeight, Dest, dxtType );

			// Copy into the BLP structure.
			::memcpy(aBLPFile.pMips[iLevel], Dest, convertedMipBytes);

			delete[] Dest;

			if (BLPTYPE_DXT1_ALPHA0 == destFormat)
				delete[] Src;
		}
		else
		{
			LOG("ERROR: Internal error.\n");
			return false;
		}
	}

	// Set the palette.
	if (m_bPalettized)
	{
		aBLPFile.SetPalette((png_color*) m_palette);
	}

	// Create a new blp.
	//LOG("-> %s:\n", filename);
	if (!aBLPFile.Save(filename))
		return false;

	LOG("...done!\n");

	return true;
}

bool MemImage::Palettize()
{
	if (m_bPalettized)
		return true;

	DWORD pixelCount = m_width * m_height;

	// Create a temp, 24-bit version of the RGBA image.
	BYTE* rgbPlane = new BYTE[pixelCount * 3];
	if (m_bHasAlpha)
	{
		for (DWORD ii = 0; ii < pixelCount; ++ii)
		{
			rgbPlane[ii*3 + 0] = m_buffer[ii*4 + 0];
			rgbPlane[ii*3 + 1] = m_buffer[ii*4 + 1];
			rgbPlane[ii*3 + 2] = m_buffer[ii*4 + 2];
		}
	}
	else
	{
		::memcpy(rgbPlane, m_buffer, 3*pixelCount);
	}

	// Create a buffer to hold the palettized data.
	BYTE* pOutputBuffer = new BYTE[pixelCount];
	if (NULL == pOutputBuffer)
	{
		LOG("ERROR: Couldn't allocate memory.\n");
		return false;
	}

	BYTE* alphaBuffer = NULL;
	BYTE bpp = 3;
	if (m_bHasAlpha)
	{
		bpp = 4;

		// Save alpha.
		alphaBuffer = new BYTE[pixelCount];
		for (DWORD ii = 0; ii < pixelCount; ++ii)
		{
			alphaBuffer[ii] = m_buffer[ii*4 + 3];
		}
	}

	// Build a set of colors so we can figure out how many unique ones the image currently has.
	typedef std::set<DWORD> RGBASet;
	RGBASet setPalette;
	for (DWORD ii = 0; ii < pixelCount; ++ii)
	{
		DWORD newColor = 0;
		newColor = m_buffer[ii*bpp+0];
		newColor += m_buffer[ii*bpp+1] << 8;
		newColor += m_buffer[ii*bpp+2] << 16;

		setPalette.insert(newColor);
	}

	// If 256 colors or less, lets just use this as our palette.
	if (setPalette.size() <= 256)
	{
		::memset(m_palette, 0, 256*3);

		RGBASet::iterator it;
		DWORD ii;
		for (it = setPalette.begin(), ii = 0; it != setPalette.end(); ++it, ++ii) 
		{
			DWORD color = *it;
			m_palette[ii*3 + 0] = BYTE(  color & 0x0000FF );
			m_palette[ii*3 + 1] = BYTE( (color & 0x00FF00)>>8 );
			m_palette[ii*3 + 2] = BYTE( (color & 0xFF0000)>>16 );
		}
	}
	else // More than 256 colors, so...
	{
		// Build a palette for the image using the library function.
		createPalette(rgbPlane, pixelCount, m_palette, 256);
	}

	// Sort the palette.
	SortPalette(m_palette, 256);

	// Palettize the image.
	if (!palettizePlane24to8bit(rgbPlane, pOutputBuffer, pixelCount, m_palette))
	{
		printf("ERROR: Palettization of RGB image failed.\n");
		return false;
	}

	// Clear existing buffer.
	delete[] m_buffer;
	m_buffer = NULL;
	m_bufferBytes = 0;

	// Set the new image.
	if (m_bHasAlpha)
	{
		if (!AllocateBuffer(pixelCount*2))
			return false;
		::memcpy(m_buffer, pOutputBuffer, pixelCount);
		::memcpy(&m_buffer[pixelCount], alphaBuffer, pixelCount);
		delete[] pOutputBuffer;
		delete[] alphaBuffer;
	}
	else
	{
		m_buffer = pOutputBuffer;
		m_bufferBytes = pixelCount;
	}
	m_bPalettized = true;

	// Clean up.
	delete[] rgbPlane;

	return true;
}

void MemImage::Depalettize()
{
	if (!m_bPalettized)
		return;

	// Allocate new buffer.
	DWORD bpp = m_bHasAlpha ? 4 : 3;
	DWORD pixelCount = DWORD(m_width) * DWORD(m_height);
	DWORD bufferNewBytes = pixelCount * bpp;
	BYTE* bufferNew = new BYTE[bufferNewBytes];

	DWORD bufferIx = 0;
	for (DWORD ii = 0; ii < pixelCount; ++ii)
	{
		for (WORD colorIx = 0; colorIx < 3; ++colorIx)
		{
			// The *3 is because m_palette isn't a 2-dimensional array.
			bufferNew[bufferIx++] = m_palette[m_buffer[ii]*3 + colorIx];
		}

		if (m_bHasAlpha)
			bufferNew[bufferIx++] = m_buffer[pixelCount + ii];
	}

	// Apply new buffer.
	delete[] m_buffer;
	m_buffer = bufferNew;
	m_bufferBytes = bufferNewBytes;
	m_bPalettized = false;
}

bool MemImage::RemoveAlpha()
{
	if (!m_bHasAlpha)
		return true;

	DWORD pixelCount = m_width*m_height;
	DWORD newBytes = 0;
	BYTE* newBuffer = NULL;
	if (m_bPalettized)
	{
		// Strip the alpha data off the end of the buffer.
		newBytes = pixelCount;
		newBuffer = new BYTE[newBytes];
		if (NULL == newBuffer)
		{
			LOG("ERROR: Couldn't allocate memory.\n");
			return false;
		}

		::memcpy(newBuffer, m_buffer, newBytes);
	}
	else
	{
		// Convert to an RGB buffer.
		newBytes = pixelCount*3;
		newBuffer = new BYTE[newBytes];
		if (NULL == newBuffer)
		{
			LOG("ERROR: Couldn't allocate memory.\n");
			return false;
		}

		for (DWORD ii = 0; ii < pixelCount; ++ii)
		{
			newBuffer[ii*3+0] = m_buffer[ii*4+0];
			newBuffer[ii*3+1] = m_buffer[ii*4+1];
			newBuffer[ii*3+2] = m_buffer[ii*4+2];
		}
	}
	delete[] m_buffer;
	m_buffer = newBuffer;
	m_bufferBytes = newBytes;

	m_bHasAlpha = false;
	return true;
}

bool MemImage::AddAlpha()
{
	if (m_bHasAlpha)
		return true;

	DWORD pixelCount = m_width*m_height;
	DWORD newBytes = 0;
	BYTE* newBuffer = NULL;
	if (m_bPalettized)
	{
		// Strip the alpha data off the end of the buffer.
		newBytes = pixelCount*2;
		newBuffer = new BYTE[newBytes];
		if (NULL == newBuffer)
		{
			LOG("ERROR: Couldn't allocate memory.\n");
			return false;
		}

		::memcpy(newBuffer, m_buffer, pixelCount);
		::memset(&newBuffer[pixelCount], 0xFF, pixelCount);
	}
	else
	{
		// Convert to an RGBA buffer.
		newBytes = pixelCount*4;
		newBuffer = new BYTE[newBytes];
		if (NULL == newBuffer)
		{
			LOG("ERROR: Couldn't allocate memory.\n");
			return false;
		}

		for (DWORD ii = 0; ii < pixelCount; ++ii)
		{
			newBuffer[ii*4+0] = m_buffer[ii*3+0];
			newBuffer[ii*4+1] = m_buffer[ii*3+1];
			newBuffer[ii*4+2] = m_buffer[ii*3+2];
			newBuffer[ii*4+3] = 0xFF;
		}
	}
	delete[] m_buffer;
	m_buffer = newBuffer;
	m_bufferBytes = newBytes;

	m_bHasAlpha = true;
	return true;
}

bool MemImage::SaveMipDebugImage(const char* pszBaseFilename, const MemImage* mips, int mipCount)
{
	Clear();
	m_width = mips->m_width;
	m_bHasAlpha = mips->m_bHasAlpha;
	m_bPalettized = mips->m_bPalettized;
	if (m_bPalettized)
		::memcpy(m_palette, mips->m_palette, MEMIMAGE_PALETTEBYTES);

	// Calculate height of debug image.
	m_height = 0;
	int ii;
	for (ii = 0; ii < mipCount; ++ii)
		m_height += mips[ii].m_height;

	int bytesPerPixel = 1;
	if (!m_bPalettized)
	{
		if (m_bHasAlpha)
			bytesPerPixel = 4;
		else
			bytesPerPixel = 3;
	}

	// Create the buffer for the image.
	DWORD pitch = m_width * bytesPerPixel;
	if (!AllocateBuffer(m_height * pitch * (m_bPalettized && m_bHasAlpha ? 2 : 1)))
		return false;
	::memset(m_buffer, 0, m_bufferBytes);
	
	// Create the image.
	DWORD destRow = 0;
	// Note: alphaOffsets only valid/used if pal+alpha.
	DWORD alphaOffset = m_width * m_height;
	for (ii = 0; ii < mipCount; ++ii)
	{
		const MemImage* mip = &mips[ii];

		DWORD mipAlphaOffset = mip->m_width * mip->m_height;
		for (DWORD row = 0; row < mip->m_height; ++row, ++destRow)
		{
			::memcpy(&m_buffer[destRow * pitch], 
						&mip->m_buffer[row * mip->m_width * bytesPerPixel], 
						mip->m_width * bytesPerPixel);
			if (m_bPalettized && m_bHasAlpha)
			{
				::memcpy(&m_buffer[alphaOffset + destRow*pitch], 
							&mip->m_buffer[mipAlphaOffset + row * mip->m_width * bytesPerPixel], 
							mip->m_width * bytesPerPixel);
			}
		}
	}

	char pszMipFilename[BLPCONV_MAX_PATH];
	sprintf(pszMipFilename, "%s__mips.png", pszBaseFilename);

	return SaveToPNG(pszMipFilename);
}

bool MemImage::BuildMipmap(const MemImage& sourceMip) 
{
	Clear();

	m_bHasAlpha = sourceMip.m_bHasAlpha;
	m_bPalettized = sourceMip.m_bPalettized;
	m_width = __max(sourceMip.m_width / 2, 1);
	m_height = __max(sourceMip.m_height / 2, 1);
	if (!AllocateBuffer(0))
		return false;

	const BYTE* srcBuff = sourceMip.m_buffer;
	BYTE* destBuff = m_buffer;
	DWORD destWidth = m_width;
	DWORD destHeight = m_height;
	DWORD w = sourceMip.m_width;
	DWORD h = sourceMip.m_height;

    DWORD i, j;
    for (j = 0; j < destHeight; j++)
	{
        for (i = 0; i < destWidth; i++) 
		{
			if (!m_bPalettized)
			{
				int destColor[4];
				int bpp = m_bHasAlpha ? 4 : 3;

				// Determine weights based on alpha.
				float gammaWeights[4] = { 0.25f, 0.25f, 0.25f, 0.25f };
				if (m_bHasAlpha)
				{
					float alphas[4];
					alphas[0] = float(srcBuff[OFFSET_RGB(i*2+0, j*2+0, w, h, 3, bpp)]) / 255.0f;
					alphas[1] = float(srcBuff[OFFSET_RGB(i*2+0, j*2+1, w, h, 3, bpp)]) / 255.0f;
					alphas[2] = float(srcBuff[OFFSET_RGB(i*2+1, j*2+0, w, h, 3, bpp)]) / 255.0f;
					alphas[3] = float(srcBuff[OFFSET_RGB(i*2+1, j*2+1, w, h, 3, bpp)]) / 255.0f;

					float alphaTotal = 0;
					for (int ii = 0; ii < 4; ++ii)
						alphaTotal += alphas[ii];

					if (alphaTotal > 0.1)
					{
						for (int ii = 0; ii < 4; ++ii)
							gammaWeights[ii] = alphas[ii] / alphaTotal;
					}
				}

				// Average the colors of 4 adjacent pixels in the source texture.
				int c;
				for (c = 0; c < bpp; ++c)
				{
					destColor[c] = int(	srcBuff[OFFSET_RGB(i*2+0, j*2+0, w, h, c, bpp)] * gammaWeights[0] + 
										srcBuff[OFFSET_RGB(i*2+0, j*2+1, w, h, c, bpp)] * gammaWeights[1] +
										srcBuff[OFFSET_RGB(i*2+1, j*2+0, w, h, c, bpp)] * gammaWeights[2] + 
										srcBuff[OFFSET_RGB(i*2+1, j*2+1, w, h, c, bpp)] * gammaWeights[3]);
				}

				// Store those colors in the destination texture.
				for (c = 0; c < bpp; ++c)
				{
					DWORD index = OFFSET_RGB(i, j, destWidth, destHeight, c, bpp);
					destBuff[index] = destColor[c];
				}
			}
			else
			{
				// Get the 4 colors from the source image and store them in "colors";
				png_color colors[4];
				int ii;
				for (ii = 0; ii < 4; ++ii)
				{
					BYTE palIx = srcBuff[OFFSET_RGB(i*2+SO[ii][0], j*2+SO[ii][1], w, h, 0, 1)];
					colors[ii].red = m_palette[palIx + 0];
					colors[ii].green = m_palette[palIx + 1];
					colors[ii].blue = m_palette[palIx + 2];
				}

				float alphaTotal = 0;
				float gammaWeights[4] = { 0.25f, 0.25f, 0.25f, 0.25f };
				if (m_bHasAlpha)
				{
					// Determine weights based on alpha.
					float alphas[4];
					for (ii = 0; ii < 4; ++ii)
						alphas[ii] = float(srcBuff[w*h + OFFSET_RGB(i*2+SO[ii][0], j*2+SO[ii][1], w, h, 0, 1)]) / 255.0f;

					for (int ii = 0; ii < 4; ++ii)
						alphaTotal += alphas[ii];

					// Don't bother for very low (or zero) alpha.
					if (alphaTotal > 0.1)
					{
						for (int ii = 0; ii < 4; ++ii)
							gammaWeights[ii] = alphas[ii] / alphaTotal;
					}
				}

				float averageColorFloats[3] = { 0.0f, 0.0f, 0.0f };
				for (ii = 0; ii < 4; ++ii)
				{
					averageColorFloats[0] += colors[ii].red * gammaWeights[ii];
					averageColorFloats[1] += colors[ii].green * gammaWeights[ii];
					averageColorFloats[2] += colors[ii].blue * gammaWeights[ii];
				}
				png_color average;
				average.red = png_byte(averageColorFloats[0]);
				average.green = png_byte(averageColorFloats[1]);
				average.blue = png_byte(averageColorFloats[2]);

				// Figure out which of the 4 colors deviates least from the average.  Not using any fancy
				// system here, just taking the smallest numerical deviation.
				int deltas[4];
				int smallest = 10000;
				int smallestIx = 0;
				for (ii = 0; ii < 4; ++ii)
				{
					deltas[ii] = abs(colors[ii].red - average.red) + abs(colors[ii].green - average.green) + abs(colors[ii].blue - average.blue);
					if (deltas[ii] < smallest)
					{
						smallest = deltas[ii];
						smallestIx = ii;
					}
				}

				destBuff[OFFSET_RGB(i, j, destWidth, destHeight, 0, 1)] = srcBuff[OFFSET_RGB(i*2+SO[smallestIx][0], j*2+SO[smallestIx][1], w, h, 0, 1)];

				if (m_bHasAlpha)
				{
					destBuff[destWidth*destHeight + OFFSET_RGB(i, j, destWidth, destHeight, 0, 1)] = BYTE(255.0f * alphaTotal / 4.0f);
				}
			}
        }
    }

	return true;
}
