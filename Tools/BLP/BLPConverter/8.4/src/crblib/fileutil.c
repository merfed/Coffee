#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <crblib/inc.h>
#include <crblib/strutil.h>
#include <crblib/fileutil.h>
#include <io.h>

#include <sys/utime.h>

void dprintf(const char * String, ...)
{
	va_list			ArgPtr;
    char			TempStr[4096];

	va_start(ArgPtr, String);
    vsprintf(TempStr, String, ArgPtr);
	va_end(ArgPtr);

	if ( isatty(_fileno(stdout)) )
		printf(TempStr);
	else
	{
		printf(TempStr);
		fprintf(stderr,TempStr);
	}
}

ubyte * readFile(char *name)
{
int len;
return readFile2(name,&len);
}

ubyte * readFile2(char *name,int *lenptr)
{
ubyte *buf;
ulong len;
FILE *fp;

	if ( (fp = fopen(name,"rb")) == NULL )
		return NULL;

	*lenptr = -1;

	if ( (len = FileLengthofFH(fp)) == FileLengthofFH_Error ) {
		fclose(fp);
		return NULL;
	}

	if ( (buf = malloc(len)) == NULL ) {
		fclose(fp);
		return NULL;
	}

	FRead(fp,buf,len);

	fclose(fp);

	*lenptr = len;

return buf;
}

bool writeFile(char *name,ubyte *buf,int len)
{
FILE * f;
	f = fopen(name,"wb");
	if ( ! f ) return false;

	if ( ! FWriteOk(f,buf,len) )
	{
		fclose(f);
		return false;
	}

	fclose(f);
return true;
}

/* endian-independent Number IO */

ulong fgetul(FILE *FP)
{
ulong ret;
ret  = fgetc(FP); ret<<=8;
ret += fgetc(FP); ret<<=8;
ret += fgetc(FP); ret<<=8;
ret += fgetc(FP);
return(ret);
}
void  fputul(ulong v,FILE *FP)
{
fputc((v>>24)&0xFF,FP);
fputc((v>>16)&0xFF,FP);
fputc((v>> 8)&0xFF,FP);
fputc((v    )&0xFF,FP);
}

uword fgetuw(FILE *FP)
{
uword ret;
ret  = (fgetc(FP)<<8);
ret += fgetc(FP);
return(ret);
}
void  fputuw(uword v,FILE *FP)
{
fputc(v>>8,FP);
fputc(v&0xFF,FP);
}

uword fgetuwi(FILE *FP)
{
uword ret;
ret = fgetc(FP);
ret += (fgetc(FP)<<8);
return(ret);
}
void  fputuwi(uword v,FILE *FP)
{
fputc(v&0xFF,FP);
fputc(v>>8,FP);
}

ubyte fgetub(FILE *FP) { return fgetc(FP); }
void  fputub(ubyte v,FILE *FP) { fputc(v,FP); }

/**

only Amiga and PC paths are supported! no UNIX no Mac !

this doesn't affect you if you don't use CatPaths() or FilePart()
	or libs that use them, such as Walker()

**/

/***************/

char * FilePart(char *F);
void DrivePartInsert(char *Name,char *PutIn);

/***************/

void printcommas(FILE * tofile,ulong num)
{
char tempstr[20];
strcommas(tempstr,num);
fprintf(tofile,tempstr);
}

void CutEndPath(char *Path)
{
char * EndBase = &Path[strlen(Path)-1];
if ( *EndBase == PathDelim ) *EndBase = 0;
return;
}

void CatPaths(char *Base,char *Add)
{
char * EndBase = &Base[strlen(Base)-1];

if ( *EndBase != ':' && *EndBase != PathDelim )
	{
	EndBase++;
	*EndBase++ = PathDelim;
	*EndBase = 0;
	}
strcat(EndBase,Add);
}

/** this func works even when AddTo has backwards-path
				components. i.e. it works as a full 'cd' :
		PrefixCurDir("/") or PrefixCurDir("..\..") work fine

		note : if you are in a drive, and you do cd("/") you will
			go up one dir - to the null path, which on the Amiga is
			your boot drive.  I'm not sure what the null path is under
			WinNT, but it would be nice if it were the My Computer thing.
	**/

void PrefixCurDir(char *AddTo)
{
char CurDir[2048];
char * AddToPtr;

getcwd(CurDir,1024);

if ( *AddTo == ':' || *AddTo == '\\' )
	{
	AddToPtr = CurDir + 1024;
	strcpy(AddToPtr,AddTo+1);
	DrivePartInsert(CurDir,AddTo);
	strcat(AddTo,AddToPtr);
	return;
	}
if ( *CurDir == 0 ) return;

if ( strchr(AddTo,':') )
  return;
#if 0	// change to dir of given path
  {
	char Work[1024];

  PathPartInsert(AddTo,Work);
  if ( chdir(Work) == 0 )
    {
    if ( getcwd(Work,1024) )
			{
	  	CatPaths(Work,FilePart(AddTo));
  		strcpy(AddTo,Work);
			}
		}
	return;
  }
#endif

CatPaths(CurDir,"");

AddToPtr = AddTo;

while ( *AddToPtr == '/' || ( AddToPtr[0] == '.' && AddToPtr[1] == '.' ) )
	{
	if ( *AddToPtr == '/' ) AddToPtr++;
	else
		{
		AddToPtr += 2;
		if ( *AddToPtr == '\\' ) AddToPtr++;
		}

	*(CurDir+strlen(CurDir)-1) = 0; /* cut off path delim */
	*(FilePart(CurDir)) = 0; /* cut one dir */

	if ( *CurDir == 0 ) return;
	}

while ( AddToPtr[0] == '.' && AddToPtr[1] == '\\' )
	{
	AddToPtr += 2;
	}

strcat(CurDir,AddToPtr);
strcpy(AddTo,CurDir);
}

