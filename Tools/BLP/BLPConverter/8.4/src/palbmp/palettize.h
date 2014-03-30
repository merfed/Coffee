#ifndef __PALETTIZE_H
#define __PALETTIZE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LINUX
#include <crblib/inc.h>
#else
#define NEED_CRBLIB_FUNCS
#include "port.h"
#endif

// 'palette' is a 768-byte array

extern bool palettizePlane24to8bit(ubyte *rgbPlane,ubyte *palPlane,int planeLen,ubyte *palette);

	// rgbPlane is (planeLen*3) bytes
	// palette is 768 bytes
	// "rgbPlane" and "palette" must be in the same color space, preferably YUV

extern void paletteRGBtoYUV(ubyte *from,ubyte *to);
	// RGB -> YUV in the palette
	// from & to can be the same
extern void paletteYUVtoRGB(ubyte *from,ubyte *to);

// you can create a palette with routines in "palcreate.h"

/******* if you want to do your own palettizing : ******/

typedef struct palInfo palInfo;

extern palInfo * closestPalInit(ubyte * palette);
extern void closestPalFree(palInfo *info);
extern int closestPal(int B,int G,int R,palInfo *pi);

#ifdef __cplusplus
}
#endif

#endif
