#include <crblib/inc.h>
#include <crblib/intmath.h>

#ifdef _MSC_VER
#pragma warning(disable : 4018) //signed/unsigned compare
#endif

	static unsigned char log2x16_table[256] = {	0,
	 0,16,25,32,37,41,45,48,51,53,55,57,59,61,63,64,
	65,67,68,69,70,71,72,73,74,75,76,77,78,79,79,80,
	81,81,82,83,83,84,85,85,86,86,87,87,88,88,89,89,
	90,90,91,91,92,92,93,93,93,94,94,95,95,95,96,96,
	96,97,97,97,98,98,98,99,99,99,100,100,100,101,101,101,
	101,102,102,102,103,103,103,103,104,104,104,104,105,105,105,105,
	106,106,106,106,107,107,107,107,107,108,108,108,108,109,109,109,
	109,109,110,110,110,110,110,111,111,111,111,111,111,112,112,112,
	112,112,113,113,113,113,113,113,114,114,114,114,114,114,115,115,
	115,115,115,115,116,116,116,116,116,116,116,117,117,117,117,117,
	117,117,118,118,118,118,118,118,118,119,119,119,119,119,119,119,
	119,120,120,120,120,120,120,120,121,121,121,121,121,121,121,121,
	121,122,122,122,122,122,122,122,122,123,123,123,123,123,123,123,
	123,123,124,124,124,124,124,124,124,124,124,125,125,125,125,125,
	125,125,125,125,125,126,126,126,126,126,126,126,126,126,126,127,
	127,127,127,127,127,127,127,127,127,127,128,128,128,128,128	};

#ifndef DO_ASM_LOG2S //{

uint ilog2round(uint val)
{
#ifdef _MSC_VER
	__asm
	{
		FILD val
		FSTP val
		mov eax,val
		add eax,0x257D86 // (2 - sqrt(2))*(1<<22)
		shr eax,23
		sub eax,127
	}
#else

uint L;
	for(L=1; (1ul<<L) <= val; L++) ;
	L --;
	assert( val >= (1UL<<L) );
	val <<= (16 - L);
	assert( val >= 65536 && val < 65536*2 );
	if ( val >= 92682 ) // sqrt(2) * 1<<16
		L ++;
return L;

#endif
}

uint ilog2ceil (uint val)
{
#ifdef _MSC_VER

	__asm
	{
		FILD val
		FSTP val
		mov eax,val
		add eax,0x7FFFFF // 1<<23 - 1
		shr eax,23
		sub eax,127
	}

#else

uint L;
	for(L=0; (1ul<<L) < val; L++) ;
	// (1<<L) >= val;
return L;

#endif
}

uint ilog2floor(uint val)
{
#ifdef _MSC_VER

	__asm
	{
		FILD val
		FSTP val
		mov eax,val
		shr eax,23
		sub eax,127
	}

#else

uint L;
	for(L=1; (1ul<<L) <= val; L++) ;
	// (1<<l) > val;
return (L - 1);

#endif
}

int flog2(float xf)
{
return ((*(int*)&xf) >> 23) - 127;
}

#endif //}

int flog2x16(float val)
{
int il,frac,lx16;

	if ( val <= 0.0f ) return 0;

	if ( val < 1.0f )
	{
		il = 0;
		while( val < 1.0f )
		{
			il --;
			val *= 2.0f;
		}
		
		frac = (uint)(val * 128.0f);
	}
	else
	{

		il = flog2(val);

		assert( val >= (1UL << il) );

		frac = (uint)(val * 128.0f);
		frac = frac >> il;
		
		// val = 2^(il) * (frac/128)
	}

	assert( frac >= 128 && frac < 256 );

	lx16 = il << 4;

	il = log2x16_table[ frac ] - 112;
	assert( il >= 0 && il <= 16 );

	lx16 += il;

return lx16;
}

uint ilog2x16(uint val)
{
uint il,frac,lx16;

	if ( val <= 1 ) return 0;

	il = ilog2floor(val);

	assert( val >= (1UL << il) );

	frac = (val << 7) >> il;

	// val = 2^(il) * (frac/128)

	assert( frac >= 128 && frac < 256 );

	lx16 = il << 4;

	il = log2x16_table[ frac ] - 112;
	assert( il >= 0 && il <= 16 );

	lx16 += il;

return lx16;
}

ulong square(ulong val)
{
return ( val >= 0xFFFF ) ? ULONG_MAX : val*val;
}

