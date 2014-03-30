#ifndef CINDCATR_H
#define CINDCATR_H

extern void TheCompressionIndicator(ulong UnPackedLen,ulong PackedLen,FILE * TargetF);
extern void TheDecompressionIndicator(ulong UnPackedLen,ulong PackedLen,FILE * TargetF);

extern void TheCompressionIndicatorMin(ulong UnPackedLen,ulong PackedLen,FILE *ToFile);

#endif
