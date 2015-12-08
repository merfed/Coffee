/******

problem : 

encoding/decoding when GotNumSymbols == 1 needs to be handled as
a special case.

I think encoding is Ok, but DecodeBuildTree needs to set NodeBase->Char
to be the one char to code

--

Macros need to be updated to handle this case

*********/

/*
 * notez : there's kind of a bug in FastDecode :
 *	if the last code in decodes to more than one char, you might
 *	skip more bits than were written
 *
 * this can fuck you up in merge-bit-stream cases
 *
 *
 * note: this is handled properly by FastDecodeArray
 *
 */

/*
 * notez : you must PackCodeLens BEFORE BuildEncodeTable
 *  in order to set Min & Max CodeLen
 * if you don't PackCodeLens, call SetMinMaxCodeLen
 */

/*
 *
 * Huffman2 :	7-5-96
 *
 *  not a general purpose Huffman utility
 *	does not generate Huffman codes.  Codes have same lengths, but
 *		values are ordered thus:
 *	for each code length, values are ordered by alphabetic order
 *
 * ---
 * 
 * Encode BuildTree time is nearly identical to old Huffman
 * Decode BuildTree time is MUCH shorter!
 * EncodeC time is identical
 * DecodeC time is shorter
 *
 * ---
 *
 *	7-6-95 : 
 *		FastDecode !  uses table lookup for multi-symbol-at-a-time operation!
 *	
 *	7-7-95
 *		I just peeked the IJG JPEG source & discovered that they use
 *			this method with MaxNumMadeChars = 1
 *
 */

#define DEBUG

/* DEBUG toggles some tests which
	garauntee proper operation, but take some time to do */

#define DO_MEMCLEARS

/* DO_MEMCLEARS toggles some unnecessary MemClears 
	probably a good idea to turn it on for debug purposes */

#define PACKCODES_TOPSYM
/* if this is on, then in PackCodeLens, the value of the highest
	non-zero-count symbol is written, and codes are only written up
	to that symbol ; this helps compression a good deal in most cases */

/*
 *
 * Modularized Static Huffman encoding
 *
 * Notes: works on a LBitIOInfo structure
 *			this structure must be independently allocated and freed
 *
 *	Uses the O(n) Huffman algorithm with QuickSort, RadixSort, or NoSort
 *		NOTEZ: RadixSort assumes counts are bounded in the range [0,MaxCharCount]
 *	if AlphaSize > 64 or so, use the RadixSort
 *
 *	You are responsible for calling
 *		LBitIO_FlushWrite(HI->BII);
 *	yourself.  It is no longer done in EncodeCDone
 *
 *	You must also call
 *		LBitIO_InitRead(HI->BII);
 *	yourself before doing any reads.  It is no longer done in DecodeCInit
 *
 * 7-4-95 new stuff:
 *	1. charcounts are now packed as codelengths only (much better packing)
 *	2. this creates a problem : two chars with different counts but the
 *			same code length are no longer distinguishable by the decoder,
 *			but they were distinguished by the encoder!
 *	3. result : Huffman2 : uses manual code assignment
 *
 *
 */

#include <limits.h>
#include <crblib/inc.h>
#include <crblib/memutil.h>
#include <crblib/lbitio.h>
#include <crblib/intmath.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include <crblib/huffman2.h>

//protos:
struct Huff2Info * Huff2_Init(long NumSymbols,struct LBitIOInfo * BII,long SortType);
void Huff2_CleanUp(struct Huff2Info *HI);

void Huff2_GetMaxCharCount(struct Huff2Info *HI,long * CharCounts);
void Huff2_ScaleCounts(struct Huff2Info *HI,long * CharCounts,long MaxVal);
bool Huff2_BuildCodeLens(struct Huff2Info *HI,long *CharCounts);

bool Huff2_BuildEncodeTable(struct Huff2Info *HI);
bool Huff2_BuildDecodeTable(struct Huff2Info *HI);
bool Huff2_BuildFastDecodeTable(struct Huff2Info *HI);

void Huff2_EncodeC(struct Huff2Info *HI,uword C);
uword Huff2_DecodeC(struct Huff2Info *HI);
uword Huff2_FastDecodeC(struct Huff2Info *HI);
bool Huff2_FastDecodeArray_Ubyte(struct Huff2Info *HI,ubyte * Array,long ArrayLen);

bool Huff2_FindDecodeOne(struct Huff2Info *HI);

void Huff2_SetMinMaxCodeLen(struct Huff2Info *HI);

void Huff2_PackCodeLens(struct Huff2Info *HI);
void Huff2_UnPackCodeLens(struct Huff2Info *HI);                                      

void Huff2_PackCodeLens_Delta(struct Huff2Info *HI,long * LastCodeLens);
void Huff2_UnPackCodeLens_Delta(struct Huff2Info *HI,long * LastCodeLens);

//sort protos:
bool  Huff2RadixSort(struct Huff2CodeNode ** BucketArray,
	long MaxCharCount,struct Huff2CodeNode ** Array,long ArraySize);
void Huff2QuickSort(long Left,long Right);
struct Huff2CodeNode ** Huff2QuickSortArray; //global for recursion non-stacking

//functions:

/*
 */
struct Huff2Info * Huff2_Init(long NumSymbols,struct LBitIOInfo * BII,long SortType)
{
struct Huff2Info * HI;

if ( (HI = AllocMem(sizeof(struct Huff2Info),MEMF_ANY|MEMF_CLEAR)) == NULL )
	return(NULL);

HI->GotNumSymbols = 0;
HI->NumSymbols = NumSymbols;
HI->MaxCharCount = LONG_MAX;
HI->BII = BII;
HI->SortType = SortType;
HI->CodeNodeHunkI = 0;

if ( (HI->CodeLenTable = AllocMem(HI->NumSymbols*sizeof(long),MEMF_ANY|MEMF_CLEAR)) == NULL )
	{
	Huff2_CleanUp(HI);
	return(NULL);
	}

if ( (HI->NumCodesOfLen = AllocMem(32*sizeof(long),MEMF_ANY|MEMF_CLEAR)) == NULL )
	{
	Huff2_CleanUp(HI);
	return(NULL);
	}

if ( (HI->CodePrefixByLen = AllocMem(32*sizeof(ulong),MEMF_ANY|MEMF_CLEAR)) == NULL )
	{
	Huff2_CleanUp(HI);
	return(NULL);
	}

return(HI);
}

/*
 */
void Huff2_CleanUp(struct Huff2Info * HI)
{
SmartFree(HI->NodeWork,sizeofpointer*HI->NumSymbols);
SmartFree(HI->MadeNodeWork,sizeofpointer*HI->NumSymbols*2);
SmartFree(HI->CodeLenTable,HI->NumSymbols*sizeof(long));
SmartFree(HI->NumCodesOfLen,32*sizeof(long));
SmartFree(HI->CodePrefixByLen,32*sizeof(ulong));
SmartFree(HI->CodeNodeHunk,sizeof(struct Huff2CodeNode)*2*HI->NumSymbols);
SmartFree(HI->StackArray,2*(HI->NumSymbols)*sizeof(ulong));
SmartFree(HI->EnDe_codeTable,HI->EnDe_codeTableLen);

if ( HI->SortType == HUFF2_SORT_RADIX )
	{
	SmartFree(HI->SortWork,sizeofpointer*(HI->MaxCharCount + 1));
	}

FreeMem(HI,sizeof(struct Huff2Info));
}

