#include "CM2Element.h"




bool M2Lib::CM2Element::Load( std::fstream& FileStream )
{
	if ( !DataSize )
		return true;

	Data = new UInt8[DataSize];
	FileStream.seekg( Offset, std::ios::beg );
	FileStream.read( (Char8*)Data, DataSize );

	return true;
}


bool M2Lib::CM2Element::Save( std::fstream& FileStream )
{
	if ( !DataSize )
		return true;

	FileStream.seekp( Offset );
	FileStream.write( (Char8*)Data, DataSize );

	//UInt8 Zero = 0;
	//UInt32 Padding = GetPadding();
	//while ( Padding )
	//{
	//	FileStream.write( (Char8*)&Zero, 1 );
	//	Padding--;
	//}

	return true;
}


void M2Lib::CM2Element::SetDataSize( UInt32 NewNumber, UInt32 NewDataSize, bool CopyOldData )
{
	if ( NewDataSize <= DataSize )
		return;

	if ( Align != 0 )
	{
		UInt32 Mod = NewDataSize % Align;
		if ( Mod )
		{
			NewDataSize += Align - Mod;
		}
	}

	UInt8* NewData = new UInt8[NewDataSize];
	if ( Data )
	{
		if ( CopyOldData )
		{
			memset( &NewData[DataSize], 0, NewDataSize - DataSize );
			memcpy( NewData, Data, DataSize );
		}
		else
		{
			memset( NewData, 0, NewDataSize );
		}
		delete[] Data;
	}
	Data = NewData;
	DataSize = NewDataSize;
	Number = NewNumber;
}


void M2Lib::CM2Element::Clone( CM2Element* pOut, CM2Element* pIn )
{
	pOut->SetDataSize( pIn->Number, pIn->DataSize, false );
	memcpy( pOut->Data, pIn->Data, pIn->DataSize );
}
