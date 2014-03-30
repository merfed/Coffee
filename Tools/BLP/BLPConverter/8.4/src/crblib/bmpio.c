
#include <stdio.h>
#include <ctype.h>
#include "BMPio.h"
#include "strutil.h"
#include "fileutil.h"
#include "imutil.h"

#ifdef _MSC_VER
#pragma warning( disable : 4018 )	// signed/unsigned mismatch
#endif

#ifndef ispow2
#define ispow2(x) (!( (x) & ~(-(x)) ))
#endif

void swapBGRtoRGB(ubyte *ptr,int len);

#pragma pack(1)
typedef struct 
{
	ulong      biSize;
	long       biWidth;
	long       biHeight;
	uword      biPlanes;
	uword      biBitCount;
	ulong      biCompression;
	ulong      biSizeImage;
	long       biXPelsPerMeter;
	long       biYPelsPerMeter;
	ulong      biClrUsed;
	ulong      biClrImportant;
} BITMAPINFOHEADER;

typedef struct 
{
	uword   bfType;
	ulong   bfSize;
	uword   bfReserved1;
	uword   bfReserved2;
	ulong   bfOffBits;
} BITMAPFILEHEADER;

typedef struct 
{
    ubyte    B;
    ubyte    G;
    ubyte    R;
    ubyte    rgbReserved;
} RGBQUAD;
#pragma pack()

void freeBMP(myBMP * mb)
{
	if ( mb ) 
	{
		smartfree(mb->palette);
		smartfree(mb->data);
		free(mb);
	}
}

#define BM_TAG	0x4D42		// 'BM'

myBMP * loadImage(char *name)
{
FILE * fp;

	if ( (fp = fopen(name,"rb")) == NULL )
		return NULL;

	/** go through the types I know how to read **/

	/*#*/ 
	{
	BITMAPFILEHEADER bmfh;

	if ( FReadOk(fp, &bmfh, sizeof(BITMAPFILEHEADER)) ) 
	{
		if ( bmfh.bfType == BM_TAG ) 
		{
			fclose(fp);
			return loadBMP(name);
		}
	}
	fseek(fp,0,SEEK_SET);
	/*#*/ }

	fclose(fp);
	return loadRAW(name);
}

myBMP * loadRAW(char *name)
{
FILE * fp;
myBMP *mybm;
int planes;
	
	if ( ( mybm = new(myBMP)) == NULL )
		return NULL;

	guessRawSize(name,&(mybm->width),&(mybm->height),&planes);

	switch(planes) 
	{
		case 1:
			mybm->type = MBTYPE_8BIT_GRAY;
			break;
		case 3:
			mybm->type = MBTYPE_24BIT_RGB;
			break;
		default:
			free(mybm);
			return NULL;
	}

	mybm->dataBytes = (mybm->width)*(mybm->height)*planes;

	if ( mybm->dataBytes < 1 ) 
	{
		free(mybm);
		return NULL;
	}

	if ( (fp = fopen(name,"rb")) == NULL )  
	{
		free(mybm);
		return NULL;
	}

	if ( (mybm->data = malloc(mybm->dataBytes)) == NULL ) 
	{ 
		fclose(fp); 
		free(mybm->data); 
		free(mybm);
		return NULL; 
	}		

	if ( ! FReadOk(fp, mybm->data , mybm->dataBytes ) ) 
	{ 
		fclose(fp); 
		free(mybm->data); 
		free(mybm);
		return NULL; 
	}
		
	fclose(fp);

return mybm;
}

bool saveRAW(myBMP *mybm,char *name)
{
FILE * fp;

	if ( (fp = fopen(name,"wb")) == NULL ) return false;

	if ( ! FWriteOk(fp, mybm->data , mybm->dataBytes ) ) 
	{
	fclose(fp); return false; 
	}
		
	fclose(fp);
	
return true;
}

