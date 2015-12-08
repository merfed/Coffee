
#include <crblib/inc.h>
#include <time.h>
#include <windows.h>

#pragma comment(lib,"winmm.lib")

#include "tsc.h"

#ifdef _MSC_VER //{

void readTSC(ulong *hi)
{
ulong *lo = hi + 1;
	__asm 
	{
		rdtsc
		mov EBX,hi
		mov DWORD PTR [EBX],EDX
		mov EBX,lo
		mov DWORD PTR [EBX],EAX
	}
	return;
}

double diffTSChz(ulong *tsc1,ulong *tsc2)
{

 return	(tsc2[0] - tsc1[0])*4294967296.0 +
		(double)(tsc2[1] - tsc1[1]);

}

double readTSChz(void)
{
tsc_type tsc;
	readTSC(tsc);
return	(tsc[0]*4294967296.0 + (double)tsc[1]);
}

#else // }{ PC

void readTSC(ulong *hi)
{
	hi[0] = clock();
	hi[1] = 0;
}
double diffTSChz(ulong *tsc1,ulong *tsc2)
{
	return tsc2[0] - tsc1[0]; 
}

double readTSChz(void)
{
	return	clock();
}

#endif // } PC


typedef struct tscNode tscNode;
struct tscNode 
{
	tscNode *next;
	ulong tsc[2];
};
tscNode * head = NULL;

void pushTSC(void)
{
tscNode *tn;
	tn = new(tscNode);
	if ( !tn ) return;
	tn->next = head;
	head = tn;
	readTSC(tn->tsc);
}

double popTSC(void)
{
tscNode *tn;
ulong tsc[2];
	readTSC(tsc);
	if ( ! head ) return 0.0;
	tn = head;
	head = head->next;
return diffTSC(tn->tsc,tsc);
}

double popTSChz(void)
{
tscNode *tn;
ulong tsc[2];
	readTSC(tsc);
	if ( ! head ) return 0.0;
	tn = head;
	head = head->next;
return diffTSChz(tn->tsc,tsc);
}

double diffTSC(ulong *tsc1,ulong *tsc2)
{
return diffTSChz(tsc1,tsc2) * secondsPerTSC();
}

void showPopTSC(const char *tag,FILE * fp)
{
double time;

	time = popTSC();

	showTSC(tag,fp,time);
}

void showTSC(const char *tag,FILE * fp,double time)
{	
	fprintf(fp,"%s : %f secs\n",tag,time);
}

void showPopTSCper(FILE * fp,const char *tag,int items,const char *itemTag)
{
double time;

	time = popTSC();
	showTSCper(fp,tag,items,itemTag,time);
}

void showTSCper(FILE * fp,const char *tag,int items,const char *itemTag,double time)
{
double hz,per;

	hz = time * tscMHZ() * 1000000;
	per = time/(double)items;
	
	fprintf(fp,"%s : %f secs = %2.1f cycles / %s = ",tag,time,hz/items,itemTag);

	if ( per < 0.0001 ) 
	{
		fprintf(fp,"%d %ss /sec\n",(int)(1.0/per),itemTag);
	}
	else
	{
		fprintf(fp,"%f %ss /sec\n",(1.0/per),itemTag);
	}
}

#define TSC_REGISTRY_PATH	"SOFTWARE\\cbloom"
#define TSC_REGISTRY_VAL	"tscMHZ"

bool RegistryGetValue(const char * path,const char * file,int * pValue)
{
	HKEY hKey;
	bool result;
	DWORD type;
	DWORD size;
	char work[80];

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,path,0,KEY_QUERY_VALUE,&hKey)
		 != ERROR_SUCCESS )
	{
		return false;
	}

	// Set the value for the given name as a binary value

	result = RegQueryValueEx
				(
					hKey,
					file,
					0,
					&type,
					//(BYTE *) pValue,
					(BYTE *)work,
					&size
				) == ERROR_SUCCESS;
 
	RegCloseKey (hKey);

	if ( result )
	{
		*pValue = atoi(work);
	}

	return result;
}

