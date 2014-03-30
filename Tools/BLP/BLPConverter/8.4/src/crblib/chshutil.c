
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <errno.h>

#include <crblib/inc.h>
#include <crblib/fileutil.h>

#include <io.h>

bool Skipped=0,MadeDir=0;

bool CheckAndChangeAccess(char * Name);
bool AskContinue(void);
bool AskOverwrite2(char* Target,char*Source);

/***************** delete************/

bool DeleteError=0;
bool DeleteFuncQuiet = false;

bool DeleteFunc(char * Name,bool IsDir)
{
int err;

	Skipped=0;

	if ( ! CheckAndChangeAccess(Name))
	{
		DeleteError =1;
		return( AskContinue());
	}

	/*
	{
	char curdir[1024];
	getcwd(curdir,1024);
	err = chdir("c:\\");
	perror("chdir");
	}
	*/

	if ( IsDir ) err = rmdir(Name);
	else err = remove(Name);

	if ( err )
	{
		DeleteError =1;
		fprintf(stderr," %s: ",Name); fflush(stderr);
		if( IsDir) perror("rmdir");
		else perror("remove");
		return( AskContinue());
	}

	if ( ! DeleteFuncQuiet )
		printf("	%s\n",Name);

return(1);
}

/************ copy ***************/

bool CopyFunc(char * Name,char *ToName);

static bool CopyBufUsed =0; /*semaphore */
static ubyte * CopyBuf = NULL;
static const size_t CopyBufSize =(1<<17);

bool CopyError=0;

void CopyFuncFree(void)
{
if ( CopyBufUsed ) return;
if ( CopyBuf ) { free(CopyBuf);CopyBuf =NULL; }
}

bool CopyFuncInit(void)
{
if ( CopyBuf ) return(1);
if ( (CopyBuf= malloc(CopyBufSize)) ==NULL )
	return(0);
return(1);
}


bool ShowCopyMkDirError(char *Name)
{
CopyError= 1;
fprintf(stderr," %s",Name); fflush(stderr);
perror("MakeDirerror");

return( AskContinue());
}

bool ShowCopyError(char*Name,char *ToName)
{
CopyError= 1;
fprintf(stderr," %s-> %s",Name,ToName); fflush(stderr);
perror("Copy error");

return(AskContinue() );
}

bool CopyFuncGen(char* FmName,char*ToName,bool FmIsDir)
{
Skipped=0;MadeDir=0;

if ( ! FileExists(FmName))
	{
	errputs("source filenot found");
	Skipped=1;
	return(1);
	}

if ( FileExists(ToName))
	{
	if ( FmIsDir )
		{
		if ( NameIsDir(ToName) ) /*both dirs, fine*/
			{
			Skipped=1;
			return(1);
			}
		errputs("Trying to copya directoryonto a file.");
		}
	if (! CheckAndChangeAccess(ToName) )
		{
		return(AskContinue());
		}
	if ( ! AskOverwrite2(ToName,FmName))
		{
		Skipped=1;
		return(1);
		}
	}

if ( FmIsDir )
	{
	if ( mkdir(ToName) ==0 )
		{
		MadeDir =1;
		return(1);
		}
	else
		{
		return(ShowCopyMkDirError(ToName));
		}
	}
else
	{
	return( CopyFunc(FmName,ToName));
	}
}

bool CopyFunc(char * Name,char *ToName)
{
FILE *FmFH,*ToFH;
ulong FmLen,CurLen;

Skipped=0;

if ( ! CopyBuf )
	{
	if(! CopyFuncInit()) return(0);
	}

if ( ( FmFH= fopen(Name,"rb") ) ==NULL )
	return(ShowCopyError(Name,ToName));

if ( (FmLen= FileLengthofFH(FmFH))== FileLengthofFH_Error)
	{
	fclose(FmFH);
	return(ShowCopyError(Name,ToName));
	}

if ( ( ToFH= fopen(ToName,"wb") ) ==NULL )
	{
	fclose(FmFH);
	return(ShowCopyError(Name,ToName));
	}

if(CopyBufUsed) return(0); /*wait onsemaphore */

CopyBufUsed=1;

while( FmLen > 0 )
	{
	CurLen = min(CopyBufSize,FmLen);

	if ( FRead(FmFH,CopyBuf,CurLen)!= CurLen)
		{
		fclose(FmFH); fclose(ToFH);
		CopyBufUsed=0;
		return(ShowCopyError(Name,ToName));
		}

	if ( FWrite(ToFH,CopyBuf,CurLen) !=CurLen )
		{
		fclose(FmFH); fclose(ToFH);
		CopyBufUsed=0;
		return(ShowCopyError(Name,ToName));
		}

	FmLen -= CurLen;
	}

CopyBufUsed=0;

fclose(FmFH); fclose(ToFH);

copystat(ToName,Name);

return(1);
}

