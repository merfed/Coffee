
/*****

<> ! need to support variable 'inc' and 'escinc' to compete with "scontext"

<> I should rewrite this whole thing!

*****/

#include <stdio.h>
#include <stdlib.h>

#include <crblib/arithc.h>

#define BACK(i)			((i) & ((i) - 1))	
#define FORW(i)			((i) + ((i) & - (i)))

typedef struct {
	arithInfo * arith;
	int tree_size;		/* length of tree and current length */
	int escapeP;			/* no. symbols with frequency=1 */
	int total;				/* total of all frequencies in tree */
	int totalMax,escapeMax;
	uword * tree;			/* Fenwick's binary index tree */
} context;

context * contextCreate(arithInfo * arithinfo,int length); /** pass alphabet size **/
void contextAdd(context *pContext, int symbol);
bool contextEncode(context *pContext, int symbol);
int contextDecode(context *pContext);
void contextHalve(context *pContext);
void contextFree(context *pContext);

bool contextEncodeCapped(context *pContext, int symbol,int cap);
int contextDecodeCapped(context *pContext,int cap);

void contextGetInterval(context *pContext, int *pLow, int *pHigh, int symbol);
int contextGetCumProb(context *pContext,int symbol);
int contextGetProb(context *pContext,int symbol);

context * contextCreate(arithInfo * arithinfo,int length) /** pass alphabet size **/
{
context	*pContext;
int i;
int	size;

    /*
     * increment length to accommodate the fact 
     * that symbol 0 is stored at pos 1 in the array.
     */
length++;

    /* round length up to next power of two */
size = 1;
while (size < length)	size += size;

    /* malloc context structure and array for frequencies */
if ((pContext = (context *) malloc(sizeof(context))) == NULL) return(NULL);
if ((pContext->tree = (uword *) malloc((size+1)*sizeof(uword))) == NULL)
	{ free(pContext); return(NULL); }

pContext->arith = arithinfo;
pContext->total = 0;		/* total frequency */
pContext->totalMax = arithinfo->safeProbMax;
pContext->escapeMax = max( (length >> 2), 3);
pContext->tree_size = size;	/* no. symbols before growing */
    
    /* initialise contents of tree array to zero */
for (i = 0; i < size; i++) pContext->tree[i] = 0;

pContext->escapeP = 1;

return pContext;	    		/* return a pointer to the context */
}

context * contextCreateMax(arithInfo * arithinfo,int length,int totalMax)
{
context *pContext;
if ( (pContext = contextCreate(arithinfo,length)) == NULL )
	return(NULL);

if ( totalMax < pContext->totalMax )
	pContext->totalMax = totalMax;

return(pContext);
}

/*
 *
 * install a new symbol in a context's frequency table
 * returns 0 if successful, TOO_MANY_SYMBOLS or NO_MEMORY if install fails
 *
 */
void contextAdd(context *pContext, int symbol)
{
int i;

	while ( (pContext->total+pContext->escapeP+2) > pContext->totalMax )
		contextHalve(pContext);
									/* increment because first symbol at position one */
	i = symbol + 1;	    			/* update elements in tree below */

	do {
		pContext->tree[i] ++;
		i = FORW(i);
	} while (i < pContext->tree_size);

	if ( pContext->escapeP < pContext->escapeMax ) pContext->escapeP ++;
	pContext->total ++;

}

bool contextEncode(context *pContext, int symbol) /** returns flag "coded by me or not" **/
{
int low, high;

	contextGetInterval(pContext, &low, &high, symbol+1);
		
	if (low == high)
	  {
		if (pContext->escapeP == 0) 
			{
		fprintf(stderr,"stats: cannot code zero-probability novel symbol");
		exit(1);
			}

		/* contextEncode the escape symbol if unknown symbol */
		arithEncode(pContext->arith,
			pContext->total,
			pContext->total+pContext->escapeP,
			pContext->total+pContext->escapeP);

		contextAdd(pContext,symbol);

		return FALSE;
	  }

	arithEncode(pContext->arith,low, high, pContext->total+pContext->escapeP);

	if (high-low == 1 && pContext->escapeP > 1) pContext->escapeP --;

	contextAdd(pContext,symbol);

return TRUE;
}

