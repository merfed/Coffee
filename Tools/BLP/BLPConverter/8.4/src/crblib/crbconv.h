#ifndef _CRBEQCONV_H
#define _CRBEQCONV_H

#include <crblib/inc.h>

/*Type values:*/
#define CS_DECIMAL  0
#define CS_HEX      1
#define CS_BINARY   2

extern void NumToConvStr (long num,char *out,ubyte type);
extern long ConvStrToNum (char *str);
extern void ConvAll(char *str); /*converts all non-dec. nums to dec*/
extern void ltoh(char *str,ulong ULV);
extern ulong htol(char * str);

#endif