/*
 *	Compute the codelens into Huff2Info from the CharCounts array
 *

NOTEZ: MadeNodeWork is large enough, but you may have the problem of looping
	i.e. ( (MadeNodeWorkIn - MadeNodeWorkOut) < NumSymbols )  is just fine, 
		the problem is ( MadeNodeWorkIn < NumSymbols ) isn't true
	thus all made-array lookups are done: [MadeNodeWorkIn % NumSymbols]

10-15-95 : alloced MadeNodeWork to double length so as to eliminate this.

 */

/*
 *
 * return values :
 *		1 = all ok
 *		0 = error
 *
 */
bool Huff2_BuildCodeLens(struct Huff2Info *HI,long *CharCounts)
{
register struct Huff2CodeNode * CurNode;
register struct Huff2CodeNode ** NodeWork;
register long i,NumSymbols,GotNumSymbols;
/*long swapchar;*/

if ( !HI->CodeNodeHunk ||	!HI->NodeWork || !HI->MadeNodeWork )
	{

	if ( HI->CodeNodeHunk || HI->NodeWork || HI->MadeNodeWork )
		{
		return(0);
		}

	HI->NodeWork = AllocMem(sizeofpointer*HI->NumSymbols,MEMF_ANY);
	HI->MadeNodeWork = AllocMem(sizeofpointer*2*HI->NumSymbols,MEMF_ANY);
	HI->CodeNodeHunk = AllocMem(sizeof(struct Huff2CodeNode)*2*HI->NumSymbols,MEMF_ANY);

	if ( !HI->CodeNodeHunk ||	!HI->NodeWork ||	!HI->MadeNodeWork )
		{
		return(0);
		}
	}

/* reset, if this is an already-used Huff2Info */
if ( HI->NodeBase )
	{
	HI->NodeBase = NULL;
	HI->CodeNodeHunkI = 0;

	HI->GotNumSymbols = 0;
	HI->MaxCharCount = LONG_MAX;
	HI->CodeNodeHunkI = 0;

	HI->MinCodeLen = HI->MaxCodeLen = 0;

#ifdef DO_MEMCLEARS
	MemClearFast((long *)HI->CodeNodeHunk,sizeof(struct Huff2CodeNode)*HI->NumSymbols);
	MemClearFast((long *)HI->NodeWork,HI->NumSymbols);
	MemClearFast((long *)HI->MadeNodeWork,HI->NumSymbols);
#endif // DO_MEMCLEARS

	MemClearFast((long *)HI->CodeLenTable,HI->NumSymbols);
	MemClearFast((long *)HI->NumCodesOfLen,32);
	MemClearFast((long *)HI->CodePrefixByLen,32);
	}

NodeWork = HI->NodeWork;
NumSymbols = HI->NumSymbols;

/*
 *	Read the chars into NodeWork
 *		packing out the NULLs
 *
 */
GotNumSymbols=0;
for(i=0;i<NumSymbols;i++) /* $ 12% !! */
	{
	if ( CharCounts[i] > 0 ) 
		{
		NodeWork[GotNumSymbols] = HI->CodeNodeHunk + HI->CodeNodeHunkI; HI->CodeNodeHunkI++;
		NodeWork[GotNumSymbols]->Char = i;
		NodeWork[GotNumSymbols]->Count = CharCounts[i];
		NodeWork[GotNumSymbols]->Up = NodeWork[GotNumSymbols]->Down = NULL;
		GotNumSymbols++;
		}
	}

/* !!!! you should not do any huffman coding if this ever happens ! */
/* NOTEZ: BuildEncodeTable WILL fail ! */
/* PackCodeLens & UnPackCodeLens will work */
HI->GotNumSymbols = GotNumSymbols;
if ( GotNumSymbols < 2 )
	{
	if ( GotNumSymbols == 0 )
		{
		return(1);
		}
	else
		{
		HI->NodeBase = NodeWork[0];
		HI->NodeBase->Up = NULL;
		HI->NodeBase->Down = NULL;
		HI->CodeLenTable[HI->NodeBase->Char] = 1;
		HI->NumCodesOfLen[1] = 1;
		/* call PackCodeLens anyway, so the decoder knows which char to copy */
		return(1);
		}
	}

/*
 *	sort NodeWork in order of ascending counts
 *
 */
switch(HI->SortType)
	{
	case HUFF2_SORT_NONE:
		break;
	case HUFF2_SORT_RADIX:
		if ( ! HI->SortWork )
			{
			if ( HI->MaxCharCount == LONG_MAX )
				{
				return(0);
				}
			if ( (HI->SortWork = AllocMem(sizeofpointer*(HI->MaxCharCount + 1),MEMF_ANY)) == NULL )
				{
				return(0);
				}
			}
		if ( ! Huff2RadixSort((struct Huff2CodeNode **)HI->SortWork,
			HI->MaxCharCount,NodeWork,GotNumSymbols) ) return(0);
		break;
	case HUFF2_SORT_QSORT:
		Huff2QuickSortArray = NodeWork;
		Huff2QuickSort(0,GotNumSymbols-1);
		break;
	default:
		return(0);
		break;
	}

/* sort puts lowest value of ->Count first */

/* register the tree-building variables */
 {
register long MadeNodeWorkIn,MadeNodeWorkOut,NodeWorkOut;
register struct Huff2CodeNode ** MadeNodeWork;

MadeNodeWork = HI->MadeNodeWork;
NodeWorkOut=0;
MadeNodeWorkIn  = 0;
MadeNodeWorkOut = 0;

/*
 *	Build the tree while there are more leaf-nodes to go
 *
 */
while( NodeWorkOut < (GotNumSymbols-1) ) /* $ 38 % */
	{

	CurNode = HI->CodeNodeHunk + HI->CodeNodeHunkI; HI->CodeNodeHunkI++;
	CurNode->Char = HUFF2_CODE_BRANCH;

#ifdef DEBUG
	if ( HI->CodeNodeHunkI >= (NumSymbols*2) )
		{
		errputs(" HI->CodeNodeHunkI >= (NumSymbols*2) !");	
		exit(1);
		}
#endif // DEBUG

	/*
   *	get the lowest two
   *  if counts are equal, use a madenode before a leafnode
   *
   */
	if ( MadeNodeWorkOut < MadeNodeWorkIn )
		{
		if ( NodeWork[NodeWorkOut]->Count < MadeNodeWork[MadeNodeWorkOut]->Count )
			{
			CurNode->Down = NodeWork[NodeWorkOut]; NodeWorkOut++;
			}
		else
			{
			CurNode->Down = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;
			}
		if ( MadeNodeWorkOut < MadeNodeWorkIn )
			{
			if ( NodeWork[NodeWorkOut]->Count < MadeNodeWork[MadeNodeWorkOut]->Count )
				{
				CurNode->Up = NodeWork[NodeWorkOut]; NodeWorkOut++;
				}
			else
				{
				CurNode->Up = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;
				}	
			}
		else
			{
			CurNode->Up = NodeWork[NodeWorkOut]; NodeWorkOut++;
			}
		}
	else
		{
		CurNode->Down = NodeWork[NodeWorkOut]; NodeWorkOut++;
		CurNode->Up 	= NodeWork[NodeWorkOut]; NodeWorkOut++;
		}

	/* ->Down gets the lower valued count & the lower valued character */

	/* make sure lower char is on the Down pointer */
	/* notez: this is only needed in encode ! */
 /*	don't bother swapping the counts, they're not needed anymore */
/*
	if ( CurNode->Down->Char != HUFF2_CODE_BRANCH &&
				CurNode->Up->Char != HUFF2_CODE_BRANCH )
		{
		if ( CurNode->Down->Char > CurNode->Up->Char )
			{
			swapchar = CurNode->Down->Char;
			CurNode->Down->Char = CurNode->Up->Char;
			CurNode->Up->Char = swapchar;
			}
		}
*/

  /*
   *	put them into a MadeNode
   *
   */
	MadeNodeWork[MadeNodeWorkIn] = CurNode;	MadeNodeWorkIn++;
	CurNode->Count = CurNode->Down->Count + CurNode->Up->Count;
	}

/*
 * we stop the prior loop when there are less than 2 nodework symbols left
 * if there is only one left, this loops until it is eaten off
 *
 */
while ( NodeWorkOut < GotNumSymbols && MadeNodeWorkOut < MadeNodeWorkIn )
	{
	CurNode = HI->CodeNodeHunk + HI->CodeNodeHunkI; HI->CodeNodeHunkI++;
	CurNode->Char = HUFF2_CODE_BRANCH;

#ifdef DEBUG
	if ( HI->CodeNodeHunkI >= (NumSymbols*2) )
		{
		errputs(" HI->CodeNodeHunkI >= (NumSymbols*2) !");	
		exit(1);
		}
#endif // DEBUG

	if ( NodeWork[NodeWorkOut]->Count < MadeNodeWork[MadeNodeWorkOut]->Count )
		{
		CurNode->Down = NodeWork[NodeWorkOut]; NodeWorkOut++;
		}
	else
		{
		CurNode->Down = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;
		}

	if ( MadeNodeWorkOut < MadeNodeWorkIn )
		{
		if ( NodeWorkOut < GotNumSymbols ) 
			{
			if ( NodeWork[NodeWorkOut]->Count < MadeNodeWork[MadeNodeWorkOut]->Count )
				{
				CurNode->Up = NodeWork[NodeWorkOut]; NodeWorkOut++;
				}
			else
				{
				CurNode->Up = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;
				}
			}
		else
			{
			CurNode->Up = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;
			}
		}
	else
		{
		CurNode->Up = NodeWork[NodeWorkOut]; NodeWorkOut++;
		}

	MadeNodeWork[MadeNodeWorkIn] = CurNode;	MadeNodeWorkIn++;
	CurNode->Count = CurNode->Down->Count + CurNode->Up->Count;
	}

/*
 *	there may still be extras in MadeNodeWork
 *
 */
while ( MadeNodeWorkOut < (MadeNodeWorkIn-1) )
	{
	CurNode = HI->CodeNodeHunk + HI->CodeNodeHunkI; HI->CodeNodeHunkI++;
	CurNode->Char = HUFF2_CODE_BRANCH;
	CurNode->Down = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;
	CurNode->Up = MadeNodeWork[MadeNodeWorkOut]; MadeNodeWorkOut++;

#ifdef DEBUG
	if ( HI->CodeNodeHunkI >= (NumSymbols*2) )
		{
		errputs(" HI->CodeNodeHunkI >= (NumSymbols*2) !");	
		exit(1);
		}
#endif // DEBUG

	MadeNodeWork[MadeNodeWorkIn] = CurNode;	MadeNodeWorkIn++;
	CurNode->Count = CurNode->Down->Count + CurNode->Up->Count;
	}

/*
 * grab the base node out
 *
 */
HI->NodeBase = MadeNodeWork[(MadeNodeWorkIn-1)];

if ( HI->NodeBase == NULL ) 
	{
	return(0); /* debugger catch point */
	}

 }
/* end tree-building registered variables */

/* now read out the codelens into an array */
  {
	long * CodeLenTable;
	long * NumCodesOfLen;
	register struct Huff2CodeNode * CurNode;
	register ulong CurCodeLen;
	register ulong GotC;
	register ulong * StackArray;
	register long StackI;
	
	if ( ! HI->StackArray )
		{
		if ( (HI->StackArray = AllocMem(2*(HI->NumSymbols)*sizeof(ulong),MEMF_ANY)) == NULL )
			return(0);
		}
	
	NumCodesOfLen = HI->NumCodesOfLen;
	CodeLenTable = HI->CodeLenTable; 
	StackArray = HI->StackArray;
	
	StackArray[0] = (ulong) HI->NodeBase;
	StackArray[1] = 0;
	StackI = 2;
	while ( StackI > 0 ) /* $ 15% */
		{
		StackI--;
		CurCodeLen = StackArray[StackI];
		StackI--;
		CurNode = (struct Huff2CodeNode *) StackArray[StackI];
	
	  /*	traverse all Up branches w/o Stack bumping
				remember the Down branches as you go
				once you reach the end of an Up, go back and get the	
				most recent Down & traverse it 
		*/
	
		while ( CurNode )
			{
			GotC = CurNode->Char;
			if ( GotC == HUFF2_CODE_BRANCH )
				{
				CurCodeLen++;		

#ifdef DEBUG
				if ( CurCodeLen > 31 )
					{
					fprintf(stderr,"CodeLen > 31 !");
					return(0);
					}
#endif

				if ( CurNode->Down )
					{
					GotC = CurNode->Down->Char;
					if ( GotC == HUFF2_CODE_BRANCH )
						{
						StackArray[StackI++] = (ulong) CurNode->Down;
						StackArray[StackI++] = CurCodeLen;
	
#ifdef DEBUG
						if ( StackI >= (HI->NumSymbols * 2) )
							{
							errputs("Stack usage higher than expected!");
							exit(10);
							}
#endif
	
						}
					else
						{
						CodeLenTable[GotC] = CurCodeLen;
						NumCodesOfLen[CurCodeLen] ++;
						}
					}
	
				CurNode = CurNode->Up;
				}
			else
				{
				CodeLenTable[GotC] = CurCodeLen;
				NumCodesOfLen[CurCodeLen] ++;
				CurNode = NULL;
				}
			}
	
		}
		
	}
/* end codelen-retrieval registered variables */

return(1);
}

