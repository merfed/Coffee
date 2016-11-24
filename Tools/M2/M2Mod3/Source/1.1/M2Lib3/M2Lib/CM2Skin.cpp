#include "CM2Skin.h"
#include "CM2.h"
#include "CM2.h"
#include <math.h>


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

	return M2Lib::EError_OK;
}


M2Lib::EError M2Lib::CM2Skin::Save( Char8* szFile )
{
	if ( Elements[EElement_Indices].Number > 0xFFFF )
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

	UInt16 PropertiesCount = Elements[EElement_Indices].Number;
	Elements[EElement_Properties].SetDataSize( PropertiesCount, PropertiesCount * sizeof( UInt32 ), false );
	UInt32* PropertiesNew = (UInt32*)Elements[EElement_Properties].Data;
	for ( UInt32 i = 0; i != PropertiesCount; i++ )
	{
		PropertiesNew[i] = 0;
	}

	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)pM2->Elements[CM2::EElement_Vertices].Data;
	UInt16* Indices = (UInt16*)Elements[EElement_Indices].Data;

	UInt32 BonesLookupCount = pM2->Elements[CM2::EElement_BonesLookup].Number;
	UInt16* BonesLookup = (UInt16*)pM2->Elements[CM2::EElement_BonesLookup].Data;

	UInt32 SubsetCount = Elements[EElement_Subsets].Number;
	CSubset* Subsets = (CSubset*)Elements[EElement_Subsets].Data;

	UInt32 MissedVertices = 0;
	for ( UInt32 i = 0; i != SubsetCount; i++ )
	{
		CSubset& Subset = Subsets[i];
		std::vector< UInt16 > BonesLookupVector;
		for ( SInt32 j = Subset.BoneStart; j < Subset.BoneStart + Subset.BoneCount; j++ )
		{
			BonesLookupVector.push_back( BonesLookup[j] );
		}

		UInt32 SubsetVertexEnd = Subset.IndexStart + Subset.IndexCount;
		for ( UInt32 j = Subset.IndexStart; j < SubsetVertexEnd; j++ )
		{
			M2Lib::CVertex* pVertex = &Vertices[Indices[j]];
			UInt8* pProperties = (UInt8*)&PropertiesNew[j];
			UInt32* pPropertiesInspector = (UInt32*)pProperties;

			//UInt32 TotalWeight = 0;
			//TotalWeight += pVertex->BoneWeights[0];
			//TotalWeight += pVertex->BoneWeights[1];
			//TotalWeight += pVertex->BoneWeights[2];
			//TotalWeight += pVertex->BoneWeights[3];
			//if ( TotalWeight == 0 )
			//{
			//	int pause = 1;
			//}

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

	//// compare the new to the old just to see that we arrived at same result so we know we're calculating this right
	//for ( UInt32 i = 0; i != PropertiesCount; i++ )
	//{
	//	if ( PropertiesOriginal[i] != PropertiesNew[i] )
	//		int help = 1;
	//}
}


void M2Lib::CM2Skin::BuildBoundingData()
{
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)pM2->Elements[M2Lib::CM2::EElement_Vertices].Data;
	UInt16* Indices = (UInt16*)Elements[EElement_Indices].Data;

	Float32 OurBoundingBox[2][3];
	Float32 OurBoundingBoxCenter[3];
	Float32 OurRadius = 0.0f;
	Float32 OurCenterMass[3];

	CSubset* Subsets = (CSubset*)Elements[EElement_Subsets].Data;
	UInt32 Count = Elements[EElement_Subsets].Number;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CSubset* pSubset = &Subsets[i];

		if ( pSubset->IndexCount )
		{
			bool FirstPass = true;
			OurCenterMass[0] = 0.0f;
			OurCenterMass[1] = 0.0f;
			OurCenterMass[2] = 0.0f;
			UInt32 SubsetVertexEnd = pSubset->IndexStart + pSubset->IndexCount;
			for ( UInt32 j = pSubset->IndexStart; j < SubsetVertexEnd; j++ )
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

			OurCenterMass[0] /= (Float32)pSubset->IndexCount;
			OurCenterMass[1] /= (Float32)pSubset->IndexCount;
			OurCenterMass[2] /= (Float32)pSubset->IndexCount;

			FirstPass = true;
			OurRadius = 0.0f;
			for ( UInt32 j = pSubset->IndexStart; j < SubsetVertexEnd; j++ )
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

			pSubset->CenterBoundingBox[0] = OurBoundingBoxCenter[0];
			pSubset->CenterBoundingBox[1] = OurBoundingBoxCenter[1];
			pSubset->CenterBoundingBox[2] = OurBoundingBoxCenter[2];

			pSubset->Radius = OurRadius;
		}
	}
}


