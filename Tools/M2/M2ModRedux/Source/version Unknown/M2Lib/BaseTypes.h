#pragma once

typedef unsigned char UInt8;
typedef signed char SInt8;
typedef unsigned short UInt16;
typedef signed short SInt16;
typedef unsigned int UInt32;
typedef signed int SInt32;
typedef float Float32;

typedef char Char8;
typedef wchar_t Char16;

#ifndef MakeFourCC
#define MakeFourCC(ch0, ch1, ch2, ch3) ((UInt32)(UInt8)(ch0) | ((UInt32)(UInt8)(ch1) << 8) | ((UInt32)(UInt8)(ch2) << 16) | ((UInt32)(UInt8)(ch3) << 24 ))
#endif
