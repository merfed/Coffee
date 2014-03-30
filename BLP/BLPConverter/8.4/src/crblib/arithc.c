#include <crblib/inc.h>
#include <crblib/arithc.h>
#include <crblib/arithc._h>

#pragma warning( disable : 4244 )	// loss of data in conversion

/*functions:*/

//#define PARANOID_ENCODEDONE	// @@ ?

//#define FAST_ENCODE
	/** actually makes very little difference in speed or compression
	*	presumably because the Pentium's branch prediction is doing
	*	a good job
	**/

arithInfo * arithInit(void)
{
arithInfo * ari;

	if ( (ari = new(arithInfo)) == NULL )
		return(NULL);

	ari->probMax = CumProbMax;
	ari->probMaxSafe = CumProbMax - 512;

return(ari);
}

void arithFree(arithInfo * ari)
{
	if (ari) free(ari);
}

void arithEncodeInitNoStuff(arithInfo * ari,ubyte *outBuf)
{
	ari->outBuf = outBuf;

	arithEncodeReInit(ari,outBuf - 1);
}

void arithEncodeInit(arithInfo * ari,ubyte *outBuf)
{
	ari->outBuf = outBuf;

	arithEncodeReInit(ari,outBuf);
}

void arithEncodeReInit(arithInfo * ari,ubyte *outBuf)
{

	ari->code  = 0;
	ari->range = One;
	ari->overflow_bytes = 0;
	ari->queue = 0;	// this is a waste of a byte

	// ari->outBuf unchanged
	ari->outPtr = outBuf;

}

void arithEncode(arithInfo * ari,ulong symlow,ulong symhigh,ulong symtot)
{
ulong code,range;

	code    = ari->code;
	range   = ari->range;

	assert( symlow < symhigh && symhigh <= symtot );
	assert( symtot <= CumProbMax );

	/** we want to do :
	*		 lowincr = (range * symlow) / symtot
	*	but range & symtot can both be large , so this would overflow the register	
	*	thus we instead do:
	*
	***/

#ifdef FAST_ENCODE
	/*#*/ { 
	ulong r;

	r = range / symtot;

	code += r * symlow;
	range = r * (symhigh - symlow);

	/*#*/ }
#else
	/*#*/ { 
	ulong lowincr,r;

	r = range / symtot;

	lowincr = r * symlow;
	code += lowincr;
	if ( symhigh == symtot )	range -= lowincr;
	else 						range = r * (symhigh - symlow);

	/*#*/ }
#endif

	arithEncRenorm(ari,code,range);
}

void arithEncBits(arithInfo * ari,uint val,uint bits)
{
ulong code,range;

	assert( val < (1UL<<bits) );
	assert( (1UL<<bits) <= CumProbMax );

	code    = ari->code;
	range   = ari->range;

	range >>= bits;

	code += range * val;

	arithEncRenorm(ari,code,range);
}

int arithEncodeDoneMinimal(arithInfo * ari)
{
uint rangeMask,rangeMSB;

	// set code to the maximum that won't change how it decodes.

	ari->code += ari->range - 1;

	/* first send the queue */

	if ( ari->code & One )
	{
		*(ari->outPtr)++ = (ubyte)(ari->queue + 1)&0xFF;
		while ( ari->overflow_bytes-- ) *(ari->outPtr)++ = 0;
	}
	else
	{
		*(ari->outPtr)++ = ari->queue;
		while ( ari->overflow_bytes-- ) *(ari->outPtr)++ = 0xFF;
	}

	/*****

	the minimal way to flush is to do :

		code += (range - 1);

		clear code below MSB of range

	eg. if range is 67 we do :
		
		code += 66;
		code &= ~63;

	then we just send code bytes until the remainder is zero.

	(this assumes that when the decoder reads past EOF, it reads zeros!)

	-----

	we almost always write just 1 byte
	(in fact, I think we might *always* write 1 byte)

	******/

	if ( ari->range >= (1UL<<31) )
	{
		rangeMSB = 1UL<<31;
	}
	else
	{
		for(rangeMSB=1;rangeMSB <= ari->range; rangeMSB <<= 1) ;
		rangeMSB >>= 1;
		
		assert( ari->range >= rangeMSB && ari->range < rangeMSB*2 );
	}

	// clear code under rangeMask :

	rangeMask = 0;
	rangeMSB >>= 1;
	while(rangeMSB)
	{
		rangeMask |= rangeMSB;
		rangeMSB >>= 1;
	}

	assert( rangeMask < ari->range );

	ari->code &= (~ rangeMask);
	ari->code &= CODE_MASK;

	while(ari->code)	
	{
		*(ari->outPtr)++ = (ari->code >> SHIFT_BITS) & 0xFF;
		ari->code <<= 8;
		ari->code &= CODE_MASK;
	}

	ari->outPtr[0] = 0;
	ari->outPtr[1] = 0;
	ari->outPtr[2] = 0;
	ari->outPtr[3] = 0;
	ari->outPtr[4] = 0;
	ari->outPtr[5] = 0;

return ari->outPtr - ari->outBuf;
}

