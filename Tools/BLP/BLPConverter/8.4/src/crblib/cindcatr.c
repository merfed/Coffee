#include <crblib/inc.h>
#include <stdio.h>

/*
 * you have to drop a \n on the string yourself
 *
 */
void TheCompressionIndicator(ulong UnPackedLen,ulong PackedLen,FILE *ToFile)
{
ulong BPB,BPBdec;
ulong Ratio,Ratiodec;
ulong PackedLenT,UnPackedLenT;

PackedLenT = PackedLen;
UnPackedLenT = UnPackedLen;

while ( PackedLenT >= 536870 || UnPackedLenT >= 4294966 )
  {
  PackedLenT >>= 1;
  UnPackedLenT >>= 1;
  }
if ( !PackedLenT) PackedLenT=1;
if ( !UnPackedLenT) UnPackedLenT=1;

BPB = PackedLenT * 8000 / UnPackedLenT; 
BPBdec = BPB - (BPB/1000)*1000;
BPB /= 1000;

Ratio = UnPackedLenT * 1000 / PackedLenT; 
Ratiodec = Ratio - (Ratio/1000)*1000;
Ratio /= 1000;

fprintf(ToFile," %7lu -> %7lu = %2u.%03u bpb = %2u.%03u to 1 ",
  UnPackedLen,PackedLen,BPB,BPBdec,Ratio,Ratiodec);

}

void TheDecompressionIndicator(ulong UnPackedLen,ulong PackedLen,FILE *ToFile)
{
ulong BPB,BPBdec;
ulong Ratio,Ratiodec;
ulong PackedLenT,UnPackedLenT;

PackedLenT = PackedLen;
UnPackedLenT = UnPackedLen;

while ( PackedLenT >= 536870 || UnPackedLenT >= 4294966 )
  {
  PackedLenT >>= 1;
  UnPackedLenT >>= 1;
  }
if ( !PackedLenT) PackedLenT=1;
if ( !UnPackedLenT) UnPackedLenT=1;

BPB = PackedLenT * 8000 / UnPackedLenT; 
BPBdec = BPB - (BPB/1000)*1000;
BPB /= 1000;

Ratio = UnPackedLenT * 1000 / PackedLenT; 
Ratiodec = Ratio - (Ratio/1000)*1000;
Ratio /= 1000;

fprintf(ToFile," %7lu -> %7lu = %2u.%03u bpb = %2u.%03u to 1 ",
  PackedLen,UnPackedLen,BPB,BPBdec,Ratio,Ratiodec);

}

void TheCompressionIndicatorMin(ulong UnPackedLen,ulong PackedLen,FILE *ToFile)
{
ulong BPB,BPBdec;
ulong PackedLenT,UnPackedLenT;

PackedLenT = PackedLen;
UnPackedLenT = UnPackedLen;

while ( PackedLenT >= 536870 || UnPackedLenT >= 4294966 )
  {
  PackedLenT >>= 1;
  UnPackedLenT >>= 1;
  }
if ( !PackedLenT) PackedLenT=1;
if ( !UnPackedLenT) UnPackedLenT=1;

BPB = PackedLenT * 8000 / UnPackedLenT; 
BPBdec = BPB - (BPB/1000)*1000;
BPB /= 1000;

fprintf(ToFile," %2u.%03u bpb ",BPB,BPBdec);

}
