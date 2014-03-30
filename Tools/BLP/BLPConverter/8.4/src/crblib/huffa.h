#ifndef CRB_HUFFA_H
#define CRB_HUFFA_H

/** 

	HuffArray:

	calls O0Huff and O1Huff and uses best of two

	returns true = success
				false = failure
	CompressFlag = true to pack , false to unpack
**/

extern bool HuffArray(ubyte *RawArray,ulong RawFileLen,
					 ubyte *HuffArray,ulong * HuffArrayLenPtr,int Type);

#define HUFFA_TYPE_BEST -1
#define HUFFA_TYPE_DEC  0 /* 0 for backwards compatibility */
#define HUFFA_TYPE_O1   1
#define HUFFA_TYPE_O0   2
#define HUFFA_TYPE_O0NB 3 /* no blocking */
#define HUFFA_TYPE_NONE 4

/** you should always use HuffArray with TYPE setting, instead of
the below.  They are provided as APIs just for completeness **/

extern bool O0HuffArray(ubyte *RawArray,ulong RawFileLen,
					 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag);

extern bool O0HuffArrayNoBlock(ubyte *RawArray,ulong RawFileLen,
					 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag);

extern bool O1HuffArray(ubyte *RawArray,ulong RawFileLen,
					 ubyte *HuffArray,ulong * HuffArrayLenPtr,bool CompressFlag);

extern bool O0HuffArrayBII_RT(ubyte *rawArray,ulong rawLen,struct LBitIOInfo * BII,bool cFlag);
extern bool O0HuffArrayBII(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag);
extern bool O0HuffArrayBII_block(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag);
extern bool O0HuffArrayBII_noblock(ubyte *RawArray,ulong RawLen,struct LBitIOInfo * BII,bool CompressFlag);

#endif

