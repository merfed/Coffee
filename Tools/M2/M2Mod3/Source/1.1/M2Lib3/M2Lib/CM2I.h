#pragma once

#include <vector>
#include "BaseTypes.h"
#include "M2Types.h"
#include "CM2Skin.h"


namespace M2Lib
{




class CM2I
{
public:
	class CSubset
	{
	public:
		// this subset's ID.
		UInt32 ID;

		// vertices that make up this subset, as indices into the global vertex list.
		std::vector< UInt16 > Indices;
		// triangles that make up this subset, as indices into the global vertex list.
		std::vector< M2Lib::CTriangle > Triangles;

		UInt16 RootBone;

		std::vector< M2Lib::CM2Skin::CMaterial > Materials;

	};


public:
	// the global vertex list
	std::vector< M2Lib::CVertex > VertexList;

	// list of subsets in this M2I.
	std::vector< CSubset* > Subsets;


public:
	CM2I()
	{
	}

	~CM2I()
	{
		for ( UInt32 i = 0; i < Subsets.size(); i++ )
		{
			delete Subsets[i];
		}
	}


public:


};




}