
#include <stdio.h>
#include <crblib/inc.h>
#include <crblib/memutil.h>
#include <crblib/arithc.h>
#include <crblib/context.h>
#include <crblib/o0coder.h>

struct O1coderInfo
  {
  arithInfo * arith;
  long numChars,numContexts,totMax;
	oZero * order0;
	context ** order1;
  };

/*protos:*/
void O1coder_CleanUp(struct O1coderInfo * O1I);

/*functions:*/

struct O1coderInfo * O1coder_InitMax(arithInfo * ari,long numChars,long numContexts,long totMax)
{
struct O1coderInfo * Ret;

if ( (Ret = AllocMem(sizeof(struct O1coderInfo),MEMF_CLEAR)) == NULL )
  return(NULL);

Ret->numChars = numChars;
Ret->numContexts = numContexts;
Ret->arith = ari;
Ret->totMax = totMax;

if ( (Ret->order0 = oZeroCreateMax(ari,numChars,totMax)) == NULL )
	{ O1coder_CleanUp(Ret); return(NULL); }

if ( (Ret->order1 = AllocMem(numContexts*sizeofpointer,MEMF_CLEAR)) == NULL )
	{ O1coder_CleanUp(Ret); return(NULL); }

return(Ret);
}

struct O1coderInfo * O1coder_Init (arithInfo * ari,long numChars,long numContexts)
{
return O1coder_InitMax(ari,numChars,numContexts,ari->safeProbMax);
}

void O1coder_CleanUp(struct O1coderInfo * O1I)
{
if ( O1I->order1 )
	{
	long i;
	for(i=0;i<O1I->numContexts;i++) if ( O1I->order1[i] ) contextFree(O1I->order1[i]);
	free(O1I->order1);
	}
if ( O1I->order0 ) oZeroFree(O1I->order0);
free(O1I);
}

void O1coder_EncodeC(struct O1coderInfo * O1I,long sym,long context)
{

if ( O1I->order1[context] == NULL )
	if ( (O1I->order1[context] = contextCreateMax(O1I->arith,O1I->numChars,O1I->totMax)) == NULL )
		return;

if ( ! contextEncode(O1I->order1[context],sym) )
	oZeroEncode(O1I->order0,sym);

return;
}

long O1coder_DecodeC(struct O1coderInfo * O1I,long context)
{
long sym;

if ( ! O1I->order1[context] )
	if ( (O1I->order1[context] = contextCreateMax(O1I->arith,O1I->numChars,O1I->totMax)) == NULL ) return(-1);

if ( ( sym = contextDecode(O1I->order1[context]) ) == -1 )
	{
	sym = oZeroDecode(O1I->order0);
	contextAdd(O1I->order1[context],sym);
	}

return(sym);
}

