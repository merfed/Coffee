
/*
 * MACM : Multi-Precision Arithmetic Coder Module
 *
 * A Virtual Queue Based General Arithmetic Encoder
 *
 * by Mahesh Naik
 *
 *    kiran@giasbm01.vsnl.net.in
 *    BOMAAB04@giasbm01.vsnl.net.in
 *
 *
 * commented by Charles Bloom ( cbloom@mail.utexas.edu )
 * 
 * 6-14-96
 *
 * posted to comp.compression.research
 *
 * posted to http:/ /wwwvms.utexas.edu/~cbloom/src/macm.zip
 *
 *  additional notes by Mahesh Naik posted to
 *    http:/ /wwwvms.utexas.edu/~cbloom/news/macm.html
 *
 * see also : the original Virtual Queue Skew Coder :
 *
 * http:/ /wwwvms.utexas.edu/~cbloom/src/virtskew.zip
 * http:/ /wwwvms.utexas.edu/~cbloom/papers/virtq_ps.zip
 *
 */

#include <crblib/inc.h>
#include <crblib/bbitio.h>

/* #define LOG */

#ifdef _MSC_VER
#pragma warning(disable : 4244) // ulong -> uword
#endif

struct FAI {
  long FastArithCumProbMax;
  long FastArithCumProbMaxSafe;

  struct BitIOInfo * BII;

  long Qlength;
  uword code;
  ulong range;
};

/*consts*/
static const ulong CumProbMax    = 0x08000;
static const ulong Half          = 0x08000;
static const ulong One           = 0x10000;

/*functions to be called from main:*/
struct FAI * FastArithInit(struct BitIOInfo * BII);
void FastArithCleanUp(struct FAI * FAI);

void FastArithEncodeCInit(struct FAI * FAI);
void FastArithEncodeRange(struct FAI * FAI,long symlow,long symhigh,long symtot);
void FastArithEncodeCDone(struct FAI * FAI);

void FastArithDecodeCInit(struct FAI * FAI);
void FastArithDecodeRange(struct FAI * FAI,long *target,long symtot);
void FastArithDecodeRangeRemove(struct FAI * FAI,long symlow,long symhigh,long symtot);
void FastArithDecodeCDone(struct FAI * FAI);

void FastArithEncodeCDone_Min(struct FAI * FAI);
void FastArithEncodeCDone_Safe(struct FAI * FAI);

  /** _Safe is used right now
   *  _Min assumes the decoder will be supplied with trailing zeros
   *    this is not so easy to actually do without a speed-hit,
   *    but does give us an optimal limit of arithcoder efficiency
   **/

/*functions:*/

struct FAI * FastArithInit(struct BitIOInfo * BII)
{
struct FAI * FAI;

if ( (FAI = malloc(sizeof(struct FAI))) == NULL )
  return(NULL);

FAI->FastArithCumProbMax = CumProbMax;
FAI->FastArithCumProbMaxSafe = CumProbMax - 512;
FAI->BII = BII;

return(FAI);
}

void FastArithCleanUp(struct FAI * FAI)
{
if (FAI) free(FAI);
}


void FastArithEncodeCInit(struct FAI * FAI)
{

FAI->code  = 0;
FAI->range = One;
FAI->Qlength = 0;

}


