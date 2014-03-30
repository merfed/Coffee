#ifndef CRB_SCONTEXT_H
#define CRB_SCONTEXt_H

/*
 * scontext : for coding small-alphabet contexts
 *
 *  uses a linear array, not a binary tree
 *
 *  takes care to make sure escape is set to zero whenever the
 *   context fills up
 *
 */

#include <crblib/arithc.h>

typedef struct { int private; } scontext;

extern scontext * scontextCreate(arithInfo * arithinfo,int size,int escmax,int totmax,int inc,bool noesc);
extern void scontextAdd(scontext *sc, int symbol);
extern bool scontextEncode(scontext *sc, int symbol);	/** returns "coded?" **/
extern int scontextDecode(scontext *sc);				/** returns -1 for escape **/
extern void scontextHalve(scontext *sc);
extern void scontextFree(scontext *sc);
extern bool scontextHas(scontext *sc,int symbol);
extern int scontextGetProb(scontext *sc,int symbol);

#endif

