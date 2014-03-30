#pragma once

#include "BaseTypes.h"
#include <fstream>
#include <assert.h>


namespace M2Lib
{




class CM2Element
{
public:
	UInt32 Number;		// number of sub-elements contained in Data. the definition of this structure depends on this element's usage.
	UInt32 Offset;		// offset in bytes from begining of file to where this element's data begins.
	UInt32 OffsetOriginal;	// offset of this element as loaded from the original file.

	UInt32 DataSize;	// size in bytes of Data.
	UInt8* Data;		// our copy of data. note that DataSize might be greater than sizeof( DataType ) * Number if there is padding at the end.

	UInt32 Align;		// byte alignment boundary. M2s pad the ends of elements with zeros data so they align on 16 byte boundaries.


public:
	CM2Element()
		: Number( 0 )
		, Offset( 0 )
		, DataSize( 0 )
		, Data( 0 )
		, Align( 16 )
	{
	}

	~CM2Element()
	{
		if ( Data )
		{
			delete[] Data;
		}
	}

	inline void* GetLocalPointer( UInt32 Position )
	{
		Position -= Offset;
		assert( Position < DataSize );
		return &Data[Position];
	}


public:
	bool Load( std::fstream& FileStream );
	bool Save( std::fstream& FileStream );

	// reallocates Data, either erasing existing data or preserving it.
	void SetDataSize( UInt32 NewNumber, UInt32 NewDataSize, bool CopyOldData );

	static void Clone( CM2Element* pOut, CM2Element* pIn );

};




}
