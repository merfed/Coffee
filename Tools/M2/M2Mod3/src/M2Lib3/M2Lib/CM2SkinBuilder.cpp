#include "CM2SkinBuilder.h"
#include "CM2I.h"
#include "CM2.h"
#include "CM2Skin.h"
#include <cstring>
#include <iostream>


extern int g_Verbose;


bool M2Lib::CM2SkinBuilder::CBonePartition::AddTriangle( M2Lib::CVertex* GlobalVertexList, M2Lib::CTriangle* pTriangle )
{
	// put all the bones used by the input triangle into a 1D list for easy iteration
	UInt8 TriBones[12];
	UInt32 TotalWeight;

	UInt8* TriBonesSub;
	M2Lib::CVertex* pTriVertex;

	int pause;
	TotalWeight = 0;
	TriBonesSub = &TriBones[0];
	pTriVertex = &GlobalVertexList[pTriangle->Vertices[0]];

	for ( int i = 0; i < 4; i++ )
	{
	if ( pTriVertex->Bones[i] == 0 && pTriVertex->BoneWeights[i] != 0 )
		pause = 1;
	}

	TriBonesSub[0] = pTriVertex->Bones[0]; TotalWeight += pTriVertex->BoneWeights[0];
	TriBonesSub[1] = pTriVertex->Bones[1]; TotalWeight += pTriVertex->BoneWeights[1];
	TriBonesSub[2] = pTriVertex->Bones[2]; TotalWeight += pTriVertex->BoneWeights[2];
	TriBonesSub[3] = pTriVertex->Bones[3]; TotalWeight += pTriVertex->BoneWeights[3];
	if ( TotalWeight != 255 )
	{
		int pause = 1;	// because assert sucks so hard at keeping the call stack in tact
	}

	TotalWeight = 0;
	TriBonesSub = &TriBones[4];
	pTriVertex = &GlobalVertexList[pTriangle->Vertices[1]];

	for ( int i = 0; i < 4; i++ )
	{
	if ( pTriVertex->Bones[i] == 0 && pTriVertex->BoneWeights[i] != 0 )
		pause = 1;
	}

	TriBonesSub[0] = pTriVertex->Bones[0]; TotalWeight += pTriVertex->BoneWeights[0];
	TriBonesSub[1] = pTriVertex->Bones[1]; TotalWeight += pTriVertex->BoneWeights[1];
	TriBonesSub[2] = pTriVertex->Bones[2]; TotalWeight += pTriVertex->BoneWeights[2];
	TriBonesSub[3] = pTriVertex->Bones[3]; TotalWeight += pTriVertex->BoneWeights[3];
	if ( TotalWeight != 255 )
	{
		int pause = 1;
	}

	TotalWeight = 0;
	TriBonesSub = &TriBones[8];
	pTriVertex = &GlobalVertexList[pTriangle->Vertices[2]];

	for ( int i = 0; i < 4; i++ )
	{
	if ( pTriVertex->Bones[i] == 0 && pTriVertex->BoneWeights[i] != 0 )
		pause = 1;
	}

	TriBonesSub[0] = pTriVertex->Bones[0]; TotalWeight += pTriVertex->BoneWeights[0];
	TriBonesSub[1] = pTriVertex->Bones[1]; TotalWeight += pTriVertex->BoneWeights[1];
	TriBonesSub[2] = pTriVertex->Bones[2]; TotalWeight += pTriVertex->BoneWeights[2];
	TriBonesSub[3] = pTriVertex->Bones[3]; TotalWeight += pTriVertex->BoneWeights[3];
	if ( TotalWeight != 255 )
	{
		int pause = 1;
	}

	// count the number of bones used by the triangle
	UInt32 ExtraBones = 0;
	for ( UInt32 i = 0; i < 12; i++ )
	{
		if ( TriBones[i] != 0 )
		{
			ExtraBones++;
		}
	}

	// count the number of bones from the triangle that already exist in this bone partition
	for ( UInt32 i = 0; i < 12; i++ )
	{
		if ( TriBones[i] != 0 )
		{
			if ( HasBone( TriBones[i], 0 ) )
			{
				ExtraBones--;
			}
		}
	}

	if ( ExtraBones != 0 )
	{
		// there are some bones from the input triangle that are not contained in this bone partition
		UInt32 ExtraRoom = (*pBoneLoD) - Bones.size();
		if ( ExtraBones > ExtraRoom )
		{
			// there isn't enough room for them
			return false;
		}

		// there's room for them
		for ( UInt32 i = 0; i < 12; i++ )
		{
			if ( TriBones[i] != 0 )
			{
				if ( !HasBone( TriBones[i], 0 ) )
				{
					// add the bone that isn't already contained
					Bones.push_back( TriBones[i] );
				}
			}
		}
	}

	// add triangle index and triangle to the triangle map
	assert( !HasTriangle( pTriangle->Index ) );
	TrianglesMap[pTriangle->Index] = pTriangle;

	// triangle successfully added
	return true;
}


