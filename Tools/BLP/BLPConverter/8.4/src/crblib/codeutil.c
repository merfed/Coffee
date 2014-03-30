#include <crblib/inc.h>
#include "codeutil.h"

#pragma warning( disable : 4018 )	// compare signed & unsigned

void cu_putEscaping_byte(ulong val,ubyte **streamp)
{
ubyte *stream = *streamp;
	while ( val >= 0xFF ) {
		*stream++ = 0xFF;
		val -= 0xFF;
	}
	*stream++ = val;
*streamp = stream;
}

ulong  cu_getEscaping_byte(ubyte **streamp)
{
ubyte *stream = *streamp;
ulong ret,val;
	ret = 0;
	do {
		val = *stream++;
		ret += val;
	} while( val == 0xFF );
*streamp = stream;
return ret;
}

void cu_putExpanding_byte(ulong val,ubyte **streamp)
{
ubyte *stream = *streamp;

	if ( val < 0xFF )
	{
		*stream++ = val;
	}
	else
	{
		*stream++ = 0xFF;
		val -= 0xFF;
		if ( val < 0xFFFF )
		{
			*stream++ = (val>>8);
			*stream++ = (val&0xFF);
		}
		else
		{
			*stream++ = 0xFF;
			*stream++ = 0xFF;
			val -= 0xFFFF;
			if ( val < 0xFFFFFF )
			{
				*stream++ = (val>>16);
				*stream++ = (val>>8)&0xFF;
				*stream++ = (val)&0xFF;
			}
			else
			{
				*stream++ = 0xFF;
				*stream++ = 0xFF;
				*stream++ = 0xFF;
				val -= 0xFFFFFF;

				*stream++ = (val>>24);
				*stream++ = (val>>16)&0xFF;
				*stream++ = (val>>8)&0xFF;
				*stream++ = (val)&0xFF;
			}
		}
	}
	*streamp = stream;
}

ulong  cu_getExpanding_byte(ubyte **streamp)
{
ubyte *stream = *streamp;
ulong ret,val;
	ret = 0;
	
	val = *stream++;
	ret += val;
	if ( val == 0xFF )
	{
		val = (stream[0]<<8) + stream[1];
		stream += 2;
		ret += val;

		if ( val == 0xFFFF )
		{
			val = (stream[0]<<16) + (stream[1]<<8) + stream[2];
			stream += 3;
			ret += val;

			if ( val == 0xFFFFFF )
			{
				val = (stream[0]<<24) + (stream[1]<<16) + (stream[2]<<8) + stream[3];
				stream += 4;
				ret += val;
			}
		}
	}

	*streamp = stream;
return ret;
}

void cu_putEscapingSigned_byte(long val,ubyte **streamp)
{
char *stream = *streamp;
	
	*streamp = stream + 1;

	if ( val >= 127 )
	{
		*stream = 127;
		val -= 127;
		
		cu_putEscaping_byte(val,streamp);
	}
	else if ( val <= (-128) )
	{
		*stream = (char)(-128);
		val = (-val) - 128;
		assert( val >= 0 );
		
		cu_putEscaping_byte(val,streamp);
	}
	else
	{
		*stream = val;
	}
}

long  cu_getEscapingSigned_byte(ubyte **streamp)
{
char *stream = *streamp;
long val;

	val = *stream;
	*streamp = stream + 1;

	if ( val == 127 )
	{
		val += (long)cu_getEscaping_byte(streamp);
	}
	else if ( val == -128 )
	{
		val -= (long)cu_getEscaping_byte(streamp);
	}

return val;
}

void cu_putExpandingSigned_byte(long val,ubyte **streamp)
{
char *stream = *streamp;
	
	*streamp = stream + 1;

	if ( val >= 127 )
	{
		*stream = 127;
		val -= 127;
		
		cu_putExpanding_byte(val,streamp);
	}
	else if ( val <= (-128) )
	{
		*stream = (-128);
		val = (-val) - 128;
		assert( val >= 0 );
		
		cu_putExpanding_byte(val,streamp);
	}
	else
	{
		*stream = val;
	}
}

long  cu_getExpandingSigned_byte(ubyte **streamp)
{
char *stream = *streamp;
long val;

	val = *stream;
	*streamp = stream + 1;

	if ( val == 127 )
	{
		val += (long)cu_getExpanding_byte(streamp);
	}
	else if ( val == -128 )
	{
		val -= (long)cu_getExpanding_byte(streamp);
	}

return val;
}

void cu_putEscaping_bii(ulong val,struct LBitIOInfo *stream,int escape_bits)	/** escape of (1<<escape_bits) **/
{
ulong escape;
	escape = (1<<escape_bits) - 1;
	while(val >= escape) {
		LBitIO_WriteBits(stream,escape,escape_bits);
		val -= escape;
	}
	LBitIO_WriteBits(stream,val,escape_bits);
}

ulong  cu_getEscaping_bii(struct LBitIOInfo *stream,int escape_bits)
{
ulong escape,ret,val;
	escape = (1<<escape_bits) - 1;
	ret = 0;
	do {
		LBitIO_ReadBits(stream,val,escape_bits);
		ret += val;
	} while ( val == escape);
return ret;
}

