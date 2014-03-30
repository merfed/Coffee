
#ifndef CRB_STRUTIL_H
#define CRB_STRUTIL_H

#include <string.h>

#ifdef __WATCOMC__
#define IN_COMPILER
#endif

#ifdef __BORLANDC__
#define IN_COMPILER
#endif

#ifdef _AMIGA
#define IN_COMPILER
#endif

#include <crblib/inc.h>

extern char * getParam(char *arg,char *argv[],int *iPtr);

// expandStrEscapes writes "to" from "fm" for codes like \n and \t
extern void expandStrEscapes(char *to,char *fm);

extern char * nexttok(char *str); // modifies str!
extern char * find_numeric(char *str);
extern char * skipwhitespace(char * str);
extern bool iswhitespace(char c);

extern void strcommas(char *str,ulong num);

extern char * strichr(const char *str,short c);
extern char * stristr(const char *bigstr,const char *substr);
extern char * strnchr(const char *str,short c,int len);
extern void strtolower(char *str);
extern void strins(char *to,const char *fm);
	 /* strins drops fm at head of to */
extern void strrep(char *str,char fm,char to);

#ifndef IN_COMPILER
extern int stricmp(const char *str,const char *vs);
extern char *strupr(char *str);
extern char *strrev(char *str);
#endif

#define strlwr strtolower
#define strlower strtolower
#define strupper strupr

#endif