static int bits[256] =
{
  0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

int intlog2_noif(register uword N) /** truncated **/
{
	static int throw_small[64] = {
	 0, 1, 2, 3, 4, 5, 6, 7, 9, 9, 9, 9, 9, 9, 9, 9,
	10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,
	12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,
	14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15
	};

	return throw_small[ (bits[ N>>8 ]<<3) + bits[ N & 0xFF ] ];
}

int intlog2_uw(register uword N) /** truncated **/
{
	if ( N >> 8 ) {
		return (8 + bits[N >> 8]);
	} else {
		return bits[N];
	}
}

int intlog2(ulong N) /** truncated **/
{
	if ( N >> 16 ) {
		if ( N >> 24 ) {
			return (24 + bits[N >> 24]);
		} else {
			return (16 + bits[N >> 16]);
		}
	} else {
		if ( N >> 8 ) {
			return (8 + bits[N >> 8]);
		} else {
			return bits[N];
		}
	}
}

int intlog2x10(ulong N) /** rounded **/
{
	static unsigned char log2x10_table[256] = {    0, 0,
		   10,16,20,23,26,28,30,32,33,35,36,37,38,39,40,
		41,42,42,43,44,45,45,46,46,47,48,48,49,49,50,50,
		50,51,51,52,52,52,53,53,54,54,54,55,55,55,56,56,
		56,56,57,57,57,58,58,58,58,59,59,59,59,60,60,60,
		60,60,61,61,61,61,61,62,62,62,62,62,63,63,63,63,
		63,64,64,64,64,64,64,65,65,65,65,65,65,66,66,66,
		66,66,66,66,67,67,67,67,67,67,67,68,68,68,68,68,
		68,68,68,69,69,69,69,69,69,69,69,70,70,70,70,70,
		70,70,70,70,71,71,71,71,71,71,71,71,71,71,72,72,
		72,72,72,72,72,72,72,72,73,73,73,73,73,73,73,73,
		73,73,73,74,74,74,74,74,74,74,74,74,74,74,75,75,
		75,75,75,75,75,75,75,75,75,75,75,76,76,76,76,76,
		76,76,76,76,76,76,76,76,77,77,77,77,77,77,77,77,
		77,77,77,77,77,77,77,78,78,78,78,78,78,78,78,78,
		78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,79,
		79,79,79,79,79,79,79,80,80,80,80,80,80,80,80	};

	if ( N < 256 ) {
		return log2x10_table[N];
	} else {
		int b;
		b = 9;
		while( N > ((ulong)1<<b) ) b++;
		return b*10 - 80 + log2x10_table[ N>>(b-8) ];
	}
}

int intlog2x16(ulong N) /** rounded **/
{
	if ( N < 256 ) {
		return log2x16_table[N];
	} else {
		int b;
		b = 9;
		while( N > ((ulong)1<<b) ) b++;
		return ((b - 8)<<4) + log2x16_table[ N>>(b-8) ];
	}
}

int intlog2r(ulong N) /** rounded **/
{
 static unsigned char rbits[256] = {
	0,0,1,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8
  };

	if ( N >> 16 ) {
		if ( N >> 24 ) {
			return (24 + rbits[N >> 24]);
		} else {
			return (16 + rbits[N >> 16]);
		}
	} else {
		if ( N >> 8 ) {
			return (8 + rbits[N >> 8]);
		} else {
			return rbits[N];
		}
	}
}

ulong isqrt(ulong N)
{
static const ubyte sqrtable[] = {
0, 1,1, 2,2,2,2, 3,3,3,3,3,3, 4,4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,5,5, 6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
16,16,16,16,16,16,16,16,16,16,16,16 };

	/** get a quick estimate **/

	if ( N >= 256 )
	{
		long diff;
		ulong step,s;

		if ( N >> 16 ) {
			if ( N >> 24 )	s = sqrtable[N >>24]<<12;
			else 			s = sqrtable[N >>16]<<8;
		} else {
			s = sqrtable[N >> 8]<<4;
		}

	diff = N - s*s;

	if ( ABS(diff) < 2*s ) return s;

	step = isqrt(ABS(diff));

	for(;;) {
		step >>= 1; if ( step == 0 ) break;

		if ( diff > 0 ) s += step;
		else if ( diff < 0 ) s -= step;
		else break;

		diff = N - (s*s);
		if ( ABS(diff) < 2*s ) return s;
	}

	if ( (N - (s-1)*(s-1)) < ABS(diff) ) s--;
	else if ( (N - (s+1)*(s+1)) < ABS(diff) ) s++;

	return s;
  }

return sqrtable[N];
}


int GaussianRand(int val,int step)
{
int r;

	r = 0;

restart:

	r >>= 1;	if ( ! r ) r = rand() >> 1;
	if ( r&1 ) step = - step;

	step /= 2;
	if ( step == 0 ) return val;

	for(;;)
	{
		r >>= 1;	if ( ! r ) r = rand() >> 1;
		if ( r & 1 )
			val += step;
		else
		{
			//	return val + (rand()*step)/RAND_MAX;
			// step /= 2;
			val += (rand()*step)/RAND_MAX;
			goto restart;
		}
	}
}

static uint lastFastRand = 0xA2A9; // a prime

void fastRand_Seed(uint s)
{
	lastFastRand = 0xA2A9;
	s &= 0xFFFF;
	while(s--)
	{
		lastFastRand = lastFastRand * 65539 + 3;
		lastFastRand = lastFastRand * 1009  + 7;
	}
}

uint fastRand(uint max)
{
uint a;
	lastFastRand = lastFastRand * 65539 + 3;
	a = lastFastRand >> 16;
return a % max;
}

ubyte fastRandByte(void)
{
	lastFastRand = lastFastRand * 65539 + 3;
return (ubyte)(lastFastRand >> 24);
}

