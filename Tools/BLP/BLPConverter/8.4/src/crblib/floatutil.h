#ifndef CRBLIB_FLOATUTIL_H
#define CRBLIB_FLOATUTIL_H

//--------------------------------------------------------------------

#include <crblib/inc.h>

#pragma warning (disable:4514)	// unreferenced inline function

#define	PI											((float)3.14159265358979323846f)
#define	TWOPI										((float)6.28318530717958647692f)
#define	HALFPI										((float)1.57079632679489661923f)

#define DEGS_PER_RAD								((float)0.01745329251994329576f)
#define RADS_PER_DEG								((float)57.2957795130823208767f)

// should probably be moved to trig module
INLINE float fDegToRad(float d)
{
	return d * DEGS_PER_RAD;
}

INLINE float fRadToDeg(float r)
{
	return r * RADS_PER_DEG;
}

#define EPSILON										((float)0.000797f)
#define FLOATS_EQUAL(x,y)							( ABS((x) - (y)) < EPSILON )
#define FLOAT_ISZERO(x)								( ABS(x) < EPSILON )

#ifndef M_LOG2_E
#define M_LOG2_E  (0.693147180559945309417f)
#endif

#ifndef LN2
#define LN2 M_LOG2_E
#endif

#ifndef LN2inverse
#define LN2inverse (1.442695f)
#endif

#ifndef log2
#define log2(val) (log(val)*LN2inverse)
#endif

#define MSE2PSNR(mse)	((float)( 48.165 - 10.0*log10(mse)))
#define PSNR2MSE(psnr)	((float)pow(10.0,4.8165 - (psnr)*0.1))

INLINE float fSquare(float a)
{
	return a * a;
}

INLINE float fCube(float a)
{
	return a * a * a;
}

//------------------------------

#define ftoi	fToInt

float INLINE fRoundToInt(float val) // rounds depending on how you set geCPU_FloatControl
{
	__asm
	{
		FLD  val
		FRNDINT
		FSTP val
	}
return val;
}

float INLINE fSqrt(float val)
{
	__asm 
	{
		FLD  val		// 1 clock
		FSQRT			// 30-70 clocks
		FSTP val		// 2 clocks
	}
return val;
}

float INLINE fSin(float val)
{
	__asm 
	{
		FLD  val		// 1 clock
		FSIN			// ~ 200 clocks
		FSTP val		// 2 clocks
	}
return val;
}

float INLINE fCos(float val)
{
	__asm 
	{
		FLD  val		// 1 clock
		FCOS			// ~ 200 clocks
		FSTP val		// 2 clocks
	}
return val;
}

int INLINE fToInt(float f)
{
int i;
	__asm
	{
		FLD   f
		FISTP i
	}
return i;
}

#ifdef ftoi
#undef ftoi
#endif

#define ftoi fToInt

float INLINE itof(int i)
{
float f;
	__asm
	{
		FILD i
		FSTP f
	}
return f;
}

uint INLINE ilog2ceil(uint val)
{
	__asm
	{
		FILD val
		FSTP val
		mov eax,val
		add eax,0x7FFFFF // 1<<23 - 1
		shr eax,23
		sub eax,127
	}
}

uint INLINE ilog2floor(uint val)
{
	__asm
	{
		FILD val
		FSTP val
		mov eax,val
		shr eax,23
		sub eax,127
	}
}

uint INLINE flog2(float xf) // !!! 
{
return ((*(int*)&xf) >> 23) - 127;
}

void float_RoundToZero(void);
void float_SinglePrecision(void);
bool float_DoesRoundToZero(void);


#endif // CRBLIB_FLOATUTIL_H


