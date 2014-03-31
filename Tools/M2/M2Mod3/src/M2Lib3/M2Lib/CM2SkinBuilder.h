#pragma once

#include "BaseTypes.h"
#include "M2Types.h"
#include "CM2Skin.h"
#include <vector>
#include <map>
#include <assert.h>


namespace M2Lib
{




// maximum number of bone partitions, just used to compile fixed lenth arrays because memory management is easier that way
#define M2SKINBUILDER_MAX_BONE_PARTITION_COUNT 8


class CM2I;		// forward declaration.
class CM2;		// forward declaration.
class CM2Skin;	// forward declaration.


// does the main heavy lifting of building new .skins for the M2.
class CM2SkinBuilder
{
public:
	// 
	class CBonePartition
	{
	public:
		// maximum number of bones allowed per partition.
		UInt32* pBoneLoD;
		// list of bones in this partition, indices into the global bone list. later gets consolidated into the global bone lookup list.
		std::vector< UInt16 > Bones;
		// here we keep a map of triangle index to triangle for all triangles that have successfully been added to this bone partition. this is result caching to speed up building of subset partitions when dealing out subset triangles between subset partitions.
		std::map< UInt32, M2Lib::CTriangle* > TrianglesMap;

		// offset from begining of skin's bone lookup list.
		UInt32 BoneStart;


	public:
		CBonePartition( UInt32* pBoneLoDIn )
			: pBoneLoD( pBoneLoDIn )
			, BoneStart( 0 )
		{
		}

		// attemts to add all of the bones used by input triangle. returns true if bones already exist or were added and triangle was added. returns false if there is not enough room for additional bones.
		bool AddTriangle( M2Lib::CVertex* GlobalVertexList, M2Lib::CTriangle* pTriangle );

		// returns true if bone is contained in this bone partition. if pIndexOut is supplied and function returns true, it will be set to index of where bone was found in this partition.
		bool HasBone( UInt16 Bone, UInt16* pIndexOut );

		// returns true if a triangle has been associated with this bone partition.
		bool HasTriangle( UInt32 TriangleIndex );

	};


	//
	class CSubset
	{
	public:
		class CSubsetPartition
		{
		public:
			// the bone partition used by this partitioned subset.
			CBonePartition* pBonePartition;

			// this subset partition's final index list. these are indices into the global vertex list. this gets consolidated into the single skin vertex list.
			//std::vector< UInt16 > Vertices;
			// this subset partition's final triangle list. these are indices into the above vertex index list. this gets consolidated into the single skin triangle listt
			std::vector< M2Lib::CTriangle* > Triangles;

			UInt32 IndexStart;
			UInt32 IndexCount;
			UInt32 TriangleStart;
			UInt32 TriangleCount;

			// highest number of bones used per vertex contained in this subset partition.
			UInt32 MaxBones;

			// usually always 27. i'm not sure how this is determined. vertices that have no bones (if their vertex property is set to 0) are fully influenced by the root bone. i think this becomes bone 0 for the subset partition.
			// i don't know how to use this properly, it doesn't seem to matter what it's set to, as long as every vertex in the subset partition belongs to at least one bone and weights sum up to 255 (1.0f).
			// it seems to be OK to set this to 0, just make sure that all the vertices have at least 1 bone and that any 0 bones also have 0 weight.
			UInt32 RootBone;


		public:
			CSubsetPartition( CBonePartition* pBonePartitionIn, UInt32 RootBoneIn )
				: pBonePartition( pBonePartitionIn )
				, IndexStart( 0 )
				, IndexCount( 0 )
				, TriangleStart( 0 )
				, TriangleCount( 0 )
				, MaxBones( 0 )
				, RootBone( RootBoneIn )
			{
			}

			// attempts to add a triangle to this subset partition.
			bool AddTriangle( M2Lib::CTriangle* pTriangle );

			// adds a vertex from the global vertex list to this subset's vertex list. returns index of existing or newly added vertex.
			//UInt32 AddVertex( UInt32 VertexIndex );

			// 
			//void FixVertexOffsets( SInt32 Delta );

		};


	public:
		// this subset's ID within the model.
		UInt32 ID;
		//
		std::vector< CSubsetPartition* > SubsetPartitions;
		// 
		std::vector< M2Lib::CM2Skin::CMaterial* > pMaterials;


	public:
		CSubset()
			: ID ( 0 )
		{
		}

		~CSubset()
		{
			for ( UInt32 i = 0; i < SubsetPartitions.size(); i++ )
			{
				delete SubsetPartitions[i];
			}
		}


		// adds a subset partition to the list of subset partitions in this subset. this is done in preparation for when we deal out triangles and vertices to between the various subset partitions.
		void AddSubsetPartition( CBonePartition* pBonePartition, UInt32 RootBone );

		// attempts to add a triangle to this subset.
		bool AddTriangle( M2Lib::CTriangle* pTriangle );

	};


public:
	// bone partition level of detail, this is the maximum number of bones allowed to be used per subset partition per draw call. it is a limitation imposed by the number of shader constant registers available on the GPU.
	// 256, 75, 53, 21
	UInt32 m_BoneLoD;

	// each skin has it's own vertex list. common vertices accross subsets get duplicated (i think) so they appear as many times as they are used in multiple subsets. this is because each subset occupies a sub-range of this list.
	std::vector< UInt16 > m_Indices;
	// bone lookup list used by this skin. the bone lookup lists from all the skins get consolidated into one big bone lookup list that is stored in the M2.
	std::vector< UInt16 > m_Bones;
	// indices to m_VertexLookupList.
	std::vector< UInt16 > m_Triangles;

	// list of subsets that make up this skin.
	std::vector< CSubset* > m_Subsets;

	// list of bone partitions used within this skin.
	std::vector< CBonePartition* > m_BonePartitions;


public:
	CM2SkinBuilder()
		: m_BoneLoD( 256 )
	{
	}

	~CM2SkinBuilder()
	{
		for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
		{
			delete m_Subsets[i];
		}

		for ( UInt32 i = 0; i < m_BonePartitions.size(); i++ )
		{
			delete m_BonePartitions[i];
		}
	}


public:
	void Clear();

	// builds a skin from the supplied parameters.
	bool Build( M2Lib::CM2Skin* pResult, UInt32 BoneLoD, M2Lib::CM2I* pM2I, M2Lib::CVertex* pGlobalVertexList, UInt32 BoneStart );

	// returns true if the built skin with LoD is necessary to be exported, false if can be done without.
	// this is to check for LoD that has significant room for more bones than the skin actually uses, in such case, it would not be advisable to save.
	bool IsNecessary();


};




}
