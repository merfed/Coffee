#ifndef __PALCREATE_H
#define __PALCREATE_H

#ifdef __cplusplus
extern "C" {
#endif

// #include <crblib/inc.h>

extern void createPaletteFast(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);
extern void createPaletteSlow(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);
extern void createPaletteMedium(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);

extern void createPalette(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);

	// plane24bit is (planeLen*3) bytes
	// palette is (palEntries*3) bytes
	// palette will be created in the same color space as plane24bit
	// both are RGBRGB or YUVYUV interleaved 24-bit values

#ifdef __cplusplus
}
#endif

#endif