bool contextEncodeNoAddIfEsc(context *pContext, int symbol) /** returns flag "coded by me or not" **/
{
int low, high;

	contextGetInterval(pContext, &low, &high, symbol+1);
		
	if (low == high)
	  {
		if (pContext->escapeP == 0) 
			{
		fprintf(stderr,"stats: cannot code zero-probability novel symbol");
		exit(1);
			}

		/* contextEncode the escape symbol if unknown symbol */
		arithEncode(pContext->arith,
			pContext->total,
			pContext->total+pContext->escapeP,
			pContext->total+pContext->escapeP);

		return FALSE;
	  }

	arithEncode(pContext->arith,low, high, pContext->total+pContext->escapeP);

	if (high-low == 1 && pContext->escapeP > 1) pContext->escapeP --;

	contextAdd(pContext,symbol);

return TRUE;
}

bool contextEncodeCapped(context *pContext, int symbol,int cap)
{
int low,high,total;

	total = contextGetCumProb(pContext,cap);

	contextGetInterval(pContext, &low, &high, symbol+1);
		
	if (low == high)
	{
		if (pContext->escapeP == 0) 
		{
			fprintf(stderr,"stats: cannot code zero-probability novel symbol");
			exit(1);
		}

		/* contextEncode the escape symbol if unknown symbol */
		arithEncode(pContext->arith,
			total,
			total+pContext->escapeP,
			total+pContext->escapeP);

		contextAdd(pContext,symbol);

		return FALSE;
	}

	arithEncode(pContext->arith,low, high,total+pContext->escapeP);

	if (high-low == 1 && pContext->escapeP > 1) pContext->escapeP --;

	contextAdd(pContext,symbol);

return TRUE;
}


/*
 *
 * contextDecode function is passed a context, and returns a symbol
 * returns -1 when an escape is received
 *
 */
int contextDecode(context *pContext)
{
int	mid, symbol, target;
int low, high;
    
	target = arithGet(pContext->arith,pContext->total+pContext->escapeP);

		/* check if the escape symbol has been received */
	if (target >= pContext->total) 
	{
		arithDecode(pContext->arith,
					pContext->total, 
				  pContext->total+pContext->escapeP,
				  pContext->total+pContext->escapeP);
		return -1;
	}

	/***

	symbol = 0;
	mid = pContext->tree_size >> 1;
	while (mid > 0)
	  {
		if (pContext->tree[symbol+mid] <= target)
			{
		  symbol = symbol+mid;
		  target -= pContext->tree[symbol];
			}
		mid >>= 1;
	  }

	***/

	symbol = 0;
	/* <> target -= tree[0]; */
	target ++;
	mid = pContext->tree_size >> 1;
	while (mid > 0)
	{
		if (target > pContext->tree[symbol+mid])
		{
			symbol = symbol+mid;
			target -= pContext->tree[symbol];
		}
		mid >>= 1;
	}
	/* <> symbol ++ */
    
	 /* 
	  * pass in symbol and symbol+1 instead of symbol-1 and symbol to
	  * account for array starting at 1 not 0 
	  */
	contextGetInterval(pContext, &low, &high, symbol+1);

	arithDecode(pContext->arith,low, high, pContext->total+pContext->escapeP);

	if (high-low == 1 && pContext->escapeP > 1) pContext->escapeP --;

	/* increment the symbol's frequency count */
	contextAdd(pContext,symbol);

return symbol;
}

