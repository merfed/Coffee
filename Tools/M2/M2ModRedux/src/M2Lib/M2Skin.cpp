#include "M2Skin.h"
#include "M2.h"
#include "M2.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <list>
#include <assert.h>

extern int g_Verbose;

M2Lib::EError M2Lib::M2Skin::Load( const Char16* FileName )
{
	// check path
	UInt32 Length = 0;
	while ( FileName[Length] != '\0' )
	{
		Length++;
	}
	if ( Length >= 1024 )
		return M2Lib::EError_PathTooLong;
	_FileName[Length] = '\0';
	for ( UInt32 i = 0; i != Length; i++ )
	{
		_FileName[i] = FileName[i];
	}

	// open file stream
	std::fstream FileStream;
	FileStream.open( FileName, std::ios::binary | std::ios::in );

	// find file size
	FileStream.seekg( 0, std::ios::end );
	UInt32 FileSize = (UInt32)FileStream.tellg();
	FileStream.seekg( 0, std::ios::beg );

	// load header
	FileStream.read( (char*)&Header, sizeof(Header) );

	// check header
	if ( Header.ID[0] != 'S' || Header.ID[1] != 'K' || Header.ID[2] != 'I' || Header.ID[3] != 'N' )
		return M2Lib::EError_FailedToLoadSKIN_FileMissingOrCorrupt;

	// fill elements header data
	m_LoadElements_CopyHeaderToElements();
	m_LoadElements_FindSizes( FileSize );

	// load elements
	for ( UInt32 i = 0; i != EElement__Count__; i++ )
	{
		if ( !Elements[i].Load( FileStream ) )
			return M2Lib::EError_FailedToLoadSKIN_FileMissingOrCorrupt;
	}

	// close file stream
	FileStream.close();

	//std::vector< CElement_SubMesh > SubMeshList;
	//for ( UInt32 i = 0; i < Elements[EElement_SubMesh].Count; i++ )
	//{
	//	SubMeshList.push_back( ((CElement_SubMesh*)Elements[EElement_SubMesh].Data)[i] );
	//}

	// resolve material lists
	for ( UInt32 iSubMesh = 0; iSubMesh < Header.nSubMesh; iSubMesh++ )
	{
		SubMeshList_MaterialList.push_back( std::vector< CElement_Material* >() );
	}
	CElement_Material* MaterialArray = (CElement_Material*)Elements[EElement_Material].Data;
	for ( UInt32 iMaterial = 0; iMaterial < Header.nMaterial; iMaterial++ )
	{
		SubMeshList_MaterialList[MaterialArray[iMaterial].iSubMesh].push_back(&MaterialArray[iMaterial]);
	}

	// print info
	//PrintInfo();

	// done
	return M2Lib::EError_OK;
}


M2Lib::EError M2Lib::M2Skin::Save( const Char16* FileName )
{
	// open file stream
	std::fstream FileStream;
	FileStream.open( FileName, std::ios::binary | std::ios::out | std::ios::trunc );

	// fill elements header data
	m_SaveElements_FindOffsets();
	m_SaveElements_CopyElementsToHeader();

	// save header
	FileStream.write( (Char8*)&Header, sizeof(Header) );

	// save elements
	for ( UInt32 i = 0; i != EElement__Count__; i++ )
	{
		if ( !Elements[i].Save( FileStream ) )
			return M2Lib::EError_FailedToSaveSKIN;
	}

	// close file stream
	FileStream.close();

	return M2Lib::EError_OK;
}


