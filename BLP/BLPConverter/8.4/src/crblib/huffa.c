
/* #define DEBUG */

#include <crblib/inc.h>
#include <crblib/strutil.h>
#include <crblib/fileutil.h>
#include <crblib/memutil.h>
#include <crblib/lbitio.h>
#include <crblib/huffman2.h>
#include <crblib/codeutil.h>
#include <crblib/runtrans.h>
#include <crblib/huffa.h> /* for type defines */

#include <malloc.h>
#include <assert.h>

#ifdef _MSC_VER
#pragma warning(disable : 4018)
#endif

#ifdef _DEBUG
#undef assert
#define assert(x)	if( ! (x) ) __asm{ int 03 } else
#endif

/*** protos **/

#define CleanUp(exitmess) { HuffExitMess = exitmess; goto EndOfFunc; }

long MaxIn(long *Array,long Len); /* find max utility */

bool HuffArray(ubyte *RawArray,ulong RawLen,
		 ubyte *HuffArray,ulong * HuffArrayLenPtr,int Type);

bool O0HuffArray(ubyte *RawArray,ulong RawLen,
		 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag);

bool O0HuffArrayNoBlock(ubyte *RawArray,ulong RawLen,
							 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag);

bool O0HuffArrayBII_RT(ubyte *rawArray,ulong rawLen,struct LBitIOInfo * BII,bool cFlag);
bool O0HuffArrayBII(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag);
bool O0HuffArrayBII_block(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag);
bool O0HuffArrayBII_noblock(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag);

bool O1HuffArray(ubyte *RawArray,ulong RawLen,
							 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag);

/*********************** choose order 0 or 1 or raw ***********/

bool HuffArray(ubyte *RawArray,ulong RawLen,
		 ubyte *HuffArray,ulong * HuffArrayLenPtr,int Type)
{
char * HuffExitMess = 0;

if ( RawLen == 0 ) return true;

if ( Type == HUFFA_TYPE_BEST )
	{
	ubyte *TypePtr;
	ulong BestLen,CurLen;

	TypePtr = HuffArray++;

	Type = HUFFA_TYPE_NONE;
	BestLen = RawLen;

	if ( ! O0HuffArray(RawArray,RawLen,HuffArray,&CurLen,TRUE) ) return(0);

	if ( CurLen < BestLen ) { BestLen = CurLen; Type = HUFFA_TYPE_O0; }

	if ( ! O0HuffArrayNoBlock(RawArray,RawLen,HuffArray,&CurLen,TRUE) ) return(0);

	if ( CurLen < BestLen ) { BestLen = CurLen; Type = HUFFA_TYPE_O0NB; }

	if ( ! O1HuffArray(RawArray,RawLen,HuffArray,&CurLen,TRUE) ) return(0);
	
	if ( CurLen < BestLen ) { BestLen = CurLen; Type = HUFFA_TYPE_O1; }

	if ( Type == HUFFA_TYPE_O1 ) { } /* done */
	else if ( Type == HUFFA_TYPE_O0 )
		{
		if ( ! O0HuffArray(RawArray,RawLen,HuffArray,&CurLen,1) ) return(0);
		}
	else if ( Type == HUFFA_TYPE_NONE )
		{
		memcpy(HuffArray,RawArray,RawLen);
		CurLen = RawLen;
		}

	*TypePtr = Type;
	*HuffArrayLenPtr = CurLen+1;
	}
else
	{
	bool success=FALSE,CompressFlag;

	if ( Type == HUFFA_TYPE_DEC )
		CompressFlag = FALSE;
	else
		CompressFlag = TRUE;

	if ( CompressFlag )
		*HuffArray++ = Type;
	else
		Type = *HuffArray++;

	switch(Type)
		{
		case HUFFA_TYPE_O1:
			success = O1HuffArray(RawArray,RawLen,HuffArray,HuffArrayLenPtr,CompressFlag);
			break;
		case HUFFA_TYPE_O0:
			success = O0HuffArray(RawArray,RawLen,HuffArray,HuffArrayLenPtr,CompressFlag);
			break;
		case HUFFA_TYPE_O0NB:
			success = O0HuffArrayNoBlock(RawArray,RawLen,HuffArray,HuffArrayLenPtr,CompressFlag);
			break;
		case HUFFA_TYPE_NONE:
			if ( CompressFlag )
				{
				memcpy(HuffArray,RawArray,RawLen);
				*HuffArrayLenPtr = RawLen;
				}
			else
				memcpy(RawArray,HuffArray,RawLen);
			success = TRUE;
			break;
		default:
			CleanUp("Got invalid type flag");
			break;	
		}

	if ( CompressFlag )
		*HuffArrayLenPtr = (*HuffArrayLenPtr) + 1;
	
	return(success);
	}

EndOfFunc:

if ( HuffExitMess )
  {
  errputs(HuffExitMess);
  return(FALSE);
  }
else
  {
  return(TRUE);
  }
}

