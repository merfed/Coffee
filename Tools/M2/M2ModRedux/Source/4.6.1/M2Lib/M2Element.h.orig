#pragma once

#include "BaseTypes.h"
#include <fstream>

namespace M2Lib
{

class M2Element
{
public:
	UInt32 Count;			// number of sub-elements contained in Data. the definition of this structure depends on this element's usage.
	UInt32 Offset;			// offset in bytes from begining of file to where this element's data begins.
	UInt32 OffsetOriginal;	// offset of this element as loaded from the original file.

	SInt32 DataSize;		// size in bytes of Data.
	UInt8* Data;			// our local copy of data. note that DataSize might be greater than sizeof( DataType ) * Count if there is padding at the end.

	SInt32 Align;			// byte alignment boundary. M2s pad the ends of elements with zeros data so they align on 16 byte boundaries.

public:
	M2Element();
	~M2Element();

	// given a global offset, returns a pointer to the data contained in this Element.
	// asserts if GlobalOffset lies outside of this element.
	void* GetLocalPointer( UInt32 GlobalOffset );

	// loads this element's data from a file stream. assumes that Offset and DataSize have already been set.
	bool Load( std::fstream& FileStream );
	// saves this element's data to a file stream. assumes that Offset and DataSize have already been set.
	bool Save( std::fstream& FileStream );

	// reallocates Data, either erasing existing data or preserving it.
	// adds padding to NewDataSize if necessary so that new size aligns with Align.
	void SetDataSize( UInt32 NewCount, UInt32 NewDataSize, bool CopyOldData );

	// clones this element from Source to Destination.
	static void Clone( M2Element* Source, M2Element* Destination );

};

}
