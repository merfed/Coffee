#include <crblib/inc.h>
#include <crblib/memutil.h>
#include <crblib/lbitio.h>

//protos:

struct LBitIOInfo * LBitIO_Init(ubyte *Array);
void LBitIO_CleanUp(struct LBitIOInfo * BII);
long LBitIO_FlushWrite(struct LBitIOInfo * BII); //returns length of data
void LBitIO_ResetArray(struct LBitIOInfo * BII,ubyte *Array);             
long LBitIO_GetPos(struct LBitIOInfo * BII);


const ulong LBitIOReadBitMask = 1<<31 ;

/***
struct LBitIOInfo
	{
	ulong BitBuffer;
	int BitsToGo;

	ubyte *BitArray;
	ubyte *BitArrayPtr;
	};
***/

void LBitIO_ResetArray(struct LBitIOInfo * BII,ubyte *Array)
{
BII->BitBuffer = 0;
BII->BitsToGo = 32;

BII->BitArray = Array;
BII->BitArrayPtr = Array;
}

/*
 *	Allocate and init a BII
 *	for reads, LBitIO_InitRead must also be called
 *
 */
struct LBitIOInfo * LBitIO_Init(ubyte *Array)
{
struct LBitIOInfo * BII;

if ( (BII = malloc(sizeof(struct LBitIOInfo))) == NULL )
	return(NULL);

LBitIO_ResetArray(BII,Array);

return(BII);
}

/*
 *	Free a BII after it has been written or read from
 *	call LBitIO_FlushWrite before writing to a file
 *
 */
void LBitIO_CleanUp(struct LBitIOInfo * BII)
{
free(BII);
}

/*
 *  FlushWrite sticks remaining bits into BitArray
 *  must be called before writing BitArray
 *  returns length of array to write
 *
 */
long LBitIO_FlushWrite(struct LBitIOInfo * BII)
{

BII->BitBuffer <<= BII->BitsToGo;

while ( BII->BitsToGo < 32 )
	{
	*BII->BitArrayPtr++ = BII->BitBuffer>>24;
	BII->BitBuffer <<= 8;
	BII->BitsToGo += 8;
	}

BII->BitsToGo = 32;
BII->BitBuffer = 0;
/* keep going, if you like */

return( (long)( BII->BitArrayPtr - BII->BitArray ) );
}

/*
 *  GetPos returns the current BII position, in byte
 *  does not modify the BII at all
 *
 */
long LBitIO_GetPos(struct LBitIOInfo * BII)
{
long Ret;

	Ret = (int)BII->BitArrayPtr - (int)BII->BitArray;

	if ( BII->BitsToGo < 32 ) 
	{
	long z;

		z = BII->BitsToGo;

		while ( z < 32 )
		{
		Ret++;
		z += 8;
		}
  	}

return(Ret);
}

long LBitIO_GetPosD(struct LBitIOInfo * BII)
{
long Ret,z;

	Ret = (int)BII->BitArrayPtr - (int)BII->BitArray;

	for(z = BII->BitsToGo; z >= 8; z -= 8)
	{
		// we advanced the pointer 4 to get 32 bits, but
		// we haven't used them all yet
		Ret --;
	}

return Ret;
}

ubyte * LBitIO_GetArray(struct LBitIOInfo * BII)
{
BII->BitBuffer <<= BII->BitsToGo;

while ( BII->BitsToGo < 32 )
	{
	*BII->BitArrayPtr++ = BII->BitBuffer>>24;
	BII->BitBuffer <<= 8;
	BII->BitsToGo += 8;
	}

BII->BitsToGo = 32;
BII->BitBuffer = 0;
/* keep going, if you like */

return( BII->BitArrayPtr );
}


void LBitIO_WriteUnary(struct LBitIOInfo * BII,ulong val)
{
	if ( val < 30 )
	{
		LBitIO_WriteBits(BII,1,(int)(val+1));
	}
	else
	{
		LBitIO_WriteBits(BII,1,32);
		LBitIO_WriteBits(BII,val,32);
	}
}

ulong LBitIO_ReadUnary(struct LBitIOInfo * BII)
{
ulong val = 0;

	for(;;)
	{
	bool bit;
		LBitIO_ReadBit(BII,bit);
		if ( bit )
		{
			if ( val == 31 )
			{
				LBitIO_ReadBits(BII,val,32);
			}
			return val;
		}
		val ++;
	}
}