#ifndef CRB_HUFFMAN2_H
#define CRB_HUFFMAN2_H

/*
 *  Modular Static Huffman Routines
 * 
 *  These routines do no file IO.  They write to and from a LLBitIO structure.
 *      this structure must be independently allocated and freed
 *
 *    You are responsible for calling 
 *      LBitIO_FlushWrite(HI->BII);
 *    yourself.  It is no longer done in EncodeCDone
 *
 *    You must also call
 *      LBitIO_InitRead(HI->BII);
 *    yourself before doing any reads.  It is no longer done in DecodeCInit
 *
 *
 *  Notez:
 *    Huff2_BuildCodeLens changes the values in the CharCounts table!
 *
 */


/*
 * notez : you must PackCodeLens BEFORE BuildEncodeTable
 *  in order to set Min & Max CodeLen
 * if you don't PackCodeLens, call SetMinMaxCodeLen
 */

/*
 *
 * return values for BuildCodeLens :
 *		1 = all ok
 *		0 = error
 *
 */

/*
 *	WARNING :
 * 
 *		do not call Huff2_BuildFastDecodeTable &
 *			Huff2_BuildDecodeTable both on the same Huff2Info !
 *
 *		they use the same memory space, and they use it differently!
 *
 *		you will crash when Huff2_CleanUp is called !
 *
 */

#include <crblib/mempool.h>
#include <crblib/lbitio.h>

struct Huff2CodeNode
  {
	uword Char;
	uword Count;
  struct Huff2CodeNode * Up;
  struct Huff2CodeNode * Down;
  };

#define HUFF2_CODE_BRANCH 0xFFFF

#define FD_MAXCHARSMADE 4
#define FD_MAXCHARSMADE_SUBONE 3
/* see ' !<>! ' marker */

struct Huff2Info
	{
	long NumSymbols;
	long GotNumSymbols;
	uword OneChar;
	struct LBitIOInfo * BII;

	long MinCodeLen,MaxCodeLen;
	ulong * CodePrefixByLen;
	long * NumCodesOfLen;
	long * CodeLenTable;
	void * EnDe_codeTable;
	long EnDe_codeTableLen;

	/* FastDecode buffer */
	long NumCharsWaiting;
	uword CharsWaiting[FD_MAXCHARSMADE_SUBONE];

	/* BuildCodeLen stuff */
	struct Huff2CodeNode * NodeBase;
	struct Huff2CodeNode ** NodeWork;
	struct Huff2CodeNode ** MadeNodeWork;
	struct Huff2CodeNode * CodeNodeHunk;
	long CodeNodeHunkI;
	ulong * StackArray;
	long SortType;
	void * SortWork;
	long MaxCharCount;
	};

struct FastDecodeItem
	{
	ubyte NumBitsUsed;
	ubyte NumCharsMade;
	uword CharsMade[FD_MAXCHARSMADE];
	};

#define HUFF2_SORT_NONE  1 // data must be pre-sorted !
#define HUFF2_SORT_RADIX 2
#define HUFF2_SORT_QSORT 3

//protos:
extern struct Huff2Info * Huff2_Init(long NumSymbols,struct LBitIOInfo * BII,long SortType);
extern void Huff2_CleanUp(struct Huff2Info *HI);

extern void Huff2_GetMaxCharCount(struct Huff2Info *HI,long * CharCounts);
extern void Huff2_ScaleCounts(struct Huff2Info *HI,long * CharCounts,long MaxVal);
extern bool Huff2_BuildCodeLens(struct Huff2Info *HI,long *CharCounts);

extern bool Huff2_BuildEncodeTable(struct Huff2Info *HI);
extern bool Huff2_BuildDecodeTable(struct Huff2Info *HI);
extern bool Huff2_BuildFastDecodeTable(struct Huff2Info *HI);

extern void Huff2_EncodeC(struct Huff2Info *HI,uword C);
extern uword Huff2_DecodeC(struct Huff2Info *HI);
extern uword Huff2_FastDecodeC(struct Huff2Info *HI);
extern bool Huff2_FastDecodeArray_Ubyte(struct Huff2Info *HI,ubyte * Array,long ArrayLen);

extern void Huff2_SetMinMaxCodeLen(struct Huff2Info *HI);

extern void Huff2_PackCodeLens(struct Huff2Info *HI);
extern void Huff2_UnPackCodeLens(struct Huff2Info *HI);                                      

extern void Huff2_PackCodeLens_Delta(struct Huff2Info *HI,long * LastCodeLens);
extern void Huff2_UnPackCodeLens_Delta(struct Huff2Info *HI,long * LastCodeLens);

/*
 * macros for faster operation
 *
 */

/********** EncodeC macros ************/

#define Huff2_EncodeC_Macro_Init(HI)                                       \
{                                                                 	       \
register ulong * CharToCodeTable;                                 	       \
register long * CodeLenTable;                                     	       \
register struct LBitIOInfo * BII;                                 	       \
register ulong CurCode;                                           	       \
register long CurCodeLen;                                         	       \
bool docoding=true;                                                        \
BII = HI->BII;                                                    	       \
CodeLenTable = HI->CodeLenTable;                                  	       \
CharToCodeTable = (ulong *)HI->EnDe_codeTable;                    	       \
if ( HI->GotNumSymbols < 2) docoding = false;
/* end Huff2_EncodeC_Macro_Init */

#define Huff2_EncodeC_Macro_Done(HI)                                       \
}                                                                          \
/* end Huff2_EncodeC_Macro_Done */

#define Huff2_EncodeC_Macro(HI,Symbol)                                     \
if ( ! docoding ) { } else { CurCode 	 = CharToCodeTable[Symbol];          \
CurCodeLen = CodeLenTable[Symbol];                                         \
LBitIO_WriteBits(BII,CurCode,CurCodeLen); }                                \
/* end Huff2_EncodeC_Macro */

/********** end EncodeC macros ************/
	
/********** DecodeC macros ************/

/* no decodec macros.  use FastDecodeArray */

#endif // HUFFMAN2_H
