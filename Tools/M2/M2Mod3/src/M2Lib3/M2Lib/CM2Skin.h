#pragma once

#include "BaseTypes.h"
#include "EError.h"
#include "CM2Element.h"
#include <fstream>
#include <vector>


namespace M2Lib
{




class CM2;
class CM2Skin
{
public:
	enum EElement
	{
		EElement_Indices,
		EElement_Triangles,
		EElement_Properties,
		EElement_Subsets,
		EElement_Materials,

		EElement__Count__
	};

	#pragma pack(push,1)
	// memory mapped header
	class CM2SkinHeader
	{
	public:
		Char8 ID[4];	// 'SKIN'

		UInt32 nIndices;
		UInt32 oIndices;

		UInt32 nTriangles;
		UInt32 oTriangles;

		UInt32 nProperites;
		UInt32 oProperties;

		UInt32 nSubsets;
		UInt32 oSubsets;

		UInt32 nMaterials;
		UInt32 oMaterials;

		UInt32 LevelOfDetail;

	};

	// actually, more like a subset partition, as multiple entries of these with same ID may exist.
	class CSubset
	{
	public:
		UInt32 ID;					// subset ID.
		UInt16 IndexStart;			// first vertex as index into skin's index list.
		UInt16 IndexCount;			// number of indices into the skin's index list.
		UInt16 TriangleStart;		// first triangle as index into skin's triangle list, which if you want the real triangle index, divide by 3.
		UInt16 TriangleCount;		// length of triangle list for this subset, which if you want the real triangle count, divide by 3.
		UInt16 BoneCount;			// number of bones to upload to GPU shader constant registers.
		UInt16 BoneStart;			// first bone in bone lookup list to begin upload to GPU shader constant registers.
		UInt16 MaxBones;			// max bones used per vertex? always set from 1 through 4.
		UInt16 RootBone;			// a sort of 5th bone that all vertices in this sub mesh get transformed by if all their bone indices are set to 0 i think? no idea.
		Float32 CenterMass[3];			// average position of all vertices in this subset. found by summing positions of all vertices and then dividing by the number of vertices.
		Float32 CenterBoundingBox[3];	// bounding box center. if we make a minimum axis aligned bounding box around the set of vertices in this subset and get the center of that box, this is the result.
		Float32 Radius;					// this is the distance of the vertex farthest from CenterBoundingBox.
	};

	// each subset has at least one of these, defines a texture, shader, render flags, etc. basically a render state and texture stage/unit state to use when drawing the subset. multiple instances of these can be defined per subset, enabling multi-texturing.
	class CMaterial
	{
	public:
		UInt16 Flags1;					// flags
		UInt16 Flags2;					// flags
		UInt16 iSubset;					// the subset index that this material is assigned to. 
		UInt16 iSubset2;				// same as above, don't know why this is repeated
		SInt16 iColor;					// index into the color list
		UInt16 iRenderFlags;			// 
		UInt16 iUnit;					// for subsets that have multiple material layers assigned to them
		UInt16 iShader;					// shader to use for this texture stage
		UInt16 iTexture;				// index into the texture list
		UInt16 iTexutreUnit;			// index into the texture unit list
		UInt16 iTransparency;			// index into the transparency list
		UInt16 iTextureAnimation;		// index into the texture animation list
	};

	#pragma pack(pop)


public:
	CM2Skin( M2Lib::CM2* pM2In )
		: pM2( pM2In )
	{
	}

	~CM2Skin()
	{
	}


public:
	Char8 m_szFile[1024];

	CM2SkinHeader Header;
	CM2Element Elements[5];

	// pointer to M2 that this skin belongs to.
	CM2* pM2;


public:
	M2Lib::EError Load( Char8* szFile );
	M2Lib::EError Save( Char8* szFile );

	void BuildVertexBoneIndices();
	void BuildBoundingData();
	void BuildMaxBones();

	bool PrintInfo( void* Vertices, void* Bones );

	CSubset* GetSubset( UInt32 ID );


private:
	void m_LoadElements_CopyHeaderToElements();
	void m_LoadElements_FindSizes( UInt32 FileSize );
	void m_SaveElements_FindOffsets();
	void m_SaveElements_CopyElementsToHeader();

	static SInt32 m_ReverseBoneLookup( UInt8 BoneID, UInt16* BoneLookupTable, UInt32 BoneLookupTableLength )
	{
		if ( BoneID == 0 )
			return 0;

		for ( UInt32 i = 0; i < BoneLookupTableLength; i++ )
		{
			if ( BoneLookupTable[i] == BoneID )
				return i;
		}

		return 0;
	}

};




}
