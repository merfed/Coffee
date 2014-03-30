#ifndef CRB_FASTARITH_H
#define CRB_FASTARITH_H

#include <crblib/inc.h>
#include <crblib/bbitio.h>

/*
 * Notez:
 *
 *  "totrange" must be <= FAI->FastArithCumProbMax at all times !!
 *
 */

/*
 *
 * you must call BitIO_InitRead & BitIO_FlushWrite yourself!
 *
 */

struct FAI {
	int probMax;
	int probMaxSafe; /* 256 less than FastArithCumProbMax */ 

	int Private;
};

extern void FastArithBit(struct FAI * FAI,bool bit);
extern bool FastArithGetBit(struct FAI * FAI);

extern void FastArithEncBit(struct FAI *ai,int mid,int tot,bool bit);
extern bool FastArithDecBit(struct FAI *ai,int mid,int tot);

extern struct FAI * FastArithInit(struct BitIOInfo * BII);
extern void FastArithCleanUp(struct FAI * FAI);

extern void FastArithEncodeCInit(struct FAI * FAI);
extern void FastArithEncodeRange(struct FAI * FAI,long low,long high,long totrange);
extern void FastArithEncodeCDone(struct FAI * FAI);

extern void FastArithDecodeCInit(struct FAI * FAI);
extern ulong FastArithDecodeTarget(struct FAI * FAI,ulong Tot);
extern void FastArithDecodeRange(struct FAI * FAI,long *target,long totrange);
extern void FastArithDecodeRangeRemove(struct FAI * FAI,long low,long high,long totrange);
extern void FastArithDecodeCDone(struct FAI * FAI);

extern void FastArithDecodeCDone_Safe(struct FAI * FAI);
extern void FastArithDecodeCDone_Min(struct FAI * FAI);

  /** _Safe is used right now
   *  _Min assumes the decoder will be supplied with trailing zeros
   *    this is not so easy to actually do without a speed-hit to BitIO
   *    but does give us an optimal limit of arithcoder efficiency
   **/

#endif // arithc
