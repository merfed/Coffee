#ifndef _CRBEQUTIL_H
#define _CRBEQUTIL_H

#include <crblib/inc.h>

extern void StrBump (ubyte *Dat,long Off,long Len);
extern char * CorrespondP(char *str);
extern long Min(long a,long b);
extern long UnmatchedParenthesis (char *str); /*returns 0 if all match ok*/
extern void StrUprNotQuoted(char *str);
extern void StrCutSpaceNotQuoted(char *str);
extern void MakeResult(char *Str,double V,int Precision);

#endif
