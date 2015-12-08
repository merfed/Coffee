#include <crblib/inc.h>
#include <crblib/memutil.h>

static ulong NullVall = 0; /* should never be touched */

int whichByteDiff(ubyte *buf1,ubyte *buf2)
{
int b = 0;
	while( buf1[b] == buf2[b] )
		b++;
return b;
}

APTR AllocMem(size_t size,int MemFlags)
{
void * ret;
if ( size == 0 ) return( malloc(4) );
size = PaddedSize(size);
if ( (ret = malloc(size)) == NULL ) return(NULL);
if ( (MemFlags&MEMF_CLEAR) ) MemClearLongs(ret,size>>2);
return((APTR)ret);
}

void MemCpy(void *To,void *Fm,size_t len)
{
register int L;

if ((len&3)==0)
  {
  L = len>>2;
  
  if ( (((ulong)Fm)&3) == 0 && (((ulong)To)&3) == 0 )
    {
    register ulong *FmP,*ToP;
    
    FmP = Fm; ToP = To;
    
    while(L--) *ToP++ = *FmP++;
    }
  else
    {
    register ubyte *FmP,*ToP;
    
    FmP = Fm; ToP = To;
    
    while(L--)
      { *ToP++ = *FmP++; *ToP++ = *FmP++; *ToP++ = *FmP++; *ToP++ = *FmP++; }
    }
  }
else if ((len&1)==0)
  {
  register ubyte *FmP,*ToP;
  FmP = Fm; ToP = To;

  L = len>>1;
  while(L--)
    {
    *ToP++ = *FmP++;
    *ToP++ = *FmP++;
    }
  }
else
  {
  register ubyte *FmP,*ToP;
  FmP = Fm; ToP = To;

  L = len;
  while(L--) *ToP++ = *FmP++;
  }
}

void MemClear(void *P,size_t len)
{
memset(P,0,len);
/***
register ubyte *Ptr;
register int L,X;

Ptr = P;
X = len&3;
L = len>>2;

  while(L--) {
    *Ptr++ = 0;
    *Ptr++ = 0;
    *Ptr++ = 0;
    *Ptr++ = 0;
	}

  while(X--) {
    *Ptr++ = 0;
  }
***/

}

void MemClearLongs(void *P,size_t num_longs)
{
memset(P,0,num_longs<<2);
/******
register int L = num_longs;

if ( (((ulong)P)&3) == 0 )	// if it's aligned
  {
  register ulong *Ptr = P;

  while(L--) *Ptr++ = 0;
  }
else
  {
  register ubyte *Ptr = P;

  while(L--) { *Ptr++ = 0; *Ptr++ = 0; *Ptr++ = 0; *Ptr++ = 0; }
  }
****/

}

void MemCpyLongs(void *To,void *Fm,size_t num_longs)
{
register int L = num_longs;

if ( (((ulong)Fm)&3) == 0 && (((ulong)To)&3) == 0 )
  {
  register ulong *FmP,*ToP;
  
  FmP = Fm; ToP = To;
  
  while(L--) *ToP++ = *FmP++;
  }
else
  {
  register ubyte *FmP,*ToP;
  
  FmP = Fm; ToP = To;
  
  while(L--)
    { *ToP++ = *FmP++; *ToP++ = *FmP++; *ToP++ = *FmP++; *ToP++ = *FmP++; }
  }
}
