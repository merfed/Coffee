#ifndef REPORT_H
#define REPORT_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" { 
#endif 

#ifdef DO_REPORT	//}{

//#pragma message("  PASS THIS AS A PARAMETER TO REPORT")
extern FILE * reportFP;

#pragma message("report ON")

void Report_DoReport(const char * str,int count,double tot,double totsqr);

#define REPORT_VARS(var)	static int count_##var =0; static double tot_##var = 0.0, totsqr_##var = 0.0;

#define REPORT_RESET(var)	do { count_##var =0; tot_##var = totsqr_##var = 0.0; } while(0)

#define REPORT_ADD(var)	do { count_##var ++; tot_##var += (double)(var); totsqr_##var += (double)(var)*(var); } while(0)

#define REPORT_REPORT(var)	Report_DoReport(#var,count_##var,tot_##var,totsqr_##var)

#define REPORT(x)	x

#else	//}{

#pragma message("report OFF")

#define REPORT_VARS(var)
#define REPORT_RESET(var)
#define REPORT_ADD(var)
#define REPORT_REPORT(var)

#define REPORT(x)

#endif //}{

#ifdef __cplusplus
}
#endif

#endif // REPORT_H