void M2Lib::M2Skin::BuildVertexBoneIndices()
{
	if ( !pM2 )
		return;

	M2Lib::CVertex* VertexList = (M2Lib::CVertex*)pM2->GetElement_Vertex().Data;

	UInt32 BoneLookupListLength = pM2->Elements[M2::EElement_SkinnedBoneLookup].Count;
	UInt16* BoneLookupList = (UInt16*)pM2->Elements[M2::EElement_SkinnedBoneLookup].Data;

	UInt32 VertexLookupListLength = Elements[EElement_VertexLookup].Count;
	UInt16* VertexLookupList = (UInt16*)Elements[EElement_VertexLookup].Data;

	Elements[EElement_BoneIndices].SetDataSize( VertexLookupListLength, VertexLookupListLength * sizeof( UInt32 ), false );
	UInt32* BoneIndexList = (UInt32*)Elements[EElement_BoneIndices].Data;

	UInt32 SubMeshListLength = Elements[EElement_SubMesh].Count;
	CElement_SubMesh* SubMeshList = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;

	for ( UInt32 i = 0; i != VertexLookupListLength; i++ )
	{
		BoneIndexList[i] = 0;
	}

	for ( UInt32 iSubMesh = 0; iSubMesh < SubMeshListLength; iSubMesh++ )
	{
		CElement_SubMesh& SubMesh = SubMeshList[iSubMesh];

		UInt32 MaxBonesPerVertex = 0;

		std::vector< UInt16 > BonesLookupVector;
		for ( SInt32 j = SubMesh.BoneStart; j < SubMesh.BoneStart + SubMesh.BoneCount; j++ )
		{
			BonesLookupVector.push_back( BoneLookupList[j] );
		}

		UInt32 SubMeshVertexEnd = SubMesh.VertexStart + SubMesh.VertexCount;
		for ( UInt32 j = SubMesh.VertexStart; j < SubMeshVertexEnd; j++ )
		{
			M2Lib::CVertex& Vertex = VertexList[VertexLookupList[j]];
			UInt8* BoneIndices8 = (UInt8*)&BoneIndexList[j];

			BoneIndices8[0] = m_ReverseBoneLookup( Vertex.BoneIndices[0], &BoneLookupList[SubMesh.BoneStart], SubMesh.BoneCount );
			BoneIndices8[1] = m_ReverseBoneLookup( Vertex.BoneIndices[1], &BoneLookupList[SubMesh.BoneStart], SubMesh.BoneCount );
			BoneIndices8[2] = m_ReverseBoneLookup( Vertex.BoneIndices[2], &BoneLookupList[SubMesh.BoneStart], SubMesh.BoneCount );
			BoneIndices8[3] = m_ReverseBoneLookup( Vertex.BoneIndices[3], &BoneLookupList[SubMesh.BoneStart], SubMesh.BoneCount );

			UInt32 MaxBonesPerThisVertex = 0;
			Vertex.BoneWeights[0] > 0 ? MaxBonesPerThisVertex++ : 0;
			Vertex.BoneWeights[1] > 0 ? MaxBonesPerThisVertex++ : 0;
			Vertex.BoneWeights[2] > 0 ? MaxBonesPerThisVertex++ : 0;
			Vertex.BoneWeights[3] > 0 ? MaxBonesPerThisVertex++ : 0;

			if ( MaxBonesPerThisVertex > MaxBonesPerVertex ) MaxBonesPerVertex = MaxBonesPerThisVertex;
		}

		SubMesh.MaxBonesPerVertex = MaxBonesPerVertex;
	}
}