/************** Order 0 ******************/

#define HUFF_MINLEN 8

bool O0HuffArray(ubyte *RawArray,ulong RawLen,
							 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag)
{
struct LBitIOInfo * BII;
bool ret;

if ( RawLen == 0 ) return true;

if ( (BII = LBitIO_Init(HuffArray)) == NULL )
	{ errputs("LBitIO_Init failed!"); return(0); }

if ( ! CompressFlag )
	{
	LBitIO_InitRead(BII);
	ret = O0HuffArrayBII(RawArray,RawLen,BII,CompressFlag);
	}
else
	{
	ret = O0HuffArrayBII(RawArray,RawLen,BII,CompressFlag);
	*HuffArrayLenPtr = LBitIO_FlushWrite(BII);
	}

LBitIO_CleanUp(BII);

return(ret);
}

bool O0HuffArrayNoBlock(ubyte *RawArray,ulong RawLen,
							 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag)
{
struct LBitIOInfo * BII;
bool ret;

if ( RawLen == 0 ) return true;

if ( (BII = LBitIO_Init(HuffArray)) == NULL )
	{ errputs("LBitIO_Init failed!"); return(0); }

if ( ! CompressFlag )
	{
	LBitIO_InitRead(BII);
	ret = O0HuffArrayBII_noblock(RawArray,RawLen,BII,CompressFlag);
	}
else
	{
	ret = O0HuffArrayBII_noblock(RawArray,RawLen,BII,CompressFlag);
	*HuffArrayLenPtr = LBitIO_FlushWrite(BII);
	}

LBitIO_CleanUp(BII);

return(ret);
}

/** kind of a cheezy way to do the _RT **/

bool O0HuffArrayBII_RT(ubyte *rawArray,ulong rawLen,struct LBitIOInfo * BII,bool cFlag)
{
ubyte *runArray;
ulong runLen;
bool ret;

	if ( (runArray = malloc(rawLen + (rawLen>>3) + 1024)) == NULL )
		return false;

	if ( cFlag ) {
		runLen = doRunTransform(rawArray,rawLen,runArray);
		if ( runLen < rawLen ) {
			LBitIO_WriteBit(BII,1);
			cu_putExpanding_bii(runLen,BII,14,4);
			ret = O0HuffArrayBII(runArray,runLen,BII,cFlag);
		} else {
			LBitIO_WriteBit(BII,0);
			ret = O0HuffArrayBII(rawArray,rawLen,BII,cFlag);
		}
	} else {
		bool doRT;
		LBitIO_ReadBit(BII,doRT);
		if ( doRT ) {
			runLen = cu_getExpanding_bii(BII,14,4);
			ret = O0HuffArrayBII(runArray,runLen,BII,cFlag);
			unRunTransform(rawArray,rawLen,runArray);
		} else {
			ret = O0HuffArrayBII(rawArray,rawLen,BII,cFlag);
		}
	}

	free(runArray);

return ret;
}

#define DOBLOCK_MINLEN 1024
#define DOBLOCK_DIVISOR 4