bool RegistrySetValue(const char * path,const char * file,int value)
{
	HKEY hKey;
	bool result;
	char work[80];

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,path,0,KEY_ALL_ACCESS,&hKey)
		 != ERROR_SUCCESS )
	{
		// Create the key.  The valueName, if given, is ignored.
		DWORD dwDisposition;
		if ( RegCreateKeyEx
						(
							HKEY_LOCAL_MACHINE,
							path,
							0,
							"",
							REG_OPTION_NON_VOLATILE,
							KEY_ALL_ACCESS,
							NULL,
							&hKey,
							&dwDisposition
						) != ERROR_SUCCESS )
		{
			return false;
		}
	}

	// Set the value for the given name as a binary value

	/*
	result = RegSetValueEx
				(
					hKey,
					file,
					0,
					REG_BINARY,
					(CONST BYTE *) &value,
					sizeof(value)
				) == ERROR_SUCCESS;
	*/

	itoa(value,work,10);

	result = RegSetValueEx
				(
					hKey,
					file,
					0,
					REG_SZ,
					(CONST BYTE *) work,
					strlen(work)+1
				) == ERROR_SUCCESS;

	RegCloseKey (hKey);

	return result;
}

static double secondsPerClock = 0.0f;
static bool haveSecondsPerClock = false;
static int TSC_MHZ = 0;

double secondsPerTSC(void)
{
double clocksPerSec,MHZ;

	if ( haveSecondsPerClock )
	{
		return secondsPerClock;
	}

	if ( ! RegistryGetValue(TSC_REGISTRY_PATH,TSC_REGISTRY_VAL,&TSC_MHZ) )
	{
		DWORD clock1,clock2;
		tsc_type tsc1,tsc2;
		double secondsElapsed;
		volatile int i,j;

		#define SECONDS_TO_TEST	(10)

		fprintf(stderr,"Calibrating TSC for %d seconds!\n",SECONDS_TO_TEST);

		clock1 = timeGetTime();
		readTSC(tsc1);
		i = j = 0;
		while( (timeGetTime() - clock1) < 1000*SECONDS_TO_TEST )
		{
			// on lap-tops you have to do some real math here 
			//	to prevent the cpu from sleeping !!
			//Sleep(1);
			i++;
			j += rand();
		}
		clock2 = timeGetTime();
		readTSC(tsc2);
		secondsElapsed = (clock2 - clock1)/1000.0;
		clocksPerSec = diffTSChz(tsc1,tsc2) / secondsElapsed;

		MHZ = clocksPerSec / (1000000.0);

		fprintf(stderr,"Measured MHZ before rounding : %f\n",MHZ);

		// check for common errors
		//	round to nearest multiple of 50 or 83.33
		{
			int Z50,Z83;
			Z50 = ((int)((MHZ + 25)/50))*50; // round to the nearest fifty
			Z83 = ((int)(((MHZ + 41)*12)/1000))*1000/12; // round to the nearest 83.333 == 1000/12
			if ( ABS((double)Z50 - MHZ) < 14 )
				MHZ = Z50;
			else if ( ABS((double)Z83 - MHZ) < 14 )
				MHZ = Z83;
			else
				MHZ = ((int)((MHZ + 5)/10))*10; // round to the nearest ten

			TSC_MHZ = (int) MHZ;
		}
	
		fprintf(stderr,"Storing MHZ after rounding : %d\n",TSC_MHZ);

		RegistrySetValue(TSC_REGISTRY_PATH,TSC_REGISTRY_VAL,TSC_MHZ);
	}

	clocksPerSec  = TSC_MHZ * (1000000.0);

	haveSecondsPerClock = true;
	secondsPerClock = 1.0f / clocksPerSec;

return secondsPerClock;
}

double timeTSC(void)
{
double time;
	time = readTSChz();
return time * secondsPerTSC();
}

int tscMHZ(void)
{
	secondsPerTSC();
return TSC_MHZ;
}
