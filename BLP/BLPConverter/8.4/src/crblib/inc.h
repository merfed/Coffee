#ifndef CRB_INC_H
#define CRB_INC_H

/*types:*/
typedef unsigned int uint;
typedef unsigned long  ulong;
typedef unsigned short uword;
typedef unsigned char  ubyte;
typedef short word;
typedef ubyte byte;
#ifndef __cplusplus
typedef int bool;
#endif
typedef void * voidptr;

typedef void * (*voidpCBvoid) (void);
typedef void (*voidCBvoidp) (void *);

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <crblib/memutil.h>

#ifndef LONG_MAX
#define LONG_MAX  ((long)0x7FFFFFFF)  /* max value for signed long int  */
#endif

#ifndef ULONG_MAX
#define ULONG_MAX ((ulong)~((ulong)0))  /* max value for unsigned long int  */
#endif

#ifndef UWORD_MAX
#define UWORD_MAX ((uword)~((uword)0))
#endif


/*macros:*/

#ifndef true
#define true (1)
#endif

#ifndef false
#define false (0)
#endif

#ifndef yep
#define yep (1)
#endif

#ifndef nope
#define nope 0
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL (0)
#endif

#define sizeofpointer sizeof(voidptr)

#define DoublePaddedSize(a) ((((a)-1)/8 + 1)*8)
#define PaddedSize(a) ((((a)-1)/4 + 1)*4)
#define WordPaddedSize(a) ((((a)-1)/2 + 1)*2)

#define IsOdd(a)  ( ((ulong)a)&1 )
#define SignOf(a) (((a) < 0) ? -1 : 1)

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif


#define minmax(x,lo,hi) 	( (x)<(lo)?(lo):( (x)>(hi)?(hi):(x)) )
#define putminmax(x,lo,hi) 	x = minmax(x,lo,hi)
#define clamp(x,lo,hi)		putminmax(x,lo,hi)
#define max3(a,b,c) 	max(max(a,b),c)
#define max4(a,b,c,d) 	max(a,max3(b,c,d))
#define min3(a,b,c) 	min(min(a,b),c)
#define min4(a,b,c,d) 	min(a,min3(b,c,d))
#define clamp255(x)		( ( (x) & (~0xFF) ) ? minmax(x,0,255) : (x) )

#ifndef errputs
#define errputs(str) fprintf(stderr,"%s\n",str)
#endif

#ifndef errexit
#define errexit(str) if(0) ; else { errputs(str); exit(10); }
#endif

#ifndef ABS
#define ABS(i) ((i) < 0 ? -(i) : (i))
#endif

#define CLAMP(x,lo,hi)								( (x) < (lo) ? (lo) : ( (x) > (hi) ? (hi) : (x) ) )
#define CLAMP8(x)									CLAMP(x,0,255)
#define CLAMP16(x)									CLAMP(x,0,65536)
#define BOOLSAME(x,y)								( ( (x) && (y) ) || ( !(x) && !(y) ) )

#ifndef isneg	
// #define isneg(x) (((long)(x))>>31)	<- slower
#define isneg(x)	(x<0)	// the optimizer knows the tricks (like setge)
#endif

#ifndef ispow2
#define ispow2(x) (!( (x) & ~(-(x)) ))
#endif

#ifndef swap
#define swap(a,b) do { void * c; c = (void *)a; a = b; b = c; } while(0)
#endif

#ifndef swapf
#define swapf(a,b) do { float c; c = a; a = b; b = c; } while(0)
#endif

#ifndef swapints
#define swapints(a,b) do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while(0)
#endif

extern void dbf(void); //global debug func

#define compileAssert(exp)	extern char _compileAssertHack[ (exp) ? 1 : -1 ]

/****
#ifdef _MSC_VER
#pragma warning( disable : 4025 )	// overriding a cli option
#pragma warning( disable : 4090 )	// different const qualifiers
#pragma warning( disable : 4244 )	// loss of data in conversion
#pragma warning( disable : 4761 )	// integral size conversion
#pragma warning( disable : 4101 )	// unreffed local
#pragma warning( disable : 4133 )	// casting to not same struct
#endif // MSVC sucks
******/

/* }{ compiler dependent junk */

#ifdef _MSC_VER //{ MSVC

#define INLINE static __forceinline // __fastcall
#define REGCALL __fastcall

#else			//}{

#ifdef __svr4__	//{ UNIX

#define INLINE static inline
#define REGCALL register // I think ?

#else			//}{

// other compilers ?
// gcc uses 'inline', but I don't know how to detect gcc

#define INLINE
#define REGCALL

#endif			//}
#endif			//}


#endif /*CRB_INC_H*/