bool O0HuffArrayBII(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag)
{

if ( RawLen == 0 ) return true;

if ( RawLen < DOBLOCK_MINLEN )
	{
	return( O0HuffArrayBII_noblock(RawArray,RawLen,BII,CompressFlag) );
	}
else
	{
	bool doblock=0;

	if ( CompressFlag )
		{
		long MaxCount,i;
		long * CharCounts = NULL;

		if ( (CharCounts = malloc(256*sizeof(long))) == NULL )
			{ errputs("AllocMem failed!"); return(0); }

		MemClear(CharCounts,256*sizeof(long));
		for(i=0;i<RawLen;i++) CharCounts[RawArray[i]] ++;
		MaxCount = CharCounts[MaxIn(CharCounts,256)];
		free(CharCounts);

		if ( (MaxCount*DOBLOCK_DIVISOR) >= RawLen ) doblock = 1;
		else doblock = 0;
		}

	if ( CompressFlag )	{	LBitIO_WriteBit(BII,doblock);	}
	else { LBitIO_ReadBit(BII,doblock); }

	if ( doblock ) return( O0HuffArrayBII_block(RawArray,RawLen,BII,CompressFlag) );
	else return( O0HuffArrayBII_noblock(RawArray,RawLen,BII,CompressFlag) );
	}

return(0);
}

/** this is the core routine of it all, the only one that
		actually does huffman: **/

bool O0HuffArrayBII_noblock(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag)
{
struct Huff2Info * HI = NULL;
long * CharCounts = NULL;
char * HuffExitMess = NULL;

if ( RawLen == 0 ) return true;

#ifdef DEBUG
if ( CompressFlag )
	{
	LBitIO_WriteBits(BII,0xD,5);
	}
else
	{
	int test;
	LBitIO_ReadBits(BII,test,5);
	if ( test != 0xD ) { errputs("o0noblock:init:didn't get tag"); return(0); }
	}
#endif

if ( RawLen < HUFF_MINLEN )
	{
	if ( CompressFlag )
		{
		while(RawLen--)
			{
			LBitIO_WriteBits(BII,*RawArray,8); RawArray++;
			}
		}
	else
		{
		while(RawLen--)
			{
			LBitIO_ReadBits(BII,*RawArray,8); RawArray++;
			}
		}
	return(1);
	}

if ( (HI = Huff2_Init(256,BII,HUFF2_SORT_RADIX)) == NULL )
	CleanUp("Huff2_Init failed!");

if ( ! CompressFlag )
	{
	Huff2_UnPackCodeLens(HI);
	Huff2_BuildFastDecodeTable(HI);
	Huff2_FastDecodeArray_Ubyte(HI,RawArray,RawLen);
  }
else //Encode
	{
	long i;

	if ( (CharCounts = malloc(256*sizeof(long))) == NULL )
		CleanUp("AllocMem failed!");

	MemClear(CharCounts,256*sizeof(long));
	for(i=0;i<RawLen;i++) CharCounts[RawArray[i]] ++;

	Huff2_ScaleCounts(HI,CharCounts,256);
	Huff2_BuildCodeLens(HI,CharCounts);
	Huff2_PackCodeLens(HI);
	Huff2_BuildEncodeTable(HI);

	Huff2_EncodeC_Macro_Init(HI);
	for(i=0;i<RawLen;i++)
		{
		Huff2_EncodeC_Macro(HI,RawArray[i]);
		}
	Huff2_EncodeC_Macro_Done(HI);

  }

CleanUp(NULL);

EndOfFunc:

#ifdef DEBUG
if ( CompressFlag )
	{
	LBitIO_WriteBits(BII,0xD,5);
	}
else
	{
	int test;
	LBitIO_ReadBits(BII,test,5);
	if ( test != 0xD ) { errputs("O0noblock:EOF:didn't get tag"); }
	}
#endif

smartfree(CharCounts);
if ( HI ) Huff2_CleanUp(HI);

if ( HuffExitMess )
  {
  errputs(HuffExitMess);
  return(FALSE);
  }
else
  {
  return(TRUE);
  }
}