bool M2Lib::CM2SkinBuilder::CBonePartition::HasBone( UInt16 BoneIndex, UInt16* pIndexOut )
{
	UInt32 Count = Bones.size();
	for ( UInt32 i = 0; i < Count; i++ )
	{
		if ( Bones[i] == BoneIndex )
		{
			if ( pIndexOut )
			{
				*pIndexOut = i;
			}
			return true;
		}
	}

	return false;
}


bool M2Lib::CM2SkinBuilder::CBonePartition::HasTriangle( UInt32 TriangleIndex )
{
	return ( TrianglesMap.find( TriangleIndex ) != TrianglesMap.end() );
}


bool M2Lib::CM2SkinBuilder::CSubset::CSubsetPartition::AddTriangle( M2Lib::CTriangle* pTriangle )
{
	if ( pBonePartition->HasTriangle( pTriangle->Index ) )
	{
		Triangles.push_back( pTriangle );

		return true;
	}

	return false;
}


//UInt32 M2Lib::CM2SkinBuilder::CSubset::CSubsetPartition::AddVertex( UInt32 VertexIndex )
//{
//	UInt32 Count = Vertices.size();
//	for ( UInt32 i = 0; i < Count; i++ )
//	{
//		if ( Vertices[i] == VertexIndex )
//		{
//			return i;
//		}
//	}
//
//	Vertices.push_back( VertexIndex );
//	return Count;
//}


//void M2Lib::CM2SkinBuilder::CSubset::CSubsetPartition::FixVertexOffsets( SInt32 Delta )
//{
//	for ( UInt32 i = 0; i < Triangles.size(); i++ )
//	{
//		M2Lib::CTriangle* pTriangle = &Triangles[i];
//
//		pTriangle->Vertices[0] += Delta;
//		pTriangle->Vertices[1] += Delta;
//		pTriangle->Vertices[2] += Delta;
//	}
//}


void M2Lib::CM2SkinBuilder::CSubset::AddSubsetPartition( CBonePartition* pBonePartition, UInt32 RootBone )
{
	SubsetPartitions.push_back( new CSubsetPartition( pBonePartition, RootBone ) );
}


bool M2Lib::CM2SkinBuilder::CSubset::AddTriangle( M2Lib::CTriangle* pTriangle )
{
	for ( UInt32 i = 0; i < SubsetPartitions.size(); i++ )
	{
		if ( SubsetPartitions[i]->AddTriangle( pTriangle ) )
		{
			return true;
		}
	}

	return false;
}


void M2Lib::CM2SkinBuilder::Clear()
{
	m_Indices.clear();
	m_Bones.clear();
	m_Triangles.clear();

	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		delete m_Subsets[i];
	}
	m_Subsets.clear();

	for ( UInt32 i = 0; i < m_BonePartitions.size(); i++ )
	{
		delete m_BonePartitions[i];
	}
	m_BonePartitions.clear();
}


bool M2Lib::CM2SkinBuilder::Build( M2Lib::CM2Skin* pResult, UInt32 BoneLoD, M2Lib::CM2I* pM2I, M2Lib::CVertex* pGlobalVertexList, UInt32 BoneStart )
{
	m_BoneLoD = BoneLoD;
	Clear();


	// build bone partitions
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tbuilding bone partitions..." << std::endl;
	}
	for ( UInt32 i = 0; i < pM2I->Subsets.size(); i++ )
	{
		for ( UInt32 j = 0; j < pM2I->Subsets[i]->Triangles.size(); j++ )
		{
			bool Added = false;
			for ( UInt32 k = 0; k < m_BonePartitions.size(); k++ )
			{
				if ( m_BonePartitions[k]->AddTriangle( pGlobalVertexList, &pM2I->Subsets[i]->Triangles[j] ) )
				{
					Added = true;
					break;
				}
			}

			if ( !Added )
			{
				m_BonePartitions.push_back( new CBonePartition( &m_BoneLoD ) );
				m_BonePartitions.back()->AddTriangle( pGlobalVertexList, &pM2I->Subsets[i]->Triangles[j] );
			}
		}
	}
	UInt32 iBoneStart = 0;
	for ( UInt32 i = 0; i < m_BonePartitions.size(); i++ )
	{
		m_BonePartitions[i]->BoneStart = iBoneStart + BoneStart;
		iBoneStart += m_BonePartitions[i]->Bones.size();
	}


	// build subsets list
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tbuilding subsets..." << std::endl;
	}
	for ( UInt32 i = 0; i < pM2I->Subsets.size(); i++ )
	{
		CSubset* pNewSubset = new CSubset();
		pNewSubset->ID = pM2I->Subsets[i]->ID;

		// add materials
		for ( UInt32 j = 0; j < pM2I->Subsets[i]->Materials.size(); j++ )
		{
			pNewSubset->pMaterials.push_back( &pM2I->Subsets[i]->Materials[j] );
		}

		// add subset partitions
		for ( UInt32 k = 0; k < m_BonePartitions.size(); k++ )
		{
			pNewSubset->AddSubsetPartition( m_BonePartitions[k], pM2I->Subsets[i]->RootBone );
		}

		m_Subsets.push_back( pNewSubset );
	}


	// build subsets, deal out triangles between subsets and their partitions
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tdealing out triangles to subsets..." << std::endl;
	}
	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		M2Lib::CM2I::CSubset* pSubsetI = pM2I->Subsets[i];
		for ( UInt32 j = 0; j < pSubsetI->Triangles.size(); j++ )
		{
			assert( m_Subsets[i]->AddTriangle( &pSubsetI->Triangles[j] ) );
		}
	}


	// build index and triangle lists
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tbuilding index and triangle lists..." << std::endl;
	}
	UInt32 IndexStart = 0;
	UInt32 TriangleStart = 0;
	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		for ( UInt32 j = 0; j < m_Subsets[i]->SubsetPartitions.size(); j++ )
		{
			std::map< UInt16, UInt16 > GlobalToSkinIndexMap;
			UInt32 IndexCount = 0;
			UInt32 TriangleCount = 0;
			if ( m_Subsets[i]->SubsetPartitions[j]->Triangles.size() )
			{
				for ( UInt32 k = 0; k < m_Subsets[i]->SubsetPartitions[j]->Triangles.size(); k++ )
				{
					for ( UInt32 iVert = 0; iVert < 3; iVert++ )
					{
						UInt16 VertexToMap = m_Subsets[i]->SubsetPartitions[j]->Triangles[k]->Vertices[iVert];	// this is the global vertex index
						UInt16 VertexMapped = 0;
						if ( GlobalToSkinIndexMap.find( VertexToMap ) != GlobalToSkinIndexMap.end() )
						{
							VertexMapped = GlobalToSkinIndexMap[VertexToMap];
						}
						else
						{
							VertexMapped = m_Indices.size();
							m_Indices.push_back( VertexToMap );
							GlobalToSkinIndexMap[VertexToMap] = VertexMapped;
							IndexCount++;
						}
						m_Triangles.push_back( VertexMapped );
						TriangleCount++;
					}
				}
				m_Subsets[i]->SubsetPartitions[j]->IndexStart = IndexStart;
				m_Subsets[i]->SubsetPartitions[j]->IndexCount = IndexCount;
				IndexStart += IndexCount;
				m_Subsets[i]->SubsetPartitions[j]->TriangleStart = TriangleStart;
				m_Subsets[i]->SubsetPartitions[j]->TriangleCount = TriangleCount;
				TriangleStart += TriangleCount;
			}
		}
	}


	// build skin bone list
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tbuilding skin bone lookup list..." << std::endl;
	}
	for ( UInt32 i = 0; i < m_BonePartitions.size(); i++ )
	{
		for ( UInt32 j = 0; j < m_BonePartitions[i]->Bones.size(); j++ )
		{
			m_Bones.push_back( m_BonePartitions[i]->Bones[j] );
		}
	}


	// fill result
	pResult->Header.LevelOfDetail = m_BoneLoD;
	pResult->Header.ID[0] = 'S';
	pResult->Header.ID[1] = 'K';
	pResult->Header.ID[2] = 'I';
	pResult->Header.ID[3] = 'N';

	UInt32 TotalPartitionCount = 0;
	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		for ( UInt32 j = 0; j < m_Subsets[i]->SubsetPartitions.size(); j++ )
		{
			if ( m_Subsets[i]->SubsetPartitions[j]->Triangles.size() )
			{
				TotalPartitionCount++;
			}
		}
	}

	// copy indices
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tcopying indices to element..." << std::endl;
	}
 	pResult->Elements[M2Lib::CM2Skin::EElement_Indices].SetDataSize( m_Indices.size(), m_Indices.size() * sizeof( UInt16 ), false );
	UInt16* Indices = (UInt16*)pResult->Elements[M2Lib::CM2Skin::EElement_Indices].Data;
	UInt16* IndicesIn = (UInt16*)&m_Indices[0];
	memcpy( Indices, IndicesIn, m_Indices.size() * sizeof( UInt16 ) );

	// copy triangles
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tcopying triangles to element..." << std::endl;
	}
	pResult->Elements[M2Lib::CM2Skin::EElement_Triangles].SetDataSize( m_Triangles.size(), m_Triangles.size() * sizeof( UInt16 ), false );
	UInt16* Triangles = (UInt16*)pResult->Elements[M2Lib::CM2Skin::EElement_Triangles].Data;
	UInt16* TrianglesIn = (UInt16*)&m_Triangles[0];
	memcpy( Triangles, TrianglesIn, m_Triangles.size() * sizeof( UInt16 ) );

	// copy subsets
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tcopying subsets to element..." << std::endl;
	}
	pResult->Elements[M2Lib::CM2Skin::EElement_Subsets].SetDataSize( TotalPartitionCount, TotalPartitionCount * sizeof( M2Lib::CM2Skin::CSubset ), false );
	M2Lib::CM2Skin::CSubset* SubsetsOut = (M2Lib::CM2Skin::CSubset*)pResult->Elements[M2Lib::CM2Skin::EElement_Subsets].Data;
	UInt32 iSubsetPartition = 0;
	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		CSubset* pSubsetIn = m_Subsets[i];
		for ( UInt32 j = 0; j < m_Subsets[i]->SubsetPartitions.size(); j++ )
		{
			CSubset::CSubsetPartition* pSubsetPartitionIn = m_Subsets[i]->SubsetPartitions[j];
			if ( m_Subsets[i]->SubsetPartitions[j]->Triangles.size() )
			{
				M2Lib::CM2Skin::CSubset* pSubsetOut = &SubsetsOut[iSubsetPartition];
				iSubsetPartition++;

				pSubsetOut->ID = pSubsetIn->ID;
				pSubsetOut->IndexStart = m_Subsets[i]->SubsetPartitions[j]->IndexStart;
				pSubsetOut->IndexCount= m_Subsets[i]->SubsetPartitions[j]->IndexCount;
				pSubsetOut->TriangleStart = m_Subsets[i]->SubsetPartitions[j]->TriangleStart;
				pSubsetOut->TriangleCount = m_Subsets[i]->SubsetPartitions[j]->TriangleCount;
				pSubsetOut->BoneStart = pSubsetPartitionIn->pBonePartition->BoneStart;
				pSubsetOut->BoneCount = pSubsetPartitionIn->pBonePartition->Bones.size();

				// we do the real calculation for this later
				pSubsetOut->MaxBones = 0;

				// copy root bone
				pSubsetOut->RootBone = pSubsetPartitionIn->RootBone;
			}
		}
	}

	// build bounding data
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tbuilding subset bounds data..." << std::endl;
	}
	pResult->BuildBoundingData();

	// build max bones
	// save this for later
	//pResult->BuildMaxBones();

	// build vertex properties
	// save this for later
	//pResult->BuildVertexBoneIndices();

	// build materials
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tbuilding materials..." << std::endl;
	}
	UInt32 MaterialCount = 0;
	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		CSubset* pSubsetIn = m_Subsets[i];
		for ( UInt32 j = 0; j < m_Subsets[i]->SubsetPartitions.size(); j++ )
		{
			if ( m_Subsets[i]->SubsetPartitions[j]->Triangles.size() )
			{
				MaterialCount += m_Subsets[i]->pMaterials.size();
			}
		}
	}


	pResult->Elements[M2Lib::CM2Skin::EElement_Materials].SetDataSize( MaterialCount, MaterialCount * sizeof( M2Lib::CM2Skin::CMaterial ), false );
	M2Lib::CM2Skin::CMaterial* MaterialsOut = (M2Lib::CM2Skin::CMaterial*)pResult->Elements[M2Lib::CM2Skin::EElement_Materials].Data;
	UInt32 iMaterialOut = 0;
	iSubsetPartition = 0;
	for ( UInt32 i = 0; i < m_Subsets.size(); i++ )
	{
		CSubset* pSubsetIn = m_Subsets[i];
		for ( UInt32 j = 0; j < m_Subsets[i]->SubsetPartitions.size(); j++ )
		{
			if ( m_Subsets[i]->SubsetPartitions[j]->Triangles.size() )
			{
				for ( UInt32 k = 0; k < m_Subsets[i]->pMaterials.size(); k++ )
				{
					M2Lib::CM2Skin::CMaterial* pMaterialOut = &MaterialsOut[iMaterialOut];

					m_Subsets[i]->pMaterials[k]->iSubset = iSubsetPartition;
					m_Subsets[i]->pMaterials[k]->iSubset2 = iSubsetPartition;
					memcpy( pMaterialOut, m_Subsets[i]->pMaterials[k], sizeof( M2Lib::CM2Skin::CMaterial ) );

					iMaterialOut++;
				}

				iSubsetPartition++;
			}
		}
	}

	return true;
}


bool M2Lib::CM2SkinBuilder::IsNecessary()
{
	return true;
}
