#include <crblib/inc.h>

#include "bbitio.h"

/*protos:*/

struct BitIOInfo * BitIO_Init(ubyte *Array);
void BitIO_CleanUp(struct BitIOInfo * BII);
long BitIO_FlushWrite(struct BitIOInfo * BII); /*returns length of data*/
void BitIO_ResetArray(struct BitIOInfo * BII,ubyte *Array);             

const int BBitIOReadBitMask = 0x80;

void BitIO_ResetArray(struct BitIOInfo * BII,ubyte *Array)
{
BII->BitBuffer = 0;
BII->BitsToGo = 8;

BII->BitArrayPtr = BII->BitArray = Array;
}

void BitIO_ResetArrayPtr(struct BitIOInfo * BII,ubyte *Array)
{
BII->BitBuffer = 0;
BII->BitsToGo = 8;
BII->BitArrayPtr = Array;
}

/*
 *  Allocate and init a BII
 *  for reads, BitIO_InitRead must also be called
 *
 */
struct BitIOInfo * BitIO_Init(ubyte *Array)
{
struct BitIOInfo * BII;

if ( (BII = malloc(sizeof(struct BitIOInfo))) == NULL )
  return(NULL);

BitIO_ResetArray(BII,Array);

return(BII);
}

/*
 *  Free a BII after it has been written or read from
 *  call BitIO_FlushWrite before writing to a file
 *
 */
void BitIO_CleanUp(struct BitIOInfo * BII)
{
free(BII);
}

/*
 *  FlushWrite sticks remaining bits into BitArray
 *  must be called before writing BitArray
 *  returns length of array to write
 *
 */
long BitIO_FlushWrite(struct BitIOInfo * BII)
{

if ( BII->BitsToGo != 8 )
  {
  BII->BitBuffer <<= BII->BitsToGo;
  *BII->BitArrayPtr++ = (ubyte)BII->BitBuffer;

  BII->BitsToGo = 8;
  BII->BitBuffer = 0;
  /* keep going, if you like */
  }

return( BII->BitArrayPtr - BII->BitArray );
}

void BitIO_FlushRead(struct BitIOInfo * BII)
{
	if ( BII->BitsToGo != 8 ) {
		BitIO_InitRead(BII);
	}
}

long BitIO_GetPos(struct BitIOInfo * BII)
{
long Ret;

Ret = BII->BitArrayPtr - BII->BitArray;

if ( BII->BitsToGo != 8 ) Ret++;

return(Ret);
}

long BitIO_GetPosD(struct BitIOInfo * BII)
{
long Ret;

Ret = BII->BitArrayPtr - BII->BitArray;

if ( BII->BitsToGo == 8 ) Ret--; /** we've advanced the pointer but not read from it **/

return(Ret);
}

ubyte * BitIO_GetArray(struct BitIOInfo * BII)
{
BitIO_FlushWrite(BII);
return( BII->BitArrayPtr );
}

void BitIO_WriteUnary(struct BitIOInfo * BII,ulong val)
{
	while(val)
	{
		val--;
		BitIO_WriteZeroBit(BII);
	}
	BitIO_WriteOneBit(BII);
}

ulong BitIO_ReadUnary(struct BitIOInfo * BII)
{
ulong val = 0;

	for(;;)
	{
	bool bit;
		BitIO_ReadBit(BII,bit);
		if ( bit )
			return val;
		val ++;
	}
}
