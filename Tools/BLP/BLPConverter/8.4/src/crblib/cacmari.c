#include <crblib/inc.h>
#include <crblib/bbitio.h>

/* #define MS_BIT_METHOD //else, COMPARISON_METHOD*/
/* COMPARISON_METHOD is faster*/

#ifdef _MSC_VER
#pragma warning(disable : 4244) // ulong -> uword
#endif

#define ADD_METHOD /*else SHIFT_AND_OR_METHOD*/
  /* uses x += x instead of x <<= 1
     uses x += 1 instead of x |= 1
   */ 

/* ADD_METHOD is faster*/

/* SHIFT_AND_OR_METHOD & MS_BIT_METHOD are left in solely for reference */

/* */

/* #define DEBUG */
/* #define LOG */

typedef uword arithcode;

struct CACMFAI
  {
  long CACMarithCumProbMax;
  long CACMarithCumProbMaxSafe;

  struct BitIOInfo * BII;

  long underflow_bits;
  arithcode code;
  arithcode low; 
  arithcode high;
  };

/*consts*/
static const long CACMarithCumProbMax = 16384;
static const long FirstQuarter = 16384;
static const long Half         = 32768;
static const long ThirdQuarter = 49152;

/*functions to be called from main:*/
struct CACMFAI * CACMarithInit(struct BitIOInfo * BII);
void CACMarithCleanUp(struct CACMFAI * CACMFAI);

void CACMarithEncodeCInit(struct CACMFAI * CACMFAI);
void CACMarithEncodeRange(struct CACMFAI * CACMFAI,long low,long high,long symtot);
void CACMarithEncodeCDone(struct CACMFAI * CACMFAI);

void CACMarithDecodeCInit(struct CACMFAI * CACMFAI);
void CACMarithDecodeRange(struct CACMFAI * CACMFAI,long *target,long symtot);
ulong CACMarithDecodeTarget(struct CACMFAI * CACMFAI,ulong symtot);
void CACMarithDecodeRangeRemove(struct CACMFAI * CACMFAI,long low,long high,long symtot);
void CACMarithDecodeCDone(struct CACMFAI * CACMFAI);

/*functions:*/

struct CACMFAI * CACMarithInit(struct BitIOInfo * BII)
{
struct CACMFAI * CACMFAI;

if ( (CACMFAI = malloc(sizeof(struct CACMFAI))) == NULL )
  return(NULL);

CACMFAI->CACMarithCumProbMax = CACMarithCumProbMax;
CACMFAI->CACMarithCumProbMaxSafe = CACMarithCumProbMax - 256;
CACMFAI->BII = BII;

return(CACMFAI);
}

void CACMarithCleanUp(struct CACMFAI * CACMFAI)
{
if (CACMFAI) free(CACMFAI);
}


void CACMarithEncodeCInit(struct CACMFAI * CACMFAI)
{

CACMFAI->low  = 0x0000;
CACMFAI->high = 0xFFFF;
CACMFAI->underflow_bits = 0;

}

