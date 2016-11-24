#pragma once

#include "BaseTypes.h"
#include <fstream>

namespace M2Lib
{


enum EEndianness
{
	EEndianness_Little,
	EEndianness_Big,
	EEndianness_Native,
};


class DataBinary
{
private:
	std::fstream* _Stream;
	EEndianness _Endianness;
	EEndianness _EndiannessNative;

	static void _SwitchEndianness( void* Data, UInt8 Size );
	void _Read( void* Data, UInt32 Size );
	void _Write( void* Data, UInt32 Size );

public:
	DataBinary( std::fstream* Stream, EEndianness Endianness );
	~DataBinary();

	void SwitchEndiannessIfNeeded( void* Data, UInt8 Size );

	std::fstream* GetStream();
	void SetStream( std::fstream* Stream );
	
	EEndianness GetEndianness();
	void SetEndianness( EEndianness Endianness );

	UInt32 ReadUInt32();
	SInt32 ReadSInt32();
	UInt16 ReadUInt16();
	SInt16 ReadSInt16();
	UInt8 ReadUInt8();
	SInt8 ReadSInt8();
	Float32 ReadFloat32();
	Char16 ReadChar16();
	Char8 ReadChar8();
	UInt32 ReadFourCC();

	void WriteUInt32( UInt32 Value );
	void WriteSInt32( SInt32 Value );
	void WriteUInt16( UInt16 Value );
	void WriteSInt16( SInt16 Value );
	void WriteUInt8( UInt8 Value );
	void WriteSInt8( SInt8 Value );
	void WriteFloat32( Float32 Value );
	void WriteChar16( Char16 Value );
	void WriteChar8( Char8 Value );
	void WriteFourCC( UInt32 Value );

};


}