void M2Lib::M2Skin::BuildBoundingData()
{
	M2Lib::CVertex* VertexList = (M2Lib::CVertex*)pM2->GetElement_Vertex().Data;
	UInt16* VertexLookupList = (UInt16*)Elements[EElement_VertexLookup].Data;

	UInt32 SubMeshListLength = Elements[EElement_SubMesh].Count;
	CElement_SubMesh* SubMeshList = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;

	Float32 OurBoundingBox[2][3];
	Float32 OurBoundingBoxCenter[3];
	Float32 OurRadius = 0.0f;
	Float32 OurCenterMass[3];

	for ( UInt32 iSubMesh = 0; iSubMesh < SubMeshListLength; iSubMesh++ )
	{
		CElement_SubMesh& SubMesh = SubMeshList[iSubMesh];

		if ( SubMesh.VertexCount )
		{
			bool FirstPass = true;
			OurCenterMass[0] = 0.0f;
			OurCenterMass[1] = 0.0f;
			OurCenterMass[2] = 0.0f;
			UInt32 SubMeshVertexEnd = SubMesh.VertexStart + SubMesh.VertexCount;
			for ( UInt32 j = SubMesh.VertexStart; j < SubMeshVertexEnd; j++ )
			{
				if ( FirstPass )
				{
					OurBoundingBox[0][0] = VertexList[VertexLookupList[j]].Position[0];
					OurBoundingBox[0][1] = VertexList[VertexLookupList[j]].Position[1];
					OurBoundingBox[0][2] = VertexList[VertexLookupList[j]].Position[2];

					OurBoundingBox[1][0] = VertexList[VertexLookupList[j]].Position[0];
					OurBoundingBox[1][1] = VertexList[VertexLookupList[j]].Position[1];
					OurBoundingBox[1][2] = VertexList[VertexLookupList[j]].Position[2];

					FirstPass = false;
				}
				else
				{
					if ( OurBoundingBox[0][0] > VertexList[VertexLookupList[j]].Position[0] )
					{
						OurBoundingBox[0][0] = VertexList[VertexLookupList[j]].Position[0];
					}
					if ( OurBoundingBox[0][1] > VertexList[VertexLookupList[j]].Position[1] )
					{
						OurBoundingBox[0][1] = VertexList[VertexLookupList[j]].Position[1];
					}
					if ( OurBoundingBox[0][2] > VertexList[VertexLookupList[j]].Position[2] )
					{
						OurBoundingBox[0][2] = VertexList[VertexLookupList[j]].Position[2];
					}

					if ( OurBoundingBox[1][0] < VertexList[VertexLookupList[j]].Position[0] )
					{
						OurBoundingBox[1][0] = VertexList[VertexLookupList[j]].Position[0];
					}
					if ( OurBoundingBox[1][1] < VertexList[VertexLookupList[j]].Position[1] )
					{
						OurBoundingBox[1][1] = VertexList[VertexLookupList[j]].Position[1];
					}
					if ( OurBoundingBox[1][2] < VertexList[VertexLookupList[j]].Position[2] )
					{
						OurBoundingBox[1][2] = VertexList[VertexLookupList[j]].Position[2];
					}
				}

				OurCenterMass[0] += VertexList[VertexLookupList[j]].Position[0];
				OurCenterMass[1] += VertexList[VertexLookupList[j]].Position[1];
				OurCenterMass[2] += VertexList[VertexLookupList[j]].Position[2];
			}

			OurBoundingBoxCenter[0] = ( OurBoundingBox[0][0] + OurBoundingBox[1][0] ) / 2.0f;
			OurBoundingBoxCenter[1] = ( OurBoundingBox[0][1] + OurBoundingBox[1][1] ) / 2.0f;
			OurBoundingBoxCenter[2] = ( OurBoundingBox[0][2] + OurBoundingBox[1][2] ) / 2.0f;

			OurCenterMass[0] /= (Float32)SubMesh.VertexCount;
			OurCenterMass[1] /= (Float32)SubMesh.VertexCount;
			OurCenterMass[2] /= (Float32)SubMesh.VertexCount;

			FirstPass = true;
			OurRadius = 0.0f;
			for ( UInt32 j = SubMesh.VertexStart; j < SubMeshVertexEnd; j++ )
			{
				Float32 PositionLocal[3];
				Float32 Distance;

				PositionLocal[0] = VertexList[VertexLookupList[j]].Position[0] - OurBoundingBoxCenter[0];
				PositionLocal[1] = VertexList[VertexLookupList[j]].Position[1] - OurBoundingBoxCenter[1];
				PositionLocal[2] = VertexList[VertexLookupList[j]].Position[2] - OurBoundingBoxCenter[2];

				Distance = sqrt( ( PositionLocal[0] * PositionLocal[0] ) + ( PositionLocal[1] * PositionLocal[1] ) + ( PositionLocal[2] * PositionLocal[2] ) );
				if ( Distance > OurRadius )
				{
					OurRadius = Distance;
				}
			}

			SubMesh.CenterMass[0] = OurCenterMass[0];
			SubMesh.CenterMass[1] = OurCenterMass[1];
			SubMesh.CenterMass[2] = OurCenterMass[2];

			SubMesh.CenterBounds[0] = OurBoundingBoxCenter[0];
			SubMesh.CenterBounds[1] = OurBoundingBoxCenter[1];
			SubMesh.CenterBounds[2] = OurBoundingBoxCenter[2];

			SubMesh.Radius = OurRadius;
		}
	}
}


