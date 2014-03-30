#ifndef MEMIMAGE_H
#define MEMIMAGE_H

#define BLPCONV_MAX_PATH 260
#define LOG ::printf

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#define NULL 0

#define MEMIMAGE_PALETTEBYTES (3*256)

/*
enum BLPType
{
	BLPTYPE_UNSPECIFIED,
	BLPTYPE_PAL_ALPHA0,		// compression = 0, alphaBitDepth = 0
	BLPTYPE_PAL_ALPHA1,		// compression = 0, alphaBitDepth = 1
	BLPTYPE_PAL_ALPHA4,		// compression = 0, alphaBitDepth = 4	- This has been seen but only on alpha-less textures.
	BLPTYPE_PAL_ALPHA8,		// compression = 0, alphaBitDepth = 8
	BLPTYPE_DXT1_ALPHA0,	// (DXT1) compression = 1, alphaBitDepth = 0
	BLPTYPE_DXT1_ALPHA1,	// (DXT1) compression = 1, alphaBitDepth = 1
	BLPTYPE_DXT3,			// (DXT3) compression = 1, alphaBitDepth = 8
	BLPTYPE_DXT5,			// (DXT5) compression = 1, alphaBitDepth = 8, alphaUnknown = 7
	BLPTYPE_COUNT
};
// NOTE: These arrays need to be updated when the above enum changes.
const char* BLPTypeDescriptions[];
const char* BLPTypeNames[];

enum PNGType
{
	PNGTYPE_UNSPECIFIED,
	PNGTYPE_PALETTIZED,
	PNGTYPE_PALETTIZED_ALPHAMASK,
	PNGTYPE_RGB,
	PNGTYPE_RGBA,
	PNGTYPE_COUNT
};
// NOTE: These arrays need to be updated when the above enum changes.
const char* PNGTypeDescriptions[];
const char* PNGTypeNames[];
*/

enum FORMATID
{
	FORMAT_UNSPECIFIED,

	BLPTYPE_FIRST,
		BLPTYPE_PAL_ALPHA0 = BLPTYPE_FIRST,	// 1 - compression = 0, alphaBitDepth = 0
		BLPTYPE_PAL_ALPHA1,					// 2 - compression = 0, alphaBitDepth = 1
		BLPTYPE_PAL_ALPHA4,					// 3 - compression = 0, alphaBitDepth = 4
		BLPTYPE_PAL_ALPHA8,					// 4 - compression = 0, alphaBitDepth = 8
		BLPTYPE_DXT1_ALPHA0,				// 5 - (DXT1) compression = 1, alphaBitDepth = 0
		BLPTYPE_DXT1_ALPHA1,				// 6 - (DXT1) compression = 1, alphaBitDepth = 1
		BLPTYPE_DXT3,						// 7 - (DXT3) compression = 1, alphaBitDepth = 8
		BLPTYPE_DXT5,						// 8 - (DXT5) compression = 1, alphaBitDepth = 8, alphaUnknown = 7
	BLPTYPE_LAST = BLPTYPE_DXT5,

	PNGTYPE_FIRST,
		PNGTYPE_PALETTIZED = PNGTYPE_FIRST,
		PNGTYPE_PALETTIZED_ALPHAMASK,
		PNGTYPE_RGB,
		PNGTYPE_RGBA,
	PNGTYPE_LAST = PNGTYPE_RGBA,

	FORMAT_COUNT
};
const char* FORMATIDNames[];
const char* FORMATIDDescriptions[];

#define ISBLP(format) (BLPTYPE_FIRST <= format && format <= BLPTYPE_LAST)
#define ISPNG(format) (PNGTYPE_FIRST <= format && format <= PNGTYPE_LAST)

enum FileType
{
	FILETYPE_UNSPECIFIED,
	FILETYPE_PNG,
	FILETYPE_BLP,
	FILETYPE_COUNT
};

class MemImage
{
public:
	// Public settings.
	static bool s_bCreateMipTestImage;
	static float s_fGammaFactor;	// Legacy.
	static BYTE s_byAlphaThreshold;
	static bool s_bVerbose;
	static bool s_bNoMips;
	static FORMATID s_ruleTable[FORMAT_COUNT];

protected:
	DWORD m_width;
	DWORD m_height;

	// These two variables define the 4 formats of data MemImage can contain.
	bool m_bPalettized;
	bool m_bHasAlpha;

	// This contains the actual data in the following formats:
	// - m_bPalettized == true && m_bHasAlpha = false -> 1-byte/entry
	// - m_bPalettized == true && m_bHasAlpha = true -> 1-byte/entry, then 1-alpha byte/entry [image buffer][alpha buffer]
	// - m_bPalettized == false && m_bHasAlpha == false -> 3 bpp rgb
	// - m_bPalettized == false && m_bHasAlpha == true -> 4 bpp rgba
	BYTE* m_buffer;
	DWORD m_bufferBytes;

	BYTE m_palette[MEMIMAGE_PALETTEBYTES]; // RGB

public:
	MemImage();
	MemImage(const MemImage& rhs);
	~MemImage();
	MemImage& operator=(const MemImage& rhs);
	bool Init(DWORD width, DWORD height, bool hasAlpha, bool palettized);
	void Copy(const MemImage& rhs);
	void Clear();
	// If bytes == 0 then it calculates the proper size with the CalculateBufferBytes function.
	bool AllocateBuffer(DWORD bytes);
	bool HasAlpha() { return m_bHasAlpha; }
	bool IsPalettized() { return m_bPalettized; }

	// Load functions.
	bool LoadFromBLP(const char* filename, FORMATID* blpTypeRet = NULL);
	bool LoadFromPNG(const char* filename, FORMATID* pngTypeRet = NULL);

	// Save functions.  All are guaranteed not to change the image data at all.  If conversions
	// need to be made a temp image will be created.
	bool Save(const char* filename, FORMATID type) const;
	bool SaveToBLP(const char* filename, FORMATID type = FORMAT_UNSPECIFIED) const;
	bool SaveToPNG(const char* filename, FORMATID type = FORMAT_UNSPECIFIED) const;

	// If palettizes or ff already palettized returns true.  Returns false on error.
	bool Palettize();
	// Does nothing if not palettized.
	void Depalettize();

	// Returns false on error, true otherwise.  No error if it does nothing.
	bool RemoveAlpha();
	bool AddAlpha();

	bool BuildMipmap(const MemImage& sourceMip);
	bool SaveMipDebugImage(const char* pszBaseFilename, const MemImage* mips, int mipCount);

public:
	static DWORD CalculateBufferBytes(DWORD width, DWORD height, bool bHasAlpha, bool bPalettized)
	{
		DWORD pixelCount = width*height;
		DWORD bpp = 1;
		if (bPalettized)
		{
			if (bHasAlpha)
				bpp = 2;
		}
		else
		{
			if (bHasAlpha)
				bpp = 4;
			else
				bpp = 3;
		}
		return pixelCount * bpp;
	}
};

#endif // MEMIMAGE_H