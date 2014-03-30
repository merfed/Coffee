#ifndef CRB_MATCHPAT_H
#define CRB_MATCHPAT_H

#include <crblib/inc.h>

typedef const uword * pattern; /** outside of matchpat.c , patterns are const **/

extern pattern StaticPat_Any; /* "*" */

extern char * MakePatternError; /*  if MakePat returns null, this has more info */

extern pattern MakePattern(char *str); /* returns from malloc */
extern pattern MakePatternNoCase(char *str);
extern void FreePattern(pattern pat);

extern bool IsWild(char *Str);
extern bool MatchPattern(char *VsStr,pattern PatStr);
extern bool MatchPatternNoCase(char *VsStr,pattern PatStr);

extern bool RenameByPat(pattern FmPat,char *FmStr,
						pattern ToPat,char *ToStr);
  /** given the first three, fill in the last **/

#endif
