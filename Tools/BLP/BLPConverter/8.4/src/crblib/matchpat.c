/***************
 * 
 * todo :
 *
 *		AND 
 *
 ************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crblib/inc.h>
#include <crblib/strutil.h>

#ifdef _MSC_VER
#pragma warning(disable : 4244) // ulong -> uword
#endif

/****

Pattern Spec :

A Pattern consists of alternating Tokens and Wilds

A Wild, '*' or '#?' matches any number of any character

All Tokens must be matched.

A Token consists of:

    raw characters
or  ranges of characters: [x-y] matches x,y,and all values between
or  lists of characters:  [a,b,c] matchs a or b or c

Tokens may be preceeded by a boolean-not '~'

Tokens may be OR-ed by listing options as (x|y|z)

Does not support #x functionality except for #?

 ` as a char to indicate that next is a raw literal:
 
   i.e. `~ to match the ~ character and `` to match the ` character

--------------------

Reserved characters:

`
*
?
(
|
&
)
[
]
~

--------------------

occasionally deeply recursive

can makes heavy use of the stack when multiple * or (||) branches are
involved.

--------------------

****/


typedef uword patchar;
typedef patchar * pattern;

#define WANY  256 //*
#define WCHAR 257 //?
#define WNOT  258 //~
#define WBRA  259 //[
#define WKET  260 //]

#define PBASE 0x1000 // ( | & )
#define PMASK 0x0FFF
#define PTYPE 0xF000
#define POPENOR  0x1000
#define POPENAND 0x2000
#define PDONEOR  0x3000
#define PDONEAND 0x4000
#define POR   	 0x5000
#define PAND  	 0x6000

const patchar StaticPat_Any_Array[] = { WANY, 0 };
pattern StaticPat_Any = (pattern) StaticPat_Any_Array;

char * MakePatternError = NULL;

pattern MakePattern(char *str)
{
pattern ret;
int ri,nest;
int bras,kets;

MakePatternError = NULL;

if ( (ret = malloc(sizeof(patchar)*(strlen(str)*2+256))) == NULL )
	{ MakePatternError = "malloc"; return(NULL);}

bras=kets=nest=ri=0;

/* handle ` parsing */
/* turn wild chars into masked (256 or more) */

while(*str)
	{
	switch(*str++)
		{
		case '`':
			if ( *str == 0 ) { free(ret); MakePatternError = "lit-null"; return(NULL); }
			// literal :
			ret[ri++] = *str++;
			break;

		case '[':
			/* <> check error in bra/ket formation */
			ret[ri++] = WBRA; bras++; break;
		case ']':
			ret[ri++] = WKET; kets++; break;

		case '(':
			nest++;
			ret[ri++] = POPENOR + nest; break;
		case ')':
			if ( nest == 0 ) { free(ret); MakePatternError = "unmatched or-close-paren"; return(NULL); }
			ret[ri++] = PDONEOR + nest; nest--; break;
		case '{':
			nest++;
			ret[ri++] = POPENAND + nest; break;
		case '}':
			if ( nest == 0 ) { free(ret); MakePatternError = "unmatched and-close-paren"; return(NULL); }
			ret[ri++] = PDONEAND + nest; nest--; break;
		case '|':
			if ( nest == 0 ) { free(ret); MakePatternError = "or out of paren"; return(NULL); }
			ret[ri++] = POR   + nest; break;
		case '&':
			if ( nest == 0 ) { free(ret); MakePatternError = "and out of paren"; return(NULL); }
			ret[ri++] = PAND + nest; break;

		case '?':
			ret[ri++] = WCHAR; break;
		case '~':
			ret[ri++] = WNOT; break;
		case '*':
			ret[ri++] = WANY; break;
		case '#':
			if ( *str == '?' ) { str++; ret[ri++] = WANY; }
			break;

		default: // literal
			ret[ri++] = *(str-1);
			break;
		}
	}
ret[ri] = 0;

if ( nest != 0 ) { free(ret); MakePatternError = "unmatched parens"; return(NULL); }
if ( bras != kets) { free(ret); MakePatternError = "unmatched bra/ket"; return(NULL); }

return(ret);
}

pattern MakePatternNoCase(char *str) { return(MakePattern(str)); }

void FreePattern(pattern pat)
{
if ( pat ) free(pat);
}

bool IsWild(char *Str)
{

if ( strchr(Str,'*') ||
     strchr(Str,'~') ||
     strchr(Str,'?') ||
   ( strchr(Str,'[') && strchr(Str,']') && ( strchr(Str,'-') || strchr(Str,',') ) ) ||
   ( strchr(Str,'(') && (strchr(Str,'|')||strchr(Str,'&')) && strchr(Str,')') ) )
    return(1);

return(0);
}

bool MatchPatternSub(char *VsStr,pattern PatStr,bool DoUpr)
{

#define LIT(zchar) ( DoUpr ? toupper(zchar) : zchar )

for(;;)
  {
  switch( *PatStr )
    {
    case WCHAR: /* match any one */
      PatStr++;
      if ( *VsStr == 0 ) return(0);
      VsStr++;
      break;
  
    case WBRA: /* match several */
      PatStr++;
      if ( PatStr[1] == '-' ) /* range */
        {
        if ( LIT(*VsStr) < LIT(PatStr[0]) || LIT(*VsStr) > LIT(PatStr[2]) )
          return(0);
        VsStr++;
        PatStr += 3;
        }
      else /* list */
        {
        while ( LIT(*PatStr) != LIT(*VsStr) )
          {
			PatStr++;
         	if ( *PatStr == WKET ) return(0);
			if ( LIT(*PatStr) == ',' ) PatStr++;
          }
        VsStr++;
        }
      while( *PatStr != WKET ) PatStr++;
      PatStr++;
      break;
    
    case WNOT: /* NOT */
      PatStr++;
      return( ! MatchPatternSub(VsStr,PatStr,DoUpr) );
  
    case WANY: /* WILD */
      PatStr++;
      if ( *PatStr == 0 ) return(1);

      while(*VsStr)
        {
        if ( MatchPatternSub(VsStr,PatStr,DoUpr) ) return(1);
        VsStr++;
        }
      return( MatchPatternSub(VsStr,PatStr,DoUpr) );
  
    default: /* raw character */

			if ( *PatStr >= PBASE ) /* and/or branch */
				{
				int nest;
				nest = *PatStr & PMASK;
				switch(*PatStr & PTYPE)
					{
					case POPENOR:
			      do
			        {
			        PatStr++;
			        if ( MatchPatternSub(VsStr,PatStr,DoUpr) ) return(1);
			        while ( *PatStr != (POR+nest) && *PatStr != (PDONEOR+nest) ) PatStr++;
			        } while( *PatStr != (PDONEOR+nest) );
			      return(0);

					case POPENAND:
			      do
			        {
			        PatStr++;
			        if ( ! MatchPatternSub(VsStr,PatStr,DoUpr) ) return(0);
			        while ( *PatStr != (PAND+nest) && *PatStr != (PDONEAND+nest) ) PatStr++;
			        } while( *PatStr != (PDONEAND+nest) );
			      return(1);

					case PDONEOR: /* left-overs from POPEN */
					case POR:
			      while ( *PatStr != (PDONEOR+nest) ) PatStr++;
			      PatStr++;
						break;

					case PDONEAND: /* left-overs from POPEN */
					case PAND:
			      while ( *PatStr != (PDONEAND+nest) ) PatStr++;
			      PatStr++;
						break;
					default:
						return(0); // error!
						break;	
					}
				}
			else /* raw character */
				{
	      if ( LIT(*VsStr) != LIT(*PatStr) ) return(0);
	      if ( *VsStr == 0 && *PatStr == 0 ) return(1);
	      VsStr++; PatStr++;
				}
      break;
    }
  }
return(0);
}

bool MatchPattern(char *str,pattern Pat) { return(MatchPatternSub(str,Pat,0)); }

bool MatchPatternNoCase(char *str,pattern Pat) { return(MatchPatternSub(str,Pat,1)); }

/**

temp routine, turns all wilds into *
rarely does cause unexpected not-nice behavior

ren zz*?e zz*.*e

should do: zzqae -> zzq.ae
but instead does: zzqae -> zzqa.e

**/

void RenameByPatSimplify(pattern Fm,pattern To)
{
for(;;)
  {
  switch( *Fm )
    {
    case WBRA:
      *To++ = WANY;
      while ( *Fm != WKET ) Fm++;
      break;
    case WCHAR:
      *To++ = WANY;
      break;
    case WNOT:
      *To++ = WANY;
      *To = 0;
      return;
    case 0:
      *To = 0;
      return;
    default:
			if ( *Fm & PTYPE )
				{
				int nest = *Fm & PMASK;
	      *To++ = WANY;
  	    while ( *Fm != (PDONEAND+nest) && *Fm != (PDONEOR+nest) ) Fm++;
				}
			else
				{
	      *To++ = *Fm;
				}
      break;
    }
  Fm++;
  }
}

/**

given the first three, fill in the last

**/

bool RenameByPat(pattern FmPat,char *FmStr,pattern ToPat,char *ToStr)
{
patchar FmPatT[1000],ToPatT[1000];
bool ret=0;

#define finish(val) { ret = val; goto RenameByPatDone; }

RenameByPatSimplify(FmPat,FmPatT);
RenameByPatSimplify(ToPat,ToPatT);

/** now only literals and * characters remain **/

FmPat = FmPatT;
ToPat = ToPatT;

for(;;)
  {
  while ( *ToPat != WANY )
    {
    *ToStr++ = *ToPat++;

    if ( ! *ToPat )
      {
      *ToStr = 0;
      finish(1);   /* ok for To to end early, it's a crop */
      }
    }
  while ( *FmPat != WANY )
    {
    FmPat++; FmStr++;

    if (! *FmPat )
      {
      *ToStr = 0;
      finish(0); 	/* not Ok for Fm to end early, it's an underflow of input */
      }
    }

  FmPat++; ToPat++; /* skip the stars */

	if ( *ToPat == '$' )
	{
		ToPat++;
		
	  while ( ! MatchPatternNoCase(FmStr,FmPat) ) FmStr++;
	}
	else
	{
	  while ( ! MatchPatternNoCase(FmStr,FmPat) ) *ToStr++ = *FmStr++;
	}
  }

finish(0);

RenameByPatDone:

return(ret);
}