/*
 * at this point CodeLenTable & NumCodesOfLen are filled out
 *
 */
bool Huff2_BuildEncodeTable(struct Huff2Info *HI)
{
ulong LastCodePrefix;
ulong * CodePrefixByLen;
ulong * CharToCodeTable;
long * CodeLenTable;
long * NumCodesOfLen;
long NumSymbols;
long i;
long CurCodeLen;
ulong CurCode;

if ( HI->GotNumSymbols < 2 ) return(1);

NumSymbols = HI->NumSymbols;

if ( HI->MaxCodeLen == 0 )
	return(0);
if ( HI->MaxCodeLen > 30 )
	return(0);

if ( HI->EnDe_codeTable )
	{
	CharToCodeTable = (ulong *) HI->EnDe_codeTable;
	MemClear((long *)CharToCodeTable,HI->EnDe_codeTableLen);
	}
else
	{
	if ( (CharToCodeTable = AllocMem(NumSymbols*sizeof(ulong),MEMF_ANY|MEMF_CLEAR)) == NULL )
		return(0);

	HI->EnDe_codeTable = (void *) CharToCodeTable;
	HI->EnDe_codeTableLen = NumSymbols*sizeof(ulong);
	}

CodePrefixByLen = HI->CodePrefixByLen;
CodeLenTable = HI->CodeLenTable;
NumCodesOfLen = HI->NumCodesOfLen;

LastCodePrefix = 0;
CodePrefixByLen[HI->MinCodeLen] = 0;
for(i=(HI->MinCodeLen);i<(HI->MaxCodeLen);)
	{
	LastCodePrefix = (LastCodePrefix + NumCodesOfLen[i]) << 1;
	i++;
	CodePrefixByLen[i] = LastCodePrefix;
	}

for(i=0;i<NumSymbols;i++)
	{
	CurCodeLen = CodeLenTable[i];
	if ( CurCodeLen > 0 )
		{
		CurCode = CodePrefixByLen[CurCodeLen]++;
		CharToCodeTable[i] = CurCode;
		}
	}

return(1);
}

