
//#define DEBUG

#include <stdio.h>
#include <stdlib.h>

#include <crblib/inc.h>
#include <crblib/arithc.h>

typedef struct {
	arithInfo * arith;
	int escape,total,size;
	int escmax,totmax,inc;
	int nzero;
	bool noesc;
	uword * tree;
} scontext;

scontext * scontextCreate(arithInfo * arithinfo,int size,int escmax,int totmax,int inc,bool noesc);
void scontextAdd(scontext *sc, int symbol);
bool scontextEncode(scontext *sc, int symbol);
int scontextDecode(scontext *sc);
void scontextHalve(scontext *sc);
void scontextFree(scontext *sc);
bool scontextHas(scontext *sc,int symbol);
int scontextGetProb(scontext *sc,int symbol);

scontext * scontextCreate(arithInfo * arithinfo,int size,int escmax,int totmax,int inc,bool noesc)
{
scontext *sc;
int i;

    /* malloc scontext structure and array for frequencies */
	if ( ! (sc = new(scontext)) ) return NULL;
	if ( ! (sc->tree = newarray(uword,size)) )
		{ free(sc); return(NULL); }

	sc->arith = arithinfo;
	sc->total = 0;
	sc->escape = 1;
	sc->size = size;
	sc->nzero = size;
	sc->inc = max(inc,1);
	sc->totmax = max(totmax,size+3);
	sc->escmax = min(max(escmax,3),(size>>2)+2);
	sc->noesc = noesc;
	if ( noesc ) {
		for(i=0;i<size;i++) sc->tree[i] = 1;
		sc->nzero = sc->escape = 0;
		sc->total = size;
	}

return sc;
}

void scontextAdd(scontext *sc, int sym)
{

if ( sc->tree[sym] == 0 ) {
	if ( sc->escape < sc->escmax ) {
		sc->escape ++;
	}
	if ( (--sc->nzero) == 0 ) {
		sc->escape = 0;
	}
} else if ( sc->tree[sym] == 1 ) {
	if ( sc->escape > 1 ) sc->escape --;
}

sc->tree[sym] += sc->inc;
sc->total += sc->inc;

while (sc->total > sc->totmax )
	scontextHalve(sc);

}

bool scontextEncode(scontext *sc, int sym) /** returns flag "coded by me or not" **/
{
	
if ( sc->tree[sym] == 0 ) {
#ifdef DEBUG
	if ( sc->escape == 0 || sc->nzero < 1 ) {
		errputs("stats: cannot code zero-probability novel symbol");
		fprintf(stderr,"sym: %d , esc: %d, nzero: %d\n",sym,sc->escape,sc->nzero);
		exit(1);
	}
#endif

	arithEncode(sc->arith,
		sc->total,
		sc->total+sc->escape,
		sc->total+sc->escape);

	scontextAdd(sc,sym);

	return false;
} else {
	int low, high, i;

	low=0;
	for(i=0;i<sym;i++) low += sc->tree[i];
	high = low + sc->tree[sym];

	arithEncode(sc->arith,low, high, sc->total+sc->escape);

	scontextAdd(sc,sym);
}

return true;
}


/* returns -1 when an escape is received
 */
int scontextDecode(scontext *sc)
{
int	sym, target;
int low, high;
    
target = arithGet(sc->arith,sc->total+sc->escape);

    /* check if the escape symbol has been received */
if (target >= sc->total) {
	arithDecode(sc->arith,
				sc->total, 
				sc->total+sc->escape,
				sc->total+sc->escape);
	return -1;
	}

sym = 0;
low = 0;
while( target >= sc->tree[sym] ) {
	low += sc->tree[sym];
	target -= sc->tree[sym];
	sym++;
}
high = low + sc->tree[sym];

arithDecode(sc->arith,low, high, sc->total+sc->escape);

scontextAdd(sc,sym);

return sym;
}

bool scontextHas(scontext *sc,int symbol)
{
return( scontextGetProb(sc,symbol) );
}

int scontextGetProb(scontext *sc,int symbol)
{
return( sc->tree[symbol] );
}
 
void scontextHalve(scontext *sc)
{
int i;
sc->nzero = 0;
sc->escape = 1;
sc->total = 0;
for(i=0;i<sc->size;i++) {
	sc->tree[i] >>= 1;
	if ( sc->tree[i] == 0 ) {
		if ( sc->noesc ) sc->tree[i] = 1;
		else sc->nzero ++;
	} else if ( sc->tree[i] == 1 )
		sc->escape ++;

	sc->total += sc->tree[i];
}

if ( sc->nzero == 0 ) sc->escape = 0;
if ( sc->escape > sc->escmax ) sc->escape = sc->escmax;

}

void scontextFree(scontext *sc)
{
free(sc->tree);
free(sc);
}