void M2Lib::CM2Skin::BuildMaxBones()
{
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)pM2->Elements[M2Lib::CM2::EElement_Vertices].Data;
	UInt16* Indices = (UInt16*)Elements[EElement_Indices].Data;

	CSubset* Subsets = (CSubset*)Elements[EElement_Subsets].Data;
	UInt32 Count = Elements[EElement_Subsets].Number;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CSubset* pSubset = &Subsets[i];

		UInt32 MaxBonesOriginal = pSubset->MaxBones;
		pSubset->MaxBones = 0;
		UInt32 SubsetVertexEnd = pSubset->IndexStart + pSubset->IndexCount;
		for ( UInt32 j = pSubset->IndexStart; j < SubsetVertexEnd; j++ )
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

			if ( pSubset->MaxBones < ThisVertexBoneCount )
			{
				pSubset->MaxBones = ThisVertexBoneCount;
				if ( pSubset->MaxBones == 4 )
				{
					break;
				}
			}
		}

		if ( MaxBonesOriginal != pSubset->MaxBones )
		{
			int pause = 1;
		}

		assert( pSubset->MaxBones );	// probably need at least one bone per vertex
	}
}


bool M2Lib::CM2Skin::PrintInfo( void* VerticesIn, void* BonesIn )
{
	UInt32 Count = 0;

	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)VerticesIn;
	CM2::CBone* Bones = (CM2::CBone*)BonesIn;

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

	FileStream << "Number Indices: " << Header.nIndices << std::endl;
	FileStream << "Offset Indices: " << Header.oIndices << std::endl << std::endl;

	FileStream << "Number Triangles: " << Header.nTriangles << std::endl;
	FileStream << "Offset Triangles: " << Header.oTriangles << std::endl << std::endl;

	FileStream << "Number Properties: " << Header.nProperites << std::endl;
	FileStream << "Offset Properties: " << Header.oProperties << std::endl << std::endl;

	FileStream << "Number Subsets: " << Header.nSubsets << std::endl;
	FileStream << "Offset Subsets: " << Header.oSubsets << std::endl << std::endl;

	FileStream << "Number Materials: " << Header.nMaterials << std::endl;
	FileStream << "Offset Materials: " << Header.oMaterials << std::endl << std::endl;

	FileStream << "Level Of Detail: " << Header.LevelOfDetail << std::endl << std::endl << std::endl;

	UInt16* Indices = (UInt16*)Elements[EElement_Indices].Data;
	UInt16* Triangles = (UInt16*)Elements[EElement_Triangles].Data;

	Count = Elements[EElement_Subsets].Number;
	CSubset* Subsets = (CSubset*)Elements[EElement_Subsets].Data;
	FileStream << "Subsets: " << Count << std::endl << std::endl;
	for ( UInt32 i = 0; i != Count; i++ )
	{
		CSubset& Subset = Subsets[i];

		FileStream << "[" << i << "]" << std::endl;
		FileStream << "ID: " << Subset.ID << std::endl;
		FileStream << "Vertex Start: " << Subset.IndexStart << std::endl;
		FileStream << "Vertex Count: " << Subset.IndexCount << std::endl;
		FileStream << "Triangle Start: " << Subset.TriangleStart << std::endl;
		FileStream << "Triangle Count: " << Subset.TriangleCount << std::endl;
		FileStream << "Number Bones Lookup: " << Subset.BoneCount << std::endl;
		FileStream << "Offset Bones Lookup: " << Subset.BoneStart << std::endl;

		UInt16* BonesLookup = (UInt16*)pM2->Elements[M2Lib::CM2::EElement_BonesLookup].Data;
		UInt32 BoneEnd = Subset.BoneStart + Subset.BoneCount;
		for ( UInt32 i = Subset.BoneStart; i < BoneEnd; i++ )
		{
			FileStream << "\t" << BonesLookup[i] << std::endl;
		}

		FileStream << "Max Bones Per Vertex: " << Subset.MaxBones << std::endl;
		FileStream << "Root Bone: " << Subset.RootBone << std::endl;
		FileStream << "Center Mass: (" << Subset.CenterMass[0] << ", " << Subset.CenterMass[1] << ", " << Subset.CenterMass[2] << ")" << std::endl;
		FileStream << "Center Bounding Box: (" << Subset.CenterBoundingBox[0] << ", " << Subset.CenterBoundingBox[1] << ", " << Subset.CenterBoundingBox[2] << ")" << std::endl;
		FileStream << "Radius: " << Subset.Radius << std::endl << std::endl;
	}
	FileStream << std::endl << std::endl;

	Count = Elements[EElement_Materials].Number;
	CMaterial* Materials = (CMaterial*)Elements[EElement_Materials].Data;
	FileStream << "Materials: " << Count << std::endl << std::endl;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CMaterial& Material = Materials[i];

		FileStream << "[" << i << "]" << std::endl;
		FileStream << "Flags1: " << Material.Flags1 << std::endl;
		FileStream << "Flags2: " << Material.Flags2 << std::endl;
		FileStream << "Submesh Index: " << Material.iSubset << std::endl;
		FileStream << "Submesh Index 2: " << Material.iSubset2 << std::endl;
		FileStream << "Color Index: " << Material.iColor << std::endl;
		FileStream << "Render Flags: " << Material.iRenderFlags << std::endl;
		FileStream << "Material Index: " << Material.iUnit << std::endl;
		FileStream << "iShader: " << Material.iShader << std::endl;
		FileStream << "Texture Index: " << Material.iTexture << std::endl;
		FileStream << "Texture Unit Index: " << Material.iTexutreUnit << std::endl;
		FileStream << "Transparency Index: " << Material.iTransparency << std::endl;
		FileStream << "Texture Animation Index: " << Material.iTextureAnimation << std::endl << std::endl;
	}
	FileStream << std::endl << std::endl;

	FileStream.close();

	return true;
}


