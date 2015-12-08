#if 1
#include <crblib/inc.h>
#include <crblib/strutil.h>
#else
#define NEED_CRBLIB_FUNCS
#include "port.h"
#endif 

#include "palcreate.h"
#include "palettize.h"
#include "colorconv.h"

bool convertPlanes(ubyte *fmPlane,int fmType,ubyte *toPlane,int toType,int fmPlaneLen,ubyte *palette)
{
	if ( ! ((fmType & PT_FLAG_RAW) || palette) ) return false;
	if ( ! ((toType & PT_FLAG_RAW) || palette) ) return false;

	// there are no pal <-> pal conversions

	if ( fmType == toType ) 
	{
		memcpy(toPlane,fmPlane,fmPlaneLen);
		return true;
	}

	switch(fmType)
	{
		case PT_8B_GRAY:
		switch(toType)
		{
			case PT_24B_YUV:
				// just grab the gray and call it Y
				memcpy(toPlane,fmPlane,fmPlaneLen);
				// clear U & V
				memclear(toPlane+fmPlaneLen,fmPlaneLen*2);
				break;
			default:
				return false;
		}
		case PT_24B_YUV:
		switch(toType)
		{
			case PT_24B_RGB:
				{
				ubyte *fmptr,*toptr;
				int r;

					fmptr = fmPlane;
					toptr = toPlane;
					for(r=(fmPlaneLen/3);r--;) 
					{
						YUV3_to_RGB3(fmptr,toptr);
						fmptr += 3; toptr += 3;
					}
				}
				break;
			case PT_32B_BGRX:
				{
				ubyte *fmptr,*toptr;
				int r;

					fmptr = fmPlane;
					toptr = toPlane;
					for(r=(fmPlaneLen/3);r--;) 
					{
						YUV3_to_RGB3(fmptr,fmptr);
						toptr[2] = *fmptr++;
						toptr[1] = *fmptr++;
						toptr[0] = *fmptr++;
						toptr += 4;
					}
				}
				break;
			case PT_16B_BGR_565:
				{
				int t,r,g,b,cnt;
				ubyte *fmptr,*toptr;

					fmptr = fmPlane;
					toptr = toPlane;
					for(cnt=(fmPlaneLen/3);cnt--;) 
					{
						YUV3_to_RGB3(fmptr,fmptr);
						r = *fmptr++;	g = *fmptr++;	b = *fmptr++;
						t = ((r>>3)<<11) + ((g>>2)<<5) + (b>>3);
						*toptr++ = (t>>8);
						*toptr++ = (t&0xFF);
					}
				}
				break;
			case PT_16B_BGR_555:
				{
				int t,r,g,b,cnt;
				ubyte *fmptr,*toptr;

					fmptr = fmPlane;
					toptr = toPlane;
					for(cnt=(fmPlaneLen/3);cnt--;) 
					{
						YUV3_to_RGB3(fmptr,fmptr);
						r = *fmptr++;	g = *fmptr++;	b = *fmptr++;
						t = ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
						*toptr++ = (t>>8);
						*toptr++ = (t&0xFF);
					}
				}
				break;
			case PT_8B_PAL:
				{
				ubyte paletteYUV[768];
				paletteRGBtoYUV(palette,paletteYUV);
				if ( ! palettizePlane24to8bit(fmPlane,toPlane,fmPlaneLen/3,paletteYUV) )
					return false;
				}
				break;
			case PT_8B_GRAY:
				// just grab the Y and call it gray
				memcpy(toPlane,fmPlane,(fmPlaneLen/3));
				break;
			default:
				return false;
		}
		break;
		case PT_24B_RGB:
		switch(toType)
		{
			case PT_24B_YUV:
				{
				ubyte *fmptr,*toptr;
				int r;

					fmptr = fmPlane;
					toptr = toPlane;
					for(r=(fmPlaneLen/3);r--;) 
					{
						RGB3_to_YUV3(fmptr,toptr);
						fmptr += 3; toptr += 3;
					}
				}
				break;
			case PT_16B_BGR_565:
				{
				int t,r,g,b,cnt;
				ubyte *fmptr,*toptr;

					fmptr = fmPlane;
					toptr = toPlane;
					for(cnt=(fmPlaneLen/3);cnt--;) 
					{
						r = *fmptr++;	g = *fmptr++;	b = *fmptr++;
						t = ((r>>3)<<11) + ((g>>2)<<5) + (b>>3);
						*toptr++ = (t>>8);
						*toptr++ = (t&0xFF);
					}
				}
				break;
			case PT_16B_BGR_555:
				{
				int t,r,g,b,cnt;
				ubyte *fmptr,*toptr;

					fmptr = fmPlane;
					toptr = toPlane;
					for(cnt=(fmPlaneLen/3);cnt--;) 
					{
						r = *fmptr++;	g = *fmptr++;	b = *fmptr++;
						t = ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
						*toptr++ = (t>>8);
						*toptr++ = (t&0xFF);
					}
				}
				break;
			case PT_8B_PAL:
				if ( ! palettizePlane24to8bit(fmPlane,toPlane,fmPlaneLen/3,palette) )
					return false;
				break;
			default:
				return false;
		}
		break;
		case PT_8B_PAL:
		switch(toType)
		{
			case PT_24B_RGB:
				{
				int i;
				ubyte *pal;

					while(fmPlaneLen--) 
					{
						i = *fmPlane++;
						pal = palette + i+i+i;
						*toPlane++ = *pal++;
						*toPlane++ = *pal++;
						*toPlane++ = *pal++;
					}
				}
				break;
			case PT_32B_BGRX:
				{
				int i;
				ubyte *pal;
				ubyte *toptr = toPlane;

					while(fmPlaneLen--) 
					{
						i = *fmPlane++;
						pal = palette + i+i+i;
						*toptr++ = pal[2];
						*toptr++ = pal[1];
						*toptr++ = pal[0];
						toptr++;
					}
				}
				break;
			case PT_16B_BGR_565:
				{
				int i,r,g,b;
				ubyte *pal;

					while(fmPlaneLen--) 
					{
						i = *fmPlane++;
						pal = palette + i+i+i;
						r = *pal++;	g = *pal++;	b = *pal++;
						i = ((r>>3)<<11) + ((g>>2)<<5) + (b>>3);
						*toPlane++ = (i>>8);
						*toPlane++ = (i&0xFF);
					}
				}
				break;
			case PT_16B_BGR_555:
				{
				int i,r,g,b;
				ubyte *pal;

					while(fmPlaneLen--) 
					{
						i = *fmPlane++;
						pal = palette + i+i+i;
						r = *pal++;	g = *pal++;	b = *pal++;
						i = ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
						*toPlane++ = (i>>8);
						*toPlane++ = (i&0xFF);
					}
				}
				break;
			case PT_24B_YUV:
				{
				int i;
				ubyte paletteYUV[768];
				ubyte *pal;

					paletteRGBtoYUV(palette,paletteYUV);

					while(fmPlaneLen--) 
					{
						i = *fmPlane++;
						pal = paletteYUV + i+i+i;
						*toPlane++ = *pal++;
						*toPlane++ = *pal++;
						*toPlane++ = *pal++;
					}
				}
				break;
			case PT_8B_GRAY:
				{
				int i;
				ubyte paletteYUV[768];
				ubyte *pal;

					paletteRGBtoYUV(palette,paletteYUV);

					while(fmPlaneLen--) 
					{
						i = *fmPlane++;
						pal = paletteYUV + i+i+i;
						*toPlane++ = *pal++;	// take the Y and call it gray
					}
				}
				break;
			default:
				return false;
		}
		break;
		default:
			assert("invalid source type!");
			return false;
	}
return true;
}