void cu_putEscaping_ari(ulong val,arithInfo *stream,ulong escape)	/** escape of (1<<escape_bits) **/
{
	while(val >= escape) {
		arithEncode(stream,escape,escape+1,escape+1);
		val -= escape;
	}
	arithEncode(stream,val,val+1,escape+1);
}

ulong  cu_getEscaping_ari(arithInfo *stream,ulong escape)
{
ulong ret,val;
	ret = 0;
	do {
		val = arithGet(stream,escape+1);
		arithDecode(stream,val,val+1,escape+1);
		ret += val;
	} while ( val == escape);
return ret;
}

void cu_putExpanding_bii(ulong val,struct LBitIOInfo *stream,int init_bits,int step_bits)
{
int bits;
ulong mask;

	bits = init_bits;
	mask = (1<<bits) - 1;
	while(val >= mask ) {
		LBitIO_WriteBits(stream,mask,bits);
		val -= mask;
		bits += step_bits; if ( bits > 31 ) bits = 31;
		mask = (1<<bits) - 1;
	}
	LBitIO_WriteBits(stream,val,bits);
}

ulong  cu_getExpanding_bii(struct LBitIOInfo *stream,int init_bits,int step_bits)
{
ulong bits,ret,val;
ulong mask;

	bits = init_bits;
	ret = 0;
	do {
		mask = (1<<bits) - 1;
		LBitIO_ReadBits(stream,val,bits);
		bits += step_bits;
		ret += val;
	} while( val == mask);

return ret;
}


void cu_putExpanding_ari(ulong val,arithInfo *stream,int init_max,int step_max)
{
ulong escape;

	escape = init_max;
	while(val >= escape ) {
		arithEncode(stream,escape,escape+1,escape+1);
		val -= escape;
		escape += step_max;
		if ( escape > stream->safeProbMax ) escape = stream->safeProbMax;
	}
	arithEncode(stream,val,val+1,escape+1);
}

ulong  cu_getExpanding_ari(arithInfo *stream,int init_max,int step_max)
{
ulong escape,ret,val;

	escape = init_max;
	ret = 0;
	for(;;) {
		val = arithGet(stream,escape+1);
		arithDecode(stream,val,val+1,escape+1);
		ret += val;
		if ( val != escape )
			break;
		escape += step_max;
	}

return ret;
}



void cu_putMulting_ari(ulong val,arithInfo *stream,int init_max,int step_mult)
{
ulong max;

	max = init_max;
	while ( val >= max ) {
		arithEncBitRaw(stream,1);
		val -= max;
		max *= step_mult;
		if ( max > stream->safeProbMax ) max = stream->safeProbMax;
	}
	arithEncBitRaw(stream,0);
	arithEncode(stream,val,val+1,max);
}

ulong  cu_getMulting_ari(arithInfo *stream,int init_max,int step_mult)
{
ulong max,ret;

	max = init_max;
	ret = 0;
	for(;;) {
		if ( ! arithDecBitRaw(stream) ) {
			int val;
			val = arithGet(stream,max);
			arithDecode(stream,val,val+1,max);
			return ret+val;
		}
		ret += max;
		max *= step_mult;
		if ( max > stream->safeProbMax ) max = stream->safeProbMax;
	}
}

void cu_putExpandingSigned_bii(int val,struct LBitIOInfo *stream,int init_bits,int step_bits)
{
	cu_putExpanding_bii(abs(val),stream,init_bits,step_bits);
	if ( val )
	{
		val = (val > 0) ? 1 : 0;
		LBitIO_WriteBit(stream,val);
	}
}
int  cu_getExpandingSigned_bii(struct LBitIOInfo *stream,int init_bits,int step_bits)
{
int val;
	
	val = cu_getExpanding_bii(stream,init_bits,step_bits);
	if ( val )
	{
	int sign;
		LBitIO_ReadBit(stream,sign);
		assert(sign == 0 || sign == 1);
		val *= (sign + sign - 1);
	}
return val;
}

void cu_putExpandingSigned_ari(int val,arithInfo *stream,int init_max,int step_max)
{
bool sign;
if ( val < 0 ) { sign = 1; val = -val; } else sign = 0;
arithEncBitRaw(stream,sign);
cu_putExpanding_ari(val,stream,init_max,step_max);
}
int  cu_getExpandingSigned_ari(arithInfo *stream,int init_max,int step_max)
{
int ret;
bool sign;
sign = arithDecBitRaw(stream);
ret= cu_getExpanding_ari(stream,init_max,step_max);
if ( sign ) return - ret; else return ret;
}
void cu_putMultingSigned_ari(int val,arithInfo *stream,int init_max,int step_mult)
{
bool sign;
if ( val < 0 ) { sign = 1; val = -val; } else sign = 0;
arithEncBitRaw(stream,sign);
cu_putMulting_ari(val,stream,init_max,step_mult);
}
int  cu_getMultingSigned_ari(arithInfo *stream,int init_max,int step_mult)
{
int ret;
bool sign;
sign = arithDecBitRaw(stream);
ret= cu_getMulting_ari(stream,init_max,step_mult);
if ( sign ) return - ret; else return ret;
}

