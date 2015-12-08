
// see <>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <crblib/inc.h>
#include <crblib/strutil.h>
#include <crblib/crbconv.h>

void StrBump (ubyte *Dat,long Off,long Len);
char * CorrespondP(char *str);
long Min(long a,long b);
long UnmatchedParenthesis (char *str); /*returns 0 if all match ok*/
void StrUprNotQuoted(char *str);
void StrCutSpaceNotQuoted(char *str);

#ifndef mabs
#define mabs(x) ( (x) < 0 ? - (x) : (x) )
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4244 4761) // ulong -> uword
#endif

void StrBump (ubyte *Dat,long Off,long Len)
{
Dat += Off;
memmove(Dat+1,Dat,(size_t)(Len-Off));
}

long Min(long a,long b)
{
if (a<b) return(a);
return(b);
}

char * CorrespondP(char *str)
{
str++;
while(*str!=')' && *str)
  {
  if (*str=='(') str=CorrespondP(str);
  str++;
  }
return(str);
}

long UnmatchedParenthesis (char *str) /*returns 0 if all match ok*/
{
register long NumParens=0;
while(*str)
  {
  if ( *str=='(' ) NumParens++;
  else if ( *str==')' ) NumParens--;

  str++;
  }
return(NumParens);
}

void StrUprNotQuoted(char *str)
{

while(*str)
  {
  if ( *str == '"' )
    {
    str++;
    while(*str && *str != '"' ) str++;
    }
  else if ( *str == '`' )
    {
    str++;
    while(*str && *str != '`' ) str++;
    }
  else if ( *str == 0x27 )
    {
    str++;
    while(*str && *str != 0x27 ) str++;
    }
  else
    {
    *str = toupper(*str);
    str++;
    }
  }

}

void StrCutSpaceNotQuoted(char *str)
{
char *tostr = str;

while(*str)
  {
  if ( *str == '"' )
    {
    *tostr++ = *str++;
    while(*str && *str != '"' ) *tostr++ = *str++;
    if ( !*str ) { *tostr=0; return; }
    *tostr++ = *str++;
    }
  else if ( *str == '`' )
    {
    *tostr++ = *str++;
    while(*str && *str != '`' ) *tostr++ = *str++;
    if ( !*str ) { *tostr=0; return; }
    *tostr++ = *str++;
    }
  else if ( *str == 0x27 )
    {
    *tostr++ = *str++;
    while(*str && *str != 0x27 ) *tostr++ = *str++;
    if ( !*str ) { *tostr=0; return; }
    *tostr++ = *str++;
    }
  else if ( *str == ' ' || *str == '\t' || *str == '\r' )
    {
    str++;
    }
  else
    {
    *tostr++ = *str++;
    }
  }
*tostr = 0;
}

void MakeResult(char *Str,double V,int Precision)
{
ulong ULV,ULAV; long LV;
char * StrV;
char * StrPtr;

ULAV = (ulong)(mabs(V));
ULV = (ulong)V;
LV = (long)V;
StrV = (char *)&ULV;

if ( Precision < 1 ) Precision = 1;
else if ( Precision > 9 ) Precision = 9;

Str[0] = 0;

strcat(Str," = ");
StrPtr = Str + strlen(Str);

if ( abs(V-(double)LV) < 0.0001 || abs(V-(double)ULV) < 0.0001 ) {
  if ( Precision > 4 ) {
    char FormatStr[5];
    
    FormatStr[0] = '%';
    FormatStr[1] = '.';
    FormatStr[2] = '0'+Precision;
    FormatStr[3] = 'f';
    FormatStr[4] = 0;
  
    sprintf(StrPtr,FormatStr,V);
  } else {
	
	/* <> : do commas ! */

    if ( V > 0 )
      sprintf(StrPtr,"%lu",ULV);
    else
      sprintf(StrPtr,"%ld",LV);
    }
  
  strcat(Str," = ");
  StrPtr = Str + strlen(Str);

  ltoh(StrPtr,ULV);
  
  strcat(Str," = ");
  StrPtr = Str + strlen(Str);
  
  NumToConvStr(ULV,StrPtr,CS_BINARY);
  
  if ( ULV <= 0xFF && ULV > 17 )
    {
    strcat(Str," = ");
    StrPtr = Str + strlen(Str);
    sprintf(StrPtr,"%c",ULV);
    }
} else {
  char FormatStr[5];
  
  FormatStr[0] = '%';
  FormatStr[1] = '.';
  FormatStr[2] = '0'+Precision;
  FormatStr[3] = 'g';
  FormatStr[4] = 0;

  sprintf(StrPtr,FormatStr,V);
  }

}