/*
 * at this point CodeLenTable & NumCodesOfLen are filled out
 *
 */
bool Huff2_BuildDecodeTable(struct Huff2Info *HI)
{
ulong BaseCodeByLen[32]; /* I'm lazy! So sue me! */
ulong LastCodePrefix;
ulong * CodePrefixByLen;
uword * DecodeTable;
long * CodeLenTable;
long * NumCodesOfLen;
long NumSymbols;
long i,CurCodeLen;
ulong CurCode,PackedCode;
long NumCodesOfLowerLen;

if ( HI->GotNumSymbols < 2 ) return( Huff2_FindDecodeOne(HI) );

NumSymbols = HI->NumSymbols;

if ( HI->MaxCodeLen == 0 )
	return(0);
if ( HI->MaxCodeLen > 30 )
	return(0);

if ( HI->EnDe_codeTable )
	{
	DecodeTable = (uword *) HI->EnDe_codeTable;
	MemClear((long *)DecodeTable,HI->EnDe_codeTableLen);
	}
else
	{
	HI->EnDe_codeTableLen = 2*NumSymbols*sizeof(uword);
	if ( (DecodeTable = AllocMem(HI->EnDe_codeTableLen,MEMF_ANY|MEMF_CLEAR)) == NULL )
		return(0);

	HI->EnDe_codeTable = (void *) DecodeTable;
	}

CodePrefixByLen = HI->CodePrefixByLen;
CodeLenTable = HI->CodeLenTable;
NumCodesOfLen = HI->NumCodesOfLen;

LastCodePrefix = 0;
NumCodesOfLowerLen = 0;
CodePrefixByLen[HI->MinCodeLen] = 0;
BaseCodeByLen[HI->MinCodeLen] = 0;
for(i=(HI->MinCodeLen);i<(HI->MaxCodeLen);)
	{
	LastCodePrefix = (LastCodePrefix + NumCodesOfLen[i]) << 1;
	NumCodesOfLowerLen += NumCodesOfLen[i];
	i++;
	CodePrefixByLen[i] = LastCodePrefix;
	BaseCodeByLen[i] = LastCodePrefix - NumCodesOfLowerLen;
	}

for(i=0;i<NumSymbols;i++)
	{
	CurCodeLen = CodeLenTable[i];
	if ( CurCodeLen > 0 )
		{
		CurCode = CodePrefixByLen[CurCodeLen]++;
		PackedCode = CurCode - BaseCodeByLen[CurCodeLen];
		DecodeTable[PackedCode] = CurCodeLen;
		DecodeTable[PackedCode+NumSymbols] = i;
		}
	}

for(i=(HI->MinCodeLen);i<=(HI->MaxCodeLen);i++)
	{
	CodePrefixByLen[i] = BaseCodeByLen[i];
	}

return(1);
}

/*
 * The FastDecodeTable is a raw 8 bit table (256 entries)
 *  it is indexed by the next 8 bits of the input bitstream
 *	and the value of the table is :
 *		number of bits used  (max of 8) = 3 bits -> 1byte
 *		number of chars made (max of 4) = 2 bits -> 1 byte
 *		values of chars made (max of 4) = 8 bytes
 * total size = 256*10 = 2560 bytes (not bad!)
 * 	( note that this could be as low as 256*5 = 1280 bytes )
 *
 * note that you must also keep the old decodetable around,
 *	in case the code is longer than 8 bits ( numberofcharsmade == 0 )
 * 
 * <> todo: make this routine take the number of bits of the table
 *	as a parameter
 *
 */

bool Huff2_BuildFastDecodeTable(struct Huff2Info *HI)
{
struct FastDecodeItem * FastDecodeTable;
uword * DecodeTable;
long NumSymbols;
long Huff2CodeLen;
ulong CurCode,StartCode,Huff2Code,CheckCode,ReadCodeMask,PackedCode;
long DecodeTableLen,FastDecodeTableLen;
long MinCodeLen,NumBitsUsed;
uword GotChar;
ulong * CodePrefixByLen;

if ( HI->GotNumSymbols < 2 ) return( Huff2_FindDecodeOne(HI) );

CodePrefixByLen = HI->CodePrefixByLen;
NumSymbols = HI->NumSymbols;
MinCodeLen = HI->MinCodeLen;

if ( HI->MaxCodeLen == 0 )
	return(0);
if ( HI->MaxCodeLen > 30 )
	return(0);

DecodeTableLen = 2*NumSymbols*sizeof(uword);
FastDecodeTableLen = 256*sizeof(struct FastDecodeItem);

if ( HI->EnDe_codeTable )
	{
	DecodeTable = (uword *)HI->EnDe_codeTable;
	FastDecodeTable = (struct FastDecodeItem *) ( (char *)HI->EnDe_codeTable + DecodeTableLen );
	MemClear((long *)DecodeTable,DecodeTableLen);
	MemClear((long *)FastDecodeTable,FastDecodeTableLen);
	}
else
	{
	HI->EnDe_codeTableLen = DecodeTableLen + FastDecodeTableLen;
	if ( (HI->EnDe_codeTable = AllocMem(HI->EnDe_codeTableLen,MEMF_ANY|MEMF_CLEAR)) == NULL )
		return(0);

	DecodeTable = (uword *)HI->EnDe_codeTable;
	FastDecodeTable = (struct FastDecodeItem *) ( ((char *)HI->EnDe_codeTable) + DecodeTableLen ); 
	}

/* build the old style table */
if ( ! Huff2_BuildDecodeTable(HI) )
	return(0);

/* now DecodeTable is filled out */

/* step through all the codes & find what chars correspond to them
		when you find a char for a code, propagate it through all codes which
			have that as a prefix
	 keep going until NumBitsFree < Huff2CodeLen  for all entries */
for(StartCode=0;StartCode<255;StartCode++)
	{
	NumBitsUsed = FastDecodeTable[StartCode].NumBitsUsed;
	while ( (8-NumBitsUsed) >= MinCodeLen && FastDecodeTable[StartCode].NumCharsMade < FD_MAXCHARSMADE )
		{
		CurCode = StartCode ;

		/* find char that encodes to curcode */
		Huff2CodeLen = MinCodeLen;
		Huff2Code = ( CurCode >> (8 - NumBitsUsed - Huff2CodeLen) ) & ( (1<<Huff2CodeLen) - 1 ) ;
		ReadCodeMask = 1 << ( 7 - NumBitsUsed - Huff2CodeLen ) ;
		PackedCode = Huff2Code;
		while( DecodeTable[PackedCode] != Huff2CodeLen )
			{
			Huff2Code += Huff2Code;
			if ( CurCode & ReadCodeMask ) Huff2Code++;
			ReadCodeMask >>= 1;
			Huff2CodeLen++;

			if ( ReadCodeMask == 0 )
				goto DoneWithCurCode;

			PackedCode = Huff2Code - CodePrefixByLen[Huff2CodeLen];			
			}

		if ( Huff2CodeLen > (8 - NumBitsUsed) )
			goto DoneWithCurCode;

		GotChar = DecodeTable[PackedCode+NumSymbols];

		/* step through while Huff2Code is the next code
				all occurances of Huff2Code will be contiguous */
		do
			{
			if ( FastDecodeTable[CurCode].NumCharsMade < FD_MAXCHARSMADE )
				{
				FastDecodeTable[CurCode].CharsMade[FastDecodeTable[CurCode].NumCharsMade] = GotChar;
				FastDecodeTable[CurCode].NumCharsMade++;
				FastDecodeTable[CurCode].NumBitsUsed = NumBitsUsed + Huff2CodeLen;
				}
			CurCode ++;
			NumBitsUsed = FastDecodeTable[CurCode].NumBitsUsed;
			if ( Huff2CodeLen > (8-NumBitsUsed) ) break;
			CheckCode = ( CurCode >> (8 - NumBitsUsed - Huff2CodeLen) ) & ( (1<<Huff2CodeLen) - 1 ) ;
			} while ( CheckCode == Huff2Code );

		NumBitsUsed = FastDecodeTable[StartCode].NumBitsUsed;
		}

	DoneWithCurCode: /* see Knuth : "Structured use of Goto" :^> */ 

	continue;

	}

HI->NumCharsWaiting = 0;

return(1);
}

