#pragma once

#include "BaseTypes.h"
#include "EError.h"
#include "CM2Element.h"
#include <fstream>
#include <vector>


namespace M2Lib
{




class CVertex;
class CM2;
class CM2Skin
{
public:
	enum EElement
	{
		EElement_Vertex,
		EElement_Index,
		EElement_BoneIndices,
		EElement_Subset,
		EElement_Material,
		EElement_Flags,

		EElement__Count__
	};

	#pragma pack(push,1)
	// memory mapped header
	class CM2SkinHeader
	{
	public:
		Char8 ID[4];	// 'SKIN'

		UInt32 nVertex;
		UInt32 oVertex;

		UInt32 nIndex;
		UInt32 oIndex;

		UInt32 nBoneIndices;
		UInt32 oBoneIndices;

		UInt32 nSubset;
		UInt32 oSubset;

		UInt32 nMaterial;
		UInt32 oMaterial;

		UInt32 Unknown1;	// always 0. was bone LoD in v3.

		UInt32 nFlags;
		UInt32 oFlags;

		UInt32 Unknown2;	// always 0.
	};

	class CElement_Vertex
	{
	public:
		UInt16 GlobalVertexIndex;		// index into M2's global vertex list.
	};

	class CElement_Index
	{
	public:
		UInt32 VertexIndex;				// index into SKIN's vertex list.
	};

	class CElement_BoneIndices
	{
	public:
		union
		{
			UInt32 BoneIndicesPacked;
			UInt8 BoneIndices[4];
		};
	};

	// actually, more like a subset partition, as multiple entries of these with same ID may exist.
	class CElement_Subset
	{
	public:
		// would map to Unkown2, but i don't think these are really what it means.
		enum ECategory
		{
			// these guesses taken from blood elf model
			// these values and usages seem to differ from model to model
			ECategory_FatBelt = 2,			// fat belt
			ECategory_Tabard = 3,			// tabard, shirt frills
			ECategory_LegUpper = 5,			// upper leg, skirt, armored leg, pant sleeve
			ECategory_Cape = 10,			// capes, and back peice that is used when no cape is equipped
			ECategory_LegLower = 11,		// leg lower, boots, pant frill
			ECategory_Body1 = 19,			// main body mesh, all hair styles, head plugs that close ear holes and bald hair style
			ECategory_Body2 = 27,			// main body mesh, all hair styles, head plugs that close ear holes and bald hair style
			ECategory_EyeGlow_Race = 32,
			ECategory_EyeLid = 35,
			ECategory_EyeGlow_DK = 36,
			ECategory_Features = 39,		// earings, 
			ECategory_EyeBrow = 40,			// 
			ECategory_ArmLower = 46,		// arm lower, wrist, gloves, sleeve frill
			ECategory_Ear1 = 51,			// ears, a couple earrings
			ECategory_Ear2 = 52,			// earrings
			ECategory_Ear3 = 53,			// more earrings
			ECategory_Ear4 = 57,			// more earrings
			ECategory_Ear5 = 39,			// more earrings
		};

	public:
		UInt32 ID;						// subset ID.
		UInt16 VertexStart;				// first vertex as index into skin's index list.
		UInt16 VertexCount;				// number of indices into the skin's index list.
		UInt16 IndexStart;				// first triangle as index into skin's triangle list, which if you want the real triangle index, divide by 3.
		UInt16 IndexCount;				// length of triangle list for this subset, which if you want the real triangle count, divide by 3.
		UInt16 BoneCount;				// number of bones to upload to GPU shader constant registers.
		UInt16 BoneStart;				// first bone in bone lookup list to begin upload to GPU shader constant registers.
		UInt16 Unknown1;				// always set from 0 to 4. seems not to do anything.
		UInt16 Unknown2;				// what category the submesh is. seems not to do anything.
		Float32 CenterMass[3];			// average position of all vertices in this subset. found by summing positions of all vertices and then dividing by the number of vertices.
		Float32 CenterBounds[3];		// bounding box center. if we make a minimum axis aligned bounding box around the set of vertices in this subset and get the center of that box, this is the result.
		Float32 Radius;					// this is the distance of the vertex farthest from CenterBoundingBox.
	};

	// each subset has at least one of these, defines a texture, shader, render flags, etc. basically a render state and texture stage/unit state to use when drawing the subset. multiple instances of these can be defined per subset, enabling multi-texturing.
	class CElement_Material
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

	class CElement_Flags
	{
	public:
		UInt16 Flags1;			// set to 528 (common), 272 (common), 784 (uncommon), 912 (rare).
		UInt16 Unknown1;		// always 0.
		UInt16 iSubset;			// index of a subset within this skin.
		UInt16 Flags2;			// 0 when Flags1 is set to 272, 1 when Flags 1 is set to 528, 4 when Flags1 is set to 784 or 912.
		UInt16 Unknown2;		// always set to 0 for first flags entry in file, always set to 65535, 0xFFFF, -1 for subsequent entries after first.
		UInt16 Unknown3;		// always set to 0.
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
	CM2Element Elements[EElement__Count__];

	// pointer to M2 that this skin belongs to.
	CM2* pM2;


public:
	M2Lib::EError Load( Char8* szFile );
	M2Lib::EError Save( Char8* szFile );

	void BuildVertexBoneIndices();
	void BuildBoundingData(M2Lib::CVertex* GlobalVertexList);
	void BuildMaxBones();

	void CopyMaterials( CM2Skin* pOther );

	bool PrintInfo();

	CElement_Subset* GetSubset( UInt32 ID );


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