bool saveBMP(myBMP *mybm,char *name)
{
BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;
FILE * fp;
ulong my_row_bytes,bmp_row_bytes;
int palSize;

	if ( (fp = fopen(name,"wb")) == NULL ) return false;

	bmih.biSize = sizeof(BITMAPINFOHEADER);

	bmih.biWidth = mybm->width;
#if 0
	bmih.biHeight= - (mybm->height);
#else
//	 this needs to be negative, but the morons at Adobe can't handle
//		negative height BMPs !
	bmih.biHeight= mybm->height;
#endif

	bmih.biPlanes = 1;
	bmih.biCompression = 0;
	bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 10000;
	bmih.biClrImportant = bmih.biClrUsed = 0;

	switch(mybm->type) 
	{
		case MBTYPE_8BIT_PAL	:
			bmih.biBitCount = 8; palSize = 1024;
			bmih.biClrImportant = bmih.biClrUsed = mybm->ncolors;
			assert( mybm->ncolors <= 256 );
			break;
		case MBTYPE_8BIT_GRAY	:
			bmih.biBitCount = 8; palSize = 1024;
			break;
		case MBTYPE_24BIT_RGB	:
			bmih.biBitCount = 24; palSize = 0;
			break;
	}

	if (bmih.biBitCount == 24)	my_row_bytes = bmih.biWidth * 3;
	else						my_row_bytes = bmih.biWidth;
	bmp_row_bytes = (my_row_bytes + 3)&(~3);
	bmih.biSizeImage = bmp_row_bytes * abs(bmih.biHeight);

	bmfh.bfType = BM_TAG;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + bmih.biSize + bmih.biSizeImage 
					+ palSize ;
	bmfh.bfOffBits = bmfh.bfSize - bmih.biSizeImage ;

	if ( ! FWriteOk(fp, &bmfh , sizeof(BITMAPFILEHEADER) ) ) { fclose(fp); return false; }
	if ( ! FWriteOk(fp, &bmih , sizeof(BITMAPINFOHEADER) ) ) { fclose(fp); return false; }

	if ( mybm->type != MBTYPE_24BIT_RGB ) 
	{
		RGBQUAD pal[256];
		int i;
		if ( mybm->type == MBTYPE_8BIT_PAL && mybm->palette ) 
		{
			for(i=0;i<256;i++) 
			{
				pal[i].R = mybm->palette[i*3 + 0];
				pal[i].G = mybm->palette[i*3 + 1];
				pal[i].B = mybm->palette[i*3 + 2];
			}
		}
		else 
		{
			for(i=0;i<256;i++) 
			{
				pal[i].R = pal[i].G = pal[i].B = i;
			}
		}
		if ( ! FWriteOk(fp, pal , 1024 ) ) { fclose(fp); return false; }
	}

	// <> we don't handle row padding correctly at all

	if ( bmih.biHeight > 0 )
	{
		ubyte *rowPtr;
		rowPtr = mybm->data + my_row_bytes * (mybm->height - 1);
		while( rowPtr >= mybm->data )
		{
			if ( mybm->type == MBTYPE_24BIT_RGB )
				swapBGRtoRGB(rowPtr,my_row_bytes);

			if ( ! FWriteOk(fp,rowPtr,bmp_row_bytes) ) { fclose(fp); return false; }
	
			if ( mybm->type == MBTYPE_24BIT_RGB )
				swapBGRtoRGB(rowPtr,my_row_bytes);

			rowPtr -= my_row_bytes;
		}
	}
	else
	{
		ubyte *rowPtr;
		int y;
		rowPtr = mybm->data;
		for(y=0;y<mybm->height;y++)
		{
			if ( mybm->type == MBTYPE_24BIT_RGB )
				swapBGRtoRGB(rowPtr,my_row_bytes);

			if ( ! FWriteOk(fp,rowPtr,bmp_row_bytes) ) { fclose(fp); return false; }
	
			if ( mybm->type == MBTYPE_24BIT_RGB )
				swapBGRtoRGB(rowPtr,my_row_bytes);

			rowPtr += my_row_bytes;
		}
	}

	fclose(fp);
	
return true;
}

myBMP * loadBMP(char *name)
{
FILE * inFile = NULL;
myBMP * mybm = NULL;
BITMAPINFOHEADER bmih;
BITMAPFILEHEADER bmfh;
ubyte bmpinfoheader[64];
ulong headerSize;
int mapentrysize = 0;		/* 0 indicates no colormap */
long bPad;
ulong my_row_bytes,bmp_row_bytes;

#define LBM_cleanUp(err) do { if ( inFile ) fclose(inFile); if ( mybm ) freeBMP(mybm); return( NULL); } while(0)

#define GET_2B(array,offset) ((uword) (ubyte)(array[offset+0]) + \
						 	(((uword) (ubyte)(array[offset+1])) << 8))
