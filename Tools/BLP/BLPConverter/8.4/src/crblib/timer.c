#include "timer.h"
#include <assert.h>

// PC: initializer not constant.
//FILE * timerFP = stdout;
FILE * timerFP = NULL;

int timerCount = 0;

int timerIsStarted = 0;
double time_Master = 0.0;
static tsc_type tsc_Master;

void Timer_Start(void)
{
assert( ! timerIsStarted);
timerIsStarted = 1;
readTSC(tsc_Master);
}
void Timer_Stop(void)
{
tsc_type tsc_Master2;
readTSC(tsc_Master2);
assert(timerIsStarted);
timerIsStarted = 0;
time_Master += diffTSC(tsc_Master,tsc_Master2);
}