/********************* rename****************/

bool RenameError=0;

bool RenameFunc(char * FmName,char *ToName)
{
Skipped=0;

if ( ! FileExists(FmName))
	{
	errputs("source filenot found");
	Skipped=1;
	return(1);
	}

if ( FileExists(ToName))
	{
	if (! CheckAndChangeAccess(ToName) )
		{
		return(AskContinue());
		}
	if ( ! AskOverwrite2(ToName,FmName))
		{
		Skipped =1;
		return(1);
		}	
	}

if ( rename(FmName,ToName) !=0 )
	{
	if (strcmp( DrivePart(FmName),DrivePart(ToName)) != 0 )
		{
		if ( ! CopyFuncGen(FmName,ToName,NameIsDir(FmName))) {RenameError =1; return(0);}
		if ( ! DeleteFunc(FmName,NameIsDir(FmName))) {RenameError =1; return(0);}
		return(1);
		}
	RenameError = 1;
	fprintf(stderr," %s-> %s ",FmName,ToName); fflush(stderr);
	perror("rename");
	return( AskContinue());
	}
return(1);
}

/********************* overwrite ****************/

bool HaveOverwriteAnswer=FALSE,StoredOverwriteAnswer=FALSE,OverwriteOnlyNew=FALSE;

bool AskOverwrite2(char* Target,char*Source)
{
bool DoOverWrite,DoOverWriteNew;

	DoOverWrite = DoOverWriteNew = false;

	if (HaveOverwriteAnswer ){
		DoOverWrite =StoredOverwriteAnswer;
		DoOverWriteNew = OverwriteOnlyNew;
	} else {
		int c;
	
		fprintf(stderr," %s exists; overwrite? (y/n/A/N/u/U)",Target); fflush(stderr);
	
		for(;;) {
			c = getc(stdin); if ( c != '\n') while( getc(stdin)!= '\n');

			if ( c =='n' ) {
				DoOverWrite = false;
				break;
			} else if( c== 'N' ) {
				HaveOverwriteAnswer= true;
				StoredOverwriteAnswer = DoOverWrite = false;
				OverwriteOnlyNew = false;
				break;	
			} else if ( c =='A' ) {
				HaveOverwriteAnswer= true;
				StoredOverwriteAnswer = DoOverWrite = true;
				OverwriteOnlyNew = false;
				break;	
			} else if( c== 'y' ) {
				DoOverWrite = true;
				break;
			} else if ( c == 'u' ) {
				DoOverWriteNew = true;
				break;
			} else if ( c == 'U' ) {
				HaveOverwriteAnswer= true;
				OverwriteOnlyNew = DoOverWriteNew = true;
				break;
			} else {
				fprintf(stderr," (y=yes, n=no, A=all,N=none,u=update newer,U=all)");
				fflush(stderr);
			}
		}
	}

	if ( DoOverWriteNew ) {
		struct stat SourceStat,TargetStat;	

		if ( stat(Source,&SourceStat) != 0 ){
			fprintf(stderr,"<%s> ",Source); fflush(stderr);
			perror("stat");
			return false;
		}
		if ( stat(Target,&TargetStat) != 0 ){
			fprintf(stderr,"<%s> ",Target); fflush(stderr);
			perror("stat");
			return false;
		}

		if ( SourceStat.st_mtime > TargetStat.st_mtime ) {
			// printf("%s older than %s : overwriting\n",Target,Source);
			DoOverWrite = true;
		} else {
			// printf("%s newer than %s : skipping\n",Target,Source);
			DoOverWrite = false;
		}

	}

	if ( DoOverWrite ) {
		if ( remove(Target) != 0 ) {
			fprintf(stderr,"<%s> ",Target); fflush(stderr);
			perror("remove");
			return false;
		}
	}

return DoOverWrite;
}

bool AskOverwrite(char * Name)
{
if ( HaveOverwriteAnswer ) {
	if( StoredOverwriteAnswer) {
		if ( remove(Name)!= 0 ) {
			fprintf(stderr,"< %s> ",Name); fflush(stderr);
			perror("remove");
			return(0);
		}
		return(1);
	} else{
		return(0);
	}
} else {
	int c;
	
	fprintf(stderr," %sexists; overwrite? (y/n/A/N)",Name); fflush(stderr);
	
	for(;;)
		{
		c =getc(stdin); if( c!= '\n') while( getc(stdin)!= '\n');

		if ( c =='n' )return(0);
		else if( c== 'N' )
			{ HaveOverwriteAnswer= 1; StoredOverwriteAnswer = 0;return(0); }
		else if( c== 'y' ||c == 'A' )
			{
			if ( c =='A' )
				{ HaveOverwriteAnswer= TRUE;StoredOverwriteAnswer =TRUE; }
			if ( remove(Name)!= 0 )
				{
				fprintf(stderr,"< %s > ",Name);fflush(stderr);
				perror("remove");
				return(0);
				}
			return(1);
			}
		else
			{
			fprintf(stderr," (y=yes, n=no, A=all,N=none)");	fflush(stderr);
			}
		}
	}

}

