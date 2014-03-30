#include "CM2Skin.h"
#include "CM2.h"
#include "CM2.h"
#include <math.h>
#include <iostream>


extern int g_Verbose;


M2Lib::EError M2Lib::CM2Skin::Load( Char8* szFile )
{
	// 
	UInt32 Length = 0;
	while ( szFile[Length] != '\0' )
	{
		Length++;
	}
	if ( Length >= 1024 )
		return M2Lib::EError_PathTooLong;
	m_szFile[Length] = '\0';
	for ( UInt32 i = 0; i != Length; i++ )
	{
		m_szFile[i] = szFile[i];
	}

	// open file stream
	std::fstream FileStream;
	FileStream.open( szFile, std::ios::in | std::ios::binary );

	// find file size
	FileStream.seekg( 0, std::ios::end );
	UInt32 FileSize = FileStream.tellg();
	FileStream.seekg( 0, std::ios::beg );

	// load header
	FileStream.read( (char*)&Header, sizeof(Header) );

	// check header
	if ( Header.ID[0] != 'S' || Header.ID[1] != 'K' || Header.ID[2] != 'I' || Header.ID[3] != 'N' )
		return M2Lib::EError_FailedToLoadSKIN_FileCorrupt;

	// fill elements header data
	m_LoadElements_CopyHeaderToElements();
	m_LoadElements_FindSizes( FileSize );

	// load elements
	for ( UInt32 i = 0; i != EElement__Count__; i++ )
	{
		if ( !Elements[i].Load( FileStream ) )
			return M2Lib::EError_FailedToLoadSKIN_FileCorrupt;
	}

	// close file stream
	FileStream.close();

	// print info
	//PrintInfo();

	// done
	return M2Lib::EError_OK;
}


M2Lib::EError M2Lib::CM2Skin::Save( Char8* szFile )
{
	if ( Elements[EElement_Vertex].Number > 0xFFFF )
	{
		int pause = 1;
	}

	// open file stream
	std::fstream FileStream;
	FileStream.open( szFile, std::ios::out | std::ios::trunc | std::ios::binary  );

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


void M2Lib::CM2Skin::BuildVertexBoneIndices()
{
	if ( !pM2 )
		return;

	UInt16 PropertiesCount = Elements[EElement_Vertex].Number;
	Elements[EElement_BoneIndices].SetDataSize( PropertiesCount, PropertiesCount * sizeof( UInt32 ), false );
	UInt32* PropertiesNew = (UInt32*)Elements[EElement_BoneIndices].Data;
	for ( UInt32 i = 0; i != PropertiesCount; i++ )
	{
		PropertiesNew[i] = 0;
	}

	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)pM2->GetElement_Vertex().Data;
	UInt16* Indices = (UInt16*)Elements[EElement_Vertex].Data;

	UInt32 BonesLookupCount = pM2->Elements[CM2::EElement_SkinnedBoneLookup].Number;
	UInt16* BonesLookup = (UInt16*)pM2->Elements[CM2::EElement_SkinnedBoneLookup].Data;

	UInt32 SubsetCount = Elements[EElement_Subset].Number;
	CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;

	UInt32 MissedVertices = 0;
	for ( UInt32 i = 0; i != SubsetCount; i++ )
	{
		CElement_Subset& Subset = Subsets[i];
		std::vector< UInt16 > BonesLookupVector;
		for ( SInt32 j = Subset.BoneStart; j < Subset.BoneStart + Subset.BoneCount; j++ )
		{
			BonesLookupVector.push_back( BonesLookup[j] );
		}

		UInt32 SubsetVertexEnd = Subset.VertexStart + Subset.VertexCount;
		for ( UInt32 j = Subset.VertexStart; j < SubsetVertexEnd; j++ )
		{
			M2Lib::CVertex* pVertex = &Vertices[Indices[j]];
			UInt8* pProperties = (UInt8*)&PropertiesNew[j];
			UInt32* pPropertiesInspector = (UInt32*)pProperties;

			pProperties[0] = m_ReverseBoneLookup( pVertex->Bones[0], &BonesLookup[Subset.BoneStart], Subset.BoneCount );
			pProperties[1] = m_ReverseBoneLookup( pVertex->Bones[1], &BonesLookup[Subset.BoneStart], Subset.BoneCount );
			pProperties[2] = m_ReverseBoneLookup( pVertex->Bones[2], &BonesLookup[Subset.BoneStart], Subset.BoneCount );
			pProperties[3] = m_ReverseBoneLookup( pVertex->Bones[3], &BonesLookup[Subset.BoneStart], Subset.BoneCount );
		}
	}

	if ( MissedVertices != 0 )
	{
		int pause = 1;
	}
}


