#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <crblib/inc.h>
#include <crblib/strutil.h>

/*

% for binary (1s and 0s)
0x or $ for hexadecimal
digits for decimal

    "0x4" or "$4" or "%100" or "4" are all equal
    "0xF" or "$F" or "%1111" or "15"

all routines work on signed longs

*/

void ltoh(char *str,ulong ULV);
ulong htol(char * str);

long ConvStrToNumStepping(char *str,char **endstrptr)
{
long ret;

if ( *str == '%' )
  {
  ret = 0;  
  str++;

  while( *str=='0' || *str=='1' ) 
    {
    ret <<= 1;
    ret += (*str++ - '0');
    }
  }
else if ( (*str == '0' && ((*(str+1) == 'x')||(*(str+1) == 'X'))) )
  {
  str+=2;
  ret = (long)htol(str);

  while ( isxdigit(*str) ) str++;
  }
else if ( *str == '$' )
  {
  str++;
  ret = (long)htol(str);

  while ( isxdigit(*str) ) str++;
  }
else if ( isdigit(*str) )
  {
  ret = atol(str);

  if ( *str == '-' ) str++;
  while ( isdigit(*str) ) str++;
  }
else
  {
  ret = 0;
  }

if ( endstrptr ) *endstrptr = str;
return(ret);
}

long ConvStrToNum(char *str)
{
return(ConvStrToNumStepping(str,NULL));
}

/*Type values:*/
#define CS_DECIMAL  0
#define CS_HEX      1
#define CS_BINARY   2

void NumToConvStr(long num,char *out,ubyte type)
{
char temp[40];

switch(type)
  {
  case CS_DECIMAL:
    sprintf(out,"%ld",num);
    break;
  case CS_HEX:
    ltoh(out,(ulong)num);
    break;
  case CS_BINARY:
    {
    register ulong i,j,n;

    n = (ulong)num;

    j = 0;
    for(i=0;(i<32) && n;i++)
      {
      if ( (i%4)==0 && i != 0 ) temp[j++]=',';

      if (n & (1<<i)) { n ^= (1<<i); temp[j++]='1'; }
      else temp[j++]='0';
      }
    if ( j == 0 ) temp[j++]='0';
    while( i%4 ) { temp[j++]='0'; i++; }

    if ( temp[j-1] == ',' ) temp[j-1]=0;
    else temp[j]=0;

    strrev(temp); /*lowest is on right*/
    break;
    }
  }

strins(out,temp);
}

void ConvAll(char *str) /*converts all non-dec. nums to dec*/
{
char * endstr;
char * insert;
long num;

while(*str) {

	if ( ! (*str == '0' && ((*(str+1) == 'x')||(*(str+1) == 'X'))) )
		while ( *str == '.' || isdigit(*str) )
			str++;

  num = ConvStrToNumStepping(str,&endstr);
  if ( endstr == str )
    {
    str++;
    }
  else
    {
    while( str < endstr ) *str++ = ' ';
    insert = str - 1;
    if ( num == 0 ) *insert = '0';
    while(num)
      {
      *insert-- = '0' + (char)(num%10);
      num /= 10;
      }
    }
  }

}

ulong htol(char * str)
{
ulong ret;
char c;

ret = 0;
for(;;)
  {
  c = *str++;
  
  if ( isdigit(c) )
    { ret <<= 4; ret += c - '0'; }
  else if ( islower(c) )
    { ret <<= 4; ret += c - 'a' + 10; }
  else if ( isupper(c) )
    { ret <<= 4; ret += c - 'A' + 10; }
  else
    return(ret);
  }

return(0);
}

void ltoh(char *StrPtr,ulong ULV)
{
if ( ULV <= 0xFF )
  {
  sprintf(StrPtr,"0x%02X",ULV);
  }
else if ( ULV <= 0xFFFF )
  {
  sprintf(StrPtr,"0x%04X",ULV);
  }
else if ( ULV <= 0xFFFFFF )
  {
  sprintf(StrPtr,"0x%06X",ULV);
  }
else
  {
  sprintf(StrPtr,"0x%08X",ULV);
  }
}
