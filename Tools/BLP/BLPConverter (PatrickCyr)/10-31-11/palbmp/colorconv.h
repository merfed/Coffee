#ifndef __COLORCONV_H
#define __COLORCONV_H

#ifdef __cplusplus
extern "C" {
#endif

//planeTypes :

#define PT_FLAG_RAW	(1<<10)
#define PT_FLAG_PAL	(1<<11)

// source or target formats:
#define PT_24B_YUV (0 + PT_FLAG_RAW)
#define PT_24B_RGB (1 + PT_FLAG_RAW)
#define PT_8B_GRAY (2 + PT_FLAG_RAW)

#define PT_8B_PAL  (0 + PT_FLAG_PAL)

// target formats only:
#define PT_32B_BGRX		(3 + PT_FLAG_RAW)
#define PT_16B_BGR_565	(4 + PT_FLAG_RAW)
#define PT_16B_BGR_555	(5 + PT_FLAG_RAW)

extern int convertPlanes(ubyte *fmPlane,int fmType,
							ubyte *toPlane,int toType,
							int fmPlaneLen,ubyte *palette);
	// fm & to can be the same array

extern void RGB3_to_YUV3(ubyte *RGB,ubyte *YUV);
extern void YUV3_to_RGB3(ubyte *YUV,ubyte *RGB);

extern void RGB3_to_YUV3ints(ubyte *RGB,int *Y,int *U,int *V);
extern void YUVints_to_RGB(int y,int u,int v,ubyte *RGB);
extern void RGBints_to_YUVints(int R,int G,int B,int *Y,int *U,int *V);
extern void YUVints_to_RGBints(int y,int u,int v,int *R,int *G,int *B);

#ifdef __cplusplus
}
#endif

#endif