void CACMarithEncodeRange(struct CACMFAI * CACMFAI,long symlow,long symhigh,long symtot)
{
long range;
register long reglow,reghigh,regunderflow;
#ifdef MS_BIT_METHOD
register ulong regbit;
#else
register long reghalf;
#endif

#ifdef DEBUG
if ( symlow < 0 || symlow >= symhigh || symhigh > symtot || symtot >= CACMarithCumProbMax ) DBF();
#endif

#ifdef LOG
printf("%ld / %ld / %ld\n",symlow,symhigh,symtot);
#endif

reglow       = CACMFAI->low;
reghigh      = CACMFAI->high;
regunderflow = CACMFAI->underflow_bits;

range = (long)( reghigh - reglow ) + 1;
reghigh = reglow + 
  (arithcode)(( range * symhigh ) / symtot - 1 );
reglow  = reglow + 
  (arithcode)(( range * symlow  ) / symtot );
/* these 5 lines are 16% of execution time!*/

#ifdef MS_BIT_METHOD
for ( ; ; )
  {

  /*
   * If this test passes, it means that the MSDigits match, and can
   * be sent to the output stream.
   */
  if ( ( reghigh & 0x8000 ) == ( reglow & 0x8000 ) ) /*8% of the time*/
    {
    if ( !(reghigh & 0x8000) ) regbit = 0;
    else regbit = 1;
    BitIO_WriteBit(CACMFAI->BII,regbit);

    if (!regbit ) regbit = 1;
    else regbit = 0;
    while ( regunderflow > 0 )
      {
      BitIO_WriteBit(CACMFAI->BII,regbit);
      regunderflow--;
      }
    }

  /*
   * If this test passes, the numbers are in danger of underflow, because
   * the MSDigits don't match, and the 2nd digits are just one apart.
   */
  else if ( ( reglow & 0x4000 ) && !( reghigh & 0x4000 ))
    {
    regunderflow ++;
    reglow  &= 0x3FFF;
    reghigh |= 0x4000;
    }
  else
    {
    CACMFAI->low  = reglow;
    CACMFAI->high = reghigh;
    CACMFAI->underflow_bits = regunderflow;
    return;
    }

  #ifdef ADD_METHOD
  reglow  += reglow;
  reghigh += reghigh;
  reghigh += 1;
  #else
  reglow  <<= 1;
  reghigh <<= 1;
  reghigh |= 1; 
  #endif

  }
#else
reghalf = Half;

for ( ; ; )
  {
  if ( reghigh < reghalf )
    {
    BitIO_WriteZeroBit(CACMFAI->BII);
    while ( regunderflow > 0 )
      {
      BitIO_WriteBit(CACMFAI->BII,1);
      regunderflow--;
      }
    }
  else if ( reglow >= reghalf )
    {
    BitIO_WriteBit(CACMFAI->BII,1);
    while ( regunderflow > 0 )
      {
      BitIO_WriteZeroBit(CACMFAI->BII);
      regunderflow--;
      }   
    reglow -= reghalf;
    reghigh -= reghalf;
    }
  else if ( reglow >= FirstQuarter && reghigh < ThirdQuarter )
    {
    regunderflow++;
    reglow  -= FirstQuarter;
    reghigh -= FirstQuarter;
    }
  else
    {
    CACMFAI->low  = reglow;
    CACMFAI->high = reghigh;
    CACMFAI->underflow_bits = regunderflow;
    return;
    }

  #ifdef ADD_METHOD
  reglow  += reglow;
  reghigh += reghigh;
  reghigh += 1;
  #else
  reglow  <<= 1;
  reghigh <<= 1;
  reghigh |= 1; 
  #endif

  }
#endif

}

void CACMarithEncodeCDone(struct CACMFAI * CACMFAI)
{
bool bit;

if ( CACMFAI->low & 0x4000 ) bit = 1;
else bit = 0;

BitIO_WriteBit(CACMFAI->BII,bit);

if ( bit ) bit = 0;
else bit = 1;

BitIO_WriteBit(CACMFAI->BII,bit);

if ( CACMFAI->underflow_bits > 0 )
  {
  do 
    { 
    BitIO_WriteBit(CACMFAI->BII,bit); 
    } while ( CACMFAI->underflow_bits-- > 0 );
  }

}


void CACMarithDecodeCInit(struct CACMFAI * CACMFAI)
{
long i;
bool bit;

CACMFAI->low  = 0x0000;
CACMFAI->high = 0xFFFF;

CACMFAI->code = 0;
for ( i=0; i<16; i++ )
  {
  CACMFAI->code <<= 1;
  BitIO_ReadBit(CACMFAI->BII,bit);
  if ( bit ) CACMFAI->code++;
  }

}


ulong CACMarithDecodeTarget(struct CACMFAI * CACMFAI,ulong symtot)
{
return ((((long) ( CACMFAI->code - CACMFAI->low ) + 1 ) * symtot - 1 ) / 
  ( ( (long)CACMFAI->high - (long)CACMFAI->low ) + 1 ) );
}

