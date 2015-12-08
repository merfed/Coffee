#ifndef CRB_MTF_H
#define CRB_MTF_H

#include <crblib/inc.h>

struct MTFInfo { int private; };

extern struct MTFInfo * MTF_Init(long NumSymbols);
extern void MTF_CleanUp(struct MTFInfo * MTFI);

extern int MTF_Translate(struct MTFInfo * MTFI,int val); /* -1 error */
extern int MTF_Untranslate(struct MTFInfo * MTFI,int pos); /* -1 error */

extern bool MTF_TransformArray(struct MTFInfo * MTFI,ubyte * Array,long ArrayLen);
extern bool MTF_UntransformArray(struct MTFInfo * MTFI,ubyte * Array,long ArrayLen);

#endif
