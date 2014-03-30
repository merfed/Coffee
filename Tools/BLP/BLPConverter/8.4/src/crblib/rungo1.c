#include "rungo1.h"
#include "rungae.h"

struct rungO1
{
	int numContexts;
	arithInfo *ari;
	rung_t * rungs;
};

rungO1	*	 rungO1Create(arithInfo *ari,int nc)
{
rungO1 * ro1;
int i;
	
	assert(ari && nc > 0 );

	ro1 = new(rungO1);
	if ( ! ro1 ) return NULL;

	ro1->numContexts = nc;
	ro1->ari = ari;
	ro1->rungs = newarray(rung_t,nc);
	if ( ! ro1->rungs ) {
		destroy(ro1);
		return NULL;
	}

	for(i=0;i<nc;i++)
	{
		rungModelInit(&(ro1->rungs[i]));
	}

return ro1;
}

void		rungO1Destroy(rungO1 * ro1)
{
	assert(ro1 && ro1->rungs);
	destroy(ro1->rungs);
	destroy(ro1);
}

void	rungO1Encode(rungO1 * ro1, int context, bool bit)
{
	assert(ro1);
	assert( context >= 0 && context < ro1->numContexts );
	rungModelEncBit(ro1->ari,bit,&(ro1->rungs[context]));
}

bool	rungO1Decode(rungO1 * ro1, int context)
{
	assert(ro1);
	assert( context >= 0 && context < ro1->numContexts );
return rungModelDecBit(ro1->ari,&(ro1->rungs[context]));
}