void Huff2_EncodeC(struct Huff2Info *HI,uword Symbol)
{
register ulong CurCode;
register long CurCodeLen;
register struct LBitIOInfo * BII;

if ( HI->GotNumSymbols < 2 ) return;

CurCode 	 = ((ulong *)HI->EnDe_codeTable)[Symbol];
CurCodeLen = HI->CodeLenTable[Symbol];
BII 			 = HI->BII;

#ifdef DEBUG
if ( CurCodeLen == 0 )
	{
	errputs("Got EncodeC for CodeLen == 0 symbol!");
	exit(10);
	}
#endif

LBitIO_WriteBits(BII,CurCode,CurCodeLen);

}

#define FASTDECODE_PAD 6

bool Huff2_FastDecodeArray_Ubyte(struct Huff2Info *HI,ubyte * Array,long ArrayLen)
{
register ulong PeekedCode;
register struct FastDecodeItem * FastDecodeTable;
long NumCharsMade;
uword * CharsMade;
long NumSymbols;
uword * DecodeTable;
ulong * CodePrefixByLen;
ulong CurCode,PackedCode;
long CurCodeLen;
bool bit;
ubyte *CurArrayPtr,*ArrayPtrDone;
LocalLBitIO_Variables();

if ( HI->GotNumSymbols < 2 )
	{
	if ( HI->GotNumSymbols == 0 ) return(0);
	else
		{
		while(ArrayLen--) *Array++ = HI->OneChar;
		}
	return(1);
	}

LocalLBitIO_GetState(HI->BII);
NumSymbols = HI->NumSymbols;
FastDecodeTable = (struct FastDecodeItem *) ( ((char *)HI->EnDe_codeTable) + 2*NumSymbols*sizeof(uword) );
DecodeTable = (uword *)HI->EnDe_codeTable;
CodePrefixByLen = HI->CodePrefixByLen;
CurArrayPtr = Array;
ArrayPtrDone = Array + ArrayLen - FASTDECODE_PAD;

while ( CurArrayPtr < ArrayPtrDone )
	{
	LocalLBitIO_PeekBits(PeekedCode,8);
	
	NumCharsMade = FastDecodeTable[PeekedCode].NumCharsMade;
	if ( NumCharsMade > 0 )
		{
		CurCodeLen = FastDecodeTable[PeekedCode].NumBitsUsed;
		LocalLBitIO_SkipBits(CurCodeLen);
		switch ( NumCharsMade )
			{
			case 1:
				*CurArrayPtr++ = *(FastDecodeTable[PeekedCode].CharsMade);
				break;
			case 2:
				CharsMade = FastDecodeTable[PeekedCode].CharsMade;
				*CurArrayPtr++ = *CharsMade++;
				*CurArrayPtr++ = *CharsMade;
				break;
			case 3:
				CharsMade = FastDecodeTable[PeekedCode].CharsMade;
				*CurArrayPtr++ = *CharsMade++;
				*CurArrayPtr++ = *CharsMade++;
				*CurArrayPtr++ = *CharsMade;
				break;
			case 4:
				CharsMade = FastDecodeTable[PeekedCode].CharsMade;
				*CurArrayPtr++ = *CharsMade++;
				*CurArrayPtr++ = *CharsMade++;
				*CurArrayPtr++ = *CharsMade++;
				*CurArrayPtr++ = *CharsMade;
				break;
			}
		}
	else /* use old decode method, can read 9 bits automatically */
		{
		CurCodeLen = 8;
		CurCode = PeekedCode;
		LocalLBitIO_SkipBits(8);
		PackedCode = CurCode - CodePrefixByLen[CurCodeLen];
		
		/* could read another bit here, but who cares? */	

		while( DecodeTable[PackedCode] != CurCodeLen )
			{
			CurCode += CurCode;
			LocalLBitIO_ReadBit(bit);
			CurCode += bit;
			CurCodeLen++;
		
			PackedCode = CurCode - CodePrefixByLen[CurCodeLen];
			}
		
		*CurArrayPtr++ = DecodeTable[PackedCode+NumSymbols];
		}
	}

LocalLBitIO_PutState(HI->BII);

/* do normal decode for last several to make sure we don't go too far */

ArrayPtrDone += FASTDECODE_PAD;
while ( CurArrayPtr < ArrayPtrDone )
	{
	*CurArrayPtr++ = Huff2_DecodeC(HI);
	}

return(1);
}

bool Huff2_FindDecodeOne(struct Huff2Info *HI)
{
uword i;

for(i=0;i<HI->NumSymbols;i++)
	{
	if ( HI->CodeLenTable[i] > 0 )
		{
		HI->OneChar = i;
		return(1);
		}
	}
return(0);
}

uword Huff2_FastDecodeC(struct Huff2Info *HI)
{

if ( HI->GotNumSymbols < 2 ) return(HI->OneChar);

#if FD_MAXCHARSMADE_SUBONE > 0
if ( HI->NumCharsWaiting > 0 )
	{
	HI->NumCharsWaiting--;
	return(HI->CharsWaiting[HI->NumCharsWaiting]);
	}
#endif // FD_MAXCHARSMADE_SUBONE == 0

/* variables not init-ed if there are chars waiting */
 {
struct FastDecodeItem * FastDecodeTable;
ulong PeekedCode;
struct LBitIOInfo * BII;
long NumSymbols;

NumSymbols = HI->NumSymbols;

FastDecodeTable = (struct FastDecodeItem *) ( ((char *)HI->EnDe_codeTable) + 2*NumSymbols*sizeof(uword) );
BII = HI->BII;

LBitIO_PeekBits(BII,PeekedCode,8);

if ( FastDecodeTable[PeekedCode].NumCharsMade > 0 )
	{
	LBitIO_SkipBits(BII,FastDecodeTable[PeekedCode].NumBitsUsed);
#if FD_MAXCHARSMADE_SUBONE > 0
	HI->NumCharsWaiting = FastDecodeTable[PeekedCode].NumCharsMade - 1;
	switch ( HI->NumCharsWaiting ) /* !<>! */
		{
		case 0:
			break;
		case 1:
			HI->CharsWaiting[0] = FastDecodeTable[PeekedCode].CharsMade[1];
			break;
		case 2:
			HI->CharsWaiting[1] = FastDecodeTable[PeekedCode].CharsMade[1];
			HI->CharsWaiting[0] = FastDecodeTable[PeekedCode].CharsMade[2];
			break;
		case 3:
			HI->CharsWaiting[2] = FastDecodeTable[PeekedCode].CharsMade[1];
			HI->CharsWaiting[1] = FastDecodeTable[PeekedCode].CharsMade[2];
			HI->CharsWaiting[0] = FastDecodeTable[PeekedCode].CharsMade[3];
			break;
		}
#endif // FD_MAXCHARSMADE_SUBONE == 0
	return( FastDecodeTable[PeekedCode].CharsMade[0] );
	}
else /* use old decode method, can read 9 bits automatically */
	{
	uword * DecodeTable;
	ulong * CodePrefixByLen;
	ulong CurCode,PackedCode;
	long CurCodeLen;
	bool bit;
	
	DecodeTable = (uword *)HI->EnDe_codeTable;
	CodePrefixByLen = HI->CodePrefixByLen;
	
	CurCodeLen = 8;
	CurCode = PeekedCode;
	LBitIO_SkipBits(BII,8);
	PackedCode = CurCode - CodePrefixByLen[CurCodeLen];

	/* could read another bit here, but who cares? */	

	while( DecodeTable[PackedCode] != CurCodeLen )
		{
		CurCode += CurCode;
		LBitIO_ReadBit(BII,bit);
		CurCode += bit;
		CurCodeLen++;
	
		PackedCode = CurCode - CodePrefixByLen[CurCodeLen];
		}
	
#ifdef DEBUG
	if ( CurCodeLen > HI->MaxCodeLen )
		{
		errputs("Got code longer than MaxCodeLen!");
		exit(10);
		}
#endif
	
	return(DecodeTable[PackedCode+NumSymbols]);
	}

 }
/* end variable-space */

}