void M2Lib::CM2Skin::BuildBoundingData(M2Lib::CVertex* GlobalVertexList)
{
	M2Lib::CVertex* Vertices = GlobalVertexList;
	UInt16* Indices = (UInt16*)Elements[EElement_Vertex].Data;

	Float32 OurBoundingBox[2][3];
	Float32 OurBoundingBoxCenter[3];
	Float32 OurRadius = 0.0f;
	Float32 OurCenterMass[3];

	CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;
	UInt32 Count = Elements[EElement_Subset].Number;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CElement_Subset* pSubset = &Subsets[i];

		if ( pSubset->VertexCount )
		{
			bool FirstPass = true;
			OurCenterMass[0] = 0.0f;
			OurCenterMass[1] = 0.0f;
			OurCenterMass[2] = 0.0f;
			UInt32 SubsetVertexEnd = pSubset->VertexStart + pSubset->VertexCount;
			for ( UInt32 j = pSubset->VertexStart; j < SubsetVertexEnd; j++ )
			{
				if ( FirstPass )
				{
					OurBoundingBox[0][0] = Vertices[Indices[j]].Position[0];
					OurBoundingBox[0][1] = Vertices[Indices[j]].Position[1];
					OurBoundingBox[0][2] = Vertices[Indices[j]].Position[2];

					OurBoundingBox[1][0] = Vertices[Indices[j]].Position[0];
					OurBoundingBox[1][1] = Vertices[Indices[j]].Position[1];
					OurBoundingBox[1][2] = Vertices[Indices[j]].Position[2];

					FirstPass = false;
				}
				else
				{
					if ( OurBoundingBox[0][0] > Vertices[Indices[j]].Position[0] )
					{
						OurBoundingBox[0][0] = Vertices[Indices[j]].Position[0];
					}
					if ( OurBoundingBox[0][1] > Vertices[Indices[j]].Position[1] )
					{
						OurBoundingBox[0][1] = Vertices[Indices[j]].Position[1];
					}
					if ( OurBoundingBox[0][2] > Vertices[Indices[j]].Position[2] )
					{
						OurBoundingBox[0][2] = Vertices[Indices[j]].Position[2];
					}

					if ( OurBoundingBox[1][0] < Vertices[Indices[j]].Position[0] )
					{
						OurBoundingBox[1][0] = Vertices[Indices[j]].Position[0];
					}
					if ( OurBoundingBox[1][1] < Vertices[Indices[j]].Position[1] )
					{
						OurBoundingBox[1][1] = Vertices[Indices[j]].Position[1];
					}
					if ( OurBoundingBox[1][2] < Vertices[Indices[j]].Position[2] )
					{
						OurBoundingBox[1][2] = Vertices[Indices[j]].Position[2];
					}
				}

				OurCenterMass[0] += Vertices[Indices[j]].Position[0];
				OurCenterMass[1] += Vertices[Indices[j]].Position[1];
				OurCenterMass[2] += Vertices[Indices[j]].Position[2];
			}

			OurBoundingBoxCenter[0] = ( OurBoundingBox[0][0] + OurBoundingBox[1][0] ) / 2.0f;
			OurBoundingBoxCenter[1] = ( OurBoundingBox[0][1] + OurBoundingBox[1][1] ) / 2.0f;
			OurBoundingBoxCenter[2] = ( OurBoundingBox[0][2] + OurBoundingBox[1][2] ) / 2.0f;

			OurCenterMass[0] /= (Float32)pSubset->VertexCount;
			OurCenterMass[1] /= (Float32)pSubset->VertexCount;
			OurCenterMass[2] /= (Float32)pSubset->VertexCount;

			FirstPass = true;
			OurRadius = 0.0f;
			for ( UInt32 j = pSubset->VertexStart; j < SubsetVertexEnd; j++ )
			{
				Float32 PositionLocal[3];
				Float32 Distance;

				PositionLocal[0] = Vertices[Indices[j]].Position[0] - OurBoundingBoxCenter[0];
				PositionLocal[1] = Vertices[Indices[j]].Position[1] - OurBoundingBoxCenter[1];
				PositionLocal[2] = Vertices[Indices[j]].Position[2] - OurBoundingBoxCenter[2];

				Distance = sqrt( ( PositionLocal[0] * PositionLocal[0] ) + ( PositionLocal[1] * PositionLocal[1] ) + ( PositionLocal[2] * PositionLocal[2] ) );
				if ( Distance > OurRadius )
				{
					OurRadius = Distance;
				}
			}

			pSubset->CenterMass[0] = OurCenterMass[0];
			pSubset->CenterMass[1] = OurCenterMass[1];
			pSubset->CenterMass[2] = OurCenterMass[2];

			pSubset->CenterBounds[0] = OurBoundingBoxCenter[0];
			pSubset->CenterBounds[1] = OurBoundingBoxCenter[1];
			pSubset->CenterBounds[2] = OurBoundingBoxCenter[2];

			pSubset->Radius = OurRadius;
		}
	}
}


