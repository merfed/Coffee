#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <crblib/strutil.h>

bool iswhitespace(char c)
{
	return ( c== ' ' || c== '\t' || c== '\n' || c== '\r' );
}

char * skipwhitespace(char * str)
{
	while( iswhitespace(*str) )
		str++;

return str;
}

#define Apostrophe ((char)39)	// the ''' char	

char * nexttok(char *str)	/** modifies str! **/
{
	for(;;) {
		switch( *str ) {
			case '"':
				str++;
				while(*str != '"') { if ( *str == 0 ) return NULL; str++; }
				break;
			case Apostrophe:		
				str++;
				while(*str != Apostrophe) { if ( *str == 0 ) return NULL; str++; }
				break;	
			case 0:
				return NULL;
			case ' ': case '\n': case '\t':
				*str++ = 0;
				while(*str<=32 && *str>0) *str++ = 0;
				return str;
			default:
				break;
		}
		str++;
	}
}





char * find_numeric(char *str)
{
	for(;;) {
		if ( isdigit(*str) ) return str;
		if( *str++ == 0 ) return NULL;
	}
}

void strcommas(char *str,ulong num)
{
int cnt;
char * strbase = str;

*str++ = (char)('0' + num%10); num /= 10;
cnt=1;

while(num)
  {
  if ( cnt == 3 ) { *str++ = ','; cnt = 0; }
	*str++ = (char)('0' + num%10); num /= 10;
	cnt++;
  }
*str = 0;

strrev(strbase);
}

void strrep(char *str,char fm,char to)
{
while( *str )
  {
  if ( *str == fm ) *str = to;
  str++;
  }
}

#ifndef IN_COMPILER
int stricmp(const char *str,const char *vs)
{

while( toupper(*str) == toupper(*vs) )
  {
  if ( ! *str ) return(0);
  str++; vs++;
  }

return( toupper(*str) - toupper(*vs) );
}
#endif

char * strichr(const char *str,short c)
{
short oc;

if (isupper(c)) oc = tolower(c);
else oc = toupper(c);

while ( *str )
  {
  if ( *str == c || *str == oc) return((char *)str);
  str++;
  }

return(NULL);
}

char * strnchr(const char *str,short c,int len)
{
register char *donestr;

donestr=(char *)((int)str + len);

while (str < donestr)
  {
  if (*str == c) return((char *)str);
  str++;
  }

return(NULL);
}

char * stristr(const char *StrBase,const char *SubBase)
{

while ( *StrBase )
  {
  if ( toupper(*StrBase) == toupper(*SubBase) )
	 {
	 const char * Str,* Sub;
	 Str = StrBase + 1;
	 Sub = SubBase + 1;
	 while ( *Sub && toupper(*Sub) == toupper(*Str) )
		{
		Sub++; Str++;
		}
	 if ( ! *Sub) return((char *)StrBase);
	 }
  StrBase++;
  }

return(NULL);
}

void strtolower(char *str)
{
while(*str)
  {
  *str = tolower(*str);
  str++;
  }
}

void strins(char *to,const char *fm)
{
int tolen = strlen(to);
int fmlen = strlen(fm);
char *newto,*oldto;

newto = to+fmlen+tolen; oldto = to+tolen;
tolen++;
while(tolen--) *newto-- = *oldto--;

while(fmlen--) *to++ = *fm++;

}

#ifndef IN_COMPILER
char *strupr(char *str)
{
char *strbase =str;
while(*str)
  {
  *str = toupper(*str);
  str++;
  }
return(strbase);
}

char * strrev(char *str)
{
register char *endstr,swapper;
char *strbase =str;

endstr = str;
while(*endstr) endstr++;
endstr--;

while ( endstr > str )
  {
  swapper = *str;
  *str++ = *endstr;
  *endstr-- = swapper;
  }

return(strbase);
}
#endif


char * getParam(char *arg,char *argv[],int *iPtr)
{
	while ( *arg == '-' || *arg == '/' || *arg == '=' )
	{
		arg++;
		arg = skipwhitespace(arg);
		if ( ! *arg )
		{
			(*iPtr) ++;
			arg = argv[*iPtr];
			if ( ! arg )
				return NULL;
		}
	}
return arg;
}


void expandStrEscapes(char *to,char *fm)
{
while(*fm)
	{
	if ( *fm == '\\' )
		{
		fm++;
		switch(*fm)
			{
			case 'n':	*to++ = '\n';	fm++; break;
			case 'r':	*to++ = '\r'; fm++;	break;
			case 't':	*to++ = '\t'; fm++; break;
			case '\\':	*to++ = '\\';	fm++; break;
			default:
				if ( isdigit(*fm) )
					{
					int v = 0;
					while( isdigit(*fm) && v < 26 )
						{
						v *= 10;
						v += *fm - '0' ; fm++;
						}
					*to++ = (v & 0xFF);
					}
				else
					{
					*to++ = '\\';
					*to++ = *fm++;
					}
				break;
			}
		}
	else
		{
		*to++ = *fm++;
		}
	}
*to = 0;
return;
}
