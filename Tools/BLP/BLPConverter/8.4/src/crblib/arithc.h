#ifndef CRB_ARITHC_H
#define CRB_ARITHC_H

#include <crblib/inc.h>

/*
 * Notez:
 *
 *  "totrange" must be <= ari->FastArithCumProbMax at all times !!
 *
 */

typedef struct
{
	int probMax,probMaxSafe;

	// PRIVATE !! :
	ubyte *outBuf,*outPtr;

	ulong code,range;
	ulong overflow_bytes,queue;	// encoder only
} arithInfo;

#define ArithBitScale_Shift		(13UL)
#define ArithBit_Scale			(1UL<<ArithBitScale_Shift)

#define safeProbMax probMaxSafe

extern arithInfo * arithInit(void);
extern void arithFree(arithInfo * ari);

extern void arithEncodeInitNoStuff(arithInfo * ari,ubyte *outBuf); // danger! writes to buf[-1] !!
extern void arithEncodeInit(arithInfo * ari,ubyte *outBuf);
void arithEncode(arithInfo * ari,ulong symlow,ulong symhigh,ulong symtot);
extern int arithEncodeDoneMinimal(arithInfo * ari);
extern int arithEncodeDoneSafe(arithInfo * ari);

extern void arithDecodeInitNoStuff(arithInfo * ari,ubyte *outBuf);
extern void arithDecodeInit(arithInfo * ari,ubyte *outBuf);
extern ulong arithGet(arithInfo * ari,ulong tot);
extern void arithDecode(arithInfo * ari,ulong low,ulong high,ulong totrange);
extern void arithDecodeDone(arithInfo * ari);

extern void arithEncodeReInit(arithInfo * ari,ubyte *outBuf);	// for resuming after a flush
extern void arithDecodeReInit(arithInfo * ari,ubyte *outBuf);	// for resuming after a flush

extern void arithEncBit(arithInfo * ari,ulong p0,ulong pt,bool bit);
extern bool arithDecBit(arithInfo * ari,ulong p0,ulong pt);

extern void arithModelEncBit(arithInfo * ari,ulong *p0,ulong *pt,bool bit);
extern bool arithModelDecBit(arithInfo * ari,ulong *p0,ulong *pt);

// no division version:
extern void arithEncBitScaled(arithInfo * ari,ulong p0scaled,bool bit);
extern bool arithDecBitScaled(arithInfo * ari,ulong p0scaled);

extern void arithEncBitRaw(arithInfo * ari,bool bit);
extern bool arithDecBitRaw(arithInfo * ari);
				// the returned value is gaurenteed to be 1 or 0

extern void arithEncBits(arithInfo * ari,uint val,uint bits);
extern uint arithDecBits(arithInfo * ari,uint bits);

extern int arithTellEncPos(arithInfo * ari);
extern int arithTellDecPos(arithInfo * ari);

extern int arithEncodeFlush(arithInfo * ari);
extern void arithDecodeRestart(arithInfo * ari,ulong pos);

#define arithEncodeDone(a)	arithEncodeDoneMinimal(a)

#endif // arithc
