#include <stdlib.h>
#include <string.h>
#include <crblib/inc.h>
#include <time.h>

// #define RADIX /* has bugs */

typedef struct SortNodeStruct SortNode;

struct SortNodeStruct
  {
  SortNode * Next;
  ulong Index;
  ubyte * MoreIndexPtr;
  void * Data;
  };

bool Sort_List(SortNode ** BasePtr);
bool Sort_Array(SortNode ** Array,int Num);

/* **/

void Sort_FreeStringNode(SortNode * N)
{
if ( !N ) return;
free( N->MoreIndexPtr - 4 );
free(N);
}

void Sort_FreeStringList(SortNode * N)
{
SortNode * next;
	while(N) {
		next = N->Next;
		Sort_FreeStringNode(N);
		N = next;
	}
}

static ulong Sort_MakeString_Counter = 0;

SortNode * Sort_MakeStringNode(const char * String)
{
ubyte *StrData;
SortNode * N;
size_t StringLen;

StringLen = strlen(String);

if ( (N = (SortNode *) malloc(sizeof(SortNode))) == NULL ) return(NULL);

if ( (StrData = malloc(StringLen + 1 + sizeof(ulong))) == NULL )
	{ free(N); return(NULL); }

memcpy(StrData,String,StringLen+1);
memcpy(StrData + StringLen + 1,(ubyte *)&Sort_MakeString_Counter,
	sizeof(ulong));
Sort_MakeString_Counter++;

N->Data = NULL;
N->Index = getulong(StrData);
N->MoreIndexPtr = StrData + 4;
N->Next = NULL;

return(N);
}

bool Sort_List(SortNode ** BasePtr)
{
SortNode ** Array;
int ArraySize,i;
SortNode * CurNode;

CurNode = *BasePtr;
if ( !CurNode || !CurNode->Next ) return(1);
ArraySize = 0;
while(CurNode) { ArraySize++; CurNode = CurNode->Next; }

if ( (Array = malloc(sizeofpointer*ArraySize)) == NULL )
  return(0);

i = 0;
CurNode = *BasePtr;
while(CurNode) { Array[i++] = CurNode; CurNode = CurNode->Next; }

if ( ! Sort_Array(Array,ArraySize) )
  { free(Array); return(0); }

i = 0;
CurNode = Array[0];
*BasePtr = CurNode;
i++;
while(i<ArraySize)
  {
  CurNode->Next = Array[i++];
  CurNode = CurNode->Next;
  }
CurNode->Next = NULL;

free(Array);
return(1);
}

static bool QS_InUse = 0; /* semaphore */
static SortNode ** QSArray; /* locked by semaphore */
static void QS(long L,long R);

bool Sort_Array_Past(SortNode ** Array,int ArraySize,int R_PastBytes)
{
if ( ArraySize <= 1 ) return(1);
#ifdef RADIX
if ( ArraySize <= 64 )
	{
#endif
	if ( QS_InUse ) return(0);

	QS_InUse = 1;
	QSArray = Array;
	QS(0,ArraySize-1);
	QS_InUse = 0;

	return(1);
#ifdef RADIX
	}
else
	{
	int R_NumBuckets,R_UseBytes;
	SortNode ** R_Buckets=NULL;
	SortNode ** tempArray=NULL;
	int R_Index,arrayI,tempLen,tempI;
	SortNode * curSN;

	if ( ArraySize > 8192 && R_PastBytes != 3 ) /* radix on word */
		{
		R_NumBuckets = 0x10000;
		R_UseBytes = 2;
		}
	else /* radix on byte */
		{
		R_NumBuckets = 0x100;
		R_UseBytes = 1;
		}

	if ( (R_Buckets = malloc(R_NumBuckets*sizeofpointer)) == NULL )
		goto R_Sort_Array_BadReturn;
	if ( (tempArray = malloc(ArraySize*sizeofpointer)) == NULL )
		goto R_Sort_Array_BadReturn;
	MemClearFast(R_Buckets,R_NumBuckets);

	if ( R_PastBytes >= 4 ) /* need to use MoreIndex */
		{
		int R_Offset = R_PastBytes-4;
		if(R_UseBytes==1)
			{
			for(arrayI=0;arrayI<ArraySize;arrayI++)
				{
				R_Index = Array[arrayI]->MoreIndexPtr[R_Offset];
				Array[arrayI]->Next = R_Buckets[R_Index];
				R_Buckets[R_Index] = Array[arrayI];
				}
			}
		else //(R_UseBytes==2)
			{
			for(arrayI=0;arrayI<ArraySize;arrayI++)
				{
				R_Index = (	Array[arrayI]->MoreIndexPtr[R_Offset] << 8) +
										Array[arrayI]->MoreIndexPtr[R_Offset+1];
				Array[arrayI]->Next = R_Buckets[R_Index];
				R_Buckets[R_Index] = Array[arrayI];
				}
			}
		}
	else /* do it with shifts */
		{
		int R_Mask = R_NumBuckets - 1;
		int R_ShiftBack = 32 - ((R_UseBytes+R_PastBytes)<<3);
		for(arrayI=0;arrayI<ArraySize;arrayI++)
			{
			R_Index = ((Array[arrayI]->Index) >> R_ShiftBack) & R_Mask;
			Array[arrayI]->Next = R_Buckets[R_Index];
			R_Buckets[R_Index] = Array[arrayI];
			}
		}

	arrayI=0;
	for(R_Index=0;R_Index<R_NumBuckets;R_Index++)
		{
		curSN = R_Buckets[R_Index]; tempI=0;
		while(curSN)
			{
			tempArray[tempI++] = curSN;
			curSN = curSN->Next;
			}
		tempLen = tempI;
		if ( tempLen>=1 )
			{
			if(!Sort_Array_Past(tempArray,tempLen,R_PastBytes+R_UseBytes))
				goto R_Sort_Array_BadReturn;
			for(tempI=0;tempI<tempLen;tempI++)
				Array[arrayI++] = tempArray[tempI++];
			}
		}

	free(tempArray);
	free(R_Buckets);

	if ( arrayI != ArraySize )
		goto R_Sort_Array_BadReturn;
	
	return 1;

	R_Sort_Array_BadReturn:

	if(R_Buckets) free(R_Buckets);
	if(tempArray) free(tempArray);

	return 0;
	}
#endif
return 0;
}


