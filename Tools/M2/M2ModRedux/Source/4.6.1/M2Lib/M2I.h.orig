#pragma once

#include <vector>
#include "BaseTypes.h"
#include "M2Types.h"
#include "M2Skin.h"


namespace M2Lib
{




class M2I
{
public:
	class CSubMesh
	{
	public:
		// this subset's ID.
		UInt32 ID;

		// vertices that make up this subset, as indices into the global vertex list.
		std::vector< UInt16 > Indices;
		// triangles that make up this subset, as indices into the global vertex list.
		std::vector< M2Lib::CTriangle > Triangles;

	};


public:
	// the global vertex list
	std::vector< M2Lib::CVertex > VertexList;

	// list of subsets in this M2I.
	std::vector< CSubMesh* > SubMeshList;


public:
	M2I()
	{
	}

	~M2I()
	{
		for ( UInt32 i = 0; i < SubMeshList.size(); i++ )
		{
			delete SubMeshList[i];
		}
	}


public:


};




}