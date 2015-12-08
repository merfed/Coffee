#ifndef CRB_SOZ_H
#define CRB_SOZ_H

/*
 * soz : s-context-o-zero for coding small-alphabet contexts
 *
 *  uses a linear array, not a binary tree
 *
 */

#include <crblib/arithc.h>

typedef struct { int private; } soz;

extern soz * sozCreate(arithInfo * arithinfo,int size,int totmax,int inc);
extern void sozEncode(soz *sc, int symbol);
extern int sozDecode(soz *sc);			
extern void sozHalve(soz *sc);
extern void sozFree(soz *sc);
extern void sozReset(soz *sc);

#endif

