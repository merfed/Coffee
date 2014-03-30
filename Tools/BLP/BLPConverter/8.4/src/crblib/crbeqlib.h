#ifndef _CRBEQLIB_H
#define _CRBEQLIB_H

#include <crblib/inc.h>

/* ** THE CRBEQ OPTIMIZING EQUATION COMPILER*/

/*
 * note : you may change only x,y,z,t,h variables.
 * H should always be a small value (1e-7) , you may set it if you wish
 */

typedef struct EqData CRBEQ;

struct EqData
  {
  double Result;
  double X,Y,Z,T,H;
  int ParseError;
  ubyte *ErrorMess;
  ubyte **DebugMess;

  double *UserVar; int NumUserVars;
  
  ubyte *EQ;          int EQSize;
  double *PreStored;  int PreStoredSize;
  double *Store;      int StoreSize;
  ubyte *StrEq;        int StrEqSize;
  ubyte *WrkBase;
  ubyte *WrkPtr;
  ubyte *RepPtr;
  int DebugMessNext;
  int Flags;
  int CurPreStoreNum;
  int CurStoreNum;  
  int CPos;
  int SLen;

  ubyte **Tokens;
  int TokNum;
  ubyte *TokWork;
  int TokWorkCnt;
  ubyte **OrdTerms;
  ubyte *OrdPri;
  ubyte *OrdWork;
  };

extern double		   ValStrEq(char *streq);
#define ValStrEq_Error HUGE_VAL

#define valstr ValStrEq

extern double           ValCRBEQ(CRBEQ *d);
extern CRBEQ * MakeCRBEQ(ubyte *StrEQ,int Flags);
extern void            FreeCRBEQ(CRBEQ *EqData);
extern int             CopyCRBEQ(CRBEQ *FromEqData,
                                CRBEQ **ToEqData);
extern void            HelpCRBEQ(void);

extern void PreParseDifferentials(char **strptr,int *strlptr);
/** warning : PPD may do a re-alloc of str if it runs out of space !
	pass address of alloced streq and addres of alloc len **/

/* 'Flags' values:*/
#define DEBUG    (1<<0) /*show lots of Debug info*/
#define OPTIMIZE (1<<1) /*optimize CRBEQ (post-parse optimizations)*/

#endif