void M2Lib::CM2Skin::BuildMaxBones()
{
	M2Lib::CVertex* Vertices;
	UInt16* Indices;

	Vertices = (M2Lib::CVertex*)pM2->GetElement_Vertex().Data;
	Indices = (UInt16*)Elements[EElement_Vertex].Data;

	CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;
	UInt32 Count = Elements[EElement_Subset].Number;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CElement_Subset* pSubset = &Subsets[i];

		UInt32 MaxBonesOriginal = pSubset->Unknown1;
		pSubset->Unknown1 = 0;
		UInt32 SubsetVertexEnd = pSubset->VertexStart + pSubset->VertexCount;
		for ( UInt32 j = pSubset->VertexStart; j < SubsetVertexEnd; j++ )
		{
			assert( !( ( Vertices[Indices[j]].Bones[0] == 0 ) && ( Vertices[Indices[j]].BoneWeights[0] != 0 ) ) );

			UInt32 ThisVertexBoneCount = 0;
			if ( Vertices[Indices[j]].Bones[0] != 0 )
			{
				ThisVertexBoneCount++;
			}
			if ( Vertices[Indices[j]].Bones[1] != 0 )
			{
				ThisVertexBoneCount++;
			}
			if ( Vertices[Indices[j]].Bones[2] != 0 )
			{
				ThisVertexBoneCount++;
			}
			if ( Vertices[Indices[j]].Bones[3] != 0 )
			{
				ThisVertexBoneCount++;
			}

			if ( pSubset->Unknown1 < ThisVertexBoneCount )
			{
				pSubset->Unknown1 = ThisVertexBoneCount;
				if ( pSubset->Unknown1 == 4 )
				{
					break;
				}
			}
		}

		if ( MaxBonesOriginal != pSubset->Unknown1 )
		{
			int pause = 1;
		}

		if ( pSubset->Unknown1 == 0 )
		{
			std::cout << "error: subset " << pSubset->ID << " contains one or more vertices that are not assigned to bones. make sure all vertices in the mesh are skinned to at least one bone." << std::endl;
		}
		assert( pSubset->Unknown1 );	// probably need at least one bone per vertex
	}
}


void M2Lib::CM2Skin::CopyMaterials( CM2Skin* pOther )
{
	std::vector< CElement_Material > NewMaterials;

	UInt32 SubsetsCount = Elements[EElement_Subset].Number;
	CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;

	UInt32 SubsetsOtherCount = pOther->Elements[EElement_Subset].Number;
	CElement_Subset* SubsetsOther = (CElement_Subset*)pOther->Elements[EElement_Subset].Data;

	UInt32 MaterialsOtherCount = pOther->Elements[EElement_Material].Number;
	CElement_Material* MaterialsOther = (CElement_Material*)pOther->Elements[EElement_Material].Data;

	for ( UInt32 iSubset = 0; iSubset < SubsetsCount; iSubset++ )
	{
		CElement_Subset& Subset = Subsets[iSubset];

		SInt32 iSubsetOther = -1;
		for ( UInt32 iMaterialOther = 0; iMaterialOther < MaterialsOtherCount; iMaterialOther++ )
		{
			CElement_Material& MaterialOther = MaterialsOther[iMaterialOther];

			if ( SubsetsOther[MaterialOther.iSubset].ID == Subset.ID )
			{
				if ( iSubsetOther == -1 || iSubsetOther == MaterialOther.iSubset )
				{
					iSubsetOther = MaterialOther.iSubset;
					// copy material from other to this
					CElement_Material NewMaterial = MaterialOther;
					NewMaterial.iSubset = iSubset;
					NewMaterial.iSubset2 = iSubset;
					NewMaterials.push_back( NewMaterial );
				}
			}
		}
	}

	// copy new material list to element
	Elements[EElement_Material].SetDataSize( NewMaterials.size(), sizeof( CElement_Material ) * NewMaterials.size(), false );
	memcpy( Elements[EElement_Material].Data, &NewMaterials[0], sizeof( CElement_Material ) * NewMaterials.size() );
}