uword Huff2_DecodeC(struct Huff2Info *HI)
{
uword * DecodeTable;
long NumSymbols;
ulong * CodePrefixByLen;
ulong CurCode,PackedCode;
long CurCodeLen;
struct LBitIOInfo * BII;
bool bit;

if ( HI->GotNumSymbols < 2 ) return(HI->OneChar);

NumSymbols = HI->NumSymbols;
DecodeTable = (uword *) HI->EnDe_codeTable;
CodePrefixByLen = HI->CodePrefixByLen;
BII = HI->BII;

CurCodeLen = HI->MinCodeLen;
LBitIO_ReadBits(BII,CurCode,CurCodeLen);
PackedCode = CurCode;

while( DecodeTable[PackedCode] != CurCodeLen )
	{
	CurCode += CurCode;
	LBitIO_ReadBit(BII,bit);
	CurCode += bit;
	CurCodeLen++;

	PackedCode = CurCode - CodePrefixByLen[CurCodeLen];
	}

#ifdef DEBUG
if ( CurCodeLen > HI->MaxCodeLen )
	{
	errputs("Got code longer than MaxCodeLen!");
	exit(10);
	}
#endif

return(DecodeTable[PackedCode+NumSymbols]);
}

/*
 * Scale Counts to [0,MaxVal] 
 *  (brackets indicate the range is "inclusive")
 *
 * a count which was > 0 will not be scaled to 0
 *
 */
void Huff2_ScaleCounts(struct Huff2Info *HI,long * CharCounts,long MaxVal)
{
long MaxCharCount;
long NumSymbols,i;
long OutCharCount;

NumSymbols = HI->NumSymbols;

//get maxcharcount
MaxCharCount = 0;
for(i=0;i<NumSymbols;i++)
	{
	if ( CharCounts[i] > MaxCharCount ) MaxCharCount = CharCounts[i];
	}

//scale counts
if ( MaxCharCount > MaxVal )
	{
	for(i=0;i<NumSymbols;i++)
		{
		OutCharCount = ( (long)CharCounts[i] * MaxVal ) / MaxCharCount;
		if ( OutCharCount > MaxVal ) OutCharCount = MaxVal;
		else if ( CharCounts[i] && ! OutCharCount ) OutCharCount = 1;
		CharCounts[i] = OutCharCount;
		}
	}

if ( HI->MaxCharCount != MaxVal && HI->SortType == HUFF2_SORT_RADIX &&
			HI->SortWork != NULL )
	{
	FreeMem(HI->SortWork,sizeofpointer*(HI->MaxCharCount+1));
	HI->SortWork = NULL;
	}

HI->MaxCharCount = min(MaxCharCount,MaxVal);

}

/*
 * Sets ->MaxCharCount in HI
 *
 */
void Huff2_GetMaxCharCount(struct Huff2Info *HI,long * CharCounts)
{
long MaxCharCount;
long NumSymbols,i;

NumSymbols = HI->NumSymbols;

//get maxcharcount
MaxCharCount = 0;
for(i=0;i<NumSymbols;i++)
	{
	if ( CharCounts[i] > MaxCharCount ) MaxCharCount = CharCounts[i];
	}

if ( HI->MaxCharCount != MaxCharCount && HI->SortType == HUFF2_SORT_RADIX &&
			HI->SortWork != NULL )
	{
	FreeMem(HI->SortWork,sizeofpointer*(HI->MaxCharCount+1));
	HI->SortWork = NULL;
	}

HI->MaxCharCount = MaxCharCount;
}

/*
 * sets MinCodeLen & MaxCodeLen which must be set
 *	for BuildEncodeTable
 *
 */
void Huff2_SetMinMaxCodeLen(struct Huff2Info *HI)
{
long MinCodeLen,MaxCodeLen;

if ( HI->GotNumSymbols < 2 )
	{ HI->MinCodeLen = HI->MaxCodeLen = 1; return; }

MinCodeLen = 1;
while ( HI->NumCodesOfLen[MinCodeLen] == 0 ) MinCodeLen++;
MaxCodeLen = 31;
while ( HI->NumCodesOfLen[MaxCodeLen] == 0 ) MaxCodeLen--;

HI->MaxCodeLen = MaxCodeLen;
HI->MinCodeLen = MinCodeLen;
}

/*
 * Writes codelens out to the BII
 *
 * also sets MinCodeLen & MaxCodeLen which must be set
 *	for BuildEncodeTable
 *
 */
void Huff2_PackCodeLens(struct Huff2Info *HI)
{
long MinCodeLen,MaxCodeLen;
long i,j,GotNumSymbols;
ubyte RunData[32];
long RunLen;
long CurCodeLen;
struct LBitIOInfo * BII;

BII = HI->BII;

#ifdef PACKCODES_TOPSYM
GotNumSymbols = HI->NumSymbols;
while ( HI->CodeLenTable[GotNumSymbols-1] == 0 ) GotNumSymbols--;
if ( (HI->NumSymbols - GotNumSymbols) < 32 )
	{
	LBitIO_WriteBit(BII,0);
	GotNumSymbols = HI->NumSymbols;
	}
else
	{
	LBitIO_WriteBit(BII,1);
	CurCodeLen = intlog2(HI->NumSymbols);
	LBitIO_WriteBits(BII,GotNumSymbols,CurCodeLen);
	}
#else
GotNumSymbols = HI->NumSymbols;
#endif // PACKCODES_TOPSYM

MinCodeLen = 1;
while ( HI->NumCodesOfLen[MinCodeLen] == 0 ) MinCodeLen++;
MaxCodeLen = 31;
while ( HI->NumCodesOfLen[MaxCodeLen] == 0 ) MaxCodeLen--;

HI->MaxCodeLen = MaxCodeLen;
HI->MinCodeLen = MinCodeLen;

if ( MaxCodeLen >= 0xF ) {
	LBitIO_WriteBits(BII,0xF,4);
	LBitIO_WriteBits(BII,MaxCodeLen-0xF,4);
} else {
	LBitIO_WriteBits(BII,MaxCodeLen,4);
}

for(i=0;i<GotNumSymbols;) {
	RunLen = 0;
	while ( HI->CodeLenTable[i] == 0 && RunLen < 32 && i<GotNumSymbols ) {
		RunLen++;
		i++;
	}
	if ( RunLen > 0 ) {
		LBitIO_WriteBit(BII,0);
		LBitIO_WriteBits(BII,(RunLen-1),5);
	}

	RunLen = 0;
	while ( ( HI->CodeLenTable[i] != 0 || HI->CodeLenTable[i+1] != 0 ) && RunLen < 32 && i<GotNumSymbols )
		{
		RunData[RunLen] = HI->CodeLenTable[i];
		RunLen++; i++;
		}

	if ( RunLen > 0 )
		{
		LBitIO_WriteBit(BII,1);
		LBitIO_WriteBits(BII,(RunLen-1),5);

		for(j=0;j<RunLen;j++)
			{
			CurCodeLen = MaxCodeLen - RunData[j];
			while ( CurCodeLen >= 0x7 )
				{
				LBitIO_WriteBits(BII,0x7,3);
				CurCodeLen -= 0x7;
				}
			LBitIO_WriteBits(BII,CurCodeLen,3);
			}
		}
	}

}

