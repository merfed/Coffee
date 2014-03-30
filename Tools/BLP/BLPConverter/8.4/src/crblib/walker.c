#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <crblib/inc.h>
#include <crblib/fileutil.h>

struct WalkInfo
	{
	char * Name;
	char * Path; /* contains ending path delimiter */
	int NestLevel;
	bool IsDir;
	ulong Size;
	ulong Attr;
	ulong Date;
	char FullName[1024]; /* strcpy(FullName,Path) strcat(FullName,Name) */
	
	/* private stuff: */

	struct WalkInfo * Next;
	};

struct WalkState
	{
	struct WalkState * Next;
	char Path[1024];
	bool WalkerFlag;
	int NestLevel;
	};

static bool HaveExitTrap = 0;
static char * ExitResetDir = NULL;

void WalkerExit(void);

typedef bool (*FileFuncType)(struct WalkInfo * WI); /* returns KeepGoing */
typedef bool (*DirStartFuncType)(char * FullPath,int NestLevel);
typedef bool (*DirDoneFuncType)(char * FullPath,int NestLevel);

bool WalkCurDir(DIR * DFD,char * BasePath,int BaseNestLevel,
	 bool RecurseFlag,bool DoDirs,FileFuncType FileFunc,
	 struct WalkState **BaseStatePtr)
{
bool KeepGoing = 1;
struct dirent * DE;
struct stat EntryStat;
struct WalkInfo *newWI,*WI_list;
struct WalkState * NextState,*CurState;

if ( BaseStatePtr )
	CurState = *BaseStatePtr;
else
	RecurseFlag = 0;

WI_list = NULL;

while ( KeepGoing && (DE = readdir(DFD)) )
	{
	if ( !( DE->d_name[0] == '.' && DE->d_name[1] == 0 ) &&
			 !( DE->d_name[0] == '.' && DE->d_name[1] == '.' && DE->d_name[2] == 0 ) )
		{
		if ( stat(DE->d_name,&EntryStat) == 0 )
			{
			if ( (newWI = malloc(sizeof(struct WalkInfo))) != NULL )
				{
				newWI->NestLevel = BaseNestLevel;
				newWI->Size = EntryStat.st_size; /* <> not accurate on PCs */
				newWI->Attr = EntryStat.st_mode;
				newWI->Date = EntryStat.st_mtime;
				newWI->IsDir = S_ISDIR(EntryStat.st_mode);
				if ( newWI->IsDir ) newWI->Size = 0;
	
				/** my internal stuff **/
				newWI->Next = NULL;
	
				newWI->Path = BasePath;
				strcpy(newWI->FullName,BasePath);
				CatPaths(newWI->FullName,DE->d_name);
				newWI->Name = FilePart(newWI->FullName);

				if ( newWI->IsDir && RecurseFlag )
					{
					if ( (NextState = malloc(sizeof(struct WalkState))) != NULL )
						{
						NextState->NestLevel = BaseNestLevel + 1;
						NextState->WalkerFlag = 0;
						strcpy(NextState->Path,newWI->FullName);
						NextState->Next = CurState;
						CurState = NextState;
						}
					/* else Ok = 0; */
					else
						KeepGoing = 0;
					}

				if ( (newWI->IsDir && ! DoDirs) || FileFunc == NULL )
					{
					free(newWI);
					}
				else
					{
					newWI->Next = WI_list;
					WI_list = newWI;
					}
				}
			/* else Ok = 0; */
			else
				KeepGoing = 0;
			}
		/* else Ok = 0; */
		}
	}

while(WI_list)
	{
	if ( KeepGoing ) KeepGoing = (*FileFunc)(WI_list);
	newWI = WI_list->Next;
	free(WI_list);
	WI_list = newWI;
	}

if ( BaseStatePtr )
	*BaseStatePtr = CurState;

return(KeepGoing);
}