bool O0HuffArrayBII_block(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag)
{
long * CharCounts = NULL;
char * HuffExitMess = NULL;
ubyte * BlockArray = NULL;
ubyte * LitArray = NULL;
ulong BlockLen,NumLits;
ubyte MPS0,MPS1,MPS2;

if ( RawLen == 0 ) return true;

BlockLen = ((RawLen-1)/4) + 1;
if ( (BlockArray = malloc(BlockLen)) == NULL )
	CleanUp("block malloc failed");

if ( ! CompressFlag )
	{
	long bi,v,ri,li,shift;

	LBitIO_ReadBits(BII,MPS0,8);
	LBitIO_ReadBits(BII,MPS1,8);
	LBitIO_ReadBits(BII,MPS2,8);

	if ( ! O0HuffArrayBII_noblock(BlockArray,BlockLen,BII,0) )
		CleanUp("o0_noblock failed");

	NumLits=0;
	for(bi=0;bi<BlockLen;bi++)
		{
		v=0x3;
		for(li=4;li--;)
			{
			if ( ( BlockArray[bi] & v ) == v ) NumLits++;
			v <<= 2;
			}
		}

	if ( NumLits > 0 )
		{
		if ( (LitArray = malloc(NumLits)) == NULL )
			CleanUp("lits malloc failed");

		if (! O0HuffArrayBII_noblock(LitArray,NumLits,BII,0) )
			CleanUp("o0_noblock failed");
		}

	ri = li= 0;
	for(bi=0;bi<BlockLen;bi++)
		{
		v = BlockArray[bi];
		for(shift=6;shift>=0;shift-=2)
			{
			switch( ((v>>shift)&0x3) )
				{
				case 0: RawArray[ri++] = MPS0; break;
				case 1: RawArray[ri++] = MPS1; break;
				case 2: RawArray[ri++] = MPS2; break;
				case 3: RawArray[ri++] = LitArray[li++]; break;
				}
			if ( ri == RawLen )
				{
				if ( bi != (BlockLen-1) ) CleanUp("HuffBlock:didn't read enough blocks!");
				CleanUp(NULL);
				}
			}
		if ( li > NumLits ) CleanUp("HuffBlock:Read too many literals!");
		}
	if ( ri != RawLen ) CleanUp("HuffBlock:Didn't write enough!");
  }
else //Encode
	{
	long bi,bcnt,v,li,ri,c;

	if ( (LitArray = malloc(RawLen)) == NULL )
		CleanUp("malloc failed");

	if ( (CharCounts = AllocMem(256*sizeof(long),MEMF_CLEAR)) == NULL )
		CleanUp("AllocMem failed!");

	for(ri=0;ri<RawLen;ri++) CharCounts[RawArray[ri]] ++;

	MPS0 = MaxIn(CharCounts,256); CharCounts[MPS0] = 0;
	MPS1 = MaxIn(CharCounts,256); CharCounts[MPS1] = 0;
	MPS2 = MaxIn(CharCounts,256); CharCounts[MPS2] = 0;

	LBitIO_WriteBits(BII,MPS0,8);
	LBitIO_WriteBits(BII,MPS1,8);
	LBitIO_WriteBits(BII,MPS2,8);

	ri = li= 0;
	for(bi=0;bi<BlockLen;bi++)
		{
		v = 0;
		for(bcnt=4;bcnt--;)
			{
			v <<= 2;
			if ( ri >= RawLen ) c = MPS0;
			else c = RawArray[ri++];

			if ( c == MPS0 ) v += 0;
			else if (c == MPS1 ) v += 1;
			else if ( c == MPS2 ) v += 2;
			else { v += 3; LitArray[li++] = c; }
			}
		BlockArray[bi] = v;
		}

	NumLits = li;

	if ( ! O0HuffArrayBII_noblock(BlockArray,BlockLen,BII,1) )
		CleanUp("o0_noblock failed");
 
	if ( ! O0HuffArrayBII_noblock(LitArray,NumLits,BII,1) )
		CleanUp("o0_noblock failed");

  }

CleanUp(NULL);

EndOfFunc:

smartfree(LitArray);
smartfree(BlockArray);
smartfree(CharCounts);

if ( HuffExitMess )
  {
  errputs(HuffExitMess);
  return(FALSE);
  }
else
  {
  return(TRUE);
  }
}