int arithEncodeDoneSafe(arithInfo * ari)
{
	// set code to the maximum that won't change how it decodes.

	ari->code += ari->range - 1;

	/* first send the queue */

	if ( ari->code & One )
	{
		*(ari->outPtr)++ = (ubyte)(ari->queue + 1)&0xFF;
		while ( ari->overflow_bytes-- ) *(ari->outPtr)++ = 0;
	}
	else
	{
		*(ari->outPtr)++ = ari->queue;
		while ( ari->overflow_bytes-- ) *(ari->outPtr)++ = 0xFF;
	}

	*(ari->outPtr)++ = (ari->code >> SHIFT_BITS) & 0xFF;	ari->code <<= 8;
	*(ari->outPtr)++ = (ari->code >> SHIFT_BITS) & 0xFF;	ari->code <<= 8;
	
return ari->outPtr - ari->outBuf;
}

void arithDecodeInitNoStuff(arithInfo * ari,ubyte *outBuf)
{
	ari->outBuf = ari->outPtr = outBuf;

	arithDecodeReInit(ari,outBuf-1);
}

void arithDecodeInit(arithInfo * ari,ubyte *outBuf)
{
	ari->outBuf = ari->outPtr = outBuf;

	arithDecodeReInit(ari,outBuf);
}

void arithDecodeReInit(arithInfo * ari,ubyte *outBuf)
{
ulong byte;

	ari->outPtr = outBuf + 1;

	/** the code needs to be kept filled with 31 bits ;
	*	this means we cannot just read in 4 bytes.  We must read in 3,
	*	then the 7 bits of another (EXTRA_BITS == 7) , and save that last
	*	bit in the queue 
	**/

	byte = ari->queue = *(ari->outPtr)++;
	ari->code = byte >> (TAIL_EXTRA_BITS);
	ari->range = 1 << EXTRA_BITS;
}

/** save a repeated divide computation between arithGet and arithDecode **/
static ulong dec_range_over_symtot;

ulong arithGet(arithInfo * ari,ulong symtot)
{
ulong ret,range,code;

	assert( symtot <= CumProbMax );

	arithDecRenorm(ari,&code,&range);

	dec_range_over_symtot = range / symtot;
	ret = code / dec_range_over_symtot;

	ari->range = range;
	ari->code  = code;
		
	//assert( ret < symtot );  // is this safe? ; apparently not!
	ret = ( ret >= symtot ? symtot-1 : ret );

return ret;
}

void arithDecode(arithInfo * ari,ulong symlow,ulong symhigh,ulong symtot)
{

	assert( symlow < symhigh && symhigh <= symtot );
	assert( symtot <= CumProbMax );

#ifdef FAST_ENCODE

	ari->code -= dec_range_over_symtot * symlow;
	ari->range = dec_range_over_symtot * (symhigh - symlow);

#else
{
ulong lowincr;

	lowincr = dec_range_over_symtot * symlow;
	ari->code -= lowincr;
	if ( symhigh == symtot )	ari->range -= lowincr;
	else 						ari->range = dec_range_over_symtot * (symhigh - symlow);
}
#endif

}

uint arithDecBits(arithInfo * ari,uint bits)
{
ulong code,range;
uint val;

	arithDecRenorm(ari,&code,&range);

	range >>= bits;
	val = code / range;

	code -= range * val;
	
	ari->range = range;
	ari->code  = code;

return val;
}

void arithDecodeDone(arithInfo * ari)
{
}