bool M2Lib::CM2Skin::PrintInfo()
{
	UInt32 Count = 0;

	Char8 szFileOut[1024];
	UInt32 Length = 0;
	while ( m_szFile[Length] != '\0' )
	{
		szFileOut[Length] = m_szFile[Length];
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
	CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;
	for ( UInt32 i = 0; i < Header.nSubset; i++ )
	{
		CElement_Subset& Subset = Subsets[i];
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

	FileStream << "nSubset:       " << Header.nSubset << std::endl;
	FileStream << "oSubset:       " << Header.oSubset << std::endl;
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
	//CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;
	for ( UInt32 i = 0; i < Header.nSubset; i++ )
	{
		CElement_Subset& Subset = Subsets[i];
		FileStream << "[" << i << "]" << std::endl;
		FileStream << "ID:           " << Subset.ID << std::endl;
		FileStream << "VertexStart:  " << Subset.VertexStart << std::endl;
		FileStream << "VertexCount:  " << Subset.VertexCount << std::endl;
		FileStream << "IndexStart:   " << Subset.IndexStart << std::endl;
		FileStream << "IndexCount:   " << Subset.IndexCount << std::endl;
		FileStream << "BoneCount:    " << Subset.BoneCount << std::endl;
		FileStream << "BoneStart:    " << Subset.BoneStart << std::endl;
		FileStream << "Unknown1:     " << Subset.Unknown1 << std::endl;
		FileStream << "Unknown2:     " << Subset.Unknown2 << std::endl;
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
		FileStream << "[" << i << "]    SubmeshID: " << Subsets[Flags.iSubset].ID << std::endl;
		FileStream << "Flags1:     " << Flags.Flags1 << std::endl;
		FileStream << "Unknown1:   " << Flags.Unknown1 << std::endl;
		FileStream << "iSubset:    " << Flags.iSubset << std::endl;
		FileStream << "Flags2:     " << Flags.Flags2 << std::endl;
		FileStream << "Unknown2:   " << Flags.Unknown2 << std::endl;
		FileStream << "Unknown3:   " << Flags.Unknown3 << std::endl;
		FileStream << std::endl;
	}

	FileStream.close();

	return true;
}


M2Lib::CM2Skin::CElement_Subset* M2Lib::CM2Skin::GetSubset( UInt32 ID )
{
	CElement_Subset* Subsets = (CElement_Subset*)Elements[EElement_Subset].Data;
	UInt32 Count = Elements[EElement_Subset].Number;
	for ( UInt32 i = 0; i != Count; i++ )
	{
		if ( Subsets[i].ID == ID )
		{
			return &Subsets[i];
		}
	}

	return 0;
}


void M2Lib::CM2Skin::m_LoadElements_CopyHeaderToElements()
{
	Elements[EElement_Vertex].Number = Header.nVertex;
	Elements[EElement_Vertex].Offset = Header.oVertex;

	Elements[EElement_Index].Number = Header.nIndex;
	Elements[EElement_Index].Offset = Header.oIndex;

	Elements[EElement_BoneIndices].Number = Header.nBoneIndices;
	Elements[EElement_BoneIndices].Offset = Header.oBoneIndices;

	Elements[EElement_Subset].Number = Header.nSubset;
	Elements[EElement_Subset].Offset = Header.oSubset;

	Elements[EElement_Material].Number = Header.nMaterial;
	Elements[EElement_Material].Offset = Header.oMaterial;

	Elements[EElement_Flags].Number = Header.nFlags;
	Elements[EElement_Flags].Offset = Header.oFlags;
}


void M2Lib::CM2Skin::m_LoadElements_FindSizes( UInt32 FileSize )
{
	for ( UInt32 i = 0; i != EElement__Count__; i++ )
	{
		Elements[i].OffsetOriginal = Elements[i].Offset;

		if ( ( Elements[i].Number == 0 ) || ( Elements[i].Offset == 0 ) )
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


void M2Lib::CM2Skin::m_SaveElements_FindOffsets()
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


void M2Lib::CM2Skin::m_SaveElements_CopyElementsToHeader()
{
	Header.nVertex = Elements[EElement_Vertex].Number;
	Header.oVertex = Elements[EElement_Vertex].Offset;

	Header.nIndex = Elements[EElement_Index].Number;
	Header.oIndex = Elements[EElement_Index].Offset;

	Header.nBoneIndices = Elements[EElement_BoneIndices].Number;
	Header.oBoneIndices = Elements[EElement_BoneIndices].Offset;

	Header.nSubset = Elements[EElement_Subset].Number;
	Header.oSubset = Elements[EElement_Subset].Offset;

	Header.nMaterial = Elements[EElement_Material].Number;
	Header.oMaterial = Elements[EElement_Material].Offset;

	Header.Unknown1 = 0;

	Header.nFlags = Elements[EElement_Flags].Number;
	Header.oFlags = Elements[EElement_Flags].Offset;

	Header.Unknown2 = 0;
}
