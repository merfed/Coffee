#include <crblib/inc.h>
#include <stdio.h>

/*protos:*/

struct FileBitIOInfo * FileBitIO_Init(FILE * OutF);
void FileBitIO_CleanUp(struct FileBitIOInfo * BII);
long FileBitIO_FlushWrite(struct FileBitIOInfo * BII); /*returns length of data*/

const int BFileBitIOReadBitMask = 0x80;

struct FileBitIOInfo
  {
	FILE * FP;
	int OutLen;
  int BitBuffer;
  int BitsToGo;
  };

/*
 *  Allocate and init a BII
 *  for reads, FileBitIO_InitRead must also be called
 *
 */
struct FileBitIOInfo * FileBitIO_Init(FILE * inFP)
{
struct FileBitIOInfo * BII;

if ( (BII = malloc(sizeof(struct FileBitIOInfo))) == NULL )
  return(NULL);

BII->FP = inFP;
BII->OutLen = 0;
BII->BitBuffer = 0;
BII->BitsToGo = 8;

return(BII);
}

/*
 *  Free a BII after it has been written or read from
 *  call FileBitIO_FlushWrite before writing to a file
 *
 */
void FileBitIO_CleanUp(struct FileBitIOInfo * BII)
{
free(BII);
}

/*
 *  FlushWrite sticks remaining bits into BitArray
 *  must be called before writing BitArray
 *  returns length of array to write
 *
 */
long FileBitIO_FlushWrite(struct FileBitIOInfo * BII)
{
if ( BII->BitsToGo < 8 )
  {
  BII->BitBuffer <<= BII->BitsToGo;
	fputc(BII->BitBuffer,BII->FP);
	BII->OutLen++;

  BII->BitsToGo = 8;
  BII->BitBuffer = 0;
  /* keep going, if you like */
  }

fflush(BII->FP);

return( BII->OutLen );
}
