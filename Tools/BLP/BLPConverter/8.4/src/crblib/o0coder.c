
#include <stdio.h>
#include <crblib/inc.h>
#include <crblib/memutil.h>
#include <crblib/arithc.h>
#include <crblib/context.h>

struct O0coderInfo {
	arithInfo * arith;
	long numChars;
	context * order0;
  };

/*protos:*/
void O0coder_CleanUp(struct O0coderInfo * O0I);

/*functions:*/

struct O0coderInfo * O0coder_Init (arithInfo * ari,long numChars)
{
struct O0coderInfo * Ret;

if ( (Ret = AllocMem(sizeof(struct O0coderInfo),MEMF_CLEAR)) == NULL )
  return(NULL);

if ( (Ret->order0 = contextCreate(ari,numChars)) == NULL )
	{ O0coder_CleanUp(Ret); return(NULL); }

Ret->numChars = numChars;
Ret->arith = ari;

return(Ret);
}

struct O0coderInfo * O0coder_InitMax(arithInfo * ari,long NumChars,long totMax)
{
struct O0coderInfo * Ret;

if ( (Ret = AllocMem(sizeof(struct O0coderInfo),MEMF_CLEAR)) == NULL )
  return(NULL);

if ( (Ret->order0 = contextCreateMax(ari,NumChars,totMax)) == NULL )
	{ O0coder_CleanUp(Ret); return(NULL); }

Ret->numChars = NumChars;
Ret->arith = ari;

return(Ret);
}

void O0coder_EncodeC(struct O0coderInfo * O0I,long sym)
{

if ( ! contextEncode(O0I->order0,sym) ) {
		/* use order -1 <> todo! could be better with exclusions! */
	arithEncode(O0I->arith,sym,sym+1,O0I->numChars);
}

return;
}

void O0coder_AddC(struct O0coderInfo * O0I,long sym)
{
contextAdd(O0I->order0,sym);
return;
}

long O0coder_DecodeC(struct O0coderInfo * O0I)
{
long sym;

if ( ( sym = contextDecode(O0I->order0) ) == -1 ) {
	/* use order -1 */
	sym = arithGet(O0I->arith,O0I->numChars);
	arithDecode(O0I->arith,sym,sym+1,O0I->numChars);

	contextAdd(O0I->order0,sym);
}

return(sym);
}

void O0coder_CleanUp(struct O0coderInfo * O0I)
{
if ( O0I->order0 ) contextFree(O0I->order0);
free(O0I);
}