/**************** the YUV routines : ******************/

#define YUV_SHIFT 	14
#define YUV_HALF	(1<<(YUV_SHIFT-1))
#define YUV_ONE		(1<<YUV_SHIFT)
#define Y_R   ((int)( 0.29900 * YUV_ONE ))
#define Y_G   ((int)( 0.58700 * YUV_ONE ))
#define Y_B   ((int)( 0.11400 * YUV_ONE ))
#define U_R   ((int)(-0.16874 * YUV_ONE ))
#define U_G   ((int)(-0.33126 * YUV_ONE ))
#define U_B   ((int)( 0.50000 * YUV_ONE ))
#define V_R   ((int)(-0.50000 * YUV_ONE ))	// ** important sign change of 'V' from jpeg default
#define V_G   ((int)( 0.41869 * YUV_ONE ))
#define V_B   ((int)( 0.08131 * YUV_ONE ))
#define R_Y   (    				YUV_ONE )       
#define R_U   (0)
#define R_V   ((int)(-1.40200 * YUV_ONE ))
#define G_Y   (    				YUV_ONE )       
#define G_U   ((int)(-0.34414 * YUV_ONE ))
#define G_V   ((int)( 0.71414 * YUV_ONE ))
#define B_Y   (     			YUV_ONE )       
#define B_U   ((int)( 1.77200 * YUV_ONE ))
#define B_V   (0)       

