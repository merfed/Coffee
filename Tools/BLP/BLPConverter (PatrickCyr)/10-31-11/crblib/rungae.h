#ifndef GE_RUNGAE_H
#define GE_RUNGAE_H

#include <crblib/arithc.h>

typedef int rung_t;
typedef struct srung srung;

extern const ulong asmladder;

extern void rungModelInit(rung_t * rung);
extern void rungDecoderInit(void); // arithDecodeInit() calls this for us

extern void rungModelEncBit(arithInfo * ari,bool bit,rung_t * rung);
extern bool rungModelDecBit(arithInfo * ari,rung_t * rung);

extern void rungModelBit(rung_t * rung,bool bit);

extern void rungEncodeUnary(arithInfo *ari,uint val,rung_t * rung);
extern uint rungDecodeUnary(arithInfo *ari,rung_t * rung);

extern void rungEncodeUnaryMax(arithInfo *ari,uint val,rung_t * rung,uint max);
extern uint rungDecodeUnaryMax(arithInfo *ari,rung_t * rung,uint max);

extern uint rungGetP0Shifted10(rung_t * rung);

extern void rungModelSetByCounts(rung_t * rung,int c0,int c1);

#endif // rungc
