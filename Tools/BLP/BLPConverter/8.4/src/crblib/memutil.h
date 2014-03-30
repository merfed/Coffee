#ifndef CRB_MEMUTIL_H
#define CRB_MEMUTIL_H

#include <crblib/inc.h>

#define cleararray(buf,type,cnt) memclear(buf,sizeof(type)*(cnt))

#define getulong(bptr) ( ((((ubyte *)(bptr))[0])<<24) + (((ubyte *)(bptr))[1]<<16) + (((ubyte *)(bptr))[2]<<8) + (((ubyte *)(bptr))[3]) )
#define getuword(bptr) ( (((ubyte *)(bptr))[0]<<8) + (((ubyte *)(bptr))[1]) )

#define new(dtype) calloc(1,sizeof(dtype))
#define newarray(dtype,num) calloc(num,sizeof(dtype))
#define copy(from,to,dtype)	memcpy(to,from,sizeof(dtype))
#define objcpy(to,from,dtype) copy(from,to,dtype)

#define memsame(a,b,c) (memcmp(a,b,c)==0)

#define smartfree(m) if ( (m) == NULL ) ; else { free((void *)m); m = NULL; }
#define SmartFree(m,s) smartfree(m)
#define destroy(m) smartfree(m)

/**

one advantage of AllocMem is that it doesn't return
error for a length-of-zero call

**/

#define MEMF_ANY	0
#define MEMF_FAST	MEMF_ANY
#define MEMF_CHIP	MEMF_ANY
#define MEMF_CLEAR	1

#ifndef APTR
#define APTR voidptr
#endif

extern APTR AllocMem(size_t size,int MemFlags);
#define FreeMem(mem,len) free(mem)

extern void MemClear(void *P,size_t len);
extern void MemClearLongs(void *P,size_t num_longs);

extern void MemCpy(void *To,void *Fm,size_t len);
extern void MemCpyLongs(void *To,void *fm,size_t num_longs);

/** the "Longs" routines take as 'len' the Number of ULONGS, NOT the number
**** of bytes **/

extern int whichByteDiff(ubyte *buf1,ubyte *buf2);

#define MemCpyFast MemCpyLongs
#define MemClearFast MemClearLongs

#define MemClearMacro(a,b) MemClear(a,b)
#define MemCpyMacro(a,b,c) MemCpy(a,b,c)
#define MemClearMacroFast(a,b) MemClearLongs(a,b)
#define MemCpyMacroFast(a,b,c) MemCpyFast(a,b,c)

#ifndef memclear
#define memclear MemClear
#endif

#endif /* _MEMUTIL_H */