void M2Lib::M2Skin::BuildMaxBones()
{
	M2Lib::CVertex* VertexList = (M2Lib::CVertex*)pM2->GetElement_Vertex().Data;

	UInt32 SubMeshListLength = Elements[EElement_SubMesh].Count;
	CElement_SubMesh* SubMeshList = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;

	for ( UInt32 iSubMesh = 0; iSubMesh < SubMeshListLength; iSubMesh++ )
	{
		CElement_SubMesh& SubMesh = SubMeshList[iSubMesh];

		SubMesh.MaxBonesPerVertex = 0;

		UInt32 SubMeshVertexEnd = SubMesh.VertexStart + SubMesh.VertexCount;
		for ( UInt32 j = SubMesh.VertexStart; j < SubMeshVertexEnd; j++ )
		{
			UInt32 ThisVertexBoneCount = 0;
			if ( VertexList[j].BoneIndices[0] != 0 )
			{
				ThisVertexBoneCount++;
			}
			if ( VertexList[j].BoneIndices[1] != 0 )
			{
				ThisVertexBoneCount++;
			}
			if ( VertexList[j].BoneIndices[2] != 0 )
			{
				ThisVertexBoneCount++;
			}
			if ( VertexList[j].BoneIndices[3] != 0 )
			{
				ThisVertexBoneCount++;
			}

			if ( SubMesh.MaxBonesPerVertex < ThisVertexBoneCount )
			{
				SubMesh.MaxBonesPerVertex = ThisVertexBoneCount;
				if ( SubMesh.MaxBonesPerVertex == 4 )
				{
					break;
				}
			}
		}
	}
}


void M2Lib::M2Skin::CopyMaterials( M2Skin* pOther )
{
	std::vector< CElement_Material > NewMaterialList;
	std::vector< CElement_Flags > NewFlagsList;

	UInt32 SubMeshListLength = Elements[EElement_SubMesh].Count;
	CElement_SubMesh* SubMeshList = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;

	for ( UInt32 iSubMesh = 0; iSubMesh < SubMeshListLength; iSubMesh++ )
	{
		CElement_SubMesh& SubMesh = SubMeshList[iSubMesh];

		SInt32 SubMeshOtherIndex;
		CElement_SubMesh* SubMeshOther = pOther->GetSubMesh( SubMesh.ID, SubMesh.CenterBounds, SubMeshOtherIndex );
		assert( SubMeshOther );

		SubMesh.SortIndex = SubMeshOther->SortIndex;

		if ( SubMeshOther )
		{
			std::vector< CElement_Material* > SubMeshOtherMaterialList;
			pOther->GetSubMeshMaterials( SubMeshOtherIndex, SubMeshOtherMaterialList );
			for ( UInt32 iSubMeshMaterialOther = 0; iSubMeshMaterialOther < SubMeshOtherMaterialList.size(); iSubMeshMaterialOther++ )
			{
				CElement_Material NewMaterial = *SubMeshOtherMaterialList[iSubMeshMaterialOther];
				NewMaterial.iSubMesh = iSubMesh;
				NewMaterial.iSubMesh2 = iSubMesh;
				NewMaterialList.push_back( NewMaterial );
			}

			std::vector< CElement_Flags* > SubMeshOtherFlagsList;
			pOther->GetSubMeshFlags( SubMeshOtherIndex, SubMeshOtherFlagsList );
			for ( UInt32 iSubMeshFlagsOther = 0; iSubMeshFlagsOther < SubMeshOtherFlagsList.size(); iSubMeshFlagsOther++ )
			{
				CElement_Flags NewFlags = *SubMeshOtherFlagsList[iSubMeshFlagsOther];
				NewFlags.iSubMesh = iSubMesh;
				NewFlagsList.push_back( NewFlags );
			}
		}
	}

	// copy new material list to element
	Elements[EElement_Material].SetDataSize( NewMaterialList.size(), sizeof( CElement_Material ) * NewMaterialList.size(), false );
	memcpy( Elements[EElement_Material].Data, &NewMaterialList[0], sizeof( CElement_Material ) * NewMaterialList.size() );

	// copy new flags list to element
	Elements[EElement_Flags].SetDataSize( NewFlagsList.size(), sizeof( CElement_Flags ) * NewFlagsList.size(), false );
	memcpy( Elements[EElement_Flags].Data, &NewFlagsList[0], sizeof( CElement_Flags ) * NewFlagsList.size() );
}


bool SortSubMeshesComparisonFunction( M2Lib::M2Skin::CElement_SubMesh* A, M2Lib::M2Skin::CElement_SubMesh* B )
{
	Float32 ScoreA = A->ID + ( 0.999f - ( A->VertexCount / (Float32)0xFFFF ) );
	Float32 ScoreB = B->ID + ( 0.999f - ( B->VertexCount / (Float32)0xFFFF ) );
	return ( ScoreA < ScoreB );
}


