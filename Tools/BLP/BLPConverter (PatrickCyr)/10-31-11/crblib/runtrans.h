#ifndef CRB_RUN_TRANSFORM_H
#define CRB_RUN_TRANSFORM_H

/** RunPack does a full arith on the runlengths ; it creates a literal array
*	with no run-flags in it; this is optimal for post-coding of the literals
*  RunPack : Array[ArrayLen] -> Literals[NumLiterals] + PackedRuns[NumRuns,PackedLen]
* UnRunpack: Literals + PackedRuns -> Array[ArrayLen]
*
******/

extern bool RunPack(ubyte *Array,ulong ArrayLen,ubyte * Literals,
  ulong * NumLiteralsPtr,ubyte * PackedRunArray,ulong * NumRunsPtr,
  ulong * RunPackedLenPtr);

extern bool UnRunPack(ubyte *Array,ulong ArrayLen,ubyte * Literals,
  ubyte * PackedRunArray);

/**
* 
* runTransform is a ubyte <-> ubyte reversible transform
*
* both return the len of the comp array
*
*****/

extern int doRunTransform(ubyte *raw,int rawLen,ubyte *comp);
extern int unRunTransform(ubyte *raw,int rawLen,ubyte *comp);

#endif

