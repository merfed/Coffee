
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

#ifdef _MSC_VER
#pragma warning(disable : 4244 4018) // ulong -> uword
#endif

#include <crblib/inc.h>
#include <crblib/fbitio.h>

struct FileAI
  {
  long FileArithCumProbMax;
  long FileArithCumProbMaxSafe;

  struct FileBitIOInfo * BII;

  long Qlength;
  uword code;
  ulong range;
  };

/*consts*/
static const long CumProbMax    = 0x08000;
static const long Half          = 0x08000;
static const long One           = 0x10000;

/*functions to be called from main:*/
struct FileAI * FileArithInit(struct FileBitIOInfo * BII);
void FileArithCleanUp(struct FileAI * FileAI);

void FileArithEncodeCInit(struct FileAI * FileAI);
void FileArithEncodeRange(struct FileAI * FileAI,long symlow,long symhigh,long symtot);
void FileArithEncodeCDone(struct FileAI * FileAI);

void FileArithDecodeCInit(struct FileAI * FileAI);
void FileArithDecodeRange(struct FileAI * FileAI,long *target,long symtot);
void FileArithDecodeRangeRemove(struct FileAI * FileAI,long symlow,long symhigh,long symtot);
void FileArithDecodeCDone(struct FileAI * FileAI);

void FileArithEncodeCDone_Min(struct FileAI * FileAI);
void FileArithEncodeCDone_Safe(struct FileAI * FileAI);

  /** _Safe is used right now
   *  _Min assumes the decoder will be supplied with trailing zeros
   *    this is not so easy to actually do without a speed-hit,
   *    but does give us an optimal limit of arithcoder efficiency
   **/

/*functions:*/

struct FileAI * FileArithInit(struct FileBitIOInfo * BII)
{
struct FileAI * FileAI;

if ( (FileAI = malloc(sizeof(struct FileAI))) == NULL )
  return(NULL);

FileAI->FileArithCumProbMax = CumProbMax;
FileAI->FileArithCumProbMaxSafe = CumProbMax - 512;
FileAI->BII = BII;

return(FileAI);
}

void FileArithCleanUp(struct FileAI * FileAI)
{
if (FileAI) free(FileAI);
}


void FileArithEncodeCInit(struct FileAI * FileAI)
{

FileAI->code  = 0;
FileAI->range = One;
FileAI->Qlength = 0;

}


/*
 * FileArith Encoding using the generalized BitQueue
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

void FileArithEncodeRange(struct FileAI * FileAI,long symlow,long symhigh,long symtot)
{
ulong lowincr;
register ulong regcode,regrange,regQlength;
register ulong reghalf = Half;

regcode    = FileAI->code;
regrange   = FileAI->range;
regQlength = FileAI->Qlength;

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

  FileBitIO_WriteBit(FileAI->BII,1); regQlength--;
  while ( regQlength-- ) FileBitIO_WriteZeroBit(FileAI->BII);

  however, a tad bit faster is:

  **/

  FileBitIO_WriteBit(FileAI->BII,1);
  while ( --regQlength ) FileBitIO_WriteZeroBit(FileAI->BII);
  }

while( regrange <= Half )
  {
  if ( regcode >= reghalf )
    {
    /* FileBitIO_WriteBit(FileAI->BII,1); */
    /* instead of writing a 1 bit directly, put it in the queue */
    /* queue was 01..1 , make it 01..11 */

    if ( regQlength == 0 ) /* no leading zero */
      {
      FileBitIO_WriteBit(FileAI->BII,1);
      }
    else
      {
      regQlength ++; /* tack a 1 on the queue */
      }

    regcode -= reghalf;
    }
  else
    {
    /* FileBitIO_WriteZeroBit(FileAI->BII); */
    /* instead of writing a 0 bit directly, put it in the queue */
    /* queue was 01111 , make it 011110 , flush 01111 */

    /* send old queue */

    if ( regQlength )
      {
      /* send one (0-bit) and (Qlength - 1) (1-bit)s */

      FileBitIO_WriteZeroBit(FileAI->BII);
      while ( --regQlength ) FileBitIO_WriteBit(FileAI->BII,1);
      }

    /* now put a zero bit in the queue */

    regQlength = 1;
    }

  regcode  += regcode;
  regrange += regrange;
  }

FileAI->code  = regcode;
FileAI->range = regrange;
FileAI->Qlength = regQlength;
}

void FileArithEncodeCDone_Min(struct FileAI * FileAI)
{
ulong low,code,nextcode,mask,Qlength;

/** flush the state with a minimum of output **/

low = FileAI->code;
code = low + FileAI->range - 1;
mask = 0xFFFFFFFF;
Qlength = FileAI->Qlength;

if ( code & One )
  {
  /* queue was 01111... , add one (carry) makes it 10000... */
  FileBitIO_WriteBit(FileAI->BII,1);

  return;
  }

/* find a code in [low,low+range) with the maximum number of tailing 0s */

/* first send the queue */

if ( Qlength )
  {
  FileBitIO_WriteZeroBit(FileAI->BII); Qlength--;
  while ( Qlength-- ) FileBitIO_WriteBit(FileAI->BII,1);
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
    { code -= Half; FileBitIO_WriteBit(FileAI->BII,1); }
  else FileBitIO_WriteZeroBit(FileAI->BII);

  code <<= 1;
  }

}

void FileArithEncodeCDone_Safe(struct FileAI * FileAI)
{
ulong low,mask;

/* first send the queue */
if ( FileAI->Qlength )
  {
  ulong Qlength = FileAI->Qlength;

  FileBitIO_WriteZeroBit(FileAI->BII); Qlength--;
  while ( Qlength-- ) FileBitIO_WriteBit(FileAI->BII,1);
  }

low = FileAI->code;

for ( mask = Half; mask; mask>>=1 )
  {
  if ( low & mask )
		{
		FileBitIO_WriteBit(FileAI->BII,1);
		}
  else
		{
		FileBitIO_WriteZeroBit(FileAI->BII);
		}
  }

}

void FileArithEncodeCDone(struct FileAI * FileAI)
{
FileArithEncodeCDone_Safe(FileAI);
}

void FileArithDecodeCInit(struct FileAI * FileAI)
{
long i;
bool bit;

FileAI->range = One;

FileAI->code = 0;
for ( i=0; i<16; i++ )
  {
  FileAI->code <<= 1;
  FileBitIO_ReadBit(FileAI->BII,bit);
  if ( bit ) FileAI->code++;
  }

}


ulong FileArithDecodeTarget(struct FileAI * FileAI,ulong symtot)
{
return( (( FileAI->code + 1 ) * symtot - 1 ) / FileAI->range );
}

void FileArithDecodeRange (struct FileAI * FileAI,long *target,long symtot)
{
*target = (long) FileArithDecodeTarget(FileAI,symtot);
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

void FileArithDecodeRangeRemove(struct FileAI * FileAI,long symlow,long symhigh,long symtot)
{
register long regrange,regcode;
long lowincr;
bool bit;

regrange = FileAI->range;
regcode = FileAI->code;

lowincr = ( regrange * symlow ) / symtot;
regcode -= lowincr;
regrange = (( regrange * symhigh ) / symtot) - lowincr;

while ( regrange <= Half )
  {
  regrange += regrange;
  FileBitIO_ReadBit(FileAI->BII,bit);
  regcode += regcode + bit;
  }

FileAI->range = regrange;
FileAI->code  = regcode;
return;
}

void FileArithDecodeCDone(struct FileAI * FileAI)
{
}

