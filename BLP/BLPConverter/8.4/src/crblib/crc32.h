#ifndef CRB_CRC32_H
#define CRB_CRC32_H

#define crc32 crcbuf /* old name */

extern ulong crcbuf(const ubyte * buf,int buflen);

#define CRC_START(crc)  crc = 0xFFFFFFFF
#define CRC_FINISH(crc) crc ^= 0xFFFFFFFF

#define CRC_ADDB(crc,x)	crc = addcrcb(crc,(ubyte)x)
#define CRC_ADDW(crc,x)	crc = addcrcw(crc,(uword)x)
#define CRC_ADDL(crc,x)	crc = addcrcl(crc,(ulong)x)
#define CRC_ADDStr(crc,x)	crc = addcrcstr(crc,(ubyte *)x)

extern ulong addcrcb(ulong crc,ubyte b);
extern ulong addcrcw(ulong crc,uword w);
extern ulong addcrcl(ulong crc,ulong l);
extern ulong addcrcstr(ulong crc,ubyte *str);

/* for time_t and size_t and clock_t lovers: */
#define crc_t ulong

#endif /* CRB_CRC32 */