/*
 * FastArith Encoding using the generalized BitQueue
 *
 * encoding is done in the "trivial" manner - i.e. simple
 *  arithmetic is done.  No handling of underflow or overflow
 *  is needed, because we are just doing arithmetic.
 * bits are output as they become invariant (i.e. when
 *  MSBit(reglow) == MSBit(reglow+regrange) )
 * however, we also write a 0 bit out when they still have
 *  a chance of becoming a 1 later (aka underflow problem)
 * in order to handle this we keep a queue of bits between
 *  the arithcoder and the output, i.e. :
 *
 *  arithcoder -> queue -> output
 *
 * thus, if the arithcoder needs to add to a bit it has already
 *  sent, this may be done in the queue
 * the queue always has the form:
 *  01111... , i.e. a single zero bit followed by N one bits
 * Qlength counts the number of 1 bits plus the zero bit
 *  ( Qlength == 0 is an empty queue , Qlength == 1 is just a zero bit )
 * 
 * a 'carry' is handled by adding 1 to the Queue
 *
 * this changes 01111 to 10000 , at which time the queue may be sent 
 *
 * It is a property of arithmetic coding that if a carry reaches
 *  a certain position, no later carry may again reach that position.
 * It is this property which allows the Queue to be kept as such a
 *  simple form.
 *
 *   (btw this property comes very simply from the fact that the
 *    'range' - the number added to the coded - is constantly being
 *    shrunk (in our minds, anyway).  For the same bit to be reached
 *    again, we must add a range of the same value, but since the
 *    range is smaller, this cannot happen)
 *
 * note that the decoder is trivially simple, because the bits sent
 *  by the queue are identical to those that would have been sent
 *  had infinite precision coding been used.
 *
 * note also that it is possible to have a queue of the form
 *  1111 , (i.e. no leading zero) but we need not keep this data
 *  in the queue, because no carry-over will ever touch it.  Thus,
 *  any 1 bits sent to an empty queue may be flushed immediately.
 * (an empty queue is only created by a carry-over, so any 1 bits
 *  added to that queue cannot receive a carry because then that
 *  carry would be propagated through to a point which had
 *  already received a carry).
 * thus we need not track "QueueZeroFlag" as was originally 
 *  suggested in my VirtualQueue-SkewCoder paper
 *
 */

void FastArithEncodeRange(struct FAI * FAI,long symlow,long symhigh,long symtot)
{
ulong lowincr;
register ulong regcode,regrange,regQlength;
register ulong reghalf = Half;

#ifdef LOG
printf("%d,%d,%d\n",symlow,symhigh,symtot);
#endif

regcode    = FAI->code;
regrange   = FAI->range;
regQlength = FAI->Qlength;

lowincr = ( regrange * symlow ) / symtot;
regcode += lowincr;
regrange = (( regrange * symhigh ) / symtot ) - lowincr;

    /* regrange *= (symhigh - symlow) / symtot ; */

if ( regcode & One )
  {
  /* we've added more to regcode than expected; now we must
    propagate a 'carry' into the already-queued bits */
  /* Qlength never == 0 at this point */

  regcode -= One; /* turn off the One */

  /* queue was 01111... , add one (carry) makes it 10000...

  BitIO_WriteBit(FAI->BII,1); regQlength--;
  while ( regQlength-- ) { BitIO_WriteZeroBit(FAI->BII); }

  however, a tad bit faster is:

  **/

  BitIO_WriteBit(FAI->BII,1);
  while ( --regQlength ) { BitIO_WriteZeroBit(FAI->BII); }
  }

while( regrange <= Half )
  {
  if ( regcode >= reghalf )
    {
    /* BitIO_WriteBit(FAI->BII,1); */
    /* instead of writing a 1 bit directly, put it in the queue */
    /* queue was 01..1 , make it 01..11 */

    if ( regQlength == 0 ) /* no leading zero */
      {
      BitIO_WriteBit(FAI->BII,1);
      }
    else
      {
      regQlength ++; /* tack a 1 on the queue */
      }

    regcode -= reghalf;
    }
  else
    {
    /* BitIO_WriteZeroBit(FAI->BII); */
    /* instead of writing a 0 bit directly, put it in the queue */
    /* queue was 01111 , make it 011110 , flush 01111 */

    /* send old queue */

    if ( regQlength )
      {
      /* send one (0-bit) and (Qlength - 1) (1-bit)s */

      BitIO_WriteZeroBit(FAI->BII);
      while ( --regQlength ) { BitIO_WriteBit(FAI->BII,1); }
      }

    /* now put a zero bit in the queue */

    regQlength = 1;
    }

  regcode  += regcode;
  regrange += regrange;
  }

FAI->code  = regcode;
FAI->range = regrange;
FAI->Qlength = regQlength;
}