int arithTellEncPos(arithInfo * ari)
{
int ret;

	ret = ari->outPtr - ari->outBuf;
	ret += 1 + ari->overflow_bytes;	// # of bytes in the queue
	// ret += 2;	// flush bytes
return ret;
}
int arithTellDecPos(arithInfo * ari)
{
int ret;
	ret = ari->outPtr - ari->outBuf;
	ret -= CODE_BYTES;
	// ret += 2;
return ret;
}

int arithEncodeFlush(arithInfo * ari)
{
int ret;
ubyte *saveBuf;
	ret = arithEncodeDone(ari);
	saveBuf = ari->outBuf;
	arithEncodeInit(ari,saveBuf+ret);
	ari->outBuf = saveBuf;
return ret;
}

void arithDecodeRestart(arithInfo * ari,ulong pos)
{
ubyte *saveBuf;
	arithDecodeDone(ari);
	saveBuf = ari->outBuf;
	arithDecodeInit(ari,saveBuf+pos);
	ari->outBuf = saveBuf;
}


void arithEncBit(arithInfo * ari,ulong mid,ulong tot,bool bit)
{
ulong code,range;

code    = ari->code;
range   = ari->range;

	/*#*/ { ulong r;

	r = (range / tot) * mid;

	if ( bit )
	{
		code += r;
		range -= r;
	}
	else
	{
		range = r;
	}
	/*#*/ }

	arithEncRenorm(ari,code,range);
}

bool arithDecBit(arithInfo * ari,ulong mid,ulong tot)
{
bool bit;
ulong range,code,r;

	arithDecRenorm(ari,&code,&range);

/**
*
*		r = (range / tot)
*
*	if ( (code / r) >= mid ) bit = 1; else bit = 0;
*
*	we eliminate one divide. this is the savings of the binary coder
*
**/

	r = (range / tot) * mid;

	if ( code >= r )
	{
		bit = 1;
		code -= r;
		range -= r;
	}
	else
	{
		bit = 0;
		range = r;
	}

	ari->range = range;
	ari->code  = code;

return bit;
}

#define BITMODEL_TOTMAX 5000
#define BITMODEL_INC	30

void arithModelEncBit(arithInfo * ari,ulong *p0,ulong *pt,bool bit)
{
ulong code,range;

code    = ari->code;
range   = ari->range;

	{
	ulong r;

		r = (range / (*pt)) * (*p0);

		if ( bit )
		{
			code += r;
			range -= r;
		}
		else
		{
			range = r;
			(*p0) += BITMODEL_INC;
		}
	}

	(*pt) += BITMODEL_INC;
	if ( *pt > BITMODEL_TOTMAX )
	{
		(*p0) >>= 1; 
		(*pt) >>= 1; 
		(*p0) ++; (*pt) += 2; 
	}

	arithEncRenorm(ari,code,range);
}

bool arithModelDecBit(arithInfo * ari,ulong *p0,ulong *pt)
{
bool bit;
ulong range,code,r;

	arithDecRenorm(ari,&code,&range);

	r = (range / (*pt)) * (*p0);

	if ( code >= r )
	{
		bit = 1;
		code -= r;
		range -= r;
	}
	else
	{
		bit = 0;
		range = r;
		(*p0) += BITMODEL_INC;
	}

	(*pt) += BITMODEL_INC;
	if ( (*pt) > BITMODEL_TOTMAX )
	{
		(*p0) >>= 1;
		(*pt) >>= 1; 
		(*p0) ++; (*pt) += 2; 
	}

	ari->range = range;
	ari->code  = code;

return bit;
}


void arithEncBitRaw(arithInfo * ari,bool bit)
{
ulong code,range;

code    = ari->code;
range   = ari->range;

	if ( bit )
	{
		code	+= range >> 1;
		range	-= range >> 1;
	}
	else
	{
		range >>= 1;
	}

	arithEncRenorm(ari,code,range);
}

bool arithDecBitRaw(arithInfo * ari)
{
bool bit;
ulong range,code,r;

	arithDecRenorm(ari,&code,&range);

	r = range >> 1;

	if ( code >= r )
	{
		bit = 1;
		code  -= r;
		range -= r;
	}
	else 
	{
		bit = 0;
		range = r;
	}

	ari->range = range;
	ari->code  = code;

return bit;
}
