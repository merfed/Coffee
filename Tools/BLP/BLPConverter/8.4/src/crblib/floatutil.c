#include "floatutil.h"

bool float_DoesRoundToZero(void)
{
uword control;
int i;
float f;

	__asm
	{
		FNSTCW control
	}
	
	if ( ! (control & (3<<10)) )
		return false;

	f = 1.4f;
	i = ftoi(f);
	assert( i == 1 );

	f = 2.7f;
	i = ftoi(f);
	assert( i == 2 );

	f = -3.4f;
	i = ftoi(f);
	assert( i == -3 );

	f = -4.7f;
	i = ftoi(f);
	assert( i == -4 );

return true;
}

void float_RoundToZero(void)
{
uword control;
	__asm
	{
		FNSTCW control
	}
	
	control &= ~(3<<10);
	control |=  (3<<10);

	__asm
	{	
		FLDCW control
	}
	
	assert(float_DoesRoundToZero());
}

void float_SinglePrecision(void)
{
uword control;
	__asm
	{
		FNSTCW control
	}
	
	control &= ~(3<<8);

	__asm
	{	
		FLDCW control
	}
}

