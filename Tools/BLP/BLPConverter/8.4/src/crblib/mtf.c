/*
 * routines to convert an array into its MTF position values
 *
 */

#include <stdlib.h>
#include <crblib/inc.h>
#include <crblib/memutil.h>

//protos:
struct MTFInfo * MTF_Init(long NumSymbols);
void MTF_CleanUp(struct MTFInfo * MTFI);
int MTF_Translate(struct MTFInfo * MTFI,int val); /* -1 error */
int MTF_Untranslate(struct MTFInfo * MTFI,int pos); /* -1 error */
bool MTF_TransformArray(struct MTFInfo * MTFI,ubyte * Array,long ArrayLen);
bool MTF_UntransformArray(struct MTFInfo * MTFI,ubyte * Array,long ArrayLen);

//structs:
struct MTFNode
	{
	struct MTFNode * Next;
	struct MTFNode * Prev;
	long Char;
	};

struct MTFInfo
	{
	struct MTFNode * Head;
	struct MTFNode * NodeBlock;
	long NumSymbols;
	};

/*
 *  MTF Transform stuff
 *
 */

struct MTFInfo * MTF_Init(long NumSymbols)
{
struct MTFInfo * MTFI;
struct MTFNode *CurNode;
long i;

if ( (MTFI = AllocMem(sizeof(struct MTFInfo),MEMF_ANY|MEMF_CLEAR)) == NULL )
	return(NULL);

if ( (MTFI->NodeBlock = AllocMem(sizeof(struct MTFNode)*NumSymbols,MEMF_ANY)) == NULL )
	{
	FreeMem(MTFI,sizeof(struct MTFInfo));
	return(NULL);
	}

MTFI->NumSymbols = NumSymbols;

CurNode = MTFI->Head = MTFI->NodeBlock;
CurNode->Prev = NULL;
for(i=0;i<(NumSymbols-1);i++)
	{
	CurNode->Next = MTFI->NodeBlock + i + 1;
	CurNode->Next->Prev = CurNode;
	CurNode->Char = i;
	CurNode = CurNode->Next;
	}
CurNode->Next = NULL;
CurNode->Char = NumSymbols - 1;

return(MTFI);
}

void MTF_CleanUp(struct MTFInfo * MTFI)
{
if ( MTFI )
	{
	SmartFree(MTFI->NodeBlock,sizeof(struct MTFNode)*(MTFI->NumSymbols));
	FreeMem(MTFI,sizeof(struct MTFInfo));	
	}
}

int MTF_Translate(struct MTFInfo * MTFI,int val) /* -1 error */
{
int pos;
struct MTFNode * N;
pos = 0;
N = MTFI->Head;
for(;;)
	{
	if ( N->Char == val )
		break;
	pos++;
	N = N->Next;
	if ( N == NULL )
		return(-1);
	}

if ( N->Prev )
	{
	N->Prev->Next = N->Next;
	if ( N->Next ) N->Next->Prev = N->Prev;
	N->Next = MTFI->Head;
	MTFI->Head = N;
	N->Next->Prev = N;
	N->Prev = NULL;
	}

return(pos);
}

int MTF_Untranslate(struct MTFInfo * MTFI,int pos)
{
int val;
struct MTFNode * N;
N = MTFI->Head;
while(pos>0)
	{
	pos--;
	N = N->Next;
	if ( N == NULL )
	return(-1);
	}
val = N->Char;

if ( N->Prev )
	{
	N->Prev->Next = N->Next;
	if ( N->Next ) N->Next->Prev = N->Prev;
	N->Next = MTFI->Head;
	MTFI->Head = N;
	N->Next->Prev = N;
	N->Prev = NULL;
	}

return(val);
}

bool MTF_TransformArray(struct MTFInfo * MTFI,ubyte * Array,long ArrayLen)
{
int val;
while(ArrayLen--)
	{
	val = MTF_Translate(MTFI,*Array);
	if ( val == -1 ) return(0);
	*Array++ = val;
	}
return(1);
}

bool MTF_UntransformArray(struct MTFInfo * MTFI,ubyte * Array,long ArrayLen)
{
int val;
while(ArrayLen--)
	{
	val = MTF_Untranslate(MTFI,*Array);
	if ( val == -1 ) return(0);
	*Array++ = val;
	}
return(1);
}