void FastArithEncodeCDone_Min(struct FAI * FAI)
{
ulong low,code,nextcode,mask,Qlength;

/** flush the state with a minimum of output **/

low = FAI->code;
code = low + FAI->range - 1;
mask = 0xFFFFFFFF;
Qlength = FAI->Qlength;

if ( code & One )
  {
  /* queue was 01111... , add one (carry) makes it 10000... */
  BitIO_WriteBit(FAI->BII,1);

  return;
  }

/* find a code in [low,low+range) with the maximum number of tailing 0s */

/* first send the queue */

if ( Qlength )
  {
  BitIO_WriteZeroBit(FAI->BII); Qlength--;
  while ( Qlength-- ) { BitIO_WriteBit(FAI->BII,1); }
  }

if ( ! low ) return;

nextcode = code;
do
	{
	code = nextcode;
	mask <<= 1;
	nextcode = code & mask;
	} while ( nextcode >= low );

/** identical to encoder loop, but without trailing zeros **/

while(code)
  {
  if ( code & Half )
    { code -= Half; BitIO_WriteBit(FAI->BII,1); }
  else BitIO_WriteZeroBit(FAI->BII);

  code <<= 1;
  }

}

void FastArithEncodeCDone_Safe(struct FAI * FAI)
{
ulong low,mask;

/* first send the queue */
if ( FAI->Qlength )
  {
  ulong Qlength = FAI->Qlength;

  BitIO_WriteZeroBit(FAI->BII); Qlength--;
  while ( Qlength-- ) { BitIO_WriteBit(FAI->BII,1); }
  }

low = FAI->code;

for ( mask = Half; mask; mask>>=1 )
  {
  if ( low & mask ) { BitIO_WriteBit(FAI->BII,1); }
  else { BitIO_WriteZeroBit(FAI->BII); }
  }

}

void FastArithEncodeCDone(struct FAI * FAI)
{
FastArithEncodeCDone_Safe(FAI);
}

void FastArithDecodeCInit(struct FAI * FAI)
{
long i;
bool bit;

FAI->range = One;

FAI->code = 0;
for ( i=0; i<16; i++ )
  {
  FAI->code <<= 1;
  BitIO_ReadBit(FAI->BII,bit);
  if ( bit ) FAI->code++;
  }

}


ulong FastArithDecodeTarget(struct FAI * FAI,ulong symtot)
{
return( (( FAI->code + 1 ) * symtot - 1 ) / FAI->range );
}

void FastArithDecodeRange (struct FAI * FAI,long *target,long symtot)
{
*target = (long) FastArithDecodeTarget(FAI,symtot);
}

/*
 * Just figuring out what the present symbol is doesn't remove
 * it from the input bit stream.  After the character has been
 * decoded, this routine has to be called to remove it from the
 * input stream.
 */

/*
 * The Bit Queue method has an extremely simple decoder, because
 *  the output stream contains all bits of the code exactly as they
 *  would be computed by straightforward arithmetic
 *
 */

void FastArithDecodeRangeRemove(struct FAI * FAI,long symlow,long symhigh,long symtot)
{
register ulong regrange,regcode;
ulong lowincr;
bool bit;

#ifdef LOG
printf("%d,%d,%d\n",symlow,symhigh,symtot);
#endif

regrange = FAI->range;
regcode = FAI->code;

lowincr = ( regrange * symlow ) / symtot;
regcode -= lowincr;
regrange = (( regrange * symhigh ) / symtot) - lowincr;

while ( regrange <= Half )
  {
  regrange += regrange;
  BitIO_ReadBit(FAI->BII,bit);
  regcode += regcode + bit;
  }

FAI->range = regrange;
FAI->code  = regcode;
return;
}

void FastArithDecodeCDone(struct FAI * FAI)
{
}

/** routines to insert raw bits in an arith bit stream;
	all divisions eliminated **/