#define GET_4B(array,offset) ((ulong) (ubyte)(array[offset+0]) + \
						 	(((ulong) (ubyte)(array[offset+1])) << 8) + \
						 	(((ulong) (ubyte)(array[offset+2])) << 16) + \
						 	(((ulong) (ubyte)(array[offset+3])) << 24))
	
	if ( (inFile = fopen(name,"rb")) == NULL ) 
		LBM_cleanUp("fopen");

	/* Read and verify the bitmap file header */

	if (! FReadOk(inFile, &bmfh, sizeof(BITMAPFILEHEADER)))
		LBM_cleanUp("read short");

	if ( bmfh.bfType != BM_TAG )
		LBM_cleanUp("bmp sign");

	bPad = bmfh.bfOffBits;

	/* We ignore the remaining fileheader fields */

	/* The infoheader might be 12 bytes (OS/2 1.x), 40 bytes (Windows),
	 * or 64 bytes (OS/2 2.x).	Check the first 4 bytes to find out which.
	 */
	if (! FReadOk(inFile, bmpinfoheader, 4))
		LBM_cleanUp("read short");

	headerSize = (ulong) GET_4B(bmpinfoheader,0);

	if (headerSize < 12 || headerSize > 64)
		LBM_cleanUp("bad header size");

	if (! FReadOk(inFile, bmpinfoheader+4, headerSize-4))
		LBM_cleanUp("read short");

	if ( ( mybm = new(myBMP)) == NULL )
		LBM_cleanUp("malloc");

	memcpy((char *)(&(bmih)),bmpinfoheader,	min(sizeof(BITMAPINFOHEADER),headerSize));

	switch (bmih.biBitCount) 
	{
		case 8:			/* colormapped image */
			mapentrysize = 4;		/* Windows uses RGBQUAD colormap */
			if ( bmih.biClrUsed == 0 ) bmih.biClrUsed = 256;
			break;
		case 24:			/* RGB image */
			mapentrysize = 0;
			break;
		default:
			LBM_cleanUp("bad depth");
			break;
	}

	/* Compute distance to bitmap data --- will adjust for colormap below */
	bPad -= (headerSize + 14);

	/* Read the colormap, if any */
	if (mapentrysize > 0 && bmih.biClrUsed > 0) 
	{
		RGBQUAD pal[256];
		int i;

		mybm->ncolors = bmih.biClrUsed;
		if ( mybm->ncolors > 256 ) mybm->ncolors  = 256;

		if ( (mybm->palette = malloc(256*3)) == NULL )
			LBM_cleanUp("malloc");

		if (! FReadOk(inFile, pal , (mapentrysize * mybm->ncolors)))
			LBM_cleanUp("read short");

		for(i=0;i<mybm->ncolors;i++) 
		{
			mybm->palette[i*3 + 0] = pal[i].R;
			mybm->palette[i*3 + 1] = pal[i].G;
			mybm->palette[i*3 + 2] = pal[i].B;
		}

		/* account for size of colormap */
		bPad -= mybm->ncolors * mapentrysize;
	}

	/* Skip any remaining pad bytes */
	if (bPad < 0)			/* incorrect bfOffBits value? */
		LBM_cleanUp("bad header length");
	else if ( bPad > 0 ) 
	{
		fseek(inFile,bPad,SEEK_CUR);
	}

	/* Compute row width in file, including padding to 4-byte boundary */
	if (bmih.biBitCount == 24)	my_row_bytes = bmih.biWidth * 3;
	else						my_row_bytes = bmih.biWidth;

	bmp_row_bytes = (my_row_bytes+3)&(~3) ;

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biSizeImage = bmp_row_bytes * abs(bmih.biHeight);

	mybm->width = bmih.biWidth;
	mybm->height = abs(bmih.biHeight);
	mybm->type = ( bmih.biBitCount == 24 ) ? MBTYPE_24BIT_RGB : MBTYPE_8BIT_PAL;

	mybm->dataBytes = my_row_bytes * mybm->height;

	if ( (mybm->data = malloc(mybm->dataBytes + bmp_row_bytes)) == NULL )
		LBM_cleanUp("malloc failed");

	{
	int step;
	ubyte * ptr;
	int y;
	ubyte garbage[4];

		if ( bmih.biHeight > 0 )
		{
			step = - (int)my_row_bytes;
			ptr = mybm->data + my_row_bytes * (mybm->height - 1);
		}
		else
		{
			step = my_row_bytes;
			ptr = mybm->data;
		}

		for(y=0;y<mybm->height;y++)
		{
			FRead(inFile,ptr,my_row_bytes);
			FRead(inFile,garbage,bmp_row_bytes - my_row_bytes);
			
			if (bmih.biBitCount == 24)
				swapBGRtoRGB(ptr,my_row_bytes);

			ptr += step;
		}
	}

	fclose(inFile); inFile = NULL;

return mybm;
}

void swapBGRtoRGB(ubyte *ptr,int len)
{
	while(len >= 3) 
	{
	ubyte t;
		t      = ptr[0];
		ptr[0] = ptr[2];
		ptr[2] = t;
		ptr += 3;
		len -= 3;
	}
}
