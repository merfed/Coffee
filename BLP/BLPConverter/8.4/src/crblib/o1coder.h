#ifndef CRB_O1CODER_H
#define CRB_O1CODER_H

#include <crblib/arithc.h>

struct O1coderInfo {  long Private; };
typedef struct O1CoderInfo oOne;

extern struct O1coderInfo * O1coder_Init(arithInfo * ari,long numChars,long numContexts);
extern struct O1coderInfo * O1coder_InitMax(arithInfo * ari,long numChars,long numContexts,long max);
extern void O1coder_EncodeC(struct O1coderInfo * O1I,long Char,long Context);
extern long O1coder_DecodeC(struct O1coderInfo * O1I,long Context);
extern void O1coder_CleanUp(struct O1coderInfo * O1I);
extern void O1coder_Reset(struct O1coderInfo * O1I);

#define oOneCreate 		(oOne *)O1coder_Init
#define oOneCreateMax 	(oOne *)O1coder_InitMax
#define oOneEncode(x,y,z)	O1coder_EncodeC((struct O1coderInfo *)x,y,z)
#define oOneDecode(x,y)		O1coder_DecodeC((struct O1coderInfo *)x,y)
#define oOneFree(x)     	O1coder_CleanUp((struct O1coderInfo *)x)

#endif