/**************** Order 1 ********************/

//#define MERGE_LEN 64
#define MERGE_LEN 999999

#define BASEC (0x20)	//space

void o1len_write(struct LBitIOInfo * BII,ulong len)
{
if ( len == 0 )
	{
	LBitIO_WriteBit(BII,0);
	}
else
	{
	ulong nbits,mask;

	LBitIO_WriteBit(BII,1);
	len--;

	nbits = 4;
	mask = (1<<nbits)-1;
	while( len >= mask )
		{
		LBitIO_WriteBits(BII,mask,nbits);
		len -= mask;
		nbits += 4;
		if ( nbits >= 32 ) mask = 0xFFFFFFFF;
		else mask = (1<<nbits)-1;
		}
	LBitIO_WriteBits(BII,len,nbits);
	}
return;
}

ulong o1len_read(struct LBitIOInfo *BII)
{
ulong readval;
ulong len;

LBitIO_ReadBit(BII,readval);
if ( readval == 0 )
	{
	return(0);
	}
else
	{
	ulong nbits,mask;

	len = 1;

	nbits = 4;
	do
		{
		if ( nbits >= 32 ) mask = 0xFFFFFFFF;
		else mask = (1<<nbits)-1;
		LBitIO_ReadBits(BII,readval,nbits);
		len += readval;
		nbits += 4;
		} while(readval == mask);

	return(len);
	}

}