void M2Lib::M2Skin::SortSubMeshes()
{
	UInt32 SubMeshListLength = Elements[EElement_SubMesh].Count;
	CElement_SubMesh* SubMeshList = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;

	// sort the list
	std::list< M2Lib::M2Skin::CElement_SubMesh* > SubMeshListSorted;
	for ( UInt32 iSubMesh = 0; iSubMesh < SubMeshListLength; iSubMesh++ )
	{
		SubMeshListSorted.push_back( &SubMeshList[iSubMesh] );
	}
	SubMeshListSorted.sort( SortSubMeshesComparisonFunction );

	// build index remap list
	UInt32* SubMeshReMapList = new UInt32[SubMeshListLength];
	for ( UInt32 i = 0; i < SubMeshListLength; i++ )
	{
		std::list< M2Lib::M2Skin::CElement_SubMesh* >::iterator it = SubMeshListSorted.begin();
		for ( UInt32 j = 0; j < SubMeshListLength; j++ )
		{
			if ( &SubMeshList[i] == *it )
			{
				SubMeshReMapList[i] = j;
			}
			it++;
		}
	}
	
	// remap material sub mesh indices
	UInt32 MaterialListLength = Elements[EElement_Material].Count;
	CElement_Material* MaterialList = (CElement_Material*)Elements[EElement_Material].Data;
	for ( UInt32 i = 0; i < MaterialListLength; i++ )
	{
		MaterialList[i].iSubMesh = SubMeshReMapList[MaterialList[i].iSubMesh];
		MaterialList[i].iSubMesh2 = SubMeshReMapList[MaterialList[i].iSubMesh2];
	}
	delete[] SubMeshReMapList;

	// copy sorted result
	M2Lib::M2Skin::CElement_SubMesh* NewSubMeshList = new M2Lib::M2Skin::CElement_SubMesh[SubMeshListLength];
	std::list< M2Lib::M2Skin::CElement_SubMesh* >::iterator SubMeshListSortedIterator = SubMeshListSorted.begin();
	for ( UInt32 iSubMesh = 0; iSubMesh < SubMeshListLength; iSubMesh++ )
	{
		NewSubMeshList[iSubMesh] = *(*SubMeshListSortedIterator);
		SubMeshListSortedIterator++;
	}
	memcpy( Elements[EElement_SubMesh].Data, NewSubMeshList, Elements[EElement_SubMesh].DataSize );
	delete[] NewSubMeshList;
}


M2Lib::M2Skin::CElement_SubMesh* M2Lib::M2Skin::GetSubMesh( UInt32 ID, Float32* CenterBounds, SInt32& SubMeshIndexOut )
{
	UInt32 SubMeshListLength = Elements[EElement_SubMesh].Count;
	CElement_SubMesh* SubMeshList = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;
	

	Float32 DeltaMin = 0.0f;
	SInt32 ClosestMatch = -1;
	SubMeshIndexOut = -1;

	for ( UInt32 i = 0; i != SubMeshListLength; i++ )
	{
		if ( SubMeshList[i].ID == ID )
		{
			Float32 X = SubMeshList[i].CenterBounds[0] - CenterBounds[0];
			Float32 Y = SubMeshList[i].CenterBounds[1] - CenterBounds[1];
			Float32 Z = SubMeshList[i].CenterBounds[2] - CenterBounds[2];
			Float32 Delta = sqrt( ( X * X ) + ( Y * Y ) + ( Z * Z ) );
			if ( ClosestMatch == -1 || Delta < DeltaMin )
			{
				DeltaMin = Delta;
				ClosestMatch = i;
			}
		}
	}

	if ( ClosestMatch >= 0 )
	{
		SubMeshIndexOut = ClosestMatch;
		return &SubMeshList[ClosestMatch];
	}

	return 0;
}


void M2Lib::M2Skin::GetSubMeshMaterials( UInt32 SubMeshIndex, std::vector< CElement_Material* >& Result )
{
	CElement_Material* MaterialList = (CElement_Material*)Elements[EElement_Material].Data;
	for ( UInt32 iMaterial = 0; iMaterial < Header.nMaterial; iMaterial++ )
	{
		if ( MaterialList[iMaterial].iSubMesh == SubMeshIndex )
		{
			Result.push_back( &MaterialList[iMaterial] );
		}
	}
}


