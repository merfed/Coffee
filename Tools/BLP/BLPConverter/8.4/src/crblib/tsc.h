#ifndef CRB_TSC_H
#define CRB_TSC_H

#include <crblib/inc.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	// show() will Pop() two and print the delta to log()
	// does nothing unless debug is on

extern void pushTSC(void);

	// the pop reads once & pop once & take difference

extern double popTSC(void);
extern double popTSChz(void);

	// primitives

typedef unsigned long tsc_type [2];

extern void readTSC(unsigned long *tsc);
extern double diffTSC(unsigned long *tsc1,unsigned long *tsc2);
extern double diffTSChz(unsigned long *tsc1,unsigned long *tsc2);

void showPopTSC(const char *tag,FILE * fp);
void showPopTSCper(FILE * fp,const char *tag,int items,const char *itemTag);

void showTSC(const char *tag,FILE * fp,double time);
void showTSCper(FILE * fp,const char *tag,int items,const char *itemTag,double time);

double secondsPerTSC(void);

int tscMHZ(void);

double readTSChz(void);
double timeTSC(void);

#ifdef __cplusplus
}
#endif

#endif