void CACMarithDecodeRange (struct CACMFAI * CACMFAI,long *target,long symtot)
{
*target = CACMarithDecodeTarget(CACMFAI,symtot);
return;
}

/*
 * Just figuring out what the present symbol is doesn't remove
 * it from the input bit stream.  After the character has been
 * decoded, this routine has to be called to remove it from the
 * input stream.
 */
void CACMarithDecodeRangeRemove(struct CACMFAI * CACMFAI,long symlow,long symhigh,long symtot)
{
long range;
register long reglow,reghigh,regcode;
register bool regbit;
#ifndef MS_BIT_METHOD
register long reghalf;
#endif

#ifdef DEBUG
if ( symlow >= symhigh || symhigh > symtot ) DBF();
#endif

#ifdef LOG
printf("%ld / %ld / %ld\n",symlow,symhigh,symtot);
#endif

reglow  = CACMFAI->low;
reghigh = CACMFAI->high;
regcode = CACMFAI->code;

range = (long)( reghigh - reglow ) + 1;
reghigh = reglow + 
  (arithcode)(( range * symhigh ) / symtot - 1 );
reglow  = reglow + 
  (arithcode)(( range * symlow  ) / symtot );

#ifdef MS_BIT_METHOD
/*
 * Next, any possible bits are shipped out.
 */
for ( ; ; ) 
  {
  /*
   * If the MSDigits match, the bits will be shifted out.
   */
  if ( ( reghigh & 0x8000 ) == ( reglow & 0x8000 ) )
    {
    }

  /*
   * Else, if underflow is threatening, shift out the 2nd MSDigit.
   */
  else if ( ( reglow & 0x4000 ) && !( reghigh & 0x4000 ))
    {
    regcode  ^= 0x4000;
    reglow   &= 0x3FFF;
    reghigh  |= 0x4000;
    }

  else
  /*
   * Otherwise, nothing can be shifted out, so I return.
   */
    {
    CACMFAI->low  = reglow;
    CACMFAI->high = reghigh;
    CACMFAI->code = regcode;
    return;
    }

  #ifdef ADD_METHOD
  reglow  += reglow;
  reghigh += reghigh;
  regcode += regcode;
  reghigh ++;
  BitIO_ReadBit(CACMFAI->BII,regbit);
  regcode += regbit;
  #else
  reglow  <<= 1;
  reghigh <<= 1;
  reghigh |= 1;
  regcode <<= 1;
  BitIO_ReadBit(CACMFAI->BII,regbit);
  regcode |= regbit;
  #endif

  }
#else
reghalf = Half;
for ( ; ; )
  {
  if ( reghigh < reghalf )
    {
    }
  else if ( reglow >= reghalf )
    {
    regcode -= reghalf;
    reglow  -= reghalf;
    reghigh -= reghalf;
    }
  else if ( reglow >= FirstQuarter && reghigh < ThirdQuarter )
    {
    regcode -= FirstQuarter;
    reglow  -= FirstQuarter;
    reghigh -= FirstQuarter;
    }
  else
  /*
   * Otherwise, nothing can be shifted out, so I return.
   */
    {
    CACMFAI->low  = reglow;
    CACMFAI->high = reghigh;
    CACMFAI->code = regcode;
    return;
    }

  #ifdef ADD_METHOD
  reglow  += reglow;
  reghigh += reghigh;
  regcode += regcode;
  reghigh ++;
  BitIO_ReadBit(CACMFAI->BII,regbit);
  regcode += regbit;
  #else
  reglow  <<= 1;
  reghigh <<= 1;
  reghigh |= 1;
  regcode <<= 1;
  BitIO_ReadBit(CACMFAI->BII,regbit);
  regcode |= regbit;
  #endif

  }
#endif

}

void CACMarithDecodeCDone(struct CACMFAI * CACMFAI)
{
}