bool O1HuffArray(ubyte *RawArray,ulong RawLen,
							 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag)
{
struct LBitIOInfo * BII = NULL;
char * HuffExitMess = NULL;
ubyte ** o1Arrays = NULL;
ubyte ** o1ArrayPtrs = NULL;
ulong * o1ArrayLens = NULL;
int lc;
ulong len;
ubyte *mergePtr;
ubyte * RawArrayPtr;

if ( RawLen == 0 ) return true;

if ( (BII = LBitIO_Init(HuffArray)) == NULL )
	CleanUp("LBitIO_Init failed!");

if ( (o1Arrays = malloc(257*sizeofpointer)) == NULL )
	CleanUp("malloc failed!");

if ( (o1ArrayPtrs = malloc(257*sizeofpointer)) == NULL )
	CleanUp("malloc failed!");

if ( (o1ArrayLens = malloc(257*sizeof(ulong))) == NULL )
	CleanUp("malloc failed!");

memset(o1Arrays,0,257*sizeofpointer);
memset(o1ArrayPtrs,0,257*sizeofpointer);
memset(o1ArrayLens,0,257*sizeof(ulong));

if ( ! CompressFlag )
	{
	ulong totlen=0;

	LBitIO_InitRead(BII);

	for(lc=0;lc<256;lc++)
	{
		o1ArrayLens[lc] = len = o1len_read(BII);
		assert(len <= RawLen);
	}

	for(lc=0;lc<256;lc++)
	{
		len = o1ArrayLens[lc];
		if ( len > 0 )
		{
			totlen += len;

			if ( (o1Arrays[lc] = malloc(len)) == NULL )
				CleanUp("malloc failed!");

			if ( len >= MERGE_LEN )
			{
				o1ArrayPtrs[lc] = o1Arrays[lc];
				if ( ! O0HuffArrayBII(o1Arrays[lc],len,BII,0) )
					CleanUp("O0HuffArrayBII failed");
			}
		}
	}

	assert(totlen == RawLen);

	lc = 256;
	o1ArrayLens[lc] = len = o1len_read(BII);
	assert(len <= RawLen);
	if ( len > 0 )
	{
		if ( (o1Arrays[lc] = malloc(len)) == NULL )
			CleanUp("malloc failed!");

		o1ArrayPtrs[lc] = o1Arrays[lc];
		if ( ! O0HuffArrayBII(o1Arrays[lc],len,BII,0) )
			CleanUp("O0HuffArrayBII failed");
	}

	totlen = o1ArrayLens[256];
	assert(totlen <= RawLen );
	mergePtr = o1Arrays[256];
	for(lc=0;lc<256;lc++)
	{
		if ( o1ArrayLens[lc] > 0 && o1ArrayLens[lc] < MERGE_LEN )
		{
			ubyte * aPtr;
			len = o1ArrayLens[lc];
			totlen -= len;
			assert(totlen >= 0);
			aPtr = o1Arrays[lc];
			while(len--)
			{
				*aPtr++ = *mergePtr++;
			}
			o1ArrayPtrs[lc] = o1Arrays[lc];
		}
	}

	if ( totlen != 0 )
		CleanUp("sum of merged arrays lens != MergeArrayLen");

	lc=BASEC;
	RawArrayPtr = RawArray;
	while(RawLen--)
	{
	int c;
		assert( o1ArrayPtrs[lc] && o1ArrayPtrs[lc] < (o1Arrays[lc] + o1ArrayLens[lc] ) );
		c = *o1ArrayPtrs[lc]++;
		*RawArrayPtr++ = lc = c;
	}
  }
else //Encode
	{
	ulong i;
	ubyte * CurArray;

	lc=BASEC;
	RawArrayPtr = RawArray;
	for(i=RawLen;i--;)
	{
		o1ArrayLens[lc]++;
		lc = *RawArrayPtr++;
	}

	o1ArrayLens[256] = RawLen;
	for(lc=0;lc<257;lc++)
	{
		if ( o1ArrayLens[lc] > 0 )
		{
			if ( (o1Arrays[lc] = malloc(o1ArrayLens[lc])) == NULL )
				CleanUp("malloc failed!");
		}
		o1ArrayPtrs[lc] = o1Arrays[lc];
	}
	o1ArrayLens[256] = 0;

	lc=BASEC;
	RawArrayPtr = RawArray;
	for(i=RawLen;i--;)
	{
		*o1ArrayPtrs[lc]++ = *RawArrayPtr;
		lc = *RawArrayPtr++;
	}

	mergePtr = o1Arrays[256];

	assert( _heapchk() == _HEAPOK );

	for(lc=0;lc<256;lc++)
	{
		len = o1ArrayLens[lc];
		assert( o1ArrayPtrs[lc] == o1Arrays[lc] + o1ArrayLens[lc] );
		o1len_write(BII,len);
	}

	for(lc=0;lc<256;lc++)
	{
		len = o1ArrayLens[lc];
		if ( len >= MERGE_LEN )
		{
			if ( ! O0HuffArrayBII(o1Arrays[lc],len,BII,1) )
				CleanUp("O0HuffArrayBII failed");
		}
		else if ( len > 0 )
		{
			o1ArrayLens[256] += len;
			CurArray = o1Arrays[lc];
			while(len--) 
				*mergePtr++ = *CurArray++;
		}
	}

	assert( o1ArrayLens[256] <= RawLen );
	assert( mergePtr == (o1Arrays[256] + o1ArrayLens[256]) );

	len = o1ArrayLens[256];
	o1len_write(BII,len);
	if ( len > 0 )
	{
		if ( ! O0HuffArrayBII(o1Arrays[256],len,BII,1) )
			CleanUp("O0HuffArrayBII failed")
	}

	assert( _heapchk() == _HEAPOK );

	assert( HuffArrayLenPtr );
	*HuffArrayLenPtr = LBitIO_FlushWrite(BII);
  }

CleanUp(NULL);

EndOfFunc:

smartfree(o1ArrayLens);
smartfree(o1ArrayPtrs);
if ( o1Arrays )
	{
	for(lc=0;lc<257;lc++)
	{
		if ( o1Arrays[lc] )
		{
			assert( _heapchk() == _HEAPOK );
			free( o1Arrays[lc] );
		}
	}
	free(o1Arrays);
	}
if ( BII ) LBitIO_CleanUp(BII);

if ( HuffExitMess )
  {
  errputs(HuffExitMess);
  return(FALSE);
  }
else
  {
  return(TRUE);
  }
}



long MaxIn(long *Array,long Len)
{
long Max = -1,Found = 0,i;
for(i=0;i<Len;i++) { if ( Array[i] > Max ) { Max = Array[i]; Found = i; } }
return(Found);
}