/******************* access**********************/

static bool ChangeAccess_All = FALSE;

bool CheckAndChangeAccess(char * Name)
{

#ifdef __WATCOMC__ //{
if ( access(Name,W_OK) == 0 )
#else
#ifdef _MSC_VER
if ( access(Name,2) == 0 ) // @@ !?
#else
	intentional syntax error
#endif
#endif //}
	{
	return(1);
	}
else if( errno == EACCES)
	{
	if (! ChangeAccess_All )
		{
		int c;
		fprintf(stderr,"File %snot availablefor write\n",Name);
	Access_TryAgain:
		fprintf(stderr,"(C)hange,(I)gnore, or change(A)ll?");
		fflush(stderr);
		c =getc(stdin); c = toupper(c);
		if ( c !='\n' ) while(getc(stdin) != '\n') ;
		if ( c =='I' )return(0);
		else if( c== 'A' )
			{	ChangeAccess_All= TRUE;}	
		else if( c== 'C' ) { }
		else goto Access_TryAgain;
		}

	if (chmod(Name,S_IREAD|S_IWRITE|S_IEXEC) == 0 )
		return(1);

	perror("chmod failed");
	return(0);
	}

perror("access failed");
return(0);
}

/****************************continue ********************/

static bool AskContinue_All =FALSE;

bool AskContinue(void)
{
if ( AskContinue_All ) return(1);
else
	{
	int c;
	fprintf(stderr,"(Q)uit, keep(G)oing, orignore (A)ll errors?");
	fflush(stderr);
	c = getc(stdin);
	if( c!= '\n') getc(stdin);
	if( c== 'g' ||c == 'G' ) return(1);
	if( c== 'a' ||c == 'A' )
		{
		AskContinue_All= TRUE;
		return(1);
		}
	}
return(0);
}


/****************************ts/tr stuff ********************/

/** this isn't so fast, cuz it has to go back and count \n to
		show the line number **/

void ShowFoundMatch(char *BufPtr,char *BufBase,
						bool ShowLineNum,
						bool UltraQuiet, //= false,
						bool noPrefix,
						bool vcFormat,
						char * FileName) //= false)
{
	char FoundLine[4096];
	char *FLP,*FoundBufPtr,*EOLs;
	int LineNumber=0;

	if ( vcFormat )
		ShowLineNum = 1;

	FoundBufPtr = BufPtr;

	while(*BufPtr != '\n' && BufPtr >= BufBase) BufPtr--;
	BufPtr++;

	if ( (FoundBufPtr - BufPtr) > 4000 )
		BufPtr = FoundBufPtr - 4000;

	strncpy(FoundLine,BufPtr,4095); FoundLine[4095] = 0;

	FLP = FoundLine;

	if ( noPrefix ) FLP += (FoundBufPtr - BufPtr);

	if ( ShowLineNum )
	{
		char *BufSeek = BufBase;
		LineNumber = 1;
		while ( BufSeek < BufPtr ) if ( *BufSeek++ == '\n' ) LineNumber++;
	}

	if ( strchr(FoundLine,'\n') )
		*(strchr(FoundLine,'\n')) = 0;
	if ( strchr(FoundLine,'\r') )
		*(strchr(FoundLine,'\r')) = 0;
	while ( strchr(FoundLine,'\t') )
		*(strchr(FoundLine,'\t')) = ' ';

	while( *FLP == ' ' ) FLP++;

	EOLs = FLP + strlen(FLP) - 1;
	while(*EOLs==' ') EOLs--;
	EOLs++; *EOLs = 0;

	if ( vcFormat )
	{
		printf("%s(%d) : %s\n",FileName,LineNumber,FLP);
	}
	else if ( UltraQuiet ) 
	{
		printf("%s\n",FLP);
	}
	else 
	{
		if ( ShowLineNum )
		{
			printf("%5d:%s\n",LineNumber,FLP);
		}
		else
		{
			printf("%08X:%s\n",(FoundBufPtr-BufBase),FLP);
		}
	}
}