bool WalkDir(char *StartPath,bool RecurseFlag,bool DoDirs,
	FileFuncType FileFunc,DirStartFuncType DirStartFunc,DirDoneFuncType DirDoneFunc)
{
struct WalkState *CurState,*GotState;
DIR * DFD;
bool Ok = 1;
char WasInDir[1024];
bool KeepGoing=1;

if ( !getcwd(WasInDir,1024) )
	return(0);

if ( ! HaveExitTrap ) if ( atexit(WalkerExit) == 0 ) HaveExitTrap = 1;
ExitResetDir = WasInDir;

if ( (CurState = malloc(sizeof(struct WalkState))) == NULL )
	return(0);
CurState->Next = NULL;
CurState->NestLevel = 0;
CurState->WalkerFlag = 0;

if ( !StartPath )
	{
	strcpy(CurState->Path,WasInDir);
	}
else
	{
	chdir(StartPath);
	if ( !getcwd(CurState->Path,1024) )
		{ free(CurState); return(0); }
	}

while( CurState )
	{
	GotState = CurState;
	CurState = CurState->Next;

	if ( KeepGoing )
		{
		if ( DFD = opendir(GotState->Path) )
			{
			if ( chdir(GotState->Path) == 0 )
				{
				if ( DirStartFunc && KeepGoing ) 
					KeepGoing = (*DirStartFunc)(GotState->Path,GotState->NestLevel);
	
				CatPaths(GotState->Path,"");

				if ( KeepGoing )
					KeepGoing = WalkCurDir(DFD,GotState->Path,GotState->NestLevel,
												RecurseFlag,DoDirs,FileFunc,&CurState);

				if ( KeepGoing && DirDoneFunc )
					KeepGoing = (*DirDoneFunc)(GotState->Path,GotState->NestLevel);

				closedir(DFD); DFD = NULL;
				}
			else Ok = 0;
	
			if ( DFD) closedir(DFD);
			}
		else
			{
			if ( chdir(GotState->Path) == 0 )
				{
				if ( DirStartFunc && KeepGoing )
					KeepGoing = (*DirStartFunc)(GotState->Path,GotState->NestLevel);
	
				if ( KeepGoing && DirDoneFunc )
					KeepGoing = (*DirDoneFunc)(GotState->Path,GotState->NestLevel);
				}
			}
		}

	free(GotState);
	}

chdir(WasInDir);
ExitResetDir = NULL;

return(Ok);
}

/***

normal WalkDir processes directories, then steps into them

this version processes all directories, then steps back from the deepest

this is needed by Delete

**/

bool WalkDir_DeepFirst(char *StartPath,bool RecurseFlag,bool DoDirs,
	FileFuncType FileFunc,DirStartFuncType DirStartFunc,DirDoneFuncType DirDoneFunc)
{
struct WalkState *CurState,*GotState;
DIR * DFD;
bool Ok = 1;
char WasInDir[1024];
bool KeepGoing=1;

if ( !getcwd(WasInDir,1024) )
	return(0);

if ( ! HaveExitTrap ) if ( atexit(WalkerExit) == 0 ) HaveExitTrap = 1;
ExitResetDir = WasInDir;

if ( (CurState = malloc(sizeof(struct WalkState))) == NULL )
	return(0);
CurState->Next = NULL;
CurState->NestLevel = -1;
CurState->WalkerFlag = 0;

if ( !StartPath )
	{
	strcpy(CurState->Path,WasInDir);
	}
else
	{
	int err = chdir(StartPath);
	if ( err != 0 )
		return 0;
	if ( !getcwd(CurState->Path,1024) )
		{ free(CurState); return(0); }
	}

if ( RecurseFlag)
	{
	struct WalkState * ListHead = CurState;

	do
		{
		GotState = ListHead;
		while(GotState && GotState->WalkerFlag ) GotState = GotState->Next;
	
		if ( GotState )
			{
			if ( KeepGoing )
				{
				if ( DFD = opendir(GotState->Path) )
					{
					if ( chdir(GotState->Path) == 0 )
						{
						if ( KeepGoing )
							KeepGoing = WalkCurDir(DFD,GotState->Path,GotState->NestLevel,
														1,0,NULL,&ListHead);
						}
					else
						Ok = 0;
			
					closedir(DFD);
					}
				}
			GotState->WalkerFlag = 1;
			}
		} while ( GotState );

	CurState = ListHead;
	}

while( CurState )
	{
	GotState = CurState;
	CurState = CurState->Next;

	if ( KeepGoing )
		{
		if ( DFD = opendir(GotState->Path) )
			{
			if ( chdir(GotState->Path) == 0 )
				{
				if ( DirStartFunc && KeepGoing ) 
					KeepGoing = (*DirStartFunc)(GotState->Path,GotState->NestLevel);
	
				CatPaths(GotState->Path,"");

				if ( KeepGoing )
					KeepGoing = WalkCurDir(DFD,GotState->Path,GotState->NestLevel,
												RecurseFlag,DoDirs,FileFunc,&CurState);

				if ( KeepGoing && DirDoneFunc )
					KeepGoing = (*DirDoneFunc)(GotState->Path,GotState->NestLevel);

				closedir(DFD); DFD = NULL;
				}
			else Ok = 0;
	
			if ( DFD) closedir(DFD);
			}
		else
			{
			if ( chdir(GotState->Path) == 0 )
				{
				if ( DirStartFunc && KeepGoing )
					KeepGoing = (*DirStartFunc)(GotState->Path,GotState->NestLevel);
	
				if ( KeepGoing && DirDoneFunc )
					KeepGoing = (*DirDoneFunc)(GotState->Path,GotState->NestLevel);
				}
			}
		}

	free(GotState);
	}

{
int err = chdir(WasInDir);
assert( err == 0 );
}
ExitResetDir = NULL;

return(Ok);
}