void FastArithBit(struct FAI * FAI,bool bit)
{
register ulong regcode,regrange,regQlength;

regcode    = FAI->code;
regrange   = FAI->range;
regQlength = FAI->Qlength;

if ( bit ) {
	ulong lowincr;
	lowincr = regrange>>1;
	regcode  += lowincr;
	regrange -= lowincr;
} else {
	regrange = regrange>>1;
}

/** the standard queue-flush: **/

if ( regcode & One ) {
	regcode -= One;
	BitIO_WriteBit(FAI->BII,1);
	while ( --regQlength ) { BitIO_WriteZeroBit(FAI->BII); }
}

while( regrange <= Half ) {
	if ( regcode >= Half ) {
		if ( regQlength == 0 )
	  		BitIO_WriteBit(FAI->BII,1);
		else
			regQlength ++;

		regcode -= Half;
	} else {
		if ( regQlength ) {
	  		BitIO_WriteZeroBit(FAI->BII);
	  		while ( --regQlength ) { BitIO_WriteBit(FAI->BII,1); }
	  	}
		regQlength = 1;
	}
	regcode  += regcode;
	regrange += regrange;
}

FAI->code  = regcode;
FAI->range = regrange;
FAI->Qlength = regQlength;
}

bool FastArithGetBit(struct FAI * FAI)
{
register ulong regrange,regcode;
bool bit,ret;

regrange = FAI->range;
regcode = FAI->code;

if ( (regcode+regcode + 1) < regrange ) ret = 0;	// <> instead of division
else ret = 1;

if ( ret ) {
	ulong lowincr;
	lowincr = regrange>>1;
	regcode -= lowincr;
	regrange -= lowincr;
} else {
	regrange >>= 1;
}

while ( regrange <= Half ) {
  regrange += regrange;
  BitIO_ReadBit(FAI->BII,bit);
  regcode += regcode + bit;
  }

FAI->range = regrange;
FAI->code  = regcode;

return ret;
}

void FastArithEncBit(struct FAI *ai,int mid,int tot,bool bit)
{
ulong midrange;
register ulong regcode,regrange,regQlength;

regcode    = ai->code;
regrange   = ai->range;
regQlength = ai->Qlength;

	midrange = ( regrange * mid ) / tot;

	if ( bit ) {
		regcode += midrange;
		regrange -= midrange;
	} else {
		regrange = midrange;
	}

/** the standard queue-flush: **/

	if ( regcode & One ) {
		regcode -= One;
		BitIO_WriteBit(ai->BII,1);
		while ( --regQlength ) { BitIO_WriteZeroBit(ai->BII); }
	}

	while( regrange <= Half ) {
		if ( regcode >= Half ) {
			if ( regQlength == 0 )
		  		BitIO_WriteBit(ai->BII,1);
			else
				regQlength ++;

			regcode -= Half;
		} else {
			if ( regQlength ) {
		  		BitIO_WriteZeroBit(ai->BII);
		  		while ( --regQlength ) { BitIO_WriteBit(ai->BII,1); }
		  	}
			regQlength = 1;
		}
		regcode  += regcode;
		regrange += regrange;
	}

ai->code  = regcode;
ai->range = regrange;
ai->Qlength = regQlength;
}

bool FastArithDecBit(struct FAI *ai,int mid,int tot)
{
register ulong regrange,regcode;
ulong midrange;
bool bit,gotbit;

regrange = ai->range;
regcode = ai->code;

	/** with a binary arithcoder our big win is right here :
	**	we can do an 'if' instead of a division.
	**		"bit = a / b" is replaced by
	**		if ( a < b ) bit = 0; else bit = 1;
	***/

	midrange = ( regrange * mid ) / tot;

	if ( regcode >= midrange ) {
		gotbit = 1;
		regcode -= midrange;
		regrange -= midrange;
	} else {
		gotbit = 0;
		regrange = midrange;
	}
	
	while ( regrange <= Half ) {
		regrange += regrange;
		BitIO_ReadBit(ai->BII,bit);
		regcode += regcode + bit;
	}

ai->range = regrange;
ai->code  = regcode;
return gotbit;
}