int contextDecodeCapped(context *pContext,int cap)
{
int	mid, symbol, i, target;
int low, high,total;

	total = contextGetCumProb(pContext,cap);
    
	target = arithGet(pContext->arith,total+pContext->escapeP);

	if (target >= total)
	{
		arithDecode(pContext->arith,
					total, 
				  total+pContext->escapeP,
				  total+pContext->escapeP);
		return -1;
	}

	symbol = 0;
	target ++;
	mid = pContext->tree_size >> 1;
	while (mid > 0)
	{
		if (target > pContext->tree[symbol+mid])
		{
			symbol = symbol+mid;
			target -= pContext->tree[symbol];
		}
		mid >>= 1;
	}

	i = symbol+1;
	contextGetInterval(pContext, &low, &high, i);

	arithDecode(pContext->arith,low, high, total+pContext->escapeP);

	if (high-low == 1 && pContext->escapeP > 1) pContext->escapeP --;

	contextAdd(pContext,symbol);

return symbol;
}

/*
 *
 * get the low and high limits of the frequency interval
 * occupied by a symbol.
 * this function is faster than calculating the upper bound of the two 
 * symbols individually as it exploits the shared parents of s and s-1.
 *
 */
void contextGetInterval(context *pContext, int *pLow, int *pHigh, int symbol)
{
    int low, high, shared, parent;

	assert(symbol > 0);

    /* calculate first part of high path */
    high = pContext->tree[symbol];
    parent = BACK(symbol);
    
    /* calculate first part of low path */
    symbol--;
    low = 0;
    while (symbol != parent) {
			low += pContext->tree[symbol];
			symbol = BACK(symbol);
    }

    /* sum the shared part of the path back to root */
    shared = 0;
    while (symbol > 0) {
			shared += pContext->tree[symbol];
			symbol = BACK(symbol);
    }

    *pLow		= shared+low;
    *pHigh	= shared+high;
}

int contextGetProb(context *pContext,int symbol)
{
int low,high;
contextGetInterval(pContext,&low,&high,symbol+1);
return high-low;
} 

bool contextHas(context *pContext,int symbol)
{
int low,high;
contextGetInterval(pContext,&low,&high,symbol+1);
return ( low != high );
}

int contextGetCumProb(context *pContext,int symbol)
{
int prob;

prob = 0;
while (symbol > 0)
  {
	prob += pContext->tree[symbol];
	symbol = BACK(symbol);
  }
return(prob);
}
 

/*
 *
 * contextHalve is responsible for halving all the frequency counts in a 
 * context.
 * halves context in linear time by keeping track of the old and new 
 * values of certain parts of the array
 * also recalculates the number of singletons in the new halved context.
 *
 */

void contextHalve(context *pContext)
{
    int	old_values[32], new_values[32];
    int	i, zero_count, temp, sum_old, sum_new;

    pContext->escapeP = 1;
  for (i = 1; i < pContext->tree_size; i++)
    {
	temp = i;

	/* work out position to store element in old and new values arrays */
	for (zero_count = 0; !(temp&1); temp >>= 1)
	    zero_count++;

	/* move through context halving as you go */
	old_values[zero_count] = pContext->tree[i];
	for (temp = zero_count-1, sum_old = 0, sum_new = 0; temp >=0; temp--)
	{
	    sum_old += old_values[temp];
	    sum_new += new_values[temp];
	}
	pContext->tree[i] -= sum_old;
	pContext->total -= (pContext->tree[i]>>1);
	pContext->tree[i] -= (pContext->tree[i]>>1);
	if (pContext->tree[i] == 1) pContext->escapeP ++;
	pContext->tree[i] += sum_new;
	      
	new_values[zero_count] = pContext->tree[i];
    }

if ( pContext->escapeP > pContext->escapeMax ) pContext->escapeP = pContext->escapeMax;
if ( pContext->total+pContext->escapeP > pContext->totalMax ) {
		pContext->escapeP = (pContext->totalMax*2/3) - pContext->total;
		if ( pContext->escapeP < 0 ) pContext->escapeP = 1;
		if ( pContext->total+pContext->escapeP > pContext->totalMax ) {
			errputs("too many symbols for totalMax!"); dbf();
		}
	}

}

void contextFree(context *pContext)
{
free(pContext->tree);
free(pContext);
}

