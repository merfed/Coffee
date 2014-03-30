#pragma once

#include "BaseTypes.h"
#include <stdio.h>

namespace M2Lib
{




class CBinaryStream
{
private:
	std::fstream* m_pFStream;
	SInt32 m_Length;


public:
	CBinaryStream( std::fstream* pFStream )
		: m_pFStream( pFStream )
	{
		UInt32 g = m_pFStream->tellg();
		m_pFStream->seekg( 0, std::ios::end );
		m_Length = m_pFStream->tellg();
		m_pFStream->seekg( g, std::ios::beg );
	}


	void Read( void* pBuffer, UInt32 Size )
	{
		if ( m_pFStream->tellg() >= m_Length )
		{
			int pause = 1;
		}
		m_pFStream->read( (char*)pBuffer, Size );
	}


	void Write( void* pBuffer, UInt32 Size )
	{
		m_pFStream->write( (char*)pBuffer, Size );
	}

};




}	// end namespace M2Lib
