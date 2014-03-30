#ifndef CODEUTIL_H
#define CODEUTIL_H

/** some common coding routines for variable-length integers ;

	all routines suffixed in three flavors :

		_byte : writes to an array
		_bii  : writes to a struct LBitIOInfo
		_ari  : writes to an arithInfo

	all routines prefixed by cu_

***/

#include <crblib/bbitio.h>
#include <crblib/arithc.h>
#include <crblib/lbitio.h>

void  cu_putEscaping_byte(ulong val,ubyte **stream);	/** escape of 0xFF **/
ulong cu_getEscaping_byte(ubyte **stream);	/** stream is moved ahead **/

void  cu_putEscapingSigned_byte(long val,ubyte **stream);	/** escape of 0xFF **/
long  cu_getEscapingSigned_byte(ubyte **stream);	/** stream is moved ahead **/

void  cu_putExpanding_byte(ulong val,ubyte **stream);	/** escape of 0xFF **/
ulong cu_getExpanding_byte(ubyte **stream);	/** stream is moved ahead **/

void  cu_putExpandingSigned_byte(long val,ubyte **stream);	/** escape of 0xFF **/
long  cu_getExpandingSigned_byte(ubyte **stream);	/** stream is moved ahead **/

void  cu_putEscaping_bii(ulong val,struct LBitIOInfo *stream,int escape_bits);	/** escape of (1<<escape_bits) **/
ulong cu_getEscaping_bii(struct LBitIOInfo *stream,int escape_bits);

void  cu_putEscaping_ari(ulong val,arithInfo *stream,ulong escape);	/** escape of (1<<escape_bits) **/
ulong cu_getEscaping_ari(arithInfo *stream,ulong escape);

void  cu_putExpanding_bii(ulong val,struct LBitIOInfo *stream,int init_bits,int step_bits);	/** escape of (1<<escape_bits) **/
ulong cu_getExpanding_bii(struct LBitIOInfo *stream,int init_bits,int step_bits);

void  cu_putExpanding_ari(ulong val,arithInfo *stream,int init_max,int step_max);
ulong cu_getExpanding_ari(arithInfo *stream,int init_max,int step_max);

void  cu_putMulting_ari(ulong val,arithInfo *stream,int init_max,int step_mult);
ulong cu_getMulting_ari(arithInfo *stream,int init_max,int step_mult);

void cu_putExpandingSigned_bii(int val,struct LBitIOInfo *stream,int init_bits,int step_bits);	/** escape of (1<<escape_bits) **/
int  cu_getExpandingSigned_bii(struct LBitIOInfo *stream,int init_bits,int step_bits);

void cu_putExpandingSigned_ari(int val,arithInfo *stream,int init_max,int step_max);
int  cu_getExpandingSigned_ari(arithInfo *stream,int init_max,int step_max);

void cu_putMultingSigned_ari(int val,arithInfo *stream,int init_max,int step_mult);
int  cu_getMultingSigned_ari(arithInfo *stream,int init_max,int step_mult);

#endif // CODEUTIL_H

