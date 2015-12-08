#ifndef _SORT_NODES_H
#define _SORT_NODES_H

#include <crblib/inc.h>

typedef struct SortNodeStruct SortNode;

struct SortNodeStruct
  {
  SortNode * Next;
  ulong Index;
  ubyte * MoreIndexPtr;
  void * Data;
  };

/** use these to make and free nodes for sorting null-terminated string **/
/* set and free ->Data by yourself */

extern void Sort_FreeStringNode(SortNode * N);
extern void Sort_FreeStringList(SortNode * N);
extern SortNode * Sort_MakeStringNode(const char * String);

/** pass a linked list in *BasePtr , returns *BasePtr to the same list
  in sorted order ; 
    totally changes all ->Next values to make the new list
     **/

extern bool Sort_List(SortNode ** BasePtr);

/** sorts Array of Num Nodes **/

extern bool Sort_Array(SortNode ** Array,int Num);

extern bool Sort_Array_clib(SortNode ** Array,int ArraySize);

/** _clib version uses c-library qsort() **/

/**

Sort_Array uses a QuickSort , preceeded by a RadixSort whenever NumMember
is large enough to make a Radix pre-pass favorable

----

->Next and ->Data are never touched by Sort_Array

----

comparisons will operate on 'Index' ; if two nodes have the same 'Index'
then 'MoreIndexPtr' will be consulted to differentiate.

Warning : compares of 'MoreIndexPtr' will proceed until strings
are differentiated!  If you wish to terminate compares early, back
the end of 'MoreIndexPtr' with a differentiating ID, such as the
position of the data in the array.

For example, to sort the strings:

"aa"
"bb"
"bb"
"cc"

You would pack in:

"aa0"
"bb1"
"bb2"
"cc3"

If your OS doesn't mind wild-reads (which don't hurt anything),
then you may ignore this step, and no bad side effects will occur
(except perhaps slowdown)

**/

#endif
