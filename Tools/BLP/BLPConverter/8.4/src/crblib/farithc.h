#ifndef FILEARITH_H
#define FILEARITH_H

#include <crblib/fbitio.h>

/*
 * Notez:
 *
 *  "totrange" must be <= FileAI->FileArithCumProbMax at all times !!
 *
 */

/*
 *
 * you must call FileBitIO_InitRead & FileBitIO_FlushWrite yourself!
 *
 */

struct FileAI
  {
  long FileArithCumProbMax;
  long FileArithCumProbMaxSafe; /* 256 less than FileArithCumProbMax */

  long Private;
  };

extern struct FileAI * FileArithInit(struct FileBitIOInfo * BII);
extern void FileArithCleanUp(struct FileAI * FileAI);

extern void FileArithEncodeCInit(struct FileAI * FileAI);
extern void FileArithEncodeRange(struct FileAI * FileAI,long low,long high,long totrange);
extern void FileArithEncodeCDone(struct FileAI * FileAI);

extern void FileArithDecodeCInit(struct FileAI * FileAI);
extern ulong FileArithDecodeTarget(struct FileAI * FileAI,ulong Tot);
extern void FileArithDecodeRange(struct FileAI * FileAI,long *target,long totrange);
extern void FileArithDecodeRangeRemove(struct FileAI * FileAI,long low,long high,long totrange);
extern void FileArithDecodeCDone(struct FileAI * FileAI);

extern void FileArithDecodeCDone_Safe(struct FileAI * FileAI);
extern void FileArithDecodeCDone_Min(struct FileAI * FileAI);

  /** _Safe is used right now
   *  _Min assumes the decoder will be supplied with trailing zeros
   *    this is not so easy to actually do without a speed-hit to FileBitIO
   *    but does give us an optimal limit of arithcoder efficiency
   **/

#endif