M2Lib::CM2Skin::CSubset* M2Lib::CM2Skin::GetSubset( UInt32 ID )
{
	CSubset* Subsets = (CSubset*)Elements[EElement_Subsets].Data;
	UInt32 Count = Elements[EElement_Subsets].Number;
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
	Elements[EElement_Indices].Number = Header.nIndices;
	Elements[EElement_Indices].Offset = Header.oIndices;

	Elements[EElement_Triangles].Number = Header.nTriangles;
	Elements[EElement_Triangles].Offset = Header.oTriangles;

	Elements[EElement_Properties].Number = Header.nProperites;
	Elements[EElement_Properties].Offset = Header.oProperties;

	Elements[EElement_Subsets].Number = Header.nSubsets;
	Elements[EElement_Subsets].Offset = Header.oSubsets;

	Elements[EElement_Materials].Number = Header.nMaterials;
	Elements[EElement_Materials].Offset = Header.oMaterials;
}


void M2Lib::CM2Skin::m_LoadElements_FindSizes( UInt32 FileSize )
{
	for ( UInt32 i = 0; i != EElement__Count__; i++ )
	{
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
	UInt32 CurrentOffset = sizeof(CM2SkinHeader);
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
	Header.nIndices = Elements[EElement_Indices].Number;
	Header.oIndices = Elements[EElement_Indices].Offset;

	Header.nTriangles = Elements[EElement_Triangles].Number;
	Header.oTriangles = Elements[EElement_Triangles].Offset;

	Header.nProperites = Elements[EElement_Properties].Number;
	Header.oProperties = Elements[EElement_Properties].Offset;

	Header.nSubsets = Elements[EElement_Subsets].Number;
	Header.oSubsets = Elements[EElement_Subsets].Offset;

	Header.nMaterials = Elements[EElement_Materials].Number;
	Header.oMaterials = Elements[EElement_Materials].Offset;
}