int NodeCompare(const void * v1,const void *v2)
{
SortNode *N1 = *((SortNode **)v1);
SortNode *N2 = *((SortNode **)v2);
if ( N1->Index == N2->Index )
	return strcmp(N1->MoreIndexPtr,N2->MoreIndexPtr);
return (N1->Index - N2->Index);
}

bool Sort_Array_clib(SortNode ** Array,int ArraySize)
{
qsort( Array,ArraySize,sizeofpointer,NodeCompare );
return(1);
}

bool Sort_Array(SortNode ** Array,int ArraySize)
{
//return( Sort_Array_Past(Array,ArraySize,0) );
return( Sort_Array_clib(Array,ArraySize) );
}

static void QS(long Left,long Right)
{
if ( (Right - Left) > 1 )
  {
  long i;

    {
    long j;
    ulong pivot;
    SortNode *swapper;
    SortNode ** Array;
    bool KeepGoing;
    ubyte * morepivotptr;

    Array = QSArray;

    pivot = Array[Right]->Index;
    morepivotptr = Array[Right]->MoreIndexPtr;

    i = Left-1;
    j = Right;
    for(;;)
      {
      do
        {
        ++i;
        if ( i == Right ) KeepGoing = 0;
        else
          {
          if ( Array[i]->Index < pivot ) KeepGoing = 1;
          else if ( Array[i]->Index == pivot )
            {
            ubyte * MoreIndexPtr = Array[i]->MoreIndexPtr;
            ubyte * curmorepivotptr = morepivotptr;
  
            while(*MoreIndexPtr == *curmorepivotptr)
              { MoreIndexPtr++; curmorepivotptr++; }
  
            if ( *MoreIndexPtr < *curmorepivotptr )
              KeepGoing = 1;
            else
              KeepGoing = 0;
            }
          else KeepGoing = 0;
          }
        } while(KeepGoing);

      do
        {
        --j;
        if ( Array[j]->Index > pivot ) KeepGoing = 1;
        else if ( Array[j]->Index == pivot )
          {
          ubyte * MoreIndexPtr = Array[j]->MoreIndexPtr;
          ubyte * curmorepivotptr = morepivotptr;

          while(*MoreIndexPtr == *curmorepivotptr)
            { MoreIndexPtr++; curmorepivotptr++; }

          if ( *MoreIndexPtr > *curmorepivotptr )
            KeepGoing = 1;
          else
            KeepGoing = 0;
          }
        else KeepGoing = 0;
        } while(KeepGoing && j > Left);

      if (i >= j) break;
      swapper  = Array[i];
      Array[i] = Array[j];
      Array[j] = swapper;
      }
    swapper  = Array[i];
    Array[i] = Array[Right];
    Array[Right] = swapper;
    }

  QS(Left,i-1);
  QS(i+1,Right);
  }
else
  {
  if ( Right > Left )
    {
    if ( QSArray[Right]->Index < QSArray[Left]->Index )
      {
      SortNode *swapper;

      swapper  = QSArray[Left];
      QSArray[Left] = QSArray[Right];
      QSArray[Right] = swapper;
      }
    else if ( QSArray[Right]->Index == QSArray[Left]->Index )
      {
      ubyte *rptr=QSArray[Right]->MoreIndexPtr;
      ubyte *lptr=QSArray[Left]->MoreIndexPtr;

      while(*rptr == *lptr) { rptr++; lptr++; }
      if ( *rptr < *lptr )
        {
        SortNode *swapper;

        swapper  = QSArray[Left];
        QSArray[Left] = QSArray[Right];
        QSArray[Right] = swapper;
        }
      }
    }
  }
}
