/*
 *  blp.c
 *  libblp
 *
 *  Created by Jean-Francois Roy on Wed Nov 19 2003.
 *  Copyright (c) 2003-2004 MacStorm. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/mman.h>

#include "libblp_byteorder.h"

extern unsigned long IJGRead (unsigned char *buf, 
    unsigned long buflen, 
    long *w, 
    long *h, 
    long *bpp, 
    unsigned char *destbuf);

extern unsigned long IJGWrite (unsigned char *buf, 
    unsigned long buflen, 
    unsigned char *destbuf, 
    unsigned long destbuflen, 
    int quality, 
    long image_width, 
    long image_height, 
    long image_planes);

#pragma mark -

#if !defined(BOOL)
	#define BOOL int
	#define TRUE 1
	#define YES TRUE
	#define FALSE 0
	#define NO FALSE
#endif

#if !defined(bool)
	#define bool BOOL
#endif

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

#pragma mark -

struct BLPHeader {
	char ident[4];
	unsigned long compress, nummipmaps, sizex, sizey, pictype, picsubtype;
	unsigned long poffs[16], psize[16];
};
typedef struct BLPHeader BLPHeader;

struct BLPHeader2 {
	char ident[4];
	unsigned long compress, nummipmaps, sizex, sizey, pictype, picsubtype;
	unsigned long poffs[16], psize[16];
	unsigned long JPEGHeaderSize;
};
typedef struct BLPHeader2 BLPHeader2;

typedef struct BLP2Header {
	char ident[4];
	int type;
	char flags[4];
	int width;
	int height;
	int mipmapoffsets[16];
	int mipmaplengths[16];
} BLP2Header;

#if defined(__BIG_ENDIAN__)
struct TGAHeader {
    unsigned char imageIDLength;
    unsigned char colorMapType;
    unsigned char imageType;
    unsigned short colorMapFirstEntryIndex;
    unsigned short colorMapLength;
    unsigned char colorMapEntrySize;
    unsigned short xOrigin;
    unsigned short yOrigin;
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char reserved1 : 2;
	unsigned char topToBottom : 1;
    unsigned char rightToLeft : 1;
    unsigned char alphaChannelBits : 4;
} __attribute__ ((packed));
#else
struct TGAHeader {
    unsigned char imageIDLength;
    unsigned char colorMapType;
    unsigned char imageType;
    unsigned short colorMapFirstEntryIndex;
    unsigned short colorMapLength;
    unsigned char colorMapEntrySize;
    unsigned short xOrigin;
    unsigned short yOrigin;
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char alphaChannelBits : 4;
    unsigned char rightToLeft : 1;
	unsigned char topToBottom : 1;
    unsigned char reserved1 : 2;
} __attribute__ ((packed));
#endif
typedef struct TGAHeader TGAHeader;

struct RGBAPix {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};
typedef struct RGBAPix RGBAPix;

struct PAPix {
	unsigned char i;
	unsigned char A;
};
typedef struct PAPix PAPix;

struct PPix {
	unsigned char i;
};
typedef struct PPix PPix;

typedef RGBAPix palette[256];

#pragma mark -

void SwapTGAHeader(TGAHeader *header) {
    header->colorMapFirstEntryIndex = (unsigned short)_blp_swap_int16(header->colorMapFirstEntryIndex);
    header->colorMapLength = (unsigned short)_blp_swap_int16(header->colorMapLength);
    header->xOrigin = (unsigned short)_blp_swap_int16(header->xOrigin);
    header->yOrigin = (unsigned short)_blp_swap_int16(header->yOrigin);
    header->width = (unsigned short)_blp_swap_int16(header->width);
    header->height = (unsigned short)_blp_swap_int16(header->height);
}

void SwapBLPHeader(BLPHeader *header) {
    header->compress = (unsigned long)_blp_swap_int32(header->compress);
    header->nummipmaps = (unsigned long)_blp_swap_int32(header->nummipmaps);
    header->sizex = (unsigned long)_blp_swap_int32(header->sizex);
    header->sizey = (unsigned long)_blp_swap_int32(header->sizey);
    header->pictype = (unsigned long)_blp_swap_int32(header->pictype);
    header->picsubtype = (unsigned long)_blp_swap_int32(header->picsubtype);
    
    int i = 0;
    for(; i < 16; i++)
    {
        (header->poffs)[i] = (unsigned long)_blp_swap_int32((header->poffs)[i]);
        (header->psize)[i] = (unsigned long)_blp_swap_int32((header->psize)[i]);
    }
}

void SwapBLPHeader2(BLPHeader2 *header) {
    SwapBLPHeader((BLPHeader *)header);
    header->JPEGHeaderSize = (unsigned long)_blp_swap_int32(header->JPEGHeaderSize);
}

void SwapBLP2Header(BLP2Header *header) {
	int i = 0;
	
	//char ident[4];
	header->type = (int)_blp_swap_int32(header->type);
	//char flags[4];
	header->width = (int)_blp_swap_int32(header->width);
	header->height = (int)_blp_swap_int32(header->height);
	
	for(i = 0; i < 16; i++) {
		header->mipmapoffsets[i] = (int)_blp_swap_int32(header->mipmapoffsets[i]);
		header->mipmaplengths[i] = (int)_blp_swap_int32(header->mipmaplengths[i]);
	}
}

#pragma mark -

static BOOL isFileOfType(const char *file, const char *type) {
	char *extension = strrchr(file, '.') + 1;
	if(extension == NULL || *extension == '\0') {
		return FALSE;
	}
	
	if(strcasecmp(extension, type) == 0) {
		return TRUE;
	}
	
	return FALSE;
}

#pragma mark -

static void textureHorizontalFlipInPlace(char *bufsrc, long rowsize, long numlines) {
	char *one_line = (char *)malloc(rowsize);
    long current_line = 0;
	
	unsigned long last_line_offset = (numlines - 1) * rowsize;
	
	unsigned long top_line_offset = 0;
	unsigned long bottom_line_offset = 0;
	
	for(; current_line < numlines; current_line++) {
		top_line_offset = current_line * rowsize;
		bottom_line_offset = last_line_offset - top_line_offset;
		
		// Copy bottom line
		memcpy(one_line, bufsrc + bottom_line_offset, rowsize);
		
		// Push the top line to bottom
		memcpy(bufsrc + bottom_line_offset, bufsrc + top_line_offset, rowsize);
		
		// Push the bottom line to top
		memcpy(bufsrc + top_line_offset, one_line, rowsize);
	}
	
	// Done with line buffer
	free(one_line);
}

static void textureRemoveAlphaChannel(char *bufdest, unsigned long destsize, char *bufsrc, unsigned long srcsize) {
	unsigned long i = 0, j = 0;
	for(i = 0; i < srcsize; i++) {
		if((i+1) % 4 != 0) {
			bufdest[j] = bufsrc[i];
			++j;
		}
	}
}

static void textureInvertRBAndRemoveAlphaChannel(char *bufdest, unsigned long destsize, char *bufsrc, unsigned long srcsize) {
	unsigned long i = 0, j = 0, k = srcsize / 4;
	for(i = 0; i < k; i++) {
		bufdest[j++] = bufsrc[4*i+2];
		bufdest[j++] = bufsrc[4*i+1];
		bufdest[j++] = bufsrc[4*i];
	}
}

static void textureInvertRBInPlace(char *bufsrc, unsigned long srcsize) {
	unsigned long i = 0, k = srcsize / 4;
	char c;
	for(i = 0; i < k; i++) {
		c = bufsrc[4*i];
		bufsrc[4*i] = bufsrc[4*i+2];
		bufsrc[4*i+2] = c;
	}
} 

static char *textureResample(char *srcBuf, long width, long height, long inBitsPerPixel, long outBitsPerPixel, BOOL bgrOutput) {
	char *resampledData = NULL;
	long inBytesPerPixel, outBytesPerPixel;
	
	if(outBitsPerPixel == 0 || srcBuf == NULL) return NULL;
	if(inBitsPerPixel != 32) return NULL;
	if(outBitsPerPixel != 32 && outBitsPerPixel != 24) return NULL;
	
	inBytesPerPixel = inBitsPerPixel / 8;
	outBytesPerPixel = outBitsPerPixel / 8;
	
	resampledData = (char *)malloc(width * height * outBytesPerPixel);
	
	if(inBitsPerPixel == 32 && outBitsPerPixel == 24 && !bgrOutput) {
		textureRemoveAlphaChannel(resampledData, width * height * outBytesPerPixel, srcBuf, width * height * inBytesPerPixel);
	} else if(inBitsPerPixel == 32 && outBitsPerPixel == 24 && bgrOutput) {
		textureInvertRBAndRemoveAlphaChannel(resampledData, width * height * outBytesPerPixel, srcBuf, width * height * inBytesPerPixel);
	} else if(inBitsPerPixel == 32 && outBitsPerPixel == 32 && bgrOutput) {
		memcpy(resampledData, srcBuf, width * height * outBytesPerPixel);
		textureInvertRBInPlace(resampledData, width * height * outBytesPerPixel);
	} else {
		free(resampledData);
		resampledData = NULL;
	}
	
	return resampledData;
}

static unsigned long textureScale(char *srcBuf, long width, long height, long bpp, char *destBuf, long destination_width, long destination_height) {
	// FIXME FIXME
	if(width != destination_width || height != destination_height) return 0;
	
	unsigned long textureLength = (destination_width * destination_height * bpp) / 8;
	if (destBuf == NULL) return textureLength;

	memcpy(destBuf, srcBuf, textureLength);
	return textureLength;
}

#pragma mark -

unsigned long LoadBLP2(char *destBuf, char *srcBuf, long *width, long *height) {
	// Return texture size (will be 0 on error)
	return 0;
}

#pragma mark -

unsigned long LoadBLP(char *destBuf, char *srcBuf, long *width, long *height, unsigned long *type, unsigned long *subtype) {
	BLPHeader blph;
	unsigned long curpos = 0;
	unsigned long textureSize = 0;

	if(srcBuf == NULL) {
		return 0;
	}

	memcpy(&blph, srcBuf+curpos, sizeof(BLPHeader));
	SwapBLPHeader(&blph);
    if (memcmp(blph.ident, "BLP1", 4) != 0) {
		if(width != NULL) *width = 0;
		if(height != NULL) *height = 0;
		if(type != NULL) *type = 0;
		if(subtype != NULL) *subtype = 0;
		return 0;
	}
	
	curpos += sizeof(BLPHeader);
	textureSize = blph.sizex * blph.sizey * 4;
	
	// uncompressed bitmap
	if(blph.compress == 1) {
		if(destBuf==NULL) {
			if(width != NULL) *width = blph.sizex;
			if(height != NULL) *height = blph.sizey;
			if(type != NULL) *type = blph.compress;
			if(subtype != NULL) *subtype = blph.pictype;

			return textureSize;
		}

		RGBAPix Pal[256];
		memcpy(&Pal, srcBuf+curpos, 256*4);
		curpos += 256*4;
		
		// alpha channel
		if(blph.pictype == 4) {
			PAPix *tdata = (PAPix *)(char *)malloc(blph.sizex * blph.sizey * 2);
			memcpy(tdata, srcBuf+curpos, blph.sizex * blph.sizey * 2);
			curpos += blph.sizex * blph.sizey * 2;

			RGBAPix *pic = (RGBAPix *)(char *)malloc(blph.sizex*blph.sizey*4);
			
			unsigned long k, j, i;

			j = 0;
			i = (blph.sizex * blph.sizey) / 2;
			for(k = 0; k < i; ++k) {
				pic[j].R = Pal[tdata[k].i].B;
				pic[j].G = Pal[tdata[k].i].G;
				pic[j].B = Pal[tdata[k].i].R;
				pic[j].A = tdata[k+i].i;
				++j;

				pic[j].R = Pal[tdata[k].A].B;
				pic[j].G = Pal[tdata[k].A].G;
				pic[j].B = Pal[tdata[k].A].R;
				pic[j].A = tdata[k+i].A;
				++j;
			}

			free(tdata);
			
			memcpy(destBuf, pic, textureSize);
			free(pic);
			
			if(width != NULL) *width = blph.sizex;
			if(height != NULL) *height = blph.sizey;
			if(type != NULL) *type = blph.compress;
			if(subtype != NULL) *subtype = blph.pictype;

			return textureSize;
		}
		
		// no alpha channel
		if(blph.pictype == 5) {
			PPix *tdata = (PPix *)(char *)malloc(blph.sizex * blph.sizey);
			memcpy(tdata, srcBuf + curpos, blph.sizex * blph.sizey);
			curpos+=blph.sizex*blph.sizey;

			RGBAPix *pic = (RGBAPix *)(char *)malloc(textureSize);
			
			unsigned long k,i;

			i=blph.sizex*blph.sizey;
			for(k = 0; k < i; ++k) {
				pic[k].R = Pal[tdata[k].i].B;
				pic[k].G = Pal[tdata[k].i].G;
				pic[k].B = Pal[tdata[k].i].R;
				pic[k].A = 255-Pal[tdata[k].i].A;
			}

			free(tdata);
			
			memcpy(destBuf, pic, textureSize);
			free(pic);
			
			if(width != NULL) *width = blph.sizex;
			if(height != NULL) *height = blph.sizey;
			if(type != NULL) *type = blph.compress;
			if(subtype != NULL) *subtype = blph.pictype;

			return textureSize;
		}
	}
	
	// JPEG compressed
	if(blph.compress == 0) {
		if(destBuf == NULL) {
			if(width != NULL) *width = blph.sizex;
			if(height != NULL) *height = blph.sizey;
			if(type != NULL) *type = blph.compress;
			if(subtype != NULL) *subtype = blph.pictype;

			return textureSize;
		}
		
		long JPEGHeaderSize;
		memcpy(&JPEGHeaderSize, srcBuf + curpos, 4);
        JPEGHeaderSize = _blp_swap_int32(JPEGHeaderSize);
		curpos += 4;
		
		unsigned char *tdata = (unsigned char*)malloc(blph.psize[0] + JPEGHeaderSize);
		memcpy(tdata, srcBuf+curpos, JPEGHeaderSize);
		
		curpos=blph.poffs[0];
		memcpy((tdata+JPEGHeaderSize), srcBuf+curpos, blph.psize[0]);

		if(IJGRead(tdata, blph.psize[0] + JPEGHeaderSize, NULL, NULL, NULL, (unsigned char*)destBuf) == 0) {
			free(tdata);
			if(width != NULL) *width = 0;
			if(height != NULL) *height = 0;
			if(type != NULL) *type = 0;
			if(subtype != NULL) *subtype = 0;

			return (0);
		}
		
		free(tdata);
		
		// Output should be RGBA, BLPs use BGRA
		textureInvertRBInPlace(destBuf, textureSize);

		if(width != NULL)	*width = blph.sizex;
		if(height != NULL) *height = blph.sizey;
		if(type != NULL) *type = blph.compress;
		if(subtype != NULL) *subtype = blph.pictype;

		return textureSize;
	}

	if(width != NULL)	*width = 0;
	if(height != NULL) *height = 0;
	if(type != NULL) *type = 0;
	if(subtype != NULL) *subtype = 0;

	return 0;
}

unsigned long MakeBLP(char *destBuf, char *srcBuf, long width, long height, int quality) {
	BLPHeader2 blph;
	long blphsize = sizeof(BLPHeader2);
	unsigned long JPEGHeaderSize = 10L;
	unsigned long padsize = 0L;
	
	memset(&blph, 0, blphsize);
	
	BOOL bSizeOnly = FALSE;
	if(destBuf == NULL) {
		bSizeOnly = TRUE;
	}
    
	long nummipmapstocompute = 0;
	long t = min(width, height);
	while(t % 2 == 0) {
		t /= 2;
		++nummipmapstocompute;
	}
	
	// FIXME FIXME
	nummipmapstocompute = 1;

	if(t != 1) return 0;

	t = max(width, height);
	while(t % 2 == 0) t /= 2;
	if(t != 1) return 0;

	blph.compress = 0; // yes, compressed
	memcpy(blph.ident, "BLP1", 4);
	blph.nummipmaps = nummipmapstocompute;
	blph.picsubtype = 1;
	blph.pictype = 4;
	blph.sizex = width;
	blph.sizey = height;
	memset(&blph.poffs, 0, 16*4);
	memset(&blph.psize, 0, 16*4);
	blph.JPEGHeaderSize = JPEGHeaderSize;

	char *currentTexture;
	char *resizedTexture;
	
	unsigned long nn;
	int k = 0;

	long curWidth = width;
	long curHeight = height;
	
	// Input is assumed to be RGBA
	// BLPs want, however, BGRA
	char *sourceTexture = malloc(width * height * 4);
	memcpy(sourceTexture, srcBuf, width * height * 4);
	textureInvertRBInPlace(sourceTexture, width * height * 4);

	for(k = 0; k < nummipmapstocompute + 2; ++k) {
		nn = textureScale(sourceTexture, width, height, 32, NULL, curWidth, curHeight);
		resizedTexture = (char *)malloc(nn);
		textureScale(sourceTexture, width, height, 32, resizedTexture, curWidth, curHeight);
		
		nn = IJGWrite((unsigned char*)resizedTexture, 10000 + curWidth * curHeight * 4, NULL, 0, quality, curWidth, curHeight, 4);

		if (nn <= JPEGHeaderSize) {
			goto perilexit;
		}

		currentTexture = (char *)malloc(nn);
		IJGWrite((unsigned char*)resizedTexture, 10000 + curWidth * curHeight * 4, (unsigned char*)currentTexture, nn, quality, curWidth, curHeight, 4);

		if(k == 0) {
			blph.poffs[0] = blphsize + JPEGHeaderSize + padsize;
			blph.psize[0] = nn - JPEGHeaderSize;
		} else {
			blph.poffs[k] = blph.poffs[k-1] + blph.psize[k-1];
			blph.psize[k] = nn - JPEGHeaderSize;
		}		

		if(bSizeOnly == FALSE) {
			memcpy(destBuf + blph.poffs[k], currentTexture + JPEGHeaderSize, nn - JPEGHeaderSize);
		}
		
		if((k == 0) && (bSizeOnly == FALSE)) {
			memcpy(destBuf + blphsize, currentTexture, JPEGHeaderSize);
			memset(destBuf + blphsize + JPEGHeaderSize, 0, padsize);
		}

		free(currentTexture);
		free(resizedTexture);

		curWidth /= 2; if(curWidth < 1) curWidth = 1;
		curHeight /= 2; if(curHeight < 1) curHeight = 1;
	}

perilexit:

	free(sourceTexture);
	
	if(bSizeOnly == FALSE) {
		memcpy(destBuf, &blph, blphsize);
        SwapBLPHeader2((BLPHeader2 *)destBuf);
    }

	return blph.poffs[k-1] + blph.psize[k-1];
}

#pragma mark -

unsigned long LoadTGA(char *destbuf, char *srcbuf, long *width, long *height, long *bpp, long *type) {    
	TGAHeader header;
    
    unsigned long bytesPerPixel;
	unsigned long imageSize;

	unsigned long curpos = 0;
    
	memcpy((char *)&header, srcbuf, sizeof(TGAHeader));
    SwapTGAHeader(&header);
    curpos+=sizeof(TGAHeader);
    
    if( header.colorMapType != 0 ||
        (header.imageType != 2 && header.imageType != 10) ||
        header.xOrigin != 0 ||
        header.yOrigin != 0 ||
        header.width == 0 ||
        header.height == 0 ||
        (header.bpp != 32 && header.bpp != 24))
	{
	   if(width != NULL) *width = 0;
	   if(height != NULL) *height = 0;
	   if(bpp != NULL) *bpp = 0;
       if(type != NULL) *type = 0;
	   return 0;
	}

	if(width != NULL) *width = header.width;
	if(height != NULL) *height = header.height;
	if(bpp != NULL) *bpp = 32; // if we have 24, we'll add an 8 bit alpha channel to each pixel
	if(type!=NULL) *type = header.imageType;

	bytesPerPixel = header.bpp / 8;
	imageSize = header.width * header.height * 4; // if we have 24, we'll add an 8 bit alpha channel to each pixel
	
	if(destbuf == NULL) return imageSize;
	
	// Make sure the dimensions are powers of 2
    long t = min(header.width, header.height);
	while(t % 2 == 0) t /= 2;

	if(t != 1) {
	   if(width != NULL) *width = 0;
	   if(height != NULL) *height = 0;
	   if(bpp != NULL) *bpp = 0;
       if(type != NULL) *type = 0;
	   return 0;
	}	

	t = max(header.width, header.height);
	while(t % 2 == 0) t /= 2;

	if(t!=1) {
	   if(width != NULL) *width = 0;
	   if(height != NULL) *height = 0;
	   if(bpp != NULL) *bpp = 0;
       if(type != NULL) *type = 0;
	   return 0;
	}
	
	// Increment curpos to skip the image ID field if it's there
    curpos += header.imageIDLength;
    
    if(header.imageType == 2) {
        if(bytesPerPixel == 4) {
            memcpy(destbuf, srcbuf+curpos, imageSize);
            curpos+=imageSize;
        } else if(bytesPerPixel == 3) {
            // We have to process each pixel
            char *sourcePtr = srcbuf+curpos;
            char *destPtr = destbuf;
            
            while((unsigned long)(destPtr - destbuf) < imageSize) {
                memcpy(destPtr, sourcePtr, bytesPerPixel);
                destPtr += bytesPerPixel;
                sourcePtr += bytesPerPixel;
                *(destPtr++) = 0xFF; // Fully opaque
            }
        }
    } else if(header.imageType == 10) {
        // RLE compressed
        
        char *sourcePtr = srcbuf+curpos;
        char *destPtr = destbuf;
        RGBAPix onePixel = {0, 0, 0, 0xFF}; // 4 bytes, fully opaque alpha
        unsigned long i = 0;
        unsigned char rlCount = 0;
        
        while((unsigned long)(destPtr - destbuf) < imageSize) {
            if(*sourcePtr & 0x80) {
                // Run-length packet
                rlCount = (*sourcePtr & 0x7F) + 1;
                memcpy(&onePixel, ++sourcePtr, bytesPerPixel);
                for(i = 0; i < rlCount; i++) {
                    memcpy(destPtr, &onePixel, 4);
                    destPtr += 4;
                }
                
                sourcePtr += bytesPerPixel;
            } else {
                // Raw packet
                rlCount = (*sourcePtr & 0x7F) + 1;
                sourcePtr++;
                
                if(bytesPerPixel == 4) {
                    memcpy(destPtr, sourcePtr, rlCount * bytesPerPixel);
                    sourcePtr += rlCount * bytesPerPixel;
                    destPtr += rlCount * bytesPerPixel;
                } else if(bytesPerPixel == 3) {
                    // We have to process each pixel
                    for(i = 0; i < rlCount; i++) {
                        memcpy(destPtr, sourcePtr, bytesPerPixel);
                        destPtr += bytesPerPixel;
                        sourcePtr += bytesPerPixel;
                        *(destPtr++) = 0xFF; // Fully opaque
                    }
                }
            }
        }
    }
                
	if(!(header.topToBottom)) {
		// Need to flip
		textureHorizontalFlipInPlace(destbuf, header.width * 4, header.height);
	}
	
	// Targas are BGR, so convert to RGB (preserving the alpha channel, of course)
	textureInvertRBInPlace(destbuf, imageSize);
	
	return imageSize;
}

unsigned long MakeTGA(char *destBuf, char *srcBuf, long width, long height, long bpp) {
	unsigned long curpos = 0;

	// Only support 24 or 32 bits per pixel output formats
	if(bpp != 24 && bpp != 32) return 0;
	
	TGAHeader header;
    header.imageIDLength = 0;
    header.colorMapType = 0;
    header.imageType = 2;
    header.colorMapFirstEntryIndex = 0;
    header.colorMapLength = 0;
    header.colorMapEntrySize = 0;
    header.xOrigin = 0;
    header.yOrigin = 0;
    header.width = width;
    header.height = height;
    header.bpp = bpp;
    if(bpp == 32) {
        header.alphaChannelBits = 8;
	} else if(bpp == 24) {
        header.alphaChannelBits = 0;
	}
	header.rightToLeft = 0;
	header.topToBottom = 1;
	header.reserved1 = 0;
    
    // Make sure the dimensions are powers of 2
    long t = min(header.width, header.height);
	while(t % 2 == 0) t /= 2;
	if(t != 1) return 0;

	t = max(header.width, header.height);
	while(t % 2 == 0) t /= 2;
	if(t != 1) return 0;
	
	int colorMode = bpp / 8;
	if (destBuf == NULL) return sizeof(TGAHeader) + width * height * colorMode;
	
	SwapTGAHeader(&header);
    memcpy(destBuf, &header, sizeof(TGAHeader));
    curpos += sizeof(TGAHeader);
	
	// Input is assumed to be RGBA
	// Targas are BGR(A)
	char *textureData = textureResample(srcBuf, width, height, 32, bpp, YES);
	if(!textureData) return 0;
	
	memcpy(destBuf + curpos, textureData, width * height * colorMode);
	free(textureData);
	
	return sizeof(TGAHeader) + width * height * colorMode;
}

#pragma mark -

unsigned long MakeJPG(char *destBuf, char *srcBuf, long width, long height, int quality) {
	unsigned long jpgsize = 0;
	char *textureData = NULL;
	
	// Input is assumed to be RGBA
	// JPEG wants RGB
	textureData = textureResample(srcBuf, width, height, 32, 24, NO);
	if(!textureData) return 0;
	
	jpgsize = IJGWrite((unsigned char*)textureData, width * height * 3, NULL, 0, quality, width, height, 3);
	
	if(!destBuf) {
		free(textureData);
		return jpgsize;
	}
	
	IJGWrite((unsigned char*)textureData, width * height * 3, (unsigned char*)destBuf, jpgsize, quality, width, height, 3);
	free(textureData);
	
	return jpgsize;
}

#pragma mark -

int BLP2Everything(char *srcbuf, unsigned long srcbuflen, const char *szFileDest) {
	int hFile;
	long r, w, h;
	
	char *rawImageData = NULL;
	unsigned long rawImageSize = 0;
	
	char *processedImageData = NULL;
	unsigned long processedImageSize = 0;
	
	rawImageSize = LoadBLP(NULL, srcbuf, &w, &h, NULL, NULL);
	rawImageData = (char *)malloc(rawImageSize);
	
	LoadBLP(rawImageData, srcbuf, &w, &h, NULL, NULL);
	
	// !!! Assume image data to have 32 bits per pixel !!!

	int quality = 100;
	if(isFileOfType(szFileDest, "jpg")) {
		processedImageSize = MakeJPG(NULL, rawImageData, w, h, quality);
		processedImageData = malloc(processedImageSize);
		MakeJPG(processedImageData, rawImageData, w, h, quality);
	} else if(isFileOfType(szFileDest, "tga")) {
		processedImageSize = MakeTGA(NULL, rawImageData, w, h, 32);
		processedImageData = malloc(processedImageSize);
		MakeTGA(processedImageData, rawImageData, w, h, 32);
	} else {
		free(rawImageData);
		return 0;
	}
	
	free(rawImageData);
        
	hFile = open(szFileDest, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(hFile == -1) return 0;

	r = pwrite(hFile, processedImageData, processedImageSize, 0);
	close(hFile);
		
	free(processedImageData);
	return 1;
}
