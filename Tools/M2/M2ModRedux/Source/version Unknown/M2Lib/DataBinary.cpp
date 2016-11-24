#include "DataBinary.h"
#include <assert.h>

void M2Lib::DataBinary::_SwitchEndianness( void* Data, UInt8 Size )
{
	if ( Size <= 1 )
	{
		return;
	}

	UInt8* DataOut = new UInt8[Size];
	UInt8* DataIn = (UInt8*)Data;
	for (SInt32 i = 0, out = Size - 1; i < Size; i++, out-- )
	{
		DataOut[out] = DataIn[i];
	}
	for (SInt32 i = 0; i < Size; i++)
	{
		DataIn[i] = DataOut[i];
	}
	delete[] DataOut;
}

void M2Lib::DataBinary::_Read( void* Data, UInt32 Size )
{
	_Stream->read( (char*)Data, Size );
	if ( _Endianness != _EndiannessNative )
	{
		_SwitchEndianness( Data, Size );
	}
}

void M2Lib::DataBinary::_Write( void* Data, UInt32 Size )
{
	if ( _Endianness != _EndiannessNative )
	{
		_SwitchEndianness( Data, Size );
	}
	_Stream->write( (const char*)Data, Size );
}

M2Lib::DataBinary::DataBinary( std::fstream* Stream, EEndianness Endianness )
{
	UInt8 EndianTest[2] = { 1, 0 };
	_EndiannessNative = ( (*(SInt16*)EndianTest == 1) ? EEndianness_Little : EEndianness_Big );

	assert( Stream );
	_Stream = Stream;
	_Endianness = Endianness;
}

M2Lib::DataBinary::~DataBinary()
{
}

void M2Lib::DataBinary::SwitchEndiannessIfNeeded( void* Data, UInt8 Size )
{
	if ( _Endianness != _EndiannessNative )
	{
		_SwitchEndianness( Data, Size );
	}
}

std::fstream* M2Lib::DataBinary::GetStream()
{
	return _Stream;
}

void M2Lib::DataBinary::SetStream( std::fstream* Stream )
{
	assert( Stream );
	_Stream = Stream;
}
	
M2Lib::EEndianness M2Lib::DataBinary::GetEndianness()
{
	return _Endianness;
}

void M2Lib::DataBinary::SetEndianness( EEndianness Endianness )
{
	_Endianness = Endianness;
}

UInt32 M2Lib::DataBinary::ReadUInt32()
{
	UInt32 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

SInt32 M2Lib::DataBinary::ReadSInt32()
{
	SInt32 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

UInt16 M2Lib::DataBinary::ReadUInt16()
{
	UInt16 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

SInt16 M2Lib::DataBinary::ReadSInt16()
{
	SInt16 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

UInt8 M2Lib::DataBinary::ReadUInt8()
{
	UInt8 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

SInt8 M2Lib::DataBinary::ReadSInt8()
{
	SInt8 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

Float32 M2Lib::DataBinary::ReadFloat32()
{
	Float32 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

Char16 M2Lib::DataBinary::ReadChar16()
{
	Char16 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

Char8 M2Lib::DataBinary::ReadChar8()
{
	Char8 Result;
	_Read( &Result, sizeof( Result ) );
	return Result;
}

UInt32 M2Lib::DataBinary::ReadFourCC()
{
	UInt32 Result = 0;
	Char8 CharA;
	Char8 CharB;
	Char8 CharC;
	Char8 CharD;
	_Read( &CharA, 1 );
	_Read( &CharB, 1 );
	_Read( &CharC, 1 );
	_Read( &CharD, 1 );
	return MakeFourCC( CharA, CharB, CharC, CharD );
}

void M2Lib::DataBinary::WriteUInt32( UInt32 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteSInt32( SInt32 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteUInt16( UInt16 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteSInt16( SInt16 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteUInt8( UInt8 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteSInt8( SInt8 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteFloat32( Float32 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteChar16( Char16 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteChar8( Char8 Value )
{
	_Write( &Value, sizeof( Value ) );
}

void M2Lib::DataBinary::WriteFourCC( UInt32 Value )
{
	Char8 CharA = ( Value );
	Char8 CharB = ( Value >> 8 );
	Char8 CharC = ( Value >> 16 );
	Char8 CharD = ( Value >> 24 );
	_Write( &CharA, 1 );
	_Write( &CharB, 1 );
	_Write( &CharC, 1 );
	_Write( &CharD, 1 );
}
