#ifndef FILE_BIT_IO_H
#define FILE_BIT_IO_H

#include <crblib/inc.h>
#include <stdio.h>

struct FileBitIOInfo
  {
	FILE * FP;
	int OutLen;
  int BitBuffer;
  int BitsToGo;
  };

/*externs in FileBitIO.c :*/

extern struct FileBitIOInfo * FileBitIO_Init(FILE * FP);
extern void FileBitIO_CleanUp(struct FileBitIOInfo * BII);
extern long FileBitIO_FlushWrite(struct FileBitIOInfo * BII); /*returns length of data*/

/*macros:*/

/*

BII = (struct FileBitIOInfo * BII)

void FileBitIO_WriteBit(BII,ubyte Bit);
void FileBitIO_WriteZeroBit(BII);
void FileBitIO_InitRead(BII);
void FileBitIO_ReadBit(BII,bool Bit); / /fills out Bit

*/


/*
 *  Write a Bit to the BII - Bit may be True or False (need not be 0 or 1)
 *  params: struct FileBitIOInfo * BII,ubyte Bit
 *
 */
#define FileBitIO_WriteBit(BII,Bit)               \
{                                                 \
BII->BitBuffer += BII->BitBuffer + Bit;           \
BII->BitsToGo--;                                  \
if ( ! BII->BitsToGo )                            \
  {                                               \
  fputc(BII->BitBuffer,BII->FP); BII->OutLen++;   \
  BII->BitsToGo = 8;                              \
  BII->BitBuffer = 0;                             \
  }                                               \
}                                                 \
/* End FileBitIO_WriteBit */

#define FileBitIO_WriteZeroBit(BII)               \
{                                                 \
BII->BitBuffer += BII->BitBuffer;                 \
BII->BitsToGo--;                                  \
if ( ! BII->BitsToGo )                            \
  {                                               \
  fputc(BII->BitBuffer,BII->FP); BII->OutLen++;   \
  BII->BitsToGo = 8;                              \
  BII->BitBuffer = 0;                             \
  }                                               \
}                                                 \
/* End FileBitIO_WriteZeroBit */

/*
 *  Reads bits into BitBuffer from BitArray
 *  must be done before any _Read commands
 *
 */
#define FileBitIO_InitRead(BII) BII->BitBuffer = fgetc(BII->FP);
/* End FileBitIO_InitRead */

extern const int BFileBitIOReadBitMask;

/*
 *  Read a bit from BII into (bool Bit)
 *
 */
#define FileBitIO_ReadBit(BII,Bit)                \
{                                                 \
if ( BII->BitBuffer & BFileBitIOReadBitMask )     \
  Bit = 1;                                        \
else                                              \
  Bit = 0;                                        \
BII->BitBuffer += BII->BitBuffer;                 \
BII->BitsToGo --;                                 \
if ( BII->BitsToGo == 0 )                         \
  {                                               \
	BII->BitBuffer = fgetc(BII->FP);								\
  BII->BitsToGo = 8;                              \
  }                                               \
}                                                 \
/* End FileBitIO_ReadBit */

#endif /* BIT_IO_H*/