/*
 * reads packed codelens to CodeLenTable
 *
 * also sets NumCodesOfLen
 *
 */
void Huff2_UnPackCodeLens(struct Huff2Info *HI)
{
long i,j,GotNumSymbols;
long MinCodeLen,MaxCodeLen,CurCodeLen;
long RunLen,CurCode;
struct LBitIOInfo * BII;
bool bit;

HI->GotNumSymbols = 0;

MemClearMacroFast(HI->CodeLenTable,HI->NumSymbols);
MemClearMacroFast(HI->NumCodesOfLen,32);
HI->MinCodeLen = HI->MaxCodeLen = 0;

BII = HI->BII;

#ifdef PACKCODES_TOPSYM
LBitIO_ReadBit(BII,bit);
if ( bit )
	{
	CurCodeLen = intlog2(HI->NumSymbols);
	LBitIO_ReadBits(BII,GotNumSymbols,CurCodeLen);
	}
else
	{
	GotNumSymbols = HI->NumSymbols;
	}
#else
GotNumSymbols = HI->NumSymbols;
#endif // PACKCODES_TOPSYM

LBitIO_ReadBits(BII,CurCode,4);
if ( CurCode == 0xF )
	{
	LBitIO_ReadBits(BII,CurCode,4);
	MaxCodeLen = CurCode + 0xF;
	}
else
	{
	MaxCodeLen = CurCode;
	}
HI->MaxCodeLen = MaxCodeLen;

MinCodeLen = MaxCodeLen;

for(i=0;i<GotNumSymbols; )
	{
	LBitIO_ReadBit(BII,bit);
	LBitIO_ReadBits(BII,RunLen,5);
	RunLen++;
	if ( bit == 0 )
		{
		for(j=0;j<RunLen;j++)
			{
			HI->CodeLenTable[i++] = 0;
			}
		}
	else
		{
		HI->GotNumSymbols += RunLen;
		for(j=0;j<RunLen;j++)
			{
			CurCodeLen = 0;
			LBitIO_ReadBits(BII,CurCode,3);
			while( CurCode == 0x7 )
				{
				CurCodeLen += 0x7;
				LBitIO_ReadBits(BII,CurCode,3);
				}
			CurCodeLen += CurCode;

			CurCodeLen = MaxCodeLen - CurCodeLen;

			HI->CodeLenTable[i++] = CurCodeLen;
			if ( CurCodeLen > 0 )	HI->NumCodesOfLen[CurCodeLen] ++;

			if ( CurCodeLen < MinCodeLen && CurCodeLen > 0 ) MinCodeLen = CurCodeLen;
			}
		}
	}

HI->MinCodeLen = MinCodeLen;
}

/*
 * Writes codelens out to the BII
 *
 *	compresses differences for an earlier huff build
 *	 you must allocate lastcodelens, & set to 0 initially
 *
 * also sets MinCodeLen & MaxCodeLen which must be set
 *	for BuildEncodeTable
 *
 */
void Huff2_PackCodeLens_Delta(struct Huff2Info *HI,long * LastCodeLens)
{
long MinCodeLen,MaxCodeLen;
long i,j,GotNumSymbols;
ubyte RunData[512],RunDataLast[512];
long RunLen;
long CurCodeLen;
struct LBitIOInfo * BII;

BII = HI->BII;

#ifdef PACKCODES_TOPSYM
GotNumSymbols = HI->NumSymbols;
while ( HI->CodeLenTable[GotNumSymbols-1] == 0 ) GotNumSymbols--;
if ( (HI->NumSymbols - GotNumSymbols) < 32 )
	{
	LBitIO_WriteBit(BII,0);
	GotNumSymbols = HI->NumSymbols;
	}
else
	{
	LBitIO_WriteBit(BII,1);
	CurCodeLen = intlog2(HI->NumSymbols);
	LBitIO_WriteBits(BII,GotNumSymbols,CurCodeLen);
	}
#else
GotNumSymbols = HI->NumSymbols;
#endif // PACKCODES_TOPSYM

MinCodeLen = 1;
while ( HI->NumCodesOfLen[MinCodeLen] == 0 ) MinCodeLen++;
MaxCodeLen = 31;
while ( HI->NumCodesOfLen[MaxCodeLen] == 0 ) MaxCodeLen--;

HI->MaxCodeLen = MaxCodeLen;
HI->MinCodeLen = MinCodeLen;

if ( MaxCodeLen >= 0xF )
	{
	LBitIO_WriteBits(BII,0xF,4);
	LBitIO_WriteBits(BII,MaxCodeLen-0xF,4);
	}
else
	{
	LBitIO_WriteBits(BII,MaxCodeLen,4);
	}

for(i=0;i<GotNumSymbols;)
	{
	RunLen = 0;
	while ( ( HI->CodeLenTable[i] - LastCodeLens[i] ) == 0 && i<GotNumSymbols )
		{
		RunLen++;
		i++;
		}
	if ( RunLen >= 0x03 )
		{
		LBitIO_WriteBits(BII,0x03,2);
		RunLen -= 0x3;
		if ( RunLen >= 0x07 )
			{
			long Thresh,Bits;

			LBitIO_WriteBits(BII,0x07,3);
			RunLen -= 0x7;

			Thresh = 0xF; Bits = 4;
			while ( RunLen >= Thresh )
				{
				LBitIO_WriteBits(BII,Thresh,Bits);
				RunLen -= Thresh;
				Thresh += Thresh + 1; Bits++;
				}
			LBitIO_WriteBits(BII,RunLen,Bits);
			}
		else
			{
			LBitIO_WriteBits(BII,RunLen,3);
			}	
		}
	else
		{
		LBitIO_WriteBits(BII,RunLen,2);
		}
	if ( i == GotNumSymbols ) break;

	RunLen = 0;
	while ( ( ( HI->CodeLenTable[i] - LastCodeLens[i] ) != 0 ) && i<GotNumSymbols )
		{
		RunData[RunLen] = HI->CodeLenTable[i];
		RunDataLast[RunLen] = LastCodeLens[i];
		RunLen++; i++;
		if ( RunLen == 512 ) return; /* BAD ! */
		}
	if ( RunLen >= 0x03 )
		{
		j = RunLen;
		LBitIO_WriteBits(BII,0x03,2);
		RunLen -= 0x3;
		if ( RunLen >= 0x07 )
			{
			long Thresh,Bits;

			LBitIO_WriteBits(BII,0x07,3);
			RunLen -= 0x7;

			Thresh = 0xF; Bits = 4;
			while ( RunLen >= Thresh )
				{
				LBitIO_WriteBits(BII,Thresh,Bits);
				RunLen -= Thresh;
				Thresh += Thresh + 1; Bits++;
				}
			LBitIO_WriteBits(BII,RunLen,Bits);
			}
		else
			{
			LBitIO_WriteBits(BII,RunLen,3);
			}	
		RunLen = j;
		}
	else
		{
		LBitIO_WriteBits(BII,RunLen,2);
		}

	if ( RunLen > 0 )
		{
		for(j=0;j<RunLen;j++)
			{
			if ( RunDataLast[j] == 0 )
				{
				CurCodeLen = MaxCodeLen - RunData[j];
				while ( CurCodeLen >= 0x7 )
					{
					LBitIO_WriteBits(BII,0x7,3);
					CurCodeLen -= 0x7;
					}
				LBitIO_WriteBits(BII,CurCodeLen,3);				
				}
			else
				{
				CurCodeLen = RunData[j] - RunDataLast[j];
				if ( CurCodeLen < 0 )
					{
					LBitIO_WriteBit(BII,0);
					CurCodeLen = - (CurCodeLen + 1);
					}
				else
					{
					LBitIO_WriteBit(BII,1);
					CurCodeLen--;
					}
				while ( CurCodeLen > 0 )
					{
					LBitIO_WriteBit(BII,0);
					CurCodeLen --;
					}
				LBitIO_WriteBit(BII,1);
				}
			}
		}
	}

for(i=0;i<HI->NumSymbols;i++)
	{
	LastCodeLens[i] = HI->CodeLenTable[i];
	}

}