void M2Lib::M2Skin::GetSubMeshFlags( UInt32 SubMeshIndex, std::vector< CElement_Flags* >& Result )
{
	CElement_Flags* FlagsList = (CElement_Flags*)Elements[EElement_Flags].Data;
	for ( UInt32 iFlags = 0; iFlags < Header.nFlags; iFlags++ )
	{
		if ( FlagsList[iFlags].iSubMesh == SubMeshIndex )
		{
			Result.push_back( &FlagsList[iFlags] );
		}
	}
}


bool M2Lib::M2Skin::PrintInfo()
{
	UInt32 Count = 0;

	Char16 szFileOut[1024];
	UInt32 Length = 0;
	while ( _FileName[Length] != '\0' )
	{
		szFileOut[Length] = _FileName[Length];
		Length++;
	}
	szFileOut[Length++] = '.';
	szFileOut[Length++] = 't';
	szFileOut[Length++] = 'x';
	szFileOut[Length++] = 't';
	szFileOut[Length++] = '\0';

	std::fstream FileStream;
	FileStream.open( szFileOut, std::ios::out | std::ios::trunc );

	SInt32 MaxBones = 0;
	CElement_SubMesh* SubMeshes = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;
	for ( UInt32 i = 0; i < Header.nSubMesh; i++ )
	{
		CElement_SubMesh& Subset = SubMeshes[i];
		if ( Subset.BoneCount > MaxBones )
		{
			MaxBones = Subset.BoneCount;
		}
	}
	FileStream << "MaxBones:      " << MaxBones << std::endl;
	FileStream << std::endl;

	FileStream << "nVertex:       " << Header.nVertex << std::endl;
	FileStream << "oVertex:       " << Header.oVertex << std::endl;
	FileStream << std::endl;

	FileStream << "nIndex:        " << Header.nIndex << std::endl;
	FileStream << "oIndex:        " << Header.oIndex << std::endl;
	FileStream << std::endl;

	FileStream << "nBoneIndices:  " << Header.nBoneIndices << std::endl;
	FileStream << "oBoneIndices:  " << Header.oBoneIndices << std::endl;
	FileStream << std::endl;

	FileStream << "nSubMesh:       " << Header.nSubMesh << std::endl;
	FileStream << "oSubMesh:       " << Header.oSubMesh << std::endl;
	FileStream << std::endl;

	FileStream << "nMaterial:     " << Header.nMaterial << std::endl;
	FileStream << "oMaterial:     " << Header.oMaterial << std::endl;
	FileStream << std::endl;

	FileStream << "Unknown1:      " << Header.Unknown1 << std::endl;
	FileStream << std::endl;

	FileStream << "nFlags:        " << Header.nFlags << std::endl;
	FileStream << "oFlags:        " << Header.oFlags << std::endl;
	FileStream << std::endl;

	FileStream << "Unknown2:      " << Header.Unknown2 << std::endl;
	FileStream << std::endl;


	FileStream << "//" << std::endl;
	FileStream << "// SUBSETS" << std::endl;
	FileStream << std::endl;
	//CElement_SubMesh* SubMeshes = (CElement_SubMesh*)Elements[EElement_SubMesh].Data;
	for ( UInt32 i = 0; i < Header.nSubMesh; i++ )
	{
		CElement_SubMesh& Subset = SubMeshes[i];
		FileStream << "[" << i << "]" << std::endl;
		FileStream << "ID:           " << Subset.ID << std::endl;
		FileStream << "VertexStart:  " << Subset.VertexStart << std::endl;
		FileStream << "VertexCount:  " << Subset.VertexCount << std::endl;
		FileStream << "IndexStart:   " << Subset.IndexStart << std::endl;
		FileStream << "IndexCount:   " << Subset.IndexCount << std::endl;
		FileStream << "BoneCount:    " << Subset.BoneCount << std::endl;
		FileStream << "BoneStart:    " << Subset.BoneStart << std::endl;
		FileStream << "MaxBonesPerVertex:     " << Subset.MaxBonesPerVertex << std::endl;
		FileStream << "Unknown2:     " << Subset.SortIndex << std::endl;
		FileStream << "CenterMass:   " << "( " << Subset.CenterMass[0] << ", " << Subset.CenterMass[1] << ", " << Subset.CenterMass[2] << " ) " << std::endl;
		FileStream << "CenterBounds: " << "( " << Subset.CenterBounds[0] << ", " << Subset.CenterBounds[1] << ", " << Subset.CenterBounds[2] << " ) " << std::endl;
		FileStream << "Radius:       " << Subset.Radius << std::endl;
		FileStream << std::endl;
	}


	FileStream << "//" << std::endl;
	FileStream << "// FLAGS" << std::endl;
	FileStream << std::endl;
	for ( UInt32 i = 0; i < Header.nFlags; i++ )
	{
		CElement_Flags& Flags = ((CElement_Flags*)Elements[EElement_Flags].Data)[i];
		FileStream << "[" << i << "]    SubmeshID: " << SubMeshes[Flags.iSubMesh].ID << std::endl;
		FileStream << "Flags1:     " << Flags.Flags1 << std::endl;
		FileStream << "Unknown1:   " << Flags.Unknown1 << std::endl;
		FileStream << "iSubMesh:    " << Flags.iSubMesh << std::endl;
		FileStream << "Flags2:     " << Flags.Flags2 << std::endl;
		FileStream << "Unknown2:   " << Flags.Unknown2 << std::endl;
		FileStream << "Unknown3:   " << Flags.Unknown3 << std::endl;
		FileStream << std::endl;
	}

	FileStream.close();

	return true;
}


