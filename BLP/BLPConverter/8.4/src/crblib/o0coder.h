#ifndef CRB_O0CODER_H
#define CRB_O0CODER_H

#include <crblib/arithc.h>

struct O0coderInfo {  long Private; };
typedef struct O0coderInfo ozero;

extern struct O0coderInfo * O0coder_Init(arithInfo *ari,long NumChars);
extern struct O0coderInfo * O0coder_InitMax(arithInfo *ari,long NumChars,long TotProbMax);
extern void O0coder_EncodeC(struct O0coderInfo * O0I,long Char);
extern long O0coder_DecodeC(struct O0coderInfo * O0I);
extern void O0coder_CleanUp(struct O0coderInfo * O0I);
extern void O0coder_AddC(struct O0coderInfo * O0I,long Char);

#define ozeroCreate(x,y) (ozero *)O0coder_Init(x,y)
#define ozeroCreateMax(x,y,z) (ozero *)O0coder_InitMax(x,y,z)
#define ozeroEncode(x,y) O0coder_EncodeC((struct O0coderInfo *)x,y)
#define ozeroAdd(x,y) 	 O0coder_AddC((struct O0coderInfo *)x,y)
#define ozeroDecode(x)   O0coder_DecodeC((struct O0coderInfo *)x)
#define ozeroFree(x)     O0coder_CleanUp((struct O0coderInfo *)x)

#define oZero 			ozero
#define oZeroCreate ozeroCreate
#define oZeroCreateMax ozeroCreateMax
#define oZeroEncode ozeroEncode
#define oZeroAdd    ozeroAdd
#define oZeroDecode ozeroDecode
#define oZeroFree		ozeroFree

#endif
