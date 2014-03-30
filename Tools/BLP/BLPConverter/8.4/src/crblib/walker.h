#ifndef CRBLIB_WALKER_H
#define CRBLIB_WALKER_H

#include <crblib/fileutil.h>
#include <sys/stat.h>	// for Attr

struct WalkInfo
{
	char * Name;
	char * Path; /* contains ending path delimiter; may be wrong! */
	int NestLevel;
	bool IsDir;
	ulong Size;
	ulong Attr;
	ulong Date;

	char FullName[1024]; /* strcpy(FullName,Path) strcat(FullName,Name) */

	/**********/

	ulong Private;

	/** THIS IS A READ-ONLY STRUCTURE !!! ***/

};

typedef bool (*FileFuncType)(struct WalkInfo * WI);
typedef bool (*DirStartFuncType)(char * FullPath,int NestLevel);
typedef bool (*DirDoneFuncType)(char * FullPath,int NestLevel);
 /* functions return KeepGoing flag */

extern bool WalkDir(char *StartPath,bool RecurseFlag,bool DoDirs,
	FileFuncType FileFunc,DirStartFuncType DirStartFunc,DirDoneFuncType DirDoneFunc);

extern bool WalkDir_DeepFirst(char *StartPath,bool RecurseFlag,bool DoDirs,
	FileFuncType FileFunc,DirStartFuncType DirStartFunc,DirDoneFuncType DirDoneFunc);

extern bool WalkDir_TwoPass(char *StartPath,bool RecurseFlag,bool DoDirs,
	FileFuncType FileFunc,DirStartFuncType DirStartFunc,DirDoneFuncType DirDoneFunc);


/***

normal WalkDir processes directories, then steps into them

_DeepFirst processes all directories, then steps back from the deepest

only relevant when Recurseflag is on
	really only matters when DoDirsFlag is also on

this functionality is needed by Delete

---------

_TwoPass runs FileFunc on all entries in a dir before building
recursing information, then performs another pass to get that info

only relevant when DoDirsFlag && Recurseflag are both on

this functionality is needed by Rename

**/

#endif