void M2Lib::M2Skin::m_LoadElements_CopyHeaderToElements()
{
	Elements[EElement_VertexLookup].Count = Header.nVertex;
	Elements[EElement_VertexLookup].Offset = Header.oVertex;

	Elements[EElement_Index].Count = Header.nIndex;
	Elements[EElement_Index].Offset = Header.oIndex;

	Elements[EElement_BoneIndices].Count = Header.nBoneIndices;
	Elements[EElement_BoneIndices].Offset = Header.oBoneIndices;

	Elements[EElement_SubMesh].Count = Header.nSubMesh;
	Elements[EElement_SubMesh].Offset = Header.oSubMesh;

	Elements[EElement_Material].Count = Header.nMaterial;
	Elements[EElement_Material].Offset = Header.oMaterial;

	Elements[EElement_Flags].Count = Header.nFlags;
	Elements[EElement_Flags].Offset = Header.oFlags;
}


void M2Lib::M2Skin::m_LoadElements_FindSizes( UInt32 FileSize )
{
	for ( UInt32 i = 0; i != EElement__Count__; i++ )
	{
		Elements[i].OffsetOriginal = Elements[i].Offset;

		if ( ( Elements[i].Count == 0 ) || ( Elements[i].Offset == 0 ) )
		{
			Elements[i].DataSize = 0;
		}
		else
		{
			UInt32 NextOffset = FileSize;
			for ( UInt32 j = i + 1; j < EElement__Count__; j++ )
			{
				if ( Elements[j].Offset )
				{
					NextOffset = Elements[j].Offset;
					break;
				}
			}
			Elements[i].DataSize = NextOffset - Elements[i].Offset;
		}
	}
}


void M2Lib::M2Skin::m_SaveElements_FindOffsets()
{
	UInt32 CurrentOffset = sizeof(Header);
	//UInt32 Remainder = CurrentOffset % 64;
	//CurrentOffset += 64 - Remainder;

	for ( UInt32 i = 0; i < EElement__Count__; i++ )
	{
		if ( Elements[i].DataSize )
		{
			Elements[i].Offset = CurrentOffset;
			CurrentOffset += Elements[i].DataSize;
		}
	}
}


void M2Lib::M2Skin::m_SaveElements_CopyElementsToHeader()
{
	Header.nVertex = Elements[EElement_VertexLookup].Count;
	Header.oVertex = Elements[EElement_VertexLookup].Offset;

	Header.nIndex = Elements[EElement_Index].Count;
	Header.oIndex = Elements[EElement_Index].Offset;

	Header.nBoneIndices = Elements[EElement_BoneIndices].Count;
	Header.oBoneIndices = Elements[EElement_BoneIndices].Offset;

	Header.nSubMesh = Elements[EElement_SubMesh].Count;
	Header.oSubMesh = Elements[EElement_SubMesh].Offset;

	Header.nMaterial = Elements[EElement_Material].Count;
	Header.oMaterial = Elements[EElement_Material].Offset;

	Header.Unknown1 = 0;

	Header.nFlags = Elements[EElement_Flags].Count;
	Header.oFlags = Elements[EElement_Flags].Offset;

	Header.Unknown2 = 0;
}