#define Y_RGB(R,G,B) (( Y_R * (R) + Y_G * (G) + Y_B * (B) + YUV_HALF ) >> YUV_SHIFT)
#define U_RGB(R,G,B) (( U_R * (R) + U_G * (G) + U_B * (B) + YUV_HALF ) >> YUV_SHIFT)
#define V_RGB(R,G,B) (( V_R * (R) + V_G * (G) + V_B * (B) + YUV_HALF ) >> YUV_SHIFT)
#define R_YUV(Y,U,V) (( R_Y * (Y) + R_U * (U) + R_V * (V) + YUV_HALF ) >> YUV_SHIFT)
#define G_YUV(Y,U,V) (( G_Y * (Y) + G_U * (U) + G_V * (V) + YUV_HALF ) >> YUV_SHIFT)
#define B_YUV(Y,U,V) (( B_Y * (Y) + B_U * (U) + B_V * (V) + YUV_HALF ) >> YUV_SHIFT)

void RGB3_to_YUV3(ubyte *RGB,ubyte *YUV)
{
int R = RGB[0], G = RGB[1], B = RGB[2];
	YUV[0] = Y_RGB(R,G,B);
	YUV[1] = U_RGB(R,G,B) + 127;
	YUV[2] = V_RGB(R,G,B) + 127;
}

void YUV3_to_RGB3(ubyte *YUV,ubyte *RGB)
{
int y,u,v,r,g,b;
	y = YUV[0];
	u = YUV[1] - 127;
	v = YUV[2] - 127;
	r = R_YUV(y,u,v);
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);
	RGB[0] = minmax(r,0,255);	// <> we could get negative ones and whatnot
	RGB[1] = minmax(g,0,255);	//	because the y,u,v are not really 24 bits;
	RGB[2] = minmax(b,0,255);	//	there are regions of YUV space that will never be reached by RGB3_to_YUV3
}


void RGB3_to_YUV3ints(ubyte *RGB,int *Y,int *U,int *V)
{
int R = RGB[0], G = RGB[1], B = RGB[2];
	*Y = Y_RGB(R,G,B);
	*U = U_RGB(R,G,B) + 127;
	*V = V_RGB(R,G,B) + 127;
}

void YUVints_to_RGB(int y,int u,int v,ubyte *RGB)
{
int r,g,b;
	putminmax(y,0,255);
	putminmax(u,0,255);
	putminmax(v,0,255);
	u -= 127;
	v -= 127;
	r = R_YUV(y,u,v);
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);
	RGB[0] = minmax(r,0,255);	// <> we could get negative ones and whatnot
	RGB[1] = minmax(g,0,255);	//	because the y,u,v are not really 24 bits;
	RGB[2] = minmax(b,0,255);	//	there are regions of YUV space that will never be reached by RGB3_to_YUV3
}

void RGBints_to_YUVints(int R,int G,int B,int *Y,int *U,int *V)
{
	*Y = Y_RGB(R,G,B);
	*U = U_RGB(R,G,B) + 127;
	*V = V_RGB(R,G,B) + 127;
}

void YUVints_to_RGBints(int y,int u,int v,int *R,int *G,int *B)
{
int r,g,b;
	putminmax(y,0,255);
	putminmax(u,0,255);
	putminmax(v,0,255);
	u -= 127;
	v -= 127;
	r = R_YUV(y,u,v);
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);
	*R = minmax(r,0,255);	// <> we could get negative ones and whatnot
	*G = minmax(g,0,255);	//	because the y,u,v are not really 24 bits;
	*B = minmax(b,0,255);	//	there are regions of YUV space that will never be reached by RGB3_to_YUV3
}
