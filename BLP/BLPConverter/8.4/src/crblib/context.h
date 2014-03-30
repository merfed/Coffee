#ifndef CRB_CONTEXT_H
#define CRB_CONTEXt_H

#include <crblib/arithc.h>

typedef struct { int private; } context;

extern context * contextCreate(arithInfo * arithinfo,int alphabet_size);
extern context * contextCreateMax(arithInfo * arithinfo,int length,int totalMax);
extern bool contextEncode(context *pContext, int symbol); 	/* returns "was char written?" */
extern int  contextDecode(context *pContext); 							/* returns -1 for escape; after escape you MUST call contextAdd! */
extern void contextAdd   (context *pContext, int symbol); 	/* same as Decode_GotC */
extern void contextHalve (context *pContext); 							/* halve counts */
extern void contextFree  (context *pContext); 							/* purge */
extern bool contextHas   (context *pContext,int symbol);

extern bool contextEncodeNoAddIfEsc(context *pContext, int symbol);

/******** not for the general public: ********/

extern void contextGetInterval(context *pContext, int *pLow, int *pHigh, int symbol);
	// pass symbol+1 to GetInterval!!
extern int contextGetCumProb(context *pContext,int symbol); 
extern int contextGetProb(context *pContext,int symbol); 

/******** same as normal encode/decode , but use cap as NumSymbols;
		i.e. symbol < cap always *******/

extern bool contextEncodeCapped(context *pContext, int symbol,int cap);
extern int contextDecodeCapped(context *pContext,int cap);

#endif

