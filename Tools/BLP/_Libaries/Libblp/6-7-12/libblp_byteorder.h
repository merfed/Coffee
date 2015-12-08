/*
 *  libblp_byteorder.h
 *  libblp
 *
 *  Created by Jean-Francois Roy on Wed Nov 19 2003.
 *  Copyright (c) 2003 MacStorm. All rights reserved.
 *
 */

#if !defined(__LIBBLP_BYTEORDER__)
#define __LIBBLP_BYTEORDER__ 1

#ifdef __BIG_ENDIAN__
#ifdef __APPLE__

#include <CoreFoundation/CFByteOrder.h>

#define _blp_swap_int16(x) CFSwapInt16(x)
#define _blp_swap_int32(x) CFSwapInt32(x)

#else
#error unsupported operating system
#endif

#else
	
#define _blp_swap_int16(x) (x)
#define _blp_swap_int32(x) (x)

#endif

#endif // __LIBBLP_BYTEORDER__
