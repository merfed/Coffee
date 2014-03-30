#ifndef BMPIO_H
#define BMPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <crblib/inc.h>

#define MBTYPE_8BIT_PAL		0
#define MBTYPE_8BIT_GRAY	1
#define MBTYPE_24BIT_RGB	2

typedef struct 
{
	int width,height,type,dataBytes,ncolors;
	ubyte * palette;
	ubyte * data;
} myBMP;

extern void freeBMP(myBMP * mb);

// loadImage will try to detect the image type

extern myBMP * loadImage(char *name);

extern bool saveBMP(myBMP *mybm,char *name);
extern myBMP * loadBMP(char *name);

extern bool saveRAW(myBMP *mybm,char *name);
extern myBMP * loadRAW(char *name);

//	save & load will do this for you, but you may want to do it
//		yourself (for example, to CreateDIBitmap)

extern void swapBGRtoRGB(ubyte *ptr,int len);

#ifdef __cplusplus
}
#endif

#endif // BMPIO_H