ulong FileLengthofFH(FILE * fh)
{
long start,end;
start = ftell(fh);
fseek(fh,0,SEEK_END);
end = ftell(fh);
fseek(fh,start,SEEK_SET);
return((ulong)end);
}


ulong fileLength(char *name)
{
FILE * fp;
ulong len = FileLengthofFH_Error;

	if ( (fp = fopen(name,"rb")) == NULL )
		return len;

	len = FileLengthofFH(fp);

	fclose(fp);

return len;
}

void DrivePartInsert(char *Name,char *PutIn)
{
char * P;

strcpy(PutIn,Name);

if ( ( P = strchr(PutIn,':') ) == NULL )
	*PutIn = 0;

P++;
#ifndef _AMIGA
if ( *P == '\\' ) P++;
#endif

*P = 0;
}

char DrivePartRet[1024];
char * DrivePart(char *F)
{
DrivePartInsert(F,DrivePartRet);
return(DrivePartRet);
}

void PathPartInsert(char *F,char *Insert)
{
char * FP;
strcpy(Insert,F);
FP = FilePart(Insert);
if ( FP ) *FP = 0; 
return;
}

char PathPartRet[1024];
char * PathPart(char *F)
{
PathPartInsert(F,PathPartRet);
return(PathPartRet);
}

char * FilePart(char *F)
{
char *a;

if ( a = strrchr(F,PathDelim) )
	return(a+1);

if ( a = strrchr(F,':') )
	return(a+1);

return(F);
}

bool FileExists(char *Name)
{
struct stat st;

if ( stat(Name,&st) == 0 ) return(1);

return(0);
}

bool NameIsDir(char *Name)
{
struct stat st;
char * NamePath;

if ( stat(Name,&st) == 0 )
	{
	if ( S_ISDIR(st.st_mode) ) return(1);
	return(0);
	}

if ( NamePath = malloc(strlen(Name)+1) )
	{
  strcpy(NamePath,Name);
  CutEndPath(NamePath);
	
	if ( stat(NamePath,&st) == 0 )
		{
		if ( S_ISDIR(st.st_mode) )
			{
			CatPaths(NamePath,"");
			if ( strcmp(NamePath,Name) == 0 )
				{
				free(NamePath);
				return(1);
				}
			}
		}
	free(NamePath);
	}

return(0);
}

#ifdef _MSC_VER //{ // direct functions , msc doesn't provide these !! assholes
#include <io.h>

struct DIR
{
	long handle;
	struct _finddata_t data;
	struct dirent entry;
	bool didFirst;
};

int	closedir( DIR * d )
{
	_findclose(d->handle);
	destroy(d);
return 0;
}

DIR	*opendir( const char * name )
{
DIR * d;
char tempName[1024];
	d = new(DIR);
	if ( ! d ) return NULL;
	strcpy(tempName,name);
	CatPaths(tempName,"*");
	d->handle = _findfirst(tempName,&(d->data));
	if ( d->handle == -1 )
	{
		destroy(d);
		return NULL;
	}
	strcpy(d->entry.d_name,d->data.name);
	d->didFirst = true;
return d;
}

struct dirent *readdir( DIR * d )
{
if ( ! d ) return NULL;
else
	{
	struct dirent * ret = &(d->entry);

	if ( d->didFirst )
		d->didFirst = false;
	else
		if ( _findnext(d->handle,&(d->data)) == 0 )
			strcpy(d->entry.d_name,d->data.name);
		else
			return NULL;
	return ret;
	}
}

#else //}{ not MSVC

#endif //} MSVC

bool copystat(const char * tofile,const char *fmfile)
{
struct stat s;
	if ( stat(fmfile,&s) != 0 )
		return false;
return setstat(tofile,&s);
}

bool setstat(const char * file,const struct stat *s)
{
struct utimbuf ut;

	chmod(file,S_IREAD|S_IWRITE|S_IEXEC);

	ut.actime  = s->st_atime;
	ut.modtime = s->st_mtime;
	if ( utime(file, &ut) != 0 )
	{
		switch(errno)
		{
		case EACCES:
			errputs("utime failed : access denied");
			break;
		case EINVAL:
			errputs("utime failed : invalid date");
			break;
		case EMFILE:
			errputs("utime failed : too many files open !?");
			break;
		case ENOENT:
			errputs("utime failed : target file not found");
			break;
		default:
			errputs("utime failed : unknown error");
			break;
		}

		return false;
	}

	if ( chmod(file, s->st_mode ) != 0 )
		return false;

return true;
}

/****/
