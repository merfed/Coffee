#pragma once

#include "M2Element.h"
#include "M2Types.h"
#include <vector>

namespace M2Lib
{




	class M2;
	class M2Skin
	{
	public:
		enum EElement
		{
			EElement_VertexLookup,
			EElement_TriangleIndex,
			EElement_BoneIndices,
			EElement_SubMesh,
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

			UInt32 nTriangleIndex;
			UInt32 oTriangleIndex;

			UInt32 nBoneIndices;
			UInt32 oBoneIndices;

			UInt32 nSubMesh;
			UInt32 oSubMesh;

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
			UInt16 GlobalVertexTriangleIndex;		// index into M2's global vertex list.
		};

		class CElement_TriangleIndex
		{
		public:
			UInt32 VertexTriangleIndex;				// index into SKIN's vertex list.
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

		// each subset has at least one of these, defines a texture, shader, render flags, etc. basically a render state and texture stage/unit state to use when drawing the subset. multiple instances of these can be defined per subset, enabling multi-texturing.
		class CElement_Material
		{
		public:
			UInt16 Flags;					//
			UInt16 RenderOrder;				//
			UInt16 iSubMesh;				// the sub mesh index that this material is assigned to.
			UInt16 iSubMesh2;				// same as above, don't know why this is repeated.
			SInt16 iColor;					// index into the color list.
			SInt16 iRenderFlags;			//
			SInt16 iTexutreUnit1;			//
			SInt16 iShader;					// shader to use for this texture stage.
			SInt16 iTexture;				// index into the texture list.
			SInt16 iTexutreUnit2;			// index into the texture unit list.
			SInt16 iTransparency;			// index into the transparency list.
			SInt16 iTextureAnimation;		// index into the texture animation list.
		};

		// actually, more like a subset partition, as multiple entries of these with same ID may exist.
		class CElement_SubMesh
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
			UInt16 ID;						// subset ID.
			UInt16 Level;					// subset level.
			UInt16 VertexStart;				// first vertex as index into skin's index list.
			UInt16 VertexCount;				// number of indices into the skin's index list.
			UInt16 TriangleIndexStart;		// first triangle as index into skin's triangle list, which if you want the real triangle index, divide by 3.
			UInt16 TriangleIndexCount;		// length of triangle list for this subset, which if you want the real triangle count, divide by 3.
			UInt16 BoneCount;				// number of bones to upload to GPU shader constant registers.
			UInt16 BoneStart;				// first bone in bone lookup list to begin upload to GPU shader constant registers.
			UInt16 MaxBonesPerVertex;		// always set from 0 to 4. maximum number of bones referenced by any one vertex in this sub mesh.
			UInt16 SortTriangleIndex;		// appers to be some sort of draw order sort index or z-depth bias value.
			Float32 CenterMass[3];			// average position of all vertices in this subset. found by summing positions of all vertices and then dividing by the number of vertices.
			Float32 CenterBounds[3];		// bounding box center. if we make a minimum axis aligned bounding box around the set of vertices in this subset and get the center of that box, this is the result.
			Float32 Radius;					// this is the distance of the vertex farthest from CenterBoundingBox.
		};

		class CElement_Flags
		{
		public:
			UInt16 Flags1;			// set to 528 (common), 272 (common), 784 (uncommon), 912 (rare).
			UInt16 Unknown1;		// always 0.
			UInt16 iSubMesh;		// index of a sub mesh within this skin.
			UInt16 Flags2;			// 0 when Flags1 is set to 272, 1 when Flags 1 is set to 528, 4 when Flags1 is set to 784 or 912.
			UInt16 Unknown2;		// always set to 0 for first flags entry in file, always set to 65535/0xFFFF/-1 for subsequent entries after first.
			UInt16 Unknown3;		// always set to 0.
		};

#pragma pack(pop)

		std::vector< std::vector< CElement_Material* > > SubMeshList_MaterialList;	// list of material lists that apply to sub meshes.

	public:
		M2Skin(M2Lib::M2* pM2In)
			: pM2(pM2In)
		{
		}

		~M2Skin()
		{
		}


	public:
		Char16 _FileName[1024];

		CM2SkinHeader Header;
		M2Element Elements[EElement__Count__];

		// pointer to M2 that this skin belongs to.
		M2* pM2;


	public:
		// loads an M2 skin from a file.
		M2Lib::EError Load(const Char16* FileName);
		// saves this M2 skin to a file.
		M2Lib::EError Save(const Char16* FileName);

		void BuildVertexBoneIndices();
		void BuildBoundingData();
		void BuildMaxBones();

		// copies materials from sub meshes in another skin to equivalent sub meshes in this skin.
		void CopyMaterials(M2Skin* pOther);

		//
		void SortSubMeshes();

		bool PrintInfo();

		// returns sub mesh with ID using CenterBounds to narrow search. some times there are multiple sub meshes with the same ID, so we can narrow our search to whatever sub mesh lies closest to CenterBounds.
		CElement_SubMesh* GetSubMesh(UInt16 ID, Float32* CenterBounds, SInt32& SubMeshTriangleIndexOut);

		// gathers list of materials that affect a given sub mesh within this skin.
		void GetSubMeshMaterials(UInt32 SubMeshTriangleIndex, std::vector< CElement_Material* >& Result);

		// gathers list of flags that affect a given sub mesh within this skin.
		void GetSubMeshFlags(UInt32 SubMeshTriangleIndex, std::vector< CElement_Flags* >& Result);

	private:
		void m_LoadElements_CopyHeaderToElements();
		void m_LoadElements_FindSizes(UInt32 FileSize);
		void m_SaveElements_FindOffsets();
		void m_SaveElements_CopyElementsToHeader();

		static SInt32 m_ReverseBoneLookup(UInt8 BoneID, UInt16* BoneLookupTable, UInt32 BoneLookupTableLength)
		{
			if (BoneID == 0)
				return 0;

			for (UInt32 i = 0; i < BoneLookupTableLength; i++)
			{
				if (BoneLookupTable[i] == BoneID)
					return i;
			}

			return 0;
		}

	};




}
