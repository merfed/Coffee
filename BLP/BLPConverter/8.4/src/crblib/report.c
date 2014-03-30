
#ifndef DO_REPORT
#define DO_REPORT
#endif

#include <stdio.h>
#include <math.h> // for sqrt

#include "report.h"

#ifndef _LOG
#define _LOG
#endif

#include "log.h" 

// PC: Changed to get it to compile.
//FILE * reportFP = stdout;
FILE * reportFP = NULL;

void Report_DoReport(const char * str,int count,double tot,double totsqr)
{
double avg,avgsqr; 

	avg = tot/count;
	avgsqr = totsqr/count;

	Log_TeeFile(reportFP);
	Log_Printf("%-20s : avg = %2.1f (sdev = %2.1f), tot = %d\n", 
		str , avg, sqrt(avgsqr - avg*avg), (int)tot );
}
