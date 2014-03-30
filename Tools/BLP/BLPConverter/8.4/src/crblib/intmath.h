#ifndef CRB_INTMATH_H
#define CRB_INTMATH_H

#include <crblib/inc.h>

#ifndef ispow2
#define ispow2(x) (!( (x) & ~(-(x)) ))
#endif

extern int intlog2_noif(register uword N); /** truncated **/
extern int intlog2_uw(register uword N); /** truncated **/

extern int intlog2 (ulong N); //truncated
extern int intlog2r(ulong N); //rounded
extern int intlog2x10(ulong N); // *10.0 then rounded
extern int intlog2x16(ulong N); // *16.0 then rounded

extern ulong square(ulong val);	// int->int , handles overflow correctly

extern int ieeefloat_trick_log2(int x); // amazing! returns -127 for log2(0)

extern ulong isqrt(ulong N);

extern int GaussianRand(int val,int step);

extern void fastRand_Seed(uint s);

extern uint fastRand(uint max); // max <= 65536
extern ubyte fastRandByte(void);

extern uint ilog2x16(uint val);
extern int  flog2x16(float val); // can be negative

#define ilog2 intlog2
#define ilog2r intlog2r
#define log2x10 intlog2x10
#define ilog2x10 intlog2x10
#define log2x16 intlog2x16

#ifdef _MSC_VER
#ifndef _DEBUG
#define DO_ASM_LOG2S
#endif
#endif

#ifdef DO_ASM_LOG2S //{

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

uint INLINE ilog2round(uint val)
{
	__asm
	{
		FILD val
		FSTP val
		mov eax,val
		add eax,0x257D86 // (2 - sqrt(2))*(1<<22)
		shr eax,23
		sub eax,127
	}
}

int INLINE flog2(float xf) // !!! 
{
return ((*(int*)&xf) >> 23) - 127;
}

#else // }{

uint ilog2ceil(uint val);
uint ilog2floor(uint val);
uint ilog2round(uint val);
int flog2(float xf);

#endif //}

#endif
 
