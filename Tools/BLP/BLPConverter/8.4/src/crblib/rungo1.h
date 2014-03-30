#ifndef RUNG_O1_H
#define RUNG_O1_H

#include <crblib/inc.h>
#include <crblib/arithc.h>

typedef struct rungO1 rungO1;

rungO1 *rungO1Create(arithInfo *ari,int NumContexts);
void	rungO1Destroy(rungO1 * ro1);

void	rungO1Encode(rungO1 * ro1, int context, bool bit);
bool	rungO1Decode(rungO1 * ro1, int context);

#endif // RUNG_O1_H

