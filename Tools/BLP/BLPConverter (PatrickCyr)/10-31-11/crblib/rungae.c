/*}{**********************************************************************************/

#include <crblib/inc.h>
#include <crblib/arithc.h>
#include <crblib/rungae.h>
#include <crblib/arithc._h>
#include <crblib/ladder.h>

/*}{**********************************************************************************/

void rungDecoderInit(void)
{
}

uint rungGetP0Shifted10(rung_t * rung)
{
const srung * s;

	s = &ladder[*rung];

	return (s->p0 << (10 - RUNG_SHIFTS));
}

void rungModelInit(rung_t * rung)
{
	*rung = ladder_start;
}

void rungModelSetByCounts(rung_t * prung,int c0,int c1)
{
uint p0;
rung_t r;

	assert( c0 >= 1 && c1 >= 1);
	p0 = (c0 << RUNG_SHIFTS)/(c0 + c1);	

	r = 0;
	while( p0 > ladder[r].p0 )
	{
		r++;
		if ( r == ladder_start )
		{
			*prung = ladder_start - 1;
		}
	}
	*prung = r;
}

void rungModelEncBit(arithInfo * ari,bool bit,rung_t * rung)
{
ulong code,range;

	code    = ari->code;
	range   = ari->range;

	{
	ulong r;
	const srung * s;
	
		s = &ladder[*rung];
		assert( s->p0 > 0 && s->p0 < RUNG_ONE );

		r = (range >> RUNG_SHIFTS) * (s->p0);

		if ( bit )
		{
			code += r;
			range -= r;
			*rung = s->r1;
			assert( ladder[*rung].p0 <= s->p0 );
		}
		else 
		{
			range = r;
			*rung = s->r0;
			assert( ladder[*rung].p0 >= s->p0 );
		}
	}

	arithEncRenorm(ari,code,range);
}

/*}{**********************************************************************************/

bool rungModelDecBit(arithInfo * ari,rung_t * rung)
{
ulong range,code;

	range = ari->range;
	code = ari->code;

	assert(range > 0);
	assert(range <= One );

	while ( range <= MinRange )
	{
		range <<= 8;
		code = (code<<8) + (((ari->queue)<<EXTRA_BITS)&0xFF);	// use the top bit in the queue
		ari->queue = *(ari->outPtr)++;
		code += (ari->queue) >> (TAIL_EXTRA_BITS);
	}

	assert(range > 0);
	assert(range <= One );

	{
	ulong r;
	const srung * s;
	
		s = &ladder[*rung];
		assert( s->p0 > 0 && s->p0 < RUNG_ONE );

		r = (range >> RUNG_SHIFTS) * (s->p0);

		if ( code >= r )
		{
			code -= r;
			range -= r;
			*rung = s->r1;
			assert( ladder[*rung].p0 <= s->p0 );
			
			ari->range = range;
			ari->code  = code;

			return 1;
		}
		else 
		{
			range = r;
			*rung = s->r0;
			assert( ladder[*rung].p0 >= s->p0 );
			
			ari->range = range;
			ari->code  = code;

			return 0;
		}
	}
}

void rungModelBit(rung_t * rung,bool bit)
{
const srung * s;
	s = &ladder[*rung];
	if ( bit )
		*rung = s->r1;
	else
		*rung = s->r0;
}

void rungEncodeUnary(arithInfo *ari,uint val,rung_t * rung)
{
	while(val--)
	{
		rungModelEncBit(ari,1,rung);
	}
	rungModelEncBit(ari,0,rung);
}

uint rungDecodeUnary(arithInfo *ari,rung_t * rung)
{
uint val;
	val = 0;
	while( rungModelDecBit(ari,rung) )
	{
		val++;
	}
return val;
}

void rungEncodeUnaryMax(arithInfo *ari,uint val,rung_t * rung,uint max)
{
	if ( val == max )
	{
		while(val--)
		{
			rungModelEncBit(ari,1,rung);
		}
		rungModelBit(rung,0);
	}
	else
	{
		while(val--)
		{
			rungModelEncBit(ari,1,rung);
		}
		rungModelEncBit(ari,0,rung);
	}
}

uint rungDecodeUnaryMax(arithInfo *ari,rung_t * rung,uint max)
{
uint val;
	val = 0;
	while( rungModelDecBit(ari,rung) )
	{
		val++;
		if ( val == max )
		{
			rungModelBit(rung,0);
			return val;
		}
	}
return val;
}

/*}{**********************************************************************************/

