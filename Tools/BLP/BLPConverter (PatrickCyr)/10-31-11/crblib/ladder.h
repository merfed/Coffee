#ifndef LADDER_H
#define LADDER_H

#include "rungae.h"

struct srung
{
	rung_t r0,r1;
	ulong p0;
	ulong pad;
};

// RUNG_SHIFTS <= CumProbMaxBits
#define RUNG_SHIFTS			(10)
#define RUNG_ONE			(1<<RUNG_SHIFTS)
#define RUNG_HALF			(RUNG_ONE>>1)

extern const srung ladder[];

extern srung const * const pladder;

extern int ladder_start;

#endif // LADDER_H

