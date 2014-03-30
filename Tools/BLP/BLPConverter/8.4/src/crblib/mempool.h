#ifndef CRB_MEMPOOL_H
#define CRB_MEMPOOL_H

#include <crblib/inc.h>

struct MemBlock
{
  struct MemBlock * Next;
  char * MemBase;
  char * MemPtr;
  long MemLength;
  long MemFree;
};

typedef struct MemPoolStruct
{
  long HunkLength;
  struct MemBlock * CurMemBlock;
  struct MemBlock * MemBlock;
  long AutoExtendNumItems;
  long NumFreedHunks;
  long MaxNumFreedHunks;
  void ** FreedHunks;
  long NumItemsActive;
} MemPool;

// cool autoPool functions for really lazy people;
void * AutoPool_GetHunk( MemPool ** pPool,int * pNumHunks, int HunkSize);
void   AutoPool_FreeHunk(MemPool ** pPool,int * pNumHunks, void * Hunk);
void   AutoPool_Destroy(MemPool ** pPool,int * pNumHunks);

extern MemPool * AllocPool(long HunkLength,long NumHunks,long AutoExtendNumItems);
extern bool ExtendPool(MemPool * Pool,long NumHunks);
extern void FreePool(MemPool * Pool); /*ok to call this with Pool == NULL*/
extern void ResetPool(MemPool * Pool);
extern void * GetPoolHunk(MemPool * Pool,bool AutoExtendFlag);
extern bool FreePoolHunk(MemPool * Pool,void *Hunk);

 /* NOTEZ: GetPool clears the memory block to zeros*/

#define grabPool(x) GetPoolHunk(x,true)

#define MemPool_Create AllocPool
#define MemPool_Extend ExtendPool
#define MemPool_Destroy(p) FreePool(p)
#define MemPool_Reset ResetPool
#define MemPool_GetHunk(p) GetPoolHunk(p,true)
#define MemPool_FreeHunk FreePoolHunk

#endif /*CRB_MEMPOOL_H*/
