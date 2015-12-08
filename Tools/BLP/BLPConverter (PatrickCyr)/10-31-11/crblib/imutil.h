#ifndef IM_UTIL_H
#define IM_UTIL_H

/** some miscellanous routines for generic image stuff **/

#include <crblib/inc.h>
#include <crblib/fileutil.h>

double sd_ubytes(ubyte *buf,int len);
double sd_uwords(uword *buf,int len);

extern void ubytesCompare(ubyte *src,ubyte *comp,int size,FILE *out);

/***
*	enter best guess for width & height, zero for 'no idea'
****/
extern void guessRawSize(char *file,int *widthp,int *heightp,int *planesp);

	// YUV here is actually Y-Cb-Cr
extern void RGB_to_YUV(int R,int G,int B,int *Y,int *U,int *V);
extern void YUV_to_RGB(int Y,int U,int V,int *R,int *G,int *B);

extern void RGB_to_HSV(int R,int G,int B,int *H,int *S,int *V);
extern void HSV_to_RGB(int H,int S,int V,int *R,int *G,int *B);

extern void RGB_to_LAB(int R,int G,int B,int *L,int *a,int *b);
extern void LAB_to_RGB(int L,int a,int b,int *R,int *G,int *B);

	// warning! the float version is *not* the same as the int version!

extern void RGB_to_YUVf(float R,float G,float B,float *Y,float *U,float *V);
extern void YUV_to_RGBf(float Y,float U,float V,float *R,float *G,float *B);

#endif