bool WalkDir_TwoPass(char *StartPath,bool RecurseFlag,bool DoDirs,
	FileFuncType FileFunc,DirStartFuncType DirStartFunc,DirDoneFuncType DirDoneFunc)
{
struct WalkState *CurState,*GotState;
DIR * DFD;
bool Ok = 1;
char WasInDir[1024];
bool KeepGoing=1;

if ( !getcwd(WasInDir,1024) )
	return(0);

if ( ! HaveExitTrap ) if ( atexit(WalkerExit) == 0 ) HaveExitTrap = 1;
ExitResetDir = WasInDir;

if ( (CurState = malloc(sizeof(struct WalkState))) == NULL )
	return(0);
CurState->Next = NULL;
CurState->NestLevel = 0;
CurState->WalkerFlag = 0;

if ( !StartPath )
	{
	strcpy(CurState->Path,WasInDir);
	}
else
	{
	chdir(StartPath);
	if ( !getcwd(CurState->Path,1024) )
		{ free(CurState); return(0); }
	}

while( CurState )
	{
	GotState = CurState;
	CurState = CurState->Next;

	/** pass 1 : do FileFuncs **/

	if ( KeepGoing )
		{
		if ( DFD = opendir(GotState->Path) )
			{
			if ( chdir(GotState->Path) == 0 )
				{
				if ( DirStartFunc && KeepGoing ) 
					KeepGoing = (*DirStartFunc)(GotState->Path,GotState->NestLevel);
	
				if ( KeepGoing )
					KeepGoing = WalkCurDir(DFD,GotState->Path,GotState->NestLevel,
												0,DoDirs,FileFunc,&CurState);

				if ( KeepGoing && DirDoneFunc )
					KeepGoing = (*DirDoneFunc)(GotState->Path,GotState->NestLevel);

				closedir(DFD); DFD = NULL;
				}
			else Ok = 0;
	
			if ( DFD) closedir(DFD);
			}
		else
			{
			if ( chdir(GotState->Path) == 0 )
				{
				if ( DirStartFunc && KeepGoing )
					KeepGoing = (*DirStartFunc)(GotState->Path,GotState->NestLevel);
	
				if ( KeepGoing && DirDoneFunc )
					KeepGoing = (*DirDoneFunc)(GotState->Path,GotState->NestLevel);
				}
			}
		}

	/** pass 2 : build recurse info **/

	if ( KeepGoing && RecurseFlag )
		{
		if ( DFD = opendir(GotState->Path) )
			{
			KeepGoing = WalkCurDir(DFD,GotState->Path,GotState->NestLevel,
									 1,0,NULL,&CurState);

			closedir(DFD);
			}
		}

	free(GotState);
	}

chdir(WasInDir);
ExitResetDir = NULL;

return(Ok);
}

void WalkerExit(void)
{
if ( ExitResetDir )
	{
	chdir(ExitResetDir);
	ExitResetDir = NULL;
	}
}