/*
 * reads packed codelens to CodeLenTable
 *
 * also sets NumCodesOfLen
 *
 * <> doesn't decode new style of runlen yet
 *
 */
void Huff2_UnPackCodeLens_Delta(struct Huff2Info *HI,long * LastCodeLens)
{
long i,j,GotNumSymbols;
long MinCodeLen,MaxCodeLen,CurCodeLen;
long RunLen,CurCode;
struct LBitIOInfo * BII;
bool bit;
long sign;

BII = HI->BII;

MemClear((long *)HI->NumCodesOfLen,32*sizeof(long));

#ifdef PACKCODES_TOPSYM
LBitIO_ReadBit(BII,bit);
if ( bit )
	{
	CurCodeLen = intlog2(HI->NumSymbols);
	LBitIO_ReadBits(BII,GotNumSymbols,CurCodeLen);
	}
else
	{
	GotNumSymbols = HI->NumSymbols;
	}
#else
GotNumSymbols = HI->NumSymbols;
#endif // PACKCODES_TOPSYM

LBitIO_ReadBits(BII,CurCode,4);
if ( CurCode == 0xF )
	{
	LBitIO_ReadBits(BII,CurCode,4);
	MaxCodeLen = CurCode + 0xF;
	}
else
	{
	MaxCodeLen = CurCode;
	}
HI->MaxCodeLen = MaxCodeLen;

MinCodeLen = MaxCodeLen;

for(i=0;i<GotNumSymbols; )
	{
	LBitIO_ReadBit(BII,bit);
	LBitIO_ReadBits(BII,RunLen,5);
	RunLen++;
	if ( bit == 0 )
		{
		for(j=0;j<RunLen;j++)
			{
			HI->CodeLenTable[i] = LastCodeLens[i]; i++;
			}
		}
	else
		{
		for(j=0;j<RunLen;j++)
			{
			if ( LastCodeLens[i] > 0 )
				{
				LBitIO_ReadBit(BII,bit);
				if ( bit ) sign = -1;
				else sign = 1;
				LBitIO_ReadBit(BII,bit);
				CurCodeLen = 1;
				while( bit == 0 )
					{
					CurCodeLen++;
					LBitIO_ReadBit(BII,bit);
					}

				CurCodeLen = CurCodeLen * sign + LastCodeLens[i];
				}
			else
				{
				CurCodeLen = 0;
				LBitIO_ReadBits(BII,CurCode,3);
				while( CurCode == 0x7 )
					{
					CurCodeLen += 0x7;
					LBitIO_ReadBits(BII,CurCode,3);
					}
				CurCodeLen += CurCode;
	
				CurCodeLen = MaxCodeLen - CurCodeLen;
				}

			HI->CodeLenTable[i++] = CurCodeLen;
			if ( CurCodeLen > 0 )	HI->NumCodesOfLen[CurCodeLen] ++;

			if ( CurCodeLen < MinCodeLen && CurCodeLen > 0 ) MinCodeLen = CurCodeLen;
			}
		}
	}

for(i=0;i<HI->NumSymbols;i++)
	{
	LastCodeLens[i] = HI->CodeLenTable[i];
	}

HI->MinCodeLen = MinCodeLen;
}


/*
 * QuickSort on the NodeWork array
 *  set Huff2QuickSortArray
 *
 */
void Huff2QuickSort(long Left,long Right)
{
if ( (Right - Left) > 1 )
	{
	long i;
		{
		long j;
		long pivot;
		struct Huff2CodeNode *swapper;
		register struct Huff2CodeNode ** Array;

		Array = Huff2QuickSortArray;

		pivot = Array[Right]->Count;
		i = Left-1;
		j = Right;
		for(;;)
			{
			/* could remove i < Right && j > Left checking by putting
				dummy array slots in [-1] and [GotNumSymbols] w/ Counts 0 and LONG_MAX */
			do ++i; while(Array[i]->Count < pivot && i < Right);
			do --j; while(Array[j]->Count > pivot && j > Left);
			if (i >= j) break;
			swapper  = Array[i];
			Array[i] = Array[j];
			Array[j] = swapper;
			}
		swapper  = Array[i];
		Array[i] = Array[Right];
		Array[Right] = swapper;
		}
	Huff2QuickSort(Left,i-1);
	Huff2QuickSort(i+1,Right);
	}
else
	{
	if ( Right > Left )
		{
		if ( Huff2QuickSortArray[Right]->Count < Huff2QuickSortArray[Left]->Count )
			{
			struct Huff2CodeNode *swapper;

			swapper  = Huff2QuickSortArray[Left];
			Huff2QuickSortArray[Left] = Huff2QuickSortArray[Right];
			Huff2QuickSortArray[Right] = swapper;
			}
		}
	}
}

/*
 * Radix sort on the NodeWork
 *	counts must be bounded by [0,MaxCharCount]
 * this is much faster than QuickSort for large NumSymbols
 *
 * I think NumSymbols as small as 256 is still faster here
 *	    Radix = O(256 + 2*NumSymbols)
 *	QuickSort = O(NumSymbols*log2(NumSymbols))
 * bigOs are equal for NumSymbols == 64
 *
 * NOTEZ: uses the ->Up in the codenode structure
 *  			resets ->Up to NULL when its done
 *
 */
bool  Huff2RadixSort(struct Huff2CodeNode ** BucketArray,
	long MaxCharCount,struct Huff2CodeNode ** Array,long ArraySize)
{
register struct Huff2CodeNode * CurNode;
register long i,j;

MemClear((long *)BucketArray,sizeofpointer*(MaxCharCount+1));

/* go backwards so that alphabetic order is preserved */
for(i=(ArraySize-1);i>=0;i--)
	{
	Array[i]->Up = BucketArray[Array[i]->Count];
	BucketArray[Array[i]->Count] = Array[i];
	}

j=0;
for(i=0;i<=MaxCharCount;i++)
	{
	CurNode = BucketArray[i];
	while(CurNode) /* $ 15% */
		{
		Array[j] = CurNode;
		CurNode = CurNode->Up;
		Array[j]->Up = NULL;
		j++;
		}
	}

if ( j != ArraySize )
	{
	return(0);
	}

return(1);
}
