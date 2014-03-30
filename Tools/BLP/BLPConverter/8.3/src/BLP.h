#ifndef BLP_H
#define BLP_H

///////////////////////////////////////////////////////////////////////////////
// BLP Stuff

const char BLPID[4] = {'B', 'L', 'P', '2'};

struct BLPHeader
{
	char id[4];
	unsigned int version;
	unsigned char encoding;			// 1 = palettized, 2 = DXT
	unsigned char alphaBitDepth;	// so far, 0, 1, or 8
	unsigned char alphaEncoding;	// 7 indicates DXT5, 1 is DXT3.  
	unsigned char hasMips;			// 0 = no mips, 1 = has mips
	unsigned int xResolution;
	unsigned int yResolution;
	unsigned int mipOffsets[16];
	unsigned int mipSizes[16];
};

enum
{
	BLP_ENCODING_INVALID,
	BLP_ENCODING_PALETTIZED,
	BLP_ENCODING_COMPRESSED,
	BLP_ENCODING_COUNT
};

static const char* encodingStrings[] =
{
	"[Invalid]",
	"Palettized",
	"Compressed",
};

#endif // BLP_H