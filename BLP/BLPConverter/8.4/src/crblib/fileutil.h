#ifndef CRB_FILEUTIL_H
#define CRB_FILEUTIL_H

#include <crblib/inc.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>

#define PathDelim '\\'

extern ubyte * readFile(char *name); /** free() the returned **/
extern ubyte * readFile2(char *name,int *len); /** free() the returned **/
extern bool writeFile(char *name,ubyte *buf,int len);

/* endian-independent number-IO */

extern ulong fgetul(FILE *FP);
extern void  fputul(ulong v,FILE *FP);

extern uword fgetuw(FILE *FP);
extern void  fputuw(uword v,FILE *FP);

extern uword fgetuwi(FILE *FP);	/** intel endian **/
extern void  fputuwi(uword v,FILE *FP);

extern ubyte fgetub(FILE *FP);
extern void  fputub(ubyte v,FILE *FP);

#define readL fgetul
#define readW fgetuw
#define readB fgetub
#define writeL(fp,v) fputul(v,fp)
#define writeW(fp,v) fputuw(v,fp)
#define writeB(fp,v) fputub(v,fp)


/*{********** BEGIN switches to get dir.h ****************/

#ifdef _AMIGA /*{ set by SAS/C */

#define PathDelim '/'

#include <sys/dir.h>
#include <dos.h>

#else /*}{ not Amiga */

#define PathDelim '\\'

#ifdef __WATCOMC__  /*{ if you're on a PC, use watcom */

#include <direct.h>
#include <dos.h>

#else /*}{ not Watcom */

#ifdef __svr4__		/*{	UNIX */

#include <dirent.h>

#else /*}{ not svr4 */

#ifdef __BORLANDC__	/*{ */

#include <dirent.h>
#include <dir.h>

#else /*}{ not BORLANDC */

#ifdef _MSC_VER /*{ MSC */

#include <direct.h>

#if 0
"intentional syntax error : MSVC is not POSIX compliant!"
#endif

#else /*}{ not MSC */

#ifdef __GCC__	/*{ GCC , Cygnus-GNU-Win32 2.7.2 */
/*  note, the compiler doesn't define __GCC__ , I do in gcl.bat */

#include <sys/dirent.h>

#else	/*}{*/

/* unknown */
/* do nothing */

#endif /*} GCC */
#endif /*} MSC */
#endif /*} BORLANDC */
#endif /*} svr4 */
#endif /*} watcom */
#endif /*} _AMIGA */

/*}********** END switches to get dir.h ****************/

extern void dprintf(const char * String, ...); // dual printf to console & pipe

extern bool copystat(const char * tofile,const char *fmfile);
extern bool setstat(const char * file,const struct stat *s);

extern void PrefixCurDir(char *AddTo);

extern void CatPaths(char *Base,char *Add);
extern void CutEndPath(char *Path);

  /* note: CatPaths() only works on AMIGA and PC

    use CatPaths(Path,"") to add an ending path delimiter
  */

extern bool FileExists(char *Name);
extern bool NameIsDir(char *Name);

/***

NameIsDir behaves properly even when Name is terminated with
 a delimiter ('/','\',etc.)

***/

extern char * FilePart(char *FName);
extern char * PathPart(char *FName);
extern void PathPartInsert(char *F,char *Insert);
extern char * DrivePart(char *FName);
extern void DrivePartInsert(char *F,char *Insert);

/***

avoiding using PathPart() whenever possible,
  use PathPartInsert() instead

PathPart() may only be called & used consecutively, and
 subsequent calls destroy the data returned

PathPart() should never be used inside functions

same applies to DrivePart()

***/

extern ulong FileLengthofFH(FILE * fh);
#define FileLengthofFH_Error ((ulong)0xFFFFFFFF)

extern ulong fileLength(char *name);

#define FileLength FileLengthofFH
#define FileLengthError FileLengthofFH_Error

#define flength FileLengthofFH

#define FRead(fp,buf,len)   fread(buf,1,len,fp)
#define FWrite(fp,buf,len) fwrite(buf,1,len,fp)

#define FReadOk(fp,buf,len)  ((int)FRead(fp,buf,len) == (int)(len))
#define FWriteOk(fp,buf,len) ((int)FWrite(fp,buf,len) == (int)(len))

#ifndef APTR
#define APTR voidptr
#endif

#ifndef BPTR_T
#define BPTR_T
typedef FILE * BPTR;
#endif

#ifndef MODE_OLDFILE
#define MODE_OLDFILE "rb"
#endif

#ifndef MODE_NEWFILE
#define MODE_NEWFILE "wb"
#endif

#ifndef S_ISDIR
#define S_ISDIR( m )	(((m) & S_IFMT) == S_IFDIR)
#endif

#define Open(name,mode) fopen(name,mode)
#define Close(fh) fclose(fh)
#define Read FRead
#define Write FWrite

extern void printcommas(FILE * tofile,ulong num);

#define frewind rewind

// fucking unix sucks ass :

#ifndef EOF
#define EOF	    (-1)		/*  End of File/Error return code   */
#endif

#ifndef SEEK_SET
#define SEEK_SET    0			/*  Seek relative to start of file  */
#define SEEK_CUR    1			/*  Seek relative to current positn */
#define SEEK_END    2			/*  Seek relative to end of file    */
#endif

#ifdef _MSC_VER	// {

/***********

patches to the POSIX DIR() handling routines
Microsoft doesn't provide these, so I do!

**************/

#include <direct.h>

#define chdir	_chdir
#define getcwd	_getcwd

typedef struct DIR DIR;	// opaque

typedef struct dirent 
{
    char d_name[ 1024 ];	// file name
};

extern int	closedir( DIR * );
extern DIR	*opendir( const char * );
extern struct dirent *readdir( DIR * );

#endif // }

#endif /* FILEUTIL */
