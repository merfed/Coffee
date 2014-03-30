#include "CM2.h"
#include "CM2SkinBuilder.h"
#include "CBinaryStream.h"
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <iostream>


// level of detail for output messages
int g_Verbose = 1;


M2Lib::EError M2Lib::CM2::Load( Char8* szFile )
{
	if ( !szFile )
		return M2Lib::EError_FailedToLoadM2_NoFileSpecified;

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
	if ( FileStream.fail() )
		return M2Lib::EError_FailedToLoadM2_CouldNotOpenFile;

	// find file size
	FileStream.seekg( 0, std::ios::end );
	UInt32 FileSize = FileStream.tellg();
	m_OriginalSize = FileSize;
	FileStream.seekg( 0, std::ios::beg );

	// load entire thing
	RawData = new UInt8[FileSize];
	FileStream.read( (Char8*)RawData, FileSize );
	FileStream.seekg( 0, std::ios::beg );

	// load header
	FileStream.read( (Char8*)&Header.Description, sizeof( Header.Description ) );

	// check header
	if ( Header.Description.ID[0] != 'M' || Header.Description.ID[1] != 'D' || Header.Description.ID[2] != '2' || Header.Description.ID[3] != '0' )
		return M2Lib::EError_FailedToLoadM2_FileCorrupt;
	UInt32 VersionInt = (UInt32&)Header.Description.Version;
	if ( ( 263 > VersionInt ) || ( VersionInt > 272 ) )
		return M2Lib::EError_FailedToLoadM2_VersionNotSupported;

	// load more header
	if ( Header.Description.Flags & 0x08 )
	{
		// special long header
		FileStream.read( (Char8*)&Header.Elements, sizeof( Header.Elements ) );
	}
	else
	{
		// normal header
		FileStream.read( (Char8*)&Header.Elements, sizeof( Header.Elements ) - 8 );
		Header.Elements.nUnknown1 = 0;
		Header.Elements.oUnknown1 = 0;
	}

	// fill elements header data
	m_LoadElements_CopyHeaderToElements();
	m_LoadElements_FindSizes( FileSize );

	// load elements
	UInt32 ElementCount = EElement__Count__;
	if ( !( Header.Description.Flags & 0x08 ) )
	{
		ElementCount--;
	}
	for ( UInt32 i = 0; i != ElementCount; i++ )
	{
		Elements[i].Align = 16;
		if ( !Elements[i].Load( FileStream ) )
			return M2Lib::EError_FailedToLoadM2_FileCorrupt;
	}

	// close file stream
	FileStream.close();

	// load skins
	if ( ( Header.Elements.nSkin == 0 ) || ( Header.Elements.nSkin > 4 ) )
		return M2Lib::EError_FailedToLoadM2_FileCorrupt;
	for ( UInt32 i = 0; i != Header.Elements.nSkin; i++ )
	{
		Char8 szFileSkin[1024];
		GetFileSkin( szFileSkin, szFile, i );

		Skins[i] = new M2Lib::CM2Skin( this );
		if ( M2Lib::EError Error = Skins[i]->Load( szFileSkin ) )
			return Error;
	}

	// print info
	//PrintInfo();

	// done
	return M2Lib::EError_OK;
}


M2Lib::EError M2Lib::CM2::Save( Char8* szFile )
{
	if ( !szFile )
		return M2Lib::EError_FailedToSaveM2_NoFileSpecified;

	// open file stream
	std::fstream FileStream;
	FileStream.open( szFile, std::ios::out | std::ios::trunc | std::ios::binary  );
	if ( FileStream.fail() )
		return M2Lib::EError_FailedToSaveM2;

	// fill elements header data
	m_SaveElements_FindOffsets();
	m_SaveElements_CopyElementsToHeader();

	// save header
	FileStream.write( (Char8*)&Header.Description, sizeof( Header.Description ) );

	if ( Header.Description.Flags & 0x08 )
	{
		// long header
		FileStream.write( (Char8*)&Header.Elements, sizeof( Header.Elements ) );
	}
	else
	{
		// normal header
		FileStream.write( (Char8*)&Header.Elements, sizeof( Header.Elements ) - 8 );
	}

	// save elements
	UInt32 ElementCount = EElement__Count__;
	if ( !( Header.Description.Flags & 0x08 ) )
	{
		ElementCount--;
	}
	for ( UInt32 i = 0; i != ElementCount; i++ )
	{
		if ( !Elements[i].Save( FileStream ) )
			return M2Lib::EError_FailedToSaveM2;
	}

	if ( Element_NewVertex.DataSize )
	{
		if ( !Element_NewVertex.Save( FileStream ) )
			return M2Lib::EError_FailedToSaveM2;
	}

	// close file stream
	FileStream.close();

	// delete existing skin files
	for ( UInt32 i = 0; i < 4; i++ )
	{
		Char8 szFileSkin[1024];
		GetFileSkin( szFileSkin, szFile, i );
		remove( szFileSkin );
	}

	// save skins
	if ( ( Header.Elements.nSkin == 0 ) || ( Header.Elements.nSkin > 4 ) )
		return M2Lib::EError_FailedToSaveM2;
	for ( UInt32 i = 0; i < Header.Elements.nSkin; i++ )
	{
		Char8 szFileSkin[1024];
		GetFileSkin( szFileSkin, szFile, i );

		if ( M2Lib::EError Error = Skins[i]->Save( szFileSkin ) )
			return Error;
	}

	return M2Lib::EError_OK;
}


M2Lib::EError M2Lib::CM2::ExportM2Intermediate( Char8* szFile )
{
	std::fstream File;
	File.open( szFile, std::ios::out | std::ios::trunc | std::ios::binary );
	if ( File.fail() )
		return M2Lib::EError_FailedToExportM2I_CouldNotOpenFile;

	M2Lib::CBinaryStream BWriter( &File );

	CM2Skin* pSkin = Skins[0];

	UInt32 SubsetCount = pSkin->Elements[CM2Skin::EElement_Subset].Number;;
	CM2Skin::CElement_Subset* Subsets = (CM2Skin::CElement_Subset*)pSkin->Elements[CM2Skin::EElement_Subset].Data;

	M2Lib::CVertex* Vertices = (CVertex*)GetElement_Vertex().Data;
	UInt16* Triangles = (UInt16*)pSkin->Elements[CM2Skin::EElement_Index].Data;
	UInt16* Indices = (UInt16*)pSkin->Elements[CM2Skin::EElement_Vertex].Data;

	UInt32 BonesCount = Elements[EElement_Bone].Number;
	CElement_Bone* Bones = (CElement_Bone*)Elements[EElement_Bone].Data;

	UInt32 AttachmentsCount = Elements[EElement_Attachment].Number;
	CElement_Attachment* Attachments = (CElement_Attachment*)Elements[EElement_Attachment].Data;

	UInt32 CamerasCount = Elements[EElement_Camera].Number;
	CElement_Camera* Cameras = (CElement_Camera*)Elements[EElement_Camera].Data;

	// save signature
	UInt32 OutSignature = Signature_M2I0;
	BWriter.Write( &OutSignature, sizeof( UInt32 ) );

	// save version
	UInt16 VersionMajor = 4;
	UInt16 VersionMinor = 0;
	BWriter.Write( &VersionMajor, sizeof( UInt16 ) );
	BWriter.Write( &VersionMinor, sizeof( UInt16 ) );

	// save subsets
	UInt32 TotalVertexCount = 0;
	UInt32 TotalTriangleCount = 0;
	BWriter.Write( &SubsetCount, sizeof( UInt32 ) );
	for ( UInt32 i = 0; i < SubsetCount; i++ )
	{
		M2Lib::CM2Skin::CElement_Subset* pSubsetOut = &Subsets[i];

		// write subset ID
		UInt32 SubsetID = pSubsetOut->ID;
		BWriter.Write( &SubsetID, sizeof( UInt32 ) );

		// find materials for this subset
		std::vector< UInt32 > MaterialsIndexes;
		UInt32 MaterialCount = pSkin->Elements[M2Lib::CM2Skin::EElement_Material].Number;
		M2Lib::CM2Skin::CElement_Material* Materials = (M2Lib::CM2Skin::CElement_Material*)pSkin->Elements[M2Lib::CM2Skin::EElement_Material].Data;
		for ( UInt32 j = 0; j < MaterialCount; j++ )
		{
			if ( Materials[j].iSubset == i )
			{
				MaterialsIndexes.push_back( j );
			}
		}
		assert( MaterialsIndexes.size() );

		// save materials
		MaterialCount = MaterialsIndexes.size();
		BWriter.Write( &MaterialCount, sizeof( UInt32 ) );
		for ( UInt32 j = 0; j < MaterialCount; j++ )
		{
			M2Lib::CM2Skin::CElement_Material* pMaterial = &Materials[MaterialsIndexes[j]];

			BWriter.Write( &pMaterial->Flags1, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->Flags2, sizeof( UInt16 ) );
			//BWriter.Write( &pMaterial->iSubset, sizeof( UInt16 ) );
			//BWriter.Write( &pMaterial->iSubset2, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iColor, sizeof( SInt16 ) );
			BWriter.Write( &pMaterial->iRenderFlags, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iUnit, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iShader, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iTexture, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iTexutreUnit, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iTransparency, sizeof( UInt16 ) );
			BWriter.Write( &pMaterial->iTextureAnimation, sizeof( UInt16 ) );
		}

		// get first vertex
		UInt32 SubsetVertexStart = pSubsetOut->VertexStart;

		// write vertices
		UInt32 SubsetVertexCountOut = pSubsetOut->VertexCount;
		BWriter.Write( &SubsetVertexCountOut, sizeof( UInt32 ) );
		UInt32 VertexEnd = pSubsetOut->VertexStart + pSubsetOut->VertexCount;
		for ( UInt32 k = pSubsetOut->VertexStart; k < VertexEnd; k++ )
		{
			M2Lib::CVertex& Vertex = Vertices[Indices[k]];

			BWriter.Write( &Vertex.Position[0], sizeof( Float32 ) );
			BWriter.Write( &Vertex.Position[1], sizeof( Float32 ) );
			BWriter.Write( &Vertex.Position[2], sizeof( Float32 ) );

			BWriter.Write( &Vertex.BoneWeights[0], sizeof( UInt8 ) );
			BWriter.Write( &Vertex.BoneWeights[1], sizeof( UInt8 ) );
			BWriter.Write( &Vertex.BoneWeights[2], sizeof( UInt8 ) );
			BWriter.Write( &Vertex.BoneWeights[3], sizeof( UInt8 ) );

			BWriter.Write( &Vertex.Bones[0], sizeof( UInt8 ) );
			BWriter.Write( &Vertex.Bones[1], sizeof( UInt8 ) );
			BWriter.Write( &Vertex.Bones[2], sizeof( UInt8 ) );
			BWriter.Write( &Vertex.Bones[3], sizeof( UInt8 ) );

			BWriter.Write( &Vertex.Normal[0], sizeof( Float32 ) );
			BWriter.Write( &Vertex.Normal[1], sizeof( Float32 ) );
			BWriter.Write( &Vertex.Normal[2], sizeof( Float32 ) );

			BWriter.Write( &Vertex.Texture[0], sizeof( Float32 ) );
			BWriter.Write( &Vertex.Texture[1], sizeof( Float32 ) );
		}
		TotalVertexCount += pSubsetOut->VertexCount;

		// write triangles
		UInt32 SubsetTriangleCountOut = pSubsetOut->IndexCount / 3;
		BWriter.Write( &SubsetTriangleCountOut, sizeof( UInt32 ) );
		UInt32 IndexEnd = pSubsetOut->IndexStart + pSubsetOut->IndexCount;
		for ( UInt32 k = pSubsetOut->IndexStart; k < IndexEnd; k++ )
		{
			UInt16 IndexOut = Triangles[k] - SubsetVertexStart;
			assert( IndexOut < SubsetVertexCountOut );
			BWriter.Write( &IndexOut, sizeof( UInt16 ) );
		}
		TotalTriangleCount += pSubsetOut->IndexCount;

		// write unknown values
		BWriter.Write( &pSubsetOut->Unknown1, sizeof( UInt16 ) );
		BWriter.Write( &pSubsetOut->Unknown2, sizeof( UInt16 ) );

		// write flags
	}

	// write bones
	BWriter.Write( &BonesCount, sizeof( UInt32 ) );
	for ( UInt16 i = 0; i < BonesCount; i++ )
	{
		CElement_Bone& Bone = Bones[i];

		BWriter.Write( &i, sizeof( UInt16 ) );
		BWriter.Write( &Bone.ParentBone, sizeof( SInt16 ) );
		BWriter.Write( &Bone.Position[0], sizeof( Float32 ) );
		BWriter.Write( &Bone.Position[1], sizeof( Float32 ) );
		BWriter.Write( &Bone.Position[2], sizeof( Float32 ) );
	}

	// write attachments
	BWriter.Write( &AttachmentsCount, sizeof( UInt32 ) );
	for ( UInt16 i = 0; i < AttachmentsCount; i++ )
	{
		CElement_Attachment& Attachment = Attachments[i];

		BWriter.Write( &Attachment.ID, sizeof( UInt32 ) );
		BWriter.Write( &Attachment.ParentBone, sizeof( SInt16 ) );
		BWriter.Write( &Attachment.Position[0], sizeof( Float32 ) );
		BWriter.Write( &Attachment.Position[1], sizeof( Float32 ) );
		BWriter.Write( &Attachment.Position[2], sizeof( Float32 ) );
	}

	// write cameras
	BWriter.Write( &CamerasCount, sizeof( UInt32 ) );
	for ( UInt16 i = 0; i < CamerasCount; i++ )
	{
		CElement_Camera& Camera = Cameras[i];

		BWriter.Write( &Camera.Type, sizeof( SInt32 ) );

		// extract field of view of camera from animation block
		if ( Camera.AnimationBlock_FieldOfView.nKeys > 0 )
		{
			SExternalAnimation* ExternalAnimations = (SExternalAnimation*)Elements[EElement_Camera].GetLocalPointer( Camera.AnimationBlock_FieldOfView.oKeys );
			Float32* FieldOfView_Keys = (Float32*)(&RawData[ExternalAnimations[0].Offset]);
			BWriter.Write( &FieldOfView_Keys[0], sizeof( Float32 ) );
		}
		else
		{
			Float32 TempFoV = 0.785398163f;	// 45 degrees in radians, assuming that WoW stores camera FoV in radians. or maybe it's half FoV.
			BWriter.Write( &TempFoV, sizeof( Float32 ) );
		}
		
		BWriter.Write( &Camera.ClipFar, sizeof( Float32 ) );
		BWriter.Write( &Camera.ClipNear, sizeof( Float32 ) );
		BWriter.Write( &Camera.Position[0], sizeof( Float32 ) );
		BWriter.Write( &Camera.Position[1], sizeof( Float32 ) );
		BWriter.Write( &Camera.Position[2], sizeof( Float32 ) );
		BWriter.Write( &Camera.Target[0], sizeof( Float32 ) );
		BWriter.Write( &Camera.Target[1], sizeof( Float32 ) );
		BWriter.Write( &Camera.Target[2], sizeof( Float32 ) );
	}

	File.close();

	return M2Lib::EError_OK;
}


M2Lib::EError M2Lib::CM2::ImportM2Intermediate( Char8* szFile, bool IgnoreMaterials, bool IgnoreBones, bool IgnoreAttachments, bool IgnoreCameras, Float32 SubmeshPositionalTolerance, Float32 SubmeshAngularTolerance, Float32 BodyPositionalTolerance, Float32 BodyAngularTolerance, Float32 ClothingPositionalTolerance, Float32 ClothingAngularTolerance )
{
	if ( !szFile )
		return EError_FailedToImportM2I_NoFileSpecified;

	// check that we have an M2 already loaded and ready to be injected
	if ( Elements[EElement_Skin].Number == 0 )
		return M2Lib::EError_FailedToExportM2I_M2NotLoaded;

	if ( g_Verbose >= 1 )
	{
		std::cout << "\tloading M2I..." << std::endl;
	}

	M2Lib::CM2I InM2I;

	// imported list of vertices are stored here. this will be the new global vertex list.
	//M2Lib::CVertexListBuilder NewVertexList;
	std::vector< CVertex > NewVertexList;
	// map of indices from input file to imported
	std::vector< UInt32 > NewVertexMap;

	// open the file
	std::fstream File;
	File.open( szFile, std::ios::in | std::ios::binary );
	if ( File.fail() )
		return M2Lib::EError_FailedToImportM2I_CouldNotOpenFile;
	M2Lib::CBinaryStream BReader( &File );

	// load signature
	UInt32 InSignature = 0;
	BReader.Read( &InSignature, sizeof( UInt32 ) );
	if ( InSignature == 1 )
	{
	}
	else if ( InSignature == Signature_M2I0 )
	{
	}
	else
	{
		return M2Lib::EError_FailedToImportM2I_FileCorrupt;
	}

	// load version
	UInt16 VersionMajor;
	UInt16 VersionMinor;
	if ( InSignature == Signature_M2I0 )
	{
		BReader.Read( &VersionMajor, sizeof( UInt16 ) );
		BReader.Read( &VersionMinor, sizeof( UInt16 ) );
		if ( VersionMajor == 4 )
		{
			if ( VersionMinor == 0 )
			{
			}
			else
			{
				return EError_FailedToImportM2I_UnsupportedVersion;
			}
		}
		else
		{
			return EError_FailedToImportM2I_UnsupportedVersion;
		}
	}

	// load subsets, build new global vertex list
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tloading subsets..." << std::endl;
	}
	UInt32 VertexStart = 0;
	UInt32 InSubsetCount = 0;
	BReader.Read( &InSubsetCount, sizeof( UInt32 ) );
	UInt32 iTriangle = 0;
	for ( UInt32 i = 0; i < InSubsetCount; i++ )
	{
		M2Lib::CM2I::CSubset* pNewSubset = new M2Lib::CM2I::CSubset();

		// read id
		BReader.Read( &pNewSubset->ID, sizeof( UInt32 ) );

		if ( InSignature == 1 )
		{
			pNewSubset->Unknown1 = 0;
			BReader.Read( &pNewSubset->Unknown2, sizeof( UInt16 ) );
		}

		// read materials
		UInt32 InMaterialCount;
		BReader.Read( &InMaterialCount, sizeof( UInt32 ) );
		for ( UInt32 j = 0; j < InMaterialCount; j++ )
		{
			M2Lib::CM2Skin::CElement_Material NewMaterial;

			BReader.Read( &NewMaterial.Flags1, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.Flags2, sizeof( UInt16 ) );
			NewMaterial.iSubset = i;
			NewMaterial.iSubset2 = i;
			BReader.Read( &NewMaterial.iColor, sizeof( SInt16 ) );
			BReader.Read( &NewMaterial.iRenderFlags, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.iUnit, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.iShader, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.iTexture, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.iTexutreUnit, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.iTransparency, sizeof( UInt16 ) );
			BReader.Read( &NewMaterial.iTextureAnimation, sizeof( UInt16 ) );

			pNewSubset->Materials.push_back( NewMaterial );
		}

		// read vertices
		UInt32 InVertexCount = 0;
		BReader.Read( &InVertexCount, sizeof( UInt32 ) );
		if ( InVertexCount > 0xFFFF )
		{
			return M2Lib::EError_FailedToImportM2I_TooManyVertices;
		}
		//NewVertexList.ClearVertexCache();	// clear cache to prevent vertex crossover between subsets.
		for ( UInt32 j = 0; j < InVertexCount; j++ )
		{
			M2Lib::CVertex InVertex;

			BReader.Read( &InVertex.Position[0], sizeof( Float32 ) );
			BReader.Read( &InVertex.Position[1], sizeof( Float32 ) );
			BReader.Read( &InVertex.Position[2], sizeof( Float32 ) );

			BReader.Read( &InVertex.BoneWeights[0], sizeof( UInt8 ) );
			BReader.Read( &InVertex.BoneWeights[1], sizeof( UInt8 ) );
			BReader.Read( &InVertex.BoneWeights[2], sizeof( UInt8 ) );
			BReader.Read( &InVertex.BoneWeights[3], sizeof( UInt8 ) );

			BReader.Read( &InVertex.Bones[0], sizeof( UInt8 ) );
			BReader.Read( &InVertex.Bones[1], sizeof( UInt8 ) );
			BReader.Read( &InVertex.Bones[2], sizeof( UInt8 ) );
			BReader.Read( &InVertex.Bones[3], sizeof( UInt8 ) );

			BReader.Read( &InVertex.Normal[0], sizeof( Float32 ) );
			BReader.Read( &InVertex.Normal[1], sizeof( Float32 ) );
			BReader.Read( &InVertex.Normal[2], sizeof( Float32 ) );

			BReader.Read( &InVertex.Texture[0], sizeof( Float32 ) );
			BReader.Read( &InVertex.Texture[1], sizeof( Float32 ) );

			//UInt16 VertexIndex = NewVertexList.Add( InVertex );	// if similar vertex already exists, we will get an existing index, thereby removing a duplicate vertex.
			UInt16 VertexIndex = NewVertexList.size();
			if ( VertexIndex > 0xFFFF )
			{
				return M2Lib::EError_FailedToImportM2I_TooManyVertices;
			}
			NewVertexList.push_back( InVertex );
			NewVertexMap.push_back( VertexIndex );
			pNewSubset->Indices.push_back( VertexIndex );
		}

		// read triangles
		UInt32 InTriangleCount = 0;
		BReader.Read( &InTriangleCount, sizeof( UInt32 ) );
		for ( UInt32 j = 0; j < InTriangleCount; j++ )
		{
			M2Lib::CTriangle NewTriangle;

			NewTriangle.Index = iTriangle;
			iTriangle++;

			BReader.Read( &NewTriangle.Vertices[0], sizeof( UInt16 ) );
			BReader.Read( &NewTriangle.Vertices[1], sizeof( UInt16 ) );
			BReader.Read( &NewTriangle.Vertices[2], sizeof( UInt16 ) );

			// remap input indices from local vertex list to global vertex list
			NewTriangle.Vertices[0] += VertexStart;
			NewTriangle.Vertices[1] += VertexStart;
			NewTriangle.Vertices[2] += VertexStart;

			NewTriangle.Vertices[0] = NewVertexMap[NewTriangle.Vertices[0]];
			NewTriangle.Vertices[1] = NewVertexMap[NewTriangle.Vertices[1]];
			NewTriangle.Vertices[2] = NewVertexMap[NewTriangle.Vertices[2]];

			pNewSubset->Triangles.push_back( NewTriangle );
		}

		VertexStart += InVertexCount;

		// read unknowns
		if ( InSignature == Signature_M2I0 )
		{
			BReader.Read( &pNewSubset->Unknown1, sizeof( UInt16 ) );
			BReader.Read( &pNewSubset->Unknown2, sizeof( UInt16 ) );
		}

		InM2I.Subsets.push_back( pNewSubset );
	}

	if ( !IgnoreBones )
	{
		// read bones, overwrite existing
		if ( g_Verbose >= 2 )
		{
			std::cout << "\t\tloading bones..." << std::endl;
		}
		UInt32 BoneCount = Elements[EElement_Bone].Number;
		CElement_Bone* Bones = (CElement_Bone*)Elements[EElement_Bone].Data;
		UInt32 BoneCountIn;
		BReader.Read( &BoneCountIn, sizeof( UInt32 ) );
		for ( UInt32 i = 0; i < BoneCountIn; i++ )
		{
			UInt16 InBoneIndex = 0;
			BReader.Read( &InBoneIndex, sizeof( UInt16 ) );
			assert( InBoneIndex < BoneCount );
			CElement_Bone& BoneToMod = Bones[InBoneIndex];

			BReader.Read( &BoneToMod.ParentBone, sizeof( SInt16 ) );

			BReader.Read( &BoneToMod.Position[0], sizeof( Float32 ) );
			BReader.Read( &BoneToMod.Position[1], sizeof( Float32 ) );
			BReader.Read( &BoneToMod.Position[2], sizeof( Float32 ) );
		}
	}

	if ( !IgnoreAttachments )
	{
		// read attachments, overwrite existing
		if ( g_Verbose >= 2 )
		{
			std::cout << "\t\tloading attachments..." << std::endl;
		}
		UInt32 AttachmentsCount = Elements[EElement_Attachment].Number;
		CElement_Attachment* Attachments = (CElement_Attachment*)Elements[EElement_Attachment].Data;
		UInt32 AttachmentCountIn;
		BReader.Read( &AttachmentCountIn, sizeof( UInt32 ) );
		for ( UInt32 i = 0; i < AttachmentCountIn; i++ )
		{
			UInt32 InAttachmentID = 0;
			BReader.Read( &InAttachmentID, sizeof( UInt32 ) );
			CElement_Attachment* pAttachmentToMod = 0;
			for ( UInt32 j = 0; j < AttachmentsCount; j++ )
			{
				if ( Attachments[j].ID == InAttachmentID )
				{
					pAttachmentToMod = &Attachments[j];
					break;
				}
			}
			if ( pAttachmentToMod )
			{
				BReader.Read( &pAttachmentToMod->ParentBone, sizeof( SInt16 ) );

				BReader.Read( &pAttachmentToMod->Position[0], sizeof( Float32 ) );
				BReader.Read( &pAttachmentToMod->Position[1], sizeof( Float32 ) );
				BReader.Read( &pAttachmentToMod->Position[2], sizeof( Float32 ) );
			}
			else
			{
				UInt32 Dummy;

				BReader.Read( &Dummy, sizeof( SInt16 ) );

				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
			}
		}
	}

	if ( !IgnoreCameras )
	{
		// read cameras, overwrite existing
		if ( g_Verbose >= 2 )
		{
			std::cout << "\t\tloading cameras..." << std::endl;
		}
		UInt32 CameraCount = Elements[EElement_Camera].Number;
		CElement_Camera* Cameras = (CElement_Camera*)Elements[EElement_Camera].Data;
		UInt32 CameraCountIn;
		BReader.Read( &CameraCountIn, sizeof( UInt32 ) );
		for ( UInt32 i = 0; i < CameraCountIn; i++ )
		{
			SInt32 InType;
			BReader.Read( &InType, sizeof( SInt32 ) );

			CElement_Camera* pCameraToMod = 0;
			for ( UInt32 j = 0; j < CameraCount; j++ )
			{
				if ( Cameras[j].Type == InType )
				{
					pCameraToMod = &Cameras[j];
					break;
				}
			}
			if ( pCameraToMod )
			{
				pCameraToMod->Type = InType;

				if ( pCameraToMod->AnimationBlock_FieldOfView.nKeys > 0 )
				{
					SExternalAnimation* ExternalAnimations = (SExternalAnimation*)Elements[EElement_Camera].GetLocalPointer( pCameraToMod->AnimationBlock_FieldOfView.oKeys );
					Float32* FieldOfView_Keys = (Float32*)(&RawData[ExternalAnimations[0].Offset]);
					Float32 InFoV;
					BReader.Read( &InFoV, sizeof( Float32 ) );	// we are writing to RawData in memory, this change will not be saved to the M2 because it lies out of bounds of any elements.
					FieldOfView_Keys[0] = InFoV;
				}
				else
				{
					Float32 TempFoV;
					BReader.Read( &TempFoV, sizeof( Float32 ) );
				}
				BReader.Read( &pCameraToMod->ClipFar, sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->ClipNear, sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->Position[0], sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->Position[1], sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->Position[2], sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->Target[0], sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->Target[1], sizeof( Float32 ) );
				BReader.Read( &pCameraToMod->Target[2], sizeof( Float32 ) );
			}
			else
			{
				UInt32 Dummy;

				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
				BReader.Read( &Dummy, sizeof( Float32 ) );
			}
		}
	}

	// build the skins
	M2Lib::CVertex* GlobalVertexList = &NewVertexList[0];
	M2Lib::CM2SkinBuilder SkinBuilder;

	void* pIn;
	void* pOut;

	// this list will store the new skins
	M2Lib::CM2Skin* NewSkins[4];
	NewSkins[0] = 0;
	NewSkins[1] = 0;
	NewSkins[2] = 0;
	NewSkins[3] = 0;

	// for easy looping
	UInt32 LoDList[5];
	LoDList[0] = 256;
	LoDList[1] = 64;
	LoDList[2] = 53;
	LoDList[3] = 21;
	LoDList[4] = 0;		// extra entry needed for LoD check

	// build the skins
	std::vector< UInt16 > NewBoneLookup;
	SInt32 BoneStart = 0;
	Elements[EElement_Skin].Number = 0;
	UInt32 iSkin = 0;
	for ( UInt32 iLoD = 0; iLoD < 4; iLoD++ )
	{
		if ( g_Verbose >= 1 )
		{
			std::cout << "\tbuilding skin at LoD " << LoDList[iLoD] << "..." << std::endl;
		}
		M2Lib::CM2Skin* pNewSkin = new M2Lib::CM2Skin( this );
		if ( SkinBuilder.Build( pNewSkin, LoDList[iLoD], &InM2I, GlobalVertexList, BoneStart ) )
		{
			// if there are more bones than the next lowest level of detail
			if ( SkinBuilder.m_Bones.size() > LoDList[iLoD + 1] )
			{
				//if ( pNewSkin->Elements[M2Lib::CM2Skin::EElement_Indices].Number > 0xFFFF )
				//{
				//	return M2Lib::EError_FailedToImportM2I_SkinHasTooManyIndices;
				//}

				// copy skin to result list
				NewSkins[iSkin] = pNewSkin;
				iSkin++;

				// copy skin's bone lookup to the global bone lookup list
				for ( UInt32 i = 0; i < SkinBuilder.m_Bones.size(); i++ )
				{
					NewBoneLookup.push_back( SkinBuilder.m_Bones[i] );
				}

				// advance for where next skin's bone lookup will begin
				BoneStart += SkinBuilder.m_Bones.size();
			}
			else
			{
				if ( g_Verbose >= 1 )
				{
					std::cout << "\t\tskin discarded, bone count too low." << LoDList[iLoD] << std::endl;
				}
				delete pNewSkin;
			}
		}
		else
		{
			int WhatToDo = 1;
		}
	}

	// set skin count
	Elements[EElement_Skin].Number = iSkin;
	if ( iSkin == 0 )
	{
		int pause = 1;
	}

	if ( IgnoreMaterials )
	{
		// copy materials from old subsets to new subsets
		for ( UInt32 i = 0; i < 4; i++ )
		{
			if ( NewSkins[i] )
			{
				NewSkins[i]->CopyMaterials( Skins[0] );
			}
		}
	}

	// replace old skins with new
	for ( UInt32 i = 0; i < 4; i++ )
	{
		if ( Skins[i] )
		{
			delete Skins[i];
		}
		Skins[i] = NewSkins[i];
	}

	// copy new global vertex list
	if ( true )
	{
		Elements[EElement_Vertex].SetDataSize( NewVertexList.size(), NewVertexList.size() * sizeof( M2Lib::CVertex ), false );
		pIn = &NewVertexList[0];
		pOut = Elements[EElement_Vertex].Data;
		memcpy( pOut, pIn, NewVertexList.size() * sizeof( M2Lib::CVertex ) );
	}
	else
	{
		Element_NewVertex.SetDataSize( NewVertexList.size(), NewVertexList.size() * sizeof( M2Lib::CVertex ), false );
		Element_NewVertex.Offset = Elements[EElement_Vertex].Offset;
		Element_NewVertex.OffsetOriginal = Elements[EElement_Vertex].OffsetOriginal;
		pIn = &NewVertexList[0];
		pOut = Element_NewVertex.Data;
		memcpy( pOut, pIn, NewVertexList.size() * sizeof( M2Lib::CVertex ) );
	}

	// copy new global bone lookup
	Elements[EElement_SkinnedBoneLookup].SetDataSize( NewBoneLookup.size(), NewBoneLookup.size() * sizeof( UInt16 ), false );
	pIn = &NewBoneLookup[0];
	pOut = Elements[EElement_SkinnedBoneLookup].Data;
	memcpy( pOut, pIn, NewBoneLookup.size() * sizeof( UInt16 ) );

	// build vertex properties for each skin
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tbuilding skin vertex bone indices..." << std::endl;
	}
	BuildVertexBoneIndices();

	// build max bones used per subset
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tbuilding skin max bones per subset..." << std::endl;
	}
	BuildMaxBones();

	// fix normals within submeshes 
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tfixing gaps and normals within submeshes..." << std::endl;
	}
	FixSeamsSubmesh( SubmeshPositionalTolerance, SubmeshAngularTolerance );

	// fix normals between body submeshes
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tfixing gaps and normals between body submeshes..." << std::endl;
	}
	FixSeamsBody( BodyPositionalTolerance, BodyAngularTolerance );

	// close gaps between clothes and body
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tfixing gaps and normals between clothing submeshes and body submeshes..." << std::endl;
	}
	FixSeamsClothing( ClothingPositionalTolerance, ClothingAngularTolerance );





	//
	//
	//
	// build skins again
	// because bone indices and weights might have changed during seam and gap fixing
	NewSkins[0] = 0;
	NewSkins[1] = 0;
	NewSkins[2] = 0;
	NewSkins[3] = 0;

	NewBoneLookup.clear();
	BoneStart = 0;
	Elements[EElement_Skin].Number = 0;
	iSkin = 0;
	for ( UInt32 iLoD = 0; iLoD < 4; iLoD++ )
	{
		if ( g_Verbose >= 1 )
		{
			std::cout << "\tbuilding skin at LoD " << LoDList[iLoD] << "..." << std::endl;
		}
		M2Lib::CM2Skin* pNewSkin = new M2Lib::CM2Skin( this );
		if ( SkinBuilder.Build( pNewSkin, LoDList[iLoD], &InM2I, GlobalVertexList, BoneStart ) )
		{
			// if there are more bones than the next lowest level of detail
			if ( SkinBuilder.m_Bones.size() > LoDList[iLoD + 1] )
			{
				//if ( pNewSkin->Elements[M2Lib::CM2Skin::EElement_Indices].Number > 0xFFFF )
				//{
				//	return M2Lib::EError_FailedToImportM2I_SkinHasTooManyIndices;
				//}

				// copy skin to result list
				NewSkins[iSkin] = pNewSkin;
				iSkin++;

				// copy skin's bone lookup to the global bone lookup list
				for ( UInt32 i = 0; i < SkinBuilder.m_Bones.size(); i++ )
				{
					NewBoneLookup.push_back( SkinBuilder.m_Bones[i] );
				}

				// advance for where next skin's bone lookup will begin
				BoneStart += SkinBuilder.m_Bones.size();
			}
			else
			{
				if ( g_Verbose >= 1 )
				{
					std::cout << "\t\tskin discarded, bone count too low." << LoDList[iLoD] << std::endl;
				}
				delete pNewSkin;
			}
		}
		else
		{
			int WhatToDo = 1;
		}
	}

	// set skin count
	Elements[EElement_Skin].Number = iSkin;
	if ( iSkin == 0 )
	{
		int pause = 1;
	}

	if ( IgnoreMaterials )
	{
		// copy materials from old subsets to new subsets
		for ( UInt32 i = 0; i < 4; i++ )
		{
			if ( NewSkins[i] )
			{
				NewSkins[i]->CopyMaterials( Skins[0] );
			}
		}
	}

	// replace old skins with new
	for ( UInt32 i = 0; i < 4; i++ )
	{
		if ( Skins[i] )
		{
			delete Skins[i];
		}
		Skins[i] = NewSkins[i];
	}

	// copy new global vertex list
	if ( true )
	{
		Elements[EElement_Vertex].SetDataSize( NewVertexList.size(), NewVertexList.size() * sizeof( M2Lib::CVertex ), false );
		pIn = &NewVertexList[0];
		pOut = Elements[EElement_Vertex].Data;
		memcpy( pOut, pIn, NewVertexList.size() * sizeof( M2Lib::CVertex ) );
	}
	else
	{
		Element_NewVertex.SetDataSize( NewVertexList.size(), NewVertexList.size() * sizeof( M2Lib::CVertex ), false );
		Element_NewVertex.Offset = Elements[EElement_Vertex].Offset;
		Element_NewVertex.OffsetOriginal = Elements[EElement_Vertex].OffsetOriginal;
		pIn = &NewVertexList[0];
		pOut = Element_NewVertex.Data;
		memcpy( pOut, pIn, NewVertexList.size() * sizeof( M2Lib::CVertex ) );
	}

	// copy new global bone lookup
	Elements[EElement_SkinnedBoneLookup].SetDataSize( NewBoneLookup.size(), NewBoneLookup.size() * sizeof( UInt16 ), false );
	pIn = &NewBoneLookup[0];
	pOut = Elements[EElement_SkinnedBoneLookup].Data;
	memcpy( pOut, pIn, NewBoneLookup.size() * sizeof( UInt16 ) );


	// build vertex properties for each skin
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tbuilding skin vertex bone indices again..." << std::endl;
	}
	BuildVertexBoneIndices();

	// build max bones used per subset
	if ( g_Verbose >= 1 )
	{
		std::cout << "\tbuilding skin max bones per subset again..." << std::endl;
	}
	BuildMaxBones();


	// done, ready to be saved
	return M2Lib::EError_OK;
}


void M2Lib::CM2::PrintInfo()
{
	//
	//
	// just print out any sort of data that we want to analize when troubleshooting

	UInt32 Count = 0;

	Char8 szItoA[32];

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

	FileStream << "ID       " << Header.Description.ID[0] << Header.Description.ID[1] << Header.Description.ID[2] << Header.Description.ID[3] << std::endl;		// 'MD20'
	FileStream << "Version  ";
	FileStream << itoa( (int)Header.Description.Version[0], szItoA, 10 ) << " ";
	FileStream << itoa( (int)Header.Description.Version[1], szItoA, 10 ) << " ";
	FileStream << itoa( (int)Header.Description.Version[2], szItoA, 10 ) << " ";
	FileStream << itoa( (int)Header.Description.Version[3], szItoA, 10 ) << std::endl;
	FileStream << std::endl;

	FileStream << "nName                     " << Header.Description.nName << std::endl;
	FileStream << "oName                     " << Header.Description.oName << std::endl;
	FileStream << " Value                    " << (Char8*)Elements[EElement_Name].Data << std::endl;
	FileStream << std::endl;

	FileStream << "Flags                     " << Header.Description.Flags << std::endl;
	FileStream << std::endl;

	FileStream << "nGlobalSequences          " << Header.Elements.nGlobalSequence << std::endl;
	FileStream << "oGlobalSequences          " << Header.Elements.oGlobalSequence << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_GlobalSequence].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nAnimations               " << Header.Elements.nAnimation << std::endl;
	FileStream << "oAnimations               " << Header.Elements.oAnimation << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Animation].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nAnimationsLookup         " << Header.Elements.nAnimationLookup << std::endl;
	FileStream << "oAnimationsLookup         " << Header.Elements.oAnimationLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_AnimationLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nBones                    " << Header.Elements.nBone << std::endl;
	FileStream << "oBones                    " << Header.Elements.oBone << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Bone].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nKeyBoneLookup            " << Header.Elements.nKeyBoneLookup << std::endl;
	FileStream << "oKeyBoneLookup            " << Header.Elements.oKeyBoneLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_KeyBoneLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nVertices                 " << Header.Elements.nVertex << std::endl;
	FileStream << "oVertices                 " << Header.Elements.oVertex << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Vertex].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nViews                    " << Header.Elements.nSkin << std::endl;
	FileStream << std::endl;

	FileStream << "nColors                   " << Header.Elements.nColor << std::endl;
	FileStream << "oColors                   " << Header.Elements.oColor << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Color].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTextures                 " << Header.Elements.nTexture << std::endl;
	FileStream << "oTextures                 " << Header.Elements.oTexture << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Texture].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTransparencies           " << Header.Elements.nTransparency << std::endl;
	FileStream << "oTransparencies           " << Header.Elements.oTransparency << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Transparency].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTextureAnimation         " << Header.Elements.nTextureAnimation << std::endl;
	FileStream << "oTextureAnimation         " << Header.Elements.nTextureAnimation << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TextureAnimation].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTextureReplace           " << Header.Elements.nTextureReplace << std::endl;
	FileStream << "oTextureReplace           " << Header.Elements.oTextureReplace << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TextureReplace].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTextureFlags             " << Header.Elements.nTextureFlags << std::endl;
	FileStream << "oTextureFlags             " << Header.Elements.oTextureFlags << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TextureFlags].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nSkinnedBoneLookup        " << Header.Elements.nSkinnedBoneLookup << std::endl;
	FileStream << "oSkinnedBoneLookup        " << Header.Elements.oSkinnedBoneLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_SkinnedBoneLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTexturesLookup           " << Header.Elements.nTextureLookup << std::endl;
	FileStream << "oTexturesLookup           " << Header.Elements.oTextureLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TextureLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTextureUnitsLookup       " << Header.Elements.nTextureUnitLookup << std::endl;
	FileStream << "oTextureUnitsLookup       " << Header.Elements.oTextureUnitLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TextureUnitLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTransparenciesLookup     " << Header.Elements.nTransparencyLookup << std::endl;
	FileStream << "oTransparenciesLookup     " << Header.Elements.oTransparencyLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TransparencyLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nTextureAnimationsLookup  " << Header.Elements.nTextureAnimationLookup << std::endl;
	FileStream << "oTextureAnimationsLookup  " << Header.Elements.oTextureAnimationLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_TextureAnimationLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "Volumes " << std::endl;
	FileStream << Header.Elements.CollisionVolume.Min[0] << std::endl;
	FileStream << Header.Elements.CollisionVolume.Min[1] << std::endl;
	FileStream << Header.Elements.CollisionVolume.Min[2] << std::endl;
	FileStream << Header.Elements.CollisionVolume.Max[0] << std::endl;
	FileStream << Header.Elements.CollisionVolume.Max[1] << std::endl;
	FileStream << Header.Elements.CollisionVolume.Max[2] << std::endl;
	FileStream << Header.Elements.CollisionVolume.Radius << std::endl;
	FileStream << Header.Elements.BoundingVolume.Min[0] << std::endl;
	FileStream << Header.Elements.BoundingVolume.Min[1] << std::endl;
	FileStream << Header.Elements.BoundingVolume.Min[2] << std::endl;
	FileStream << Header.Elements.BoundingVolume.Max[0] << std::endl;
	FileStream << Header.Elements.BoundingVolume.Max[1] << std::endl;
	FileStream << Header.Elements.BoundingVolume.Max[2] << std::endl;
	FileStream << Header.Elements.BoundingVolume.Radius << std::endl;
	FileStream << std::endl;

	FileStream << "nBoundingTriangles        " << Header.Elements.nBoundingTriangle << std::endl;
	FileStream << "oBoundingTriangles        " << Header.Elements.oBoundingTriangle << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_BoundingTriangle].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nBoundingVertices         " << Header.Elements.nBoundingVertex << std::endl;
	FileStream << "oBoundingVertices         " << Header.Elements.oBoundingVertex << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_BoundingVertex].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nBoundingNormals          " << Header.Elements.nBoundingNormal << std::endl;
	FileStream << "oBoundingNormals          " << Header.Elements.oBoundingNormal << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_BoundingNormal].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nAttachments              " << Header.Elements.nAttachment << std::endl;
	FileStream << "oAttachments              " << Header.Elements.oAttachment << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Attachment].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nAttachmentsLookup        " << Header.Elements.nAttachmentLookup << std::endl;
	FileStream << "oAttachmentsLookup        " << Header.Elements.oAttachmentLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_AttachmentLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nEvents                   " << Header.Elements.nEvent << std::endl;
	FileStream << "oEvents                   " << Header.Elements.oEvent << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Event].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nLights                   " << Header.Elements.nLight << std::endl;
	FileStream << "oLights                   " << Header.Elements.oLight << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Light].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nCameras                  " << Header.Elements.nCamera << std::endl;
	FileStream << "oCameras                  " << Header.Elements.oCamera << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Camera].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nCamerasLookup            " << Header.Elements.nCameraLookup << std::endl;
	FileStream << "oCamerasLookup            " << Header.Elements.oCameraLookup << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_CameraLookup].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nRibbonEmitters           " << Header.Elements.nRibbonEmitter << std::endl;
	FileStream << "oRibbonEmitters           " << Header.Elements.oRibbonEmitter << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_RibbonEmitter].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nParticleEmitters         " << Header.Elements.nParticleEmitter << std::endl;
	FileStream << "oParticleEmitters         " << Header.Elements.oParticleEmitter << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_ParticleEmitter].DataSize << std::endl;
	FileStream << std::endl;

	FileStream << "nUnknown1                 " << Header.Elements.nUnknown1 << std::endl;
	FileStream << "oUnknown1                 " << Header.Elements.oUnknown1 << std::endl;
	FileStream << " DataSize                 " << Elements[EElement_Unknown1].DataSize << std::endl;
	FileStream << std::endl;

	FileStream.close();
}


bool M2Lib::CM2::GetFileSkin( Char8* szResult, Char8* szFileM2, UInt32 SkinIndex )
{
	// ghetto string ops
	SInt32 LastDot = -1;
	UInt32 Length = 0;
	for ( ; Length < 1024; Length++ )
	{
		if ( szFileM2[Length] == L'\0' )
			break;
		else if ( szFileM2[Length] == L'.' )
			LastDot = Length;
	}
	if ( ( LastDot == -1 ) || ( LastDot > 1018 ) )
		return false;
	UInt32 j = 0;
	for ( ; j != LastDot; j++ )
	{
		szResult[j] = szFileM2[j];
	}
	szResult[j++] = L'0';
	switch ( SkinIndex )
	{
	case 0:
		szResult[j++] = L'0';
		break;
	case 1:
		szResult[j++] = L'1';
		break;
	case 2:
		szResult[j++] = L'2';
		break;
	case 3:
		szResult[j++] = L'3';
		break;
	}
	szResult[j++] = L'.';
	szResult[j++] = L's';
	szResult[j++] = L'k';
	szResult[j++] = L'i';
	szResult[j++] = L'n';
	szResult[j] = L'\0';

	return true;
}


void M2Lib::CM2::BuildVertexBoneIndices()
{
	for ( UInt32 i = 0; i != Header.Elements.nSkin; i++ )
	{
		if ( Skins[i] )
		{
			Skins[i]->BuildVertexBoneIndices();
		}
	}
}


void M2Lib::CM2::BuildMaxBones()
{
	for ( UInt32 i = 0; i != Header.Elements.nSkin; i++ )
	{
		if ( Skins[i] )
		{
			Skins[i]->BuildMaxBones();
		}
	}
}


/*
void M2Lib::CM2::m_SmoothNormals( std::vector< M2Lib::CM2Skin::CSubset* >& SubMesh, M2Lib::CVertex* Vertices, UInt16* Indices )
{
	// gather subset vertices into one list to make the submesh vertex list
	std::vector< UInt32 > SubMeshVertices;
	std::vector< bool > SubMeshVerticesProcessed;
	for ( UInt32 iSubset = 0; iSubset < SubMesh.size(); iSubset++ )
	{
		M2Lib::CM2Skin::CSubset* pSubset = SubMesh[iSubset];

		UInt32 VertexEnd = pSubset->VertexStart + pSubset->VertexCount;
		for ( UInt32 iVertex = pSubset->VertexStart; iVertex < VertexEnd; iVertex++ )
		{
			SubMeshVertices.push_back( Indices[iVertex] );
			SubMeshVerticesProcessed.push_back( false );
		}
	}

	// gather submesh duplicates
	for ( UInt32 iVertex = 0; iVertex < SubMeshVertices.size() - 1; iVertex++ )
	{
		if ( SubMeshVerticesProcessed[iVertex] )
			continue;
		SubMeshVerticesProcessed[iVertex] = true;

		UInt32 VertexA = SubMeshVertices[iVertex];

		// gather similar vertices
		std::vector< M2Lib::CVertex* > Duplicates;
		for ( UInt32 iVertex2 = iVertex + 1; iVertex2 < SubMeshVertices.size(); iVertex2++ )
		{
			if ( SubMeshVerticesProcessed[iVertex2] )
				continue;

			UInt32 VertexB = SubMeshVertices[iVertex2];
			bool Similar = false;
			Similar = M2Lib::CVertex::CompareSimilar( Vertices[VertexA], Vertices[VertexB], false, false, 45.0f );
			if ( Similar )
			{
				if ( Duplicates.size() == 0 )
				{
					Duplicates.push_back( &Vertices[VertexA] );
					SubMeshVerticesProcessed[iVertex] = true;
				}
				Duplicates.push_back( &Vertices[VertexB] );
				SubMeshVerticesProcessed[iVertex2] = true;
			}
		}

		// average normals of duplicates
		if ( Duplicates.size() )
		{
			// sum normals
			Float32 NewNormal[3];
			NewNormal[0] = 0.0f;
			NewNormal[1] = 0.0f;
			NewNormal[2] = 0.0f;
			for ( UInt32 iDuplicate = 0; iDuplicate < Duplicates.size(); iDuplicate++ )
			{
				NewNormal[0] += Duplicates[iDuplicate]->Normal[0];
				NewNormal[1] += Duplicates[iDuplicate]->Normal[1];
				NewNormal[2] += Duplicates[iDuplicate]->Normal[2];
			}

			// normalize new normal
			Float32 Length = sqrt( ( NewNormal[0] * NewNormal[0] ) + ( NewNormal[1] * NewNormal[1] ) + ( NewNormal[2] * NewNormal[2] ) );
			if ( Length < 0.5f )
			{
				int pause = 1;
			}
			NewNormal[0] /= Length;
			NewNormal[1] /= Length;
			NewNormal[2] /= Length;

			// set to new normal
			for ( UInt32 iDuplicate = 0; iDuplicate < Duplicates.size(); iDuplicate++ )
			{
				Duplicates[iDuplicate]->Normal[0] = NewNormal[0];
				Duplicates[iDuplicate]->Normal[1] = NewNormal[1];
				Duplicates[iDuplicate]->Normal[2] = NewNormal[2];

				// also copy position of first duplicate because they could be slightly different
				Duplicates[iDuplicate]->Position[0] = Duplicates[0]->Position[0];
				Duplicates[iDuplicate]->Position[1] = Duplicates[0]->Position[1];
				Duplicates[iDuplicate]->Position[2] = Duplicates[0]->Position[2];

				// also copy bones
				Duplicates[iDuplicate]->BoneWeights[0] = Duplicates[0]->BoneWeights[0];
				Duplicates[iDuplicate]->BoneWeights[1] = Duplicates[0]->BoneWeights[1];
				Duplicates[iDuplicate]->BoneWeights[2] = Duplicates[0]->BoneWeights[2];
				Duplicates[iDuplicate]->BoneWeights[3] = Duplicates[0]->BoneWeights[3];

				Duplicates[iDuplicate]->Bones[0] = Duplicates[0]->Bones[0];
				Duplicates[iDuplicate]->Bones[1] = Duplicates[0]->Bones[1];
				Duplicates[iDuplicate]->Bones[2] = Duplicates[0]->Bones[2];
				Duplicates[iDuplicate]->Bones[3] = Duplicates[0]->Bones[3];
			}
		}
	}
}


void M2Lib::CM2::FixNormals()
{
	// gather up each subset with same ID
	// mainly, we want to gather up each subset with ID 0 or ends in 01 into a separate group beacuse those are the main body subsets
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tgathering submeshes..." << std::endl;
	}

	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)Elements[EElement_Vertex].Data;
	UInt16* Indices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Data;
	M2Lib::CM2Skin::CSubset* Subsets = (M2Lib::CM2Skin::CSubset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Number;

	std::vector< M2Lib::CM2Skin::CSubset* > SubMeshBody;	// body submeshes
	std::vector< std::vector< M2Lib::CM2Skin::CSubset* > > SubMeshesOther;	// non-body submeshes
	for ( UInt32 i = 0; i < SubsetCount; i++ )
	{
		UInt32 ThisID = Subsets[i].ID;

		// gather main body subsets
		UInt32 Mod = ThisID;
		while  ( Mod > 10 )
		{
			Mod %= 10;
		}
		if ( ThisID == 0 || ( ThisID > 10 && Mod == 1 ) )
		{
			SubMeshBody.push_back( &Subsets[i] );
		}
		// gather other subsets
		else
		{
			bool Gathered = false;
			for ( UInt32 j = 0; j < SubMeshesOther.size(); j++ )
			{
				if ( SubMeshesOther[j][0]->ID == ThisID )
				{
					Gathered = true;
					break;
				}
			}
			if ( Gathered )
				continue;

			std::vector< M2Lib::CM2Skin::CSubset* > GatheredSubsets;
			for ( UInt32 j = 0; j < SubsetCount; j++ )
			{
				if ( Subsets[j].ID == ThisID )
				{
					GatheredSubsets.push_back( &Subsets[j] );
				}
			}

			SubMeshesOther.push_back( GatheredSubsets );
		}
	}
	//SubMeshes.insert( SubMeshes.begin(), GatheredSubsetsBody );

	// smooth normals of vertices within their own sub-mesh
	if ( g_Verbose >= 2 )
	{
		std::cout << "\t\tsmoothing normals of submeshes..." << std::endl;
	}
	// smooth normals of body
	m_SmoothNormals( SubMeshBody, Vertices, Indices );
	// smooth normals of other
	for ( UInt32 iSubMesh1 = 0; iSubMesh1 < SubMeshesOther.size(); iSubMesh1++ )
	{
		m_SmoothNormals( SubMeshesOther[iSubMesh1], Vertices, Indices );
	}

	// copy vertex properties from main body vertex to duplicate clothing vertices
	//if ( g_Verbose >= 2 )
	//{
	//	std::cout << "\t\tcopying normals from body submesh to clothing submeshes..." << std::endl;
	//}
	//for ( UInt32 iSubMeshOther = 0; iSubMeshOther < SubMeshesOther.size(); iSubMeshOther++ )
	//{
	//	// for each subset vertex
	//	std::vector< M2Lib::CM2Skin::CSubset* >& SubMeshOther = SubMeshesOther[iSubMeshOther];
	//	for ( UInt32 iSubset = 0; iSubset < SubMeshOther.size(); iSubset++ )
	//	{
	//		M2Lib::CM2Skin::CSubset* pSubset = SubMeshOther[iSubset];

	//		// compare with body vertices
	//		for ( UInt32 iSubsetBody = 0; iSubsetBody < SubMeshBody.size(); iSubsetBody++ )
	//		{
	//			M2Lib::CM2Skin::CSubset* pSubsetBody = SubMeshBody[iSubsetBody];
	//			for ( SInt32 iVertex = pSubset->VertexStart; iVertex < pSubset->VertexStart + pSubset->VertexCount; iVertex++ )
	//			{
	//				UInt32 VertexA = Indices[iVertex];
	//				for ( SInt32 iVertexBody = pSubsetBody->VertexStart; iVertexBody < pSubsetBody->VertexStart + pSubsetBody->VertexCount; iVertexBody++ )
	//				{
	//					UInt32 VertexB = Indices[iVertexBody];
	//					if ( M2Lib::CVertex::CompareSimilar( Vertices[VertexA], Vertices[VertexB], false, false ) )
	//					{
	//						// copy position, normal, and bone weights, and bone indices from body vertex to other(clothing) vertex
	//						M2Lib::CVertex* pVertexClothing = &Vertices[VertexA];
	//						M2Lib::CVertex* pVertexBody = &Vertices[VertexB];

	//						pVertexClothing->Position[0] = pVertexBody->Position[0];
	//						pVertexClothing->Position[1] = pVertexBody->Position[1];
	//						pVertexClothing->Position[2] = pVertexBody->Position[2];

	//						pVertexClothing->Normal[0] = pVertexBody->Normal[0];
	//						pVertexClothing->Normal[1] = pVertexBody->Normal[1];
	//						pVertexClothing->Normal[2] = pVertexBody->Normal[2];

	//						pVertexClothing->BoneWeights[0] = pVertexBody->BoneWeights[0];
	//						pVertexClothing->BoneWeights[1] = pVertexBody->BoneWeights[1];
	//						pVertexClothing->BoneWeights[2] = pVertexBody->BoneWeights[2];
	//						pVertexClothing->BoneWeights[3] = pVertexBody->BoneWeights[3];

	//						pVertexClothing->Bones[0] = pVertexBody->Bones[0];
	//						pVertexClothing->Bones[1] = pVertexBody->Bones[1];
	//						pVertexClothing->Bones[2] = pVertexBody->Bones[2];
	//						pVertexClothing->Bones[3] = pVertexBody->Bones[3];
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}
*/


void M2Lib::CM2::FixSeamsSubmesh( Float32 PositionalTolerance, Float32 AngularTolerance )
{
	// gather up sub meshes
	std::vector< std::vector< M2Lib::CM2Skin::CElement_Subset* > > SubMeshes;

	M2Lib::CM2Skin::CElement_Subset* Subsets = (M2Lib::CM2Skin::CElement_Subset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Number;
	for ( UInt32 i = 0; i < SubsetCount; i++ )
	{
		UInt32 ThisID = Subsets[i].ID;
		bool MakeNew = true;
		for ( UInt32 j = 0; j < SubMeshes.size(); j++ )
		{
			for ( UInt32 k = 0; k < SubMeshes[j].size(); k++ )
			{
				if ( SubMeshes[j][k]->ID == ThisID )
				{
					MakeNew = false;
					SubMeshes[j].push_back( &Subsets[i] );
					break;
				}
			}
			if ( !MakeNew )
			{
				break;
			}
		}
		if ( MakeNew )
		{
			std::vector< M2Lib::CM2Skin::CElement_Subset* > NewSubmeshSubsetList;
			NewSubmeshSubsetList.push_back( &Subsets[i] );
			SubMeshes.push_back( NewSubmeshSubsetList );
		}
	}


	UInt32 VerticesCount = GetElement_Vertex().Number;
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)GetElement_Vertex().Data;

	UInt32 VertexIndicesCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Vertex].Number;
	UInt16* VertexIndices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Vertex].Data;

	std::vector< M2Lib::CVertex* > SimilarVertices;

	bool* ProcessedVertices = new bool[VerticesCount];
	for ( UInt32 iSubMesh1 = 0; iSubMesh1 < SubMeshes.size(); iSubMesh1++ )
	{
		for ( UInt32 iVertex = 0; iVertex < VerticesCount; iVertex++ )
		{
			ProcessedVertices[iVertex] = false;
		}

		for ( UInt32 iSubSet1 = 0; iSubSet1 < SubMeshes[iSubMesh1].size(); iSubSet1++ )
		{
			M2Lib::CM2Skin::CElement_Subset* pSubSet1 = SubMeshes[iSubMesh1][iSubSet1];

			UInt32 VertexEnd1 = pSubSet1->VertexStart + pSubSet1->VertexCount;
			for ( UInt32 iVertex1 = pSubSet1->VertexStart; iVertex1 < VertexEnd1; iVertex1++ )
			{
				if ( ProcessedVertices[iVertex1] )
				{
					continue;
				}

				M2Lib::CVertex& Vertex1 = Vertices[iVertex1];

				bool AddedVertex1 = false;
				for ( UInt32 iSubSet2 = 0; iSubSet2 < SubMeshes[iSubMesh1].size(); iSubSet2++ )
				{
					M2Lib::CM2Skin::CElement_Subset* pSubSet2 = SubMeshes[iSubMesh1][iSubSet2];

					UInt32 VertexEnd2 = pSubSet2->VertexStart + pSubSet2->VertexCount;
					for ( UInt32 iVertex2 = pSubSet2->VertexStart; iVertex2 < VertexEnd2; iVertex2++ )
					{
						if ( iVertex1 == iVertex2 )
						{
							continue;
						}
						if ( ProcessedVertices[iVertex2] )
						{
							continue;
						}

						M2Lib::CVertex& Vertex2 = Vertices[iVertex2];

						if ( M2Lib::CVertex::CompareSimilar( Vertex1, Vertex2, false, false, PositionalTolerance, AngularTolerance ) )
						{
							if ( !AddedVertex1 )
							{
								SimilarVertices.push_back( &Vertex1 );
								ProcessedVertices[iVertex1] = true;
								AddedVertex1 = true;
							}

							SimilarVertices.push_back( &Vertex2 );
							ProcessedVertices[iVertex2] = true;
						}
					}
				}

				ProcessedVertices[iVertex1] = true;

				if ( SimilarVertices.size() )
				{
					// sum positions and normals
					Float32 NewPosition[3];
					NewPosition[0] = 0.0f;
					NewPosition[1] = 0.0f;
					NewPosition[2] = 0.0f;

					Float32 NewNormal[3];
					NewNormal[0] = 0.0f;
					NewNormal[1] = 0.0f;
					NewNormal[2] = 0.0f;

					for ( UInt32 iSimilarVertex = 0; iSimilarVertex < SimilarVertices.size(); iSimilarVertex++ )
					{
						M2Lib::CVertex* pSimilarVertex = SimilarVertices[iSimilarVertex];

						NewPosition[0] += pSimilarVertex->Position[0];
						NewPosition[1] += pSimilarVertex->Position[1];
						NewPosition[2] += pSimilarVertex->Position[2];

						NewNormal[0] += pSimilarVertex->Normal[0];
						NewNormal[1] += pSimilarVertex->Normal[1];
						NewNormal[2] += pSimilarVertex->Normal[2];
					}

					// average position and normalize normal
					Float32 SimilarCount = (Float32)SimilarVertices.size();

					NewPosition[0] /= SimilarCount;
					NewPosition[1] /= SimilarCount;
					NewPosition[2] /= SimilarCount;

					NewNormal[0] /= SimilarCount;
					NewNormal[1] /= SimilarCount;
					NewNormal[2] /= SimilarCount;

					// assign new values back into similar vertices
					for ( UInt32 iSimilarVertex = 0; iSimilarVertex < SimilarVertices.size(); iSimilarVertex++ )
					{
						M2Lib::CVertex* pSimilarVertex = SimilarVertices[iSimilarVertex];

						pSimilarVertex->Position[0] = NewPosition[0];
						pSimilarVertex->Position[1] = NewPosition[1];
						pSimilarVertex->Position[2] = NewPosition[2];

						pSimilarVertex->Normal[0] = NewNormal[0];
						pSimilarVertex->Normal[1] = NewNormal[1];
						pSimilarVertex->Normal[2] = NewNormal[2];

						pSimilarVertex->BoneWeights[0] = Vertex1.BoneWeights[0];
						pSimilarVertex->BoneWeights[1] = Vertex1.BoneWeights[1];
						pSimilarVertex->BoneWeights[2] = Vertex1.BoneWeights[2];
						pSimilarVertex->BoneWeights[3] = Vertex1.BoneWeights[3];

						pSimilarVertex->Bones[0] = Vertex1.Bones[0];
						pSimilarVertex->Bones[1] = Vertex1.Bones[1];
						pSimilarVertex->Bones[2] = Vertex1.Bones[2];
						pSimilarVertex->Bones[3] = Vertex1.Bones[3];
					}

					SimilarVertices.clear();
				}
			}
		}
	}

	delete[] ProcessedVertices;
}


void M2Lib::CM2::FixSeamsBody( Float32 PositionalTolerance, Float32 AngularTolerance )
{
	// submeshes that are divided up accross multiple bone partitions will have multiple subset entries with the same ID in the M2
	// we need to gather each body submesh up into a list and average normals of vertices that are similar between other sub meshes
	// this function is designed to be used on character models
	// it may cause some odd behavior or do nothing at all if used on other models

	// list of submeshes that make up the body of the character
	std::vector< std::vector< M2Lib::CM2Skin::CElement_Subset* > > SubMeshes;

	// gather up the body submeshes
	M2Lib::CM2Skin::CElement_Subset* Subsets = (M2Lib::CM2Skin::CElement_Subset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Number;
	for ( UInt32 i = 0; i < SubsetCount; i++ )
	{
		// determine type of subset
		UInt32 ThisID = Subsets[i].ID;
		UInt32 Mod = ThisID;
		while  ( Mod > 10 )
		{
			Mod %= 10;
		}
		if ( ThisID == 0 || ( ThisID > 10 && Mod == 1 ) )
		{
			// this subset is part of the character's body
			// add it to the list of submeshes
			bool MakeNew = true;
			for ( UInt32 j = 0; j < SubMeshes.size(); j++ )
			{
				for ( UInt32 k = 0; k < SubMeshes[j].size(); k++ )
				{
					if ( SubMeshes[j][k]->ID == ThisID )
					{
						MakeNew = false;
						SubMeshes[j].push_back( &Subsets[i] );
						break;
					}
				}
				if ( !MakeNew )
				{
					break;
				}
			}
			if ( MakeNew )
			{
				std::vector< M2Lib::CM2Skin::CElement_Subset* > NewSubmeshSubsetList;
				NewSubmeshSubsetList.push_back( &Subsets[i] );
				SubMeshes.push_back( NewSubmeshSubsetList );
			}
		}
	}


	// now that all the body submeshes are gathered up in a list, we can average normals of duplicate vertices between them
	// but make sure not to compare against vertices that are in the same submesh
	UInt32 VerticesCount = GetElement_Vertex().Number;
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)GetElement_Vertex().Data;

	UInt32 VertexIndicesCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Vertex].Number;
	UInt16* VertexIndices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Vertex].Data;


	// list of pointers to similar vertices.
	// they will be averaged at the end of the vertex duplicate gather pass.
	std::vector< M2Lib::CVertex* > SimilarVertices;

	// go through each submesh
	for ( SInt32 iSubMesh1 = 0; iSubMesh1 < (SInt32)SubMeshes.size() - 1; iSubMesh1++ )
	{
		// go through each subset
		for ( SInt32 iSubSet1 = 0; iSubSet1 < (SInt32)SubMeshes[iSubMesh1].size(); iSubSet1++ )
		{
			// gather duplicate vertices
			
			// for each vertex in the subset, compare it against vertices in the other subsets
			// find duplicates and sum their normals
			UInt32 VertexEnd1 = SubMeshes[iSubMesh1][iSubSet1]->VertexStart + SubMeshes[iSubMesh1][iSubSet1]->VertexCount;
			for ( UInt32 iVertex1 = SubMeshes[iSubMesh1][iSubSet1]->VertexStart; iVertex1 < VertexEnd1; iVertex1++ )
			{
				// get this vertex
				M2Lib::CVertex& Vertex1 = Vertices[iVertex1];

				// gather duplicate vertices from other submeshes
				bool AddedVertex1 = false;
				for ( SInt32 iSubMesh2 = iSubMesh1 + 1; iSubMesh2 < (SInt32)SubMeshes.size(); iSubMesh2++ )
				{
					// check that we don't check against ourselves
					if ( iSubMesh2 == iSubMesh1 )
					{
						// other submesh is same as this submesh
						continue;
					}
					// go through subsets
					for ( SInt32 iSubSet2 = 0; iSubSet2 < (SInt32)SubMeshes[iSubMesh2].size(); iSubSet2++ )
					{
						// go through vertices in subset
						UInt32 VertexEnd2 = SubMeshes[iSubMesh2][iSubSet2]->VertexStart + SubMeshes[iSubMesh2][iSubSet2]->VertexCount;
						for ( UInt32 iVertex2 = SubMeshes[iSubMesh2][iSubSet2]->VertexStart; iVertex2 < VertexEnd2; iVertex2++ )
						{
							M2Lib::CVertex& Vertex2 = Vertices[iVertex2];

							if ( M2Lib::CVertex::CompareSimilar( Vertex1, Vertex2, false, false, PositionalTolerance, AngularTolerance ) )
							{
								// found a duplicate
								if ( !AddedVertex1 )
								{
									SimilarVertices.push_back( &Vertex1 );
									AddedVertex1 = true;
								}
								// add the vertex from the other sub mesh to the list of similar vertices
								SimilarVertices.push_back( &Vertex2 );
							}
						}
					}
				}

				// average normals of similar vertices
				if ( SimilarVertices.size() )
				{
					// sum positions and normals
					Float32 NewPosition[3];
					NewPosition[0] = 0.0f;
					NewPosition[1] = 0.0f;
					NewPosition[2] = 0.0f;

					Float32 NewNormal[3];
					NewNormal[0] = 0.0f;
					NewNormal[1] = 0.0f;
					NewNormal[2] = 0.0f;

					for ( UInt32 iSimilarVertex = 0; iSimilarVertex < SimilarVertices.size(); iSimilarVertex++ )
					{
						M2Lib::CVertex* pSimilarVertex = SimilarVertices[iSimilarVertex];

						NewPosition[0] += pSimilarVertex->Position[0];
						NewPosition[1] += pSimilarVertex->Position[1];
						NewPosition[2] += pSimilarVertex->Position[2];

						NewNormal[0] += pSimilarVertex->Normal[0];
						NewNormal[1] += pSimilarVertex->Normal[1];
						NewNormal[2] += pSimilarVertex->Normal[2];
					}

					// average position and normalize normal
					Float32 SimilarCount = (Float32)SimilarVertices.size();

					NewPosition[0] /= SimilarCount;
					NewPosition[1] /= SimilarCount;
					NewPosition[2] /= SimilarCount;

					NewNormal[0] /= SimilarCount;
					NewNormal[1] /= SimilarCount;
					NewNormal[2] /= SimilarCount;

					// assign new values back into similar vertices
					for ( UInt32 iSimilarVertex = 0; iSimilarVertex < SimilarVertices.size(); iSimilarVertex++ )
					{
						M2Lib::CVertex* pSimilarVertex = SimilarVertices[iSimilarVertex];

						pSimilarVertex->Position[0] = NewPosition[0];
						pSimilarVertex->Position[1] = NewPosition[1];
						pSimilarVertex->Position[2] = NewPosition[2];

						pSimilarVertex->Normal[0] = NewNormal[0];
						pSimilarVertex->Normal[1] = NewNormal[1];
						pSimilarVertex->Normal[2] = NewNormal[2];

						pSimilarVertex->BoneWeights[0] = Vertex1.BoneWeights[0];
						pSimilarVertex->BoneWeights[1] = Vertex1.BoneWeights[1];
						pSimilarVertex->BoneWeights[2] = Vertex1.BoneWeights[2];
						pSimilarVertex->BoneWeights[3] = Vertex1.BoneWeights[3];

						pSimilarVertex->Bones[0] = Vertex1.Bones[0];
						pSimilarVertex->Bones[1] = Vertex1.Bones[1];
						pSimilarVertex->Bones[2] = Vertex1.Bones[2];
						pSimilarVertex->Bones[3] = Vertex1.Bones[3];
					}

					// clear list
					SimilarVertices.clear();
				}
			}
		}
	}
}


void M2Lib::CM2::FixSeamsClothing( Float32 PositionalTolerance, Float32 AngularTolerance )
{
	// gather up each subset with same ID
	UInt32 VertexCount = GetElement_Vertex().Number;
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)GetElement_Vertex().Data;

	UInt16* VertexIndices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Vertex].Data;
	M2Lib::CM2Skin::CElement_Subset* Subsets = (M2Lib::CM2Skin::CElement_Subset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subset].Number;

	std::vector< M2Lib::CM2Skin::CElement_Subset* > SubMeshBody;	// body submeshes
	std::vector< std::vector< M2Lib::CM2Skin::CElement_Subset* > > SubMeshesOther;	// non-body submeshes
	for ( UInt32 i = 0; i < SubsetCount; i++ )
	{
		UInt32 ThisID = Subsets[i].ID;

		// gather main body subsets
		UInt32 Mod = ThisID;
		while  ( Mod > 10 )
		{
			Mod %= 10;
		}
		if ( ThisID == 0 || ( ThisID > 10 && Mod == 1 ) )
		{
			SubMeshBody.push_back( &Subsets[i] );
		}
		// gather other subsets
		else
		{
			bool Gathered = false;
			for ( UInt32 j = 0; j < SubMeshesOther.size(); j++ )
			{
				if ( SubMeshesOther[j][0]->ID == ThisID )
				{
					Gathered = true;
					break;
				}
			}
			if ( Gathered )
				continue;

			std::vector< M2Lib::CM2Skin::CElement_Subset* > GatheredSubsets;
			for ( UInt32 j = 0; j < SubsetCount; j++ )
			{
				if ( Subsets[j].ID == ThisID )
				{
					GatheredSubsets.push_back( &Subsets[j] );
				}
			}

			SubMeshesOther.push_back( GatheredSubsets );
		}
	}

	// copy vertex properties from main body vertex to duplicate clothing vertices
	for ( UInt32 iSubMeshOther = 0; iSubMeshOther < SubMeshesOther.size(); iSubMeshOther++ )
	{
		// for each subset vertex
		std::vector< M2Lib::CM2Skin::CElement_Subset* >& SubMeshOther = SubMeshesOther[iSubMeshOther];
		for ( UInt32 iSubset = 0; iSubset < SubMeshOther.size(); iSubset++ )
		{
			M2Lib::CM2Skin::CElement_Subset* pSubset = SubMeshOther[iSubset];

			// compare with body vertices
			for ( UInt32 iSubsetBody = 0; iSubsetBody < SubMeshBody.size(); iSubsetBody++ )
			{
				M2Lib::CM2Skin::CElement_Subset* pSubsetBody = SubMeshBody[iSubsetBody];
				for ( SInt32 VertexA = pSubset->VertexStart; VertexA < pSubset->VertexStart + pSubset->VertexCount; VertexA++ )
				{
					for ( SInt32 VertexB = pSubsetBody->VertexStart; VertexB < pSubsetBody->VertexStart + pSubsetBody->VertexCount; VertexB++ )
					{
						if ( M2Lib::CVertex::CompareSimilar( Vertices[VertexA], Vertices[VertexB], false, false, PositionalTolerance, 1.0f ) )
						{
							// copy position, normal, and bone weights, and bone indices from body vertex to other(clothing) vertex
							M2Lib::CVertex* pVertexClothing = &Vertices[VertexA];
							M2Lib::CVertex* pVertexBody = &Vertices[VertexB];

							pVertexClothing->Position[0] = pVertexBody->Position[0];
							pVertexClothing->Position[1] = pVertexBody->Position[1];
							pVertexClothing->Position[2] = pVertexBody->Position[2];

							// only copy normal from body normal to clothing normal if clothing normal is already within 15 degrees of body normal
							Float32 NormalDot = ( pVertexClothing->Normal[0] * pVertexBody->Normal[0] ) + ( pVertexClothing->Normal[1] * pVertexBody->Normal[1] ) + ( pVertexClothing->Normal[2] * pVertexBody->Normal[2] );
							if ( acos( NormalDot ) < ( 15.0f * 0.0174532925f ) )	// 15 degrees in radians
							{
								pVertexClothing->Normal[0] = pVertexBody->Normal[0];
								pVertexClothing->Normal[1] = pVertexBody->Normal[1];
								pVertexClothing->Normal[2] = pVertexBody->Normal[2];
							}

							pVertexClothing->BoneWeights[0] = pVertexBody->BoneWeights[0];
							pVertexClothing->BoneWeights[1] = pVertexBody->BoneWeights[1];
							pVertexClothing->BoneWeights[2] = pVertexBody->BoneWeights[2];
							pVertexClothing->BoneWeights[3] = pVertexBody->BoneWeights[3];

							pVertexClothing->Bones[0] = pVertexBody->Bones[0];
							pVertexClothing->Bones[1] = pVertexBody->Bones[1];
							pVertexClothing->Bones[2] = pVertexBody->Bones[2];
							pVertexClothing->Bones[3] = pVertexBody->Bones[3];
						}
					}
				}
			}
		}
	}
}


void M2Lib::CM2::NormalizeBoneWeights()
{
}


void M2Lib::CM2::Scale( Float32 Scale )
{
	// vertices
	{
		UInt32 VertexListLength = GetElement_Vertex().Number;
		CVertex* VertexList = (CVertex*)GetElement_Vertex().Data;
		for ( UInt32 i = 0; i < VertexListLength; i++ )
		{
			CVertex& Vertex = VertexList[i];
			Vertex.Position[0] *= Scale;
			Vertex.Position[1] *= Scale;
			Vertex.Position[2] *= Scale;
		}
	}

	// bones
	{
		UInt32 BoneListLength = Elements[EElement_Bone].Number;
		CElement_Bone* BoneList = (CElement_Bone*)Elements[EElement_Bone].Data;
		for ( UInt32 i = 0; i < BoneListLength; i++ )
		{
			CElement_Bone& Bone = BoneList[i];
			Bone.Position[0] *= Scale;
			Bone.Position[1] *= Scale;
			Bone.Position[2] *= Scale;
		}
	}

	// attachments
	{
		UInt32 AttachmentListLength = Elements[EElement_Attachment].Number;
		CElement_Attachment* AttachmentList = (CElement_Attachment*)Elements[EElement_Attachment].Data;
		for ( UInt32 i = 0; i < AttachmentListLength; i++ )
		{
			CElement_Attachment& Attachment = AttachmentList[i];
			Attachment.Position[0] *= Scale;
			Attachment.Position[1] *= Scale;
			Attachment.Position[2] *= Scale;
		}
	}

	// events
	{
		UInt32 EventListLength = Elements[EElement_Event].Number;
		CElement_Event* EventList = (CElement_Event*)Elements[EElement_Event].Data;
		for ( UInt32 i = 0; i < EventListLength; i++ )
		{
			CElement_Event& Event = EventList[i];
			Event.Position[0] *= Scale;
			Event.Position[1] *= Scale;
			Event.Position[2] *= Scale;
		}
	}

	// lights
	{
		UInt32 LightListLength = Elements[EElement_Light].Number;
		CElement_Light* LightList = (CElement_Light*)Elements[EElement_Light].Data;
		for ( UInt32 i = 0; i < LightListLength; i++ )
		{
			CElement_Light& Light = LightList[i];
			Light.Position[0] *= Scale;
			Light.Position[1] *= Scale;
			Light.Position[2] *= Scale;
		}
	}

	// cameras
	{
		UInt32 CameraListLength = Elements[EElement_Camera].Number;
		CElement_Camera* CameraList = (CElement_Camera*)Elements[EElement_Camera].Data;
		for ( UInt32 i = 0; i < CameraListLength; i++ )
		{
			CElement_Camera& Camera = CameraList[i];
			Camera.Position[0] *= Scale;
			Camera.Position[1] *= Scale;
			Camera.Position[2] *= Scale;
			Camera.Target[0] *= Scale;
			Camera.Target[1] *= Scale;
			Camera.Target[2] *= Scale;
		}
	}

	// ribbon emitters
	{
		UInt32 RibbonEmitterListLength = Elements[EElement_RibbonEmitter].Number;
		CElement_RibbonEmitter* RibbonEmitterList = (CElement_RibbonEmitter*)Elements[EElement_RibbonEmitter].Data;
		for ( UInt32 i = 0; i < RibbonEmitterListLength; i++ )
		{
			CElement_RibbonEmitter& RibbonEmitter = RibbonEmitterList[i];
			RibbonEmitter.Position[0] *= Scale;
			RibbonEmitter.Position[1] *= Scale;
			RibbonEmitter.Position[2] *= Scale;
		}
	}

	// particle emitters
	{
		UInt32 ParticleEmitterListLength = Elements[EElement_ParticleEmitter].Number;
		CElement_ParticleEmitter* ParticleEmitterList = (CElement_ParticleEmitter*)Elements[EElement_ParticleEmitter].Data;
		for ( UInt32 i = 0; i < ParticleEmitterListLength; i++ )
		{
			CElement_ParticleEmitter& ParticleEmitter = ParticleEmitterList[i];
			ParticleEmitter.Position[0] *= Scale;
			ParticleEmitter.Position[1] *= Scale;
			ParticleEmitter.Position[2] *= Scale;
		}
	}
}


void M2Lib::CM2::MirrorCamera()
{
	CElement_Camera* Cameras = (CElement_Camera*)Elements[EElement_Camera].Data;
	UInt32 CameraCount = Elements[EElement_Camera].Number;
	for ( UInt32 iCamera = 0; iCamera < CameraCount; iCamera++ )
	{
		if ( Cameras->Type == 0 )
		{
			Cameras->Position[1] = -Cameras->Position[1];
			Cameras->Target[1] = -Cameras->Target[1];
			break;
		}
	}
	
}


void M2Lib::CM2::m_LoadElements_CopyHeaderToElements()
{
	Elements[EElement_Name].Number = Header.Description.nName;
	Elements[EElement_Name].Offset = Header.Description.oName;

	Elements[EElement_GlobalSequence].Number = Header.Elements.nGlobalSequence;
	Elements[EElement_GlobalSequence].Offset = Header.Elements.oGlobalSequence;

	Elements[EElement_Animation].Number = Header.Elements.nAnimation;
	Elements[EElement_Animation].Offset = Header.Elements.oAnimation;

	Elements[EElement_AnimationLookup].Number = Header.Elements.nAnimationLookup;
	Elements[EElement_AnimationLookup].Offset = Header.Elements.oAnimationLookup;

	Elements[EElement_Bone].Number = Header.Elements.nBone;
	Elements[EElement_Bone].Offset = Header.Elements.oBone;

	Elements[EElement_KeyBoneLookup].Number = Header.Elements.nKeyBoneLookup;
	Elements[EElement_KeyBoneLookup].Offset = Header.Elements.oKeyBoneLookup;

	Elements[EElement_Vertex].Number = Header.Elements.nVertex;
	Elements[EElement_Vertex].Offset = Header.Elements.oVertex;

	Elements[EElement_Skin].Number = Header.Elements.nSkin;
	Elements[EElement_Skin].Offset = 0;

	Elements[EElement_Color].Number = Header.Elements.nColor;
	Elements[EElement_Color].Offset = Header.Elements.oColor;

	Elements[EElement_Texture].Number = Header.Elements.nTexture;
	Elements[EElement_Texture].Offset = Header.Elements.oTexture;

	Elements[EElement_Transparency].Number = Header.Elements.nTransparency;
	Elements[EElement_Transparency].Offset = Header.Elements.oTransparency;

	Elements[EElement_TextureAnimation].Number = Header.Elements.nTextureAnimation;
	Elements[EElement_TextureAnimation].Offset = Header.Elements.oTextureAnimation;

	Elements[EElement_TextureReplace].Number = Header.Elements.nTextureReplace;
	Elements[EElement_TextureReplace].Offset = Header.Elements.oTextureReplace;

	Elements[EElement_TextureFlags].Number = Header.Elements.nTextureFlags;
	Elements[EElement_TextureFlags].Offset = Header.Elements.oTextureFlags;

	Elements[EElement_SkinnedBoneLookup].Number = Header.Elements.nSkinnedBoneLookup;
	Elements[EElement_SkinnedBoneLookup].Offset = Header.Elements.oSkinnedBoneLookup;

	Elements[EElement_TextureLookup].Number = Header.Elements.nTextureLookup;
	Elements[EElement_TextureLookup].Offset = Header.Elements.oTextureLookup;

	Elements[EElement_TextureUnitLookup].Number = Header.Elements.nTextureUnitLookup;
	Elements[EElement_TextureUnitLookup].Offset = Header.Elements.oTextureUnitLookup;

	Elements[EElement_TransparencyLookup].Number = Header.Elements.nTransparencyLookup;
	Elements[EElement_TransparencyLookup].Offset = Header.Elements.oTransparencyLookup;

	Elements[EElement_TextureAnimationLookup].Number = Header.Elements.nTextureAnimationLookup;
	Elements[EElement_TextureAnimationLookup].Offset = Header.Elements.oTextureAnimationLookup;

	Elements[EElement_BoundingTriangle].Number = Header.Elements.nBoundingTriangle;
	Elements[EElement_BoundingTriangle].Offset = Header.Elements.oBoundingTriangle;

	Elements[EElement_BoundingVertex].Number = Header.Elements.nBoundingVertex;
	Elements[EElement_BoundingVertex].Offset = Header.Elements.oBoundingVertex;

	Elements[EElement_BoundingNormal].Number = Header.Elements.nBoundingNormal;
	Elements[EElement_BoundingNormal].Offset = Header.Elements.oBoundingNormal;

	Elements[EElement_Attachment].Number = Header.Elements.nAttachment;
	Elements[EElement_Attachment].Offset = Header.Elements.oAttachment;

	Elements[EElement_AttachmentLookup].Number = Header.Elements.nAttachmentLookup;
	Elements[EElement_AttachmentLookup].Offset = Header.Elements.oAttachmentLookup;

	Elements[EElement_Event].Number = Header.Elements.nEvent;
	Elements[EElement_Event].Offset = Header.Elements.oEvent;

	Elements[EElement_Light].Number = Header.Elements.nLight;
	Elements[EElement_Light].Offset = Header.Elements.oLight;

	Elements[EElement_Camera].Number = Header.Elements.nCamera;
	Elements[EElement_Camera].Offset = Header.Elements.oCamera;

	Elements[EElement_CameraLookup].Number = Header.Elements.nCameraLookup;
	Elements[EElement_CameraLookup].Offset = Header.Elements.oCameraLookup;

	Elements[EElement_RibbonEmitter].Number = Header.Elements.nRibbonEmitter;
	Elements[EElement_RibbonEmitter].Offset = Header.Elements.oRibbonEmitter;

	Elements[EElement_ParticleEmitter].Number = Header.Elements.nParticleEmitter;
	Elements[EElement_ParticleEmitter].Offset = Header.Elements.oParticleEmitter;

	Elements[EElement_Unknown1].Number = Header.Elements.nUnknown1;
	Elements[EElement_Unknown1].Offset = Header.Elements.oUnknown1;
}


void M2Lib::CM2::m_LoadElements_FindSizes( UInt32 FileSize )
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
			SInt32 NextOffset = FileSize;
			for ( UInt32 j = 0; j < EElement__Count__; j++ )
			{
				if ( ( Elements[j].Number != 0 ) && ( Elements[j].Offset > Elements[i].Offset ) )
				{
					if ( Elements[j].Offset < NextOffset )
					{
						NextOffset = Elements[j].Offset;
					}
					break;
				}
			}
			Elements[i].DataSize = NextOffset - Elements[i].Offset;
		}
	}
}


#define VERIFY_OFFSET_LOCAL( offset ) assert( ( Elements[iElement].Offset <= offset ) && ( offset < ( Elements[iElement].Offset + Elements[iElement].DataSize ) ) );
#define VERIFY_OFFSET_NOTLOCAL( offset ) assert( offset > ( Elements[iElement].OffsetOriginal + Elements[iElement].DataSize ) );
void M2Lib::CM2::m_SaveElements_FindOffsets()
{
	// thanks TP

	// find new file size
	UInt32 NewSize = sizeof( CM2Header );
	if ( !( Header.Description.Flags & 0x08 ) )
	{
		NewSize -= 8;
	}
	for ( UInt32 i = 0; i < EElement__Count__; i++ )
	{
		NewSize += Elements[i].DataSize;
	}
	SInt32 OffsetDeltaEnd = NewSize - m_OriginalSize;				// the delta to apply to animation offsets that lie past m_OriginalSize. if the file grew, this will be positive. if the file shrank this will be negative.
	UInt32 OriginalEnd = Elements[EElement__Count__ - 4].Offset;	// animation offsets that lie past this point are considered to be external because they are not stored with the element that they animate, this is probably character animation data.
	//UInt32 CameraAnimationDataSize = 0;
	//OriginalEnd += CameraAnimationDataSize;

	// fix animation offsets and find element offsets
	SInt32 CurrentOffset = 0;
	if ( Header.Description.Flags & 0x08 )
	{
		CurrentOffset = sizeof( CM2Header ) + 8;	// +8 to align data at 16 byte bounds
	}
	else
	{
		CurrentOffset = sizeof( CM2Header ) - 8;	// -8 because last 2 UInt32s are not saved
	}

	SInt32 OffsetDelta = 0;
	for ( UInt32 iElement = 0; iElement < EElement__Count__; iElement++ )
	{
		// if this element has data...
		if ( Elements[iElement].DataSize != 0 )
		{
			// if the current element's current offset doesn't match the calculated offset, some data has resized and we need to fix...
			if ( CurrentOffset != Elements[iElement].Offset )
			{
				OffsetDelta = CurrentOffset - Elements[iElement].Offset;

				switch ( iElement )
				{
					case EElement_Bone:
					{
						CElement_Bone* Bones = (CElement_Bone*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Position, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Rotation, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Scale, iElement, false );
						}
						break;
					}

					case EElement_Color:
					{
						CElement_Color* Colors = (CElement_Color*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Color, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Opacity, iElement, false );
						}
						break;
					}

					case EElement_Texture:
					{
						CElement_Texture* Textures = (CElement_Texture*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( Textures[j].oTexturePath );
							Textures[j].oTexturePath += OffsetDelta;
						}
						break;
					}

					case EElement_Transparency:
					{
						CElement_Transparency* Transparencies = (CElement_Transparency*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Transparencies[j].AnimationBlock_Transparency, iElement, false );
						}
						break;
					}

					case EElement_Attachment:
					{
						CElement_Attachment* Attachments = (CElement_Attachment*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Attachments[j].AnimationBlock_Visibility, iElement, false );
						}
						break;
					}

					case EElement_Event:
					{
						CElement_Event* Events = (CElement_Event*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( Events[j].oTimeLines );
							Events[j].oTimeLines += OffsetDelta;
						}
						break;
					}

					case EElement_Light:
					{
						CElement_Light* Lights = (CElement_Light*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientColor, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientIntensity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseColor, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseIntensity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationStart, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationEnd, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_Visibility, iElement, false );
						}
						break;
					}

					case EElement_Camera:
					{
						CElement_Camera* Cameras = (CElement_Camera*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Position, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Target, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Roll, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_FieldOfView, iElement, false );
						}
						break;
					}

					case EElement_RibbonEmitter:  
					{
						// untested!
						CElement_RibbonEmitter* RibbonEmitters = (CElement_RibbonEmitter*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( RibbonEmitters[j].oTexture );
							RibbonEmitters[j].oTexture += OffsetDelta;
							VERIFY_OFFSET_LOCAL( RibbonEmitters[j].oRenderFlag );
							RibbonEmitters[j].oRenderFlag += OffsetDelta;

							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Color, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Opacity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Above, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Below, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Unknown1, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Visibility, iElement, false );
						}
						break;
					}

					case EElement_ParticleEmitter: 
					{
						// untested!
						CElement_ParticleEmitter* ParticleEmitters = (CElement_ParticleEmitter*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( ParticleEmitters[j].oFileNameModel );
							ParticleEmitters[j].oFileNameModel += OffsetDelta;
							VERIFY_OFFSET_LOCAL( ParticleEmitters[j].oUnknown );
							ParticleEmitters[j].oUnknown += OffsetDelta;

							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitSpeed, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_SpeedVariance, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Spread, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Unknown, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Gravity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Lifespan, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitRate, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitLength, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitWidth, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_GravityStrong, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Visibility, iElement, false );
						}
						break;
					}
				}

				// set the element's new offset
				Elements[iElement].Offset = CurrentOffset;
			}
		}
		else
		{
			Elements[iElement].Offset = 0;
		}

		CurrentOffset += Elements[iElement].DataSize;
	}

	if ( Element_NewVertex.DataSize )
	{
		Element_NewVertex.Offset = CurrentOffset;
	}

	if ( OffsetDelta != 0 )
	{
		for ( UInt32 iElement = 0; iElement != EElement__Count__; iElement++ )
		{
			switch ( iElement )
			{
				case EElement_Bone:
				{
					CElement_Bone* Bones = (CElement_Bone*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Position, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Rotation, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Scale, iElement, true );
					}
					break;
				}

				case EElement_Color:
				{
					CElement_Color* Colors = (CElement_Color*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Color, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Opacity, iElement, true );
					}
					break;
				}

				case EElement_Transparency:
				{
					CElement_Transparency* Transparencies = (CElement_Transparency*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Transparencies[j].AnimationBlock_Transparency, iElement, true );
					}
					break;
				}

				case EElement_Attachment:
				{
					CElement_Attachment* Attachments = (CElement_Attachment*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Attachments[j].AnimationBlock_Visibility, iElement, true );
					}
					break;
				}

				case EElement_Event:
				{
					CElement_Event* Events = (CElement_Event*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						CElement_Event::CTimeLine* TimeLines = (CElement_Event::CTimeLine*)Elements[iElement].GetLocalPointer( Events[j].oTimeLines );
						for ( UInt32 k = 0; k < Events[j].nTimeLines; k++ )
						{
							CElement_Event::CTimeLine& TimeLine = TimeLines[k];
							if ( TimeLine.nTimeStamps && ( TimeLine.oTimeStamps > Elements[EElement_CameraLookup].Offset ) )
							{
								VERIFY_OFFSET_NOTLOCAL( TimeLine.oTimeStamps );
								TimeLine.oTimeStamps += OffsetDelta;
							}
						}
					}
					break;
				}

				case EElement_Light:
				{
					CElement_Light* Lights = (CElement_Light*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientColor, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientIntensity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseColor, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseIntensity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationStart, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationEnd, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_Visibility, iElement, true );
					}
					break;
				}

				case EElement_Camera:
				{
					CElement_Camera* Cameras = (CElement_Camera*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Position, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Target, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Roll, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_FieldOfView, iElement, true );
					}
					break;
				}

				case EElement_RibbonEmitter:
				{
					// untested!
					CElement_RibbonEmitter* RibbonEmitters = (CElement_RibbonEmitter*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Color, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Opacity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Above, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Below, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Unknown1, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Visibility, iElement, true );
					}
					break;
				}

				case EElement_ParticleEmitter:
				{
					// untested!
					CElement_ParticleEmitter* ParticleEmitters = (CElement_ParticleEmitter*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitSpeed, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_SpeedVariance, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Spread, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Unknown, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Gravity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Lifespan, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitRate, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitLength, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_EmitWidth, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_GravityStrong, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Visibility, iElement, true );
					}
					break;
				}
			}
		}
	}
}


void M2Lib::CM2::m_FixAnimationOffsets( UInt32 OriginalEnd, SInt32 OffsetDelta, CElement_AnimationBlock& AnimationBlock, SInt32 iElement, bool Post )
{
	// TP is the best
	if ( AnimationBlock.nTimes )
	{
		VERIFY_OFFSET_LOCAL( AnimationBlock.oTimes );
		CElement_AnimationBlock::CChannel* Channels = (CElement_AnimationBlock::CChannel*)Elements[iElement].GetLocalPointer( AnimationBlock.oTimes );
		for ( UInt32 i = 0; i < AnimationBlock.nTimes; i++ )
		{
			CElement_AnimationBlock::CChannel& Channel = Channels[i];
			if ( Channel.o )
			{
				bool bLastBlock = ( Channel.o > OriginalEnd );
				if ( bLastBlock && Post )
				{
					Channel.o += OffsetDelta;
				}
			}
		}

		bool bInThisElem = ( Elements[iElement].Offset < AnimationBlock.oTimes ) && ( AnimationBlock.oTimes < ( Elements[iElement].Offset + Elements[iElement].DataSize ) );
		assert( bInThisElem );
		if ( !Post )
		{
			VERIFY_OFFSET_LOCAL( AnimationBlock.oTimes );
			AnimationBlock.oTimes += OffsetDelta;
		}
	}

	if ( AnimationBlock.nKeys )
	{
		VERIFY_OFFSET_LOCAL( AnimationBlock.oKeys );
		CElement_AnimationBlock::CChannel* Channels = (CElement_AnimationBlock::CChannel*)Elements[iElement].GetLocalPointer( AnimationBlock.oKeys );
		for ( UInt32 i = 0; i < AnimationBlock.nKeys; i++ )
		{
			CElement_AnimationBlock::CChannel& Channel = Channels[i];
			if (Channel.o)
			{
				bool bLastBlock = Channel.o > OriginalEnd;
				if ( bLastBlock && Post )
				{
					Channel.o += OffsetDelta;
				}
			}
		}

		bool bInThisElem = ( Elements[iElement].Offset < AnimationBlock.oKeys ) && ( AnimationBlock.oKeys < ( Elements[iElement].Offset + Elements[iElement].DataSize ) );
		assert( bInThisElem );
		if ( !Post )
		{
			VERIFY_OFFSET_LOCAL( AnimationBlock.oKeys );
			AnimationBlock.oKeys += OffsetDelta;
		}
	}
}



void M2Lib::CM2::m_SaveElements_CopyElementsToHeader()
{
	Header.Description.nName = Elements[EElement_Name].Number;
	Header.Description.oName = Elements[EElement_Name].Offset;

	Header.Elements.nGlobalSequence = Elements[EElement_GlobalSequence].Number;
	Header.Elements.oGlobalSequence = Elements[EElement_GlobalSequence].Offset;

	Header.Elements.nAnimation = Elements[EElement_Animation].Number;
	Header.Elements.oAnimation = Elements[EElement_Animation].Offset;

	Header.Elements.nAnimationLookup = Elements[EElement_AnimationLookup].Number;
	Header.Elements.oAnimationLookup = Elements[EElement_AnimationLookup].Offset;

	Header.Elements.nBone = Elements[EElement_Bone].Number;
	Header.Elements.oBone = Elements[EElement_Bone].Offset;

	Header.Elements.nKeyBoneLookup = Elements[EElement_KeyBoneLookup].Number;
	Header.Elements.oKeyBoneLookup = Elements[EElement_KeyBoneLookup].Offset;

	Header.Elements.nVertex = Elements[EElement_Vertex].Number;
	Header.Elements.oVertex = Elements[EElement_Vertex].Offset;

	if ( Element_NewVertex.Number )
	{
		Header.Elements.nVertex = Element_NewVertex.Number;
		Header.Elements.oVertex = Element_NewVertex.Offset;
	}

	Header.Elements.nSkin = Elements[EElement_Skin].Number;
	//Header.oViews = 0;

	Header.Elements.nColor = Elements[EElement_Color].Number;
	Header.Elements.oColor = Elements[EElement_Color].Offset;

	Header.Elements.nTexture = Elements[EElement_Texture].Number;
	Header.Elements.oTexture = Elements[EElement_Texture].Offset;

	Header.Elements.nTransparency = Elements[EElement_Transparency].Number;
	Header.Elements.oTransparency = Elements[EElement_Transparency].Offset;

	Header.Elements.nTextureAnimation = Elements[EElement_TextureAnimation].Number;
	Header.Elements.oTextureAnimation = Elements[EElement_TextureAnimation].Offset;

	Header.Elements.nTextureReplace = Elements[EElement_TextureReplace].Number;
	Header.Elements.oTextureReplace = Elements[EElement_TextureReplace].Offset;

	Header.Elements.nTextureFlags = Elements[EElement_TextureFlags].Number;
	Header.Elements.oTextureFlags = Elements[EElement_TextureFlags].Offset;

	Header.Elements.nSkinnedBoneLookup = Elements[EElement_SkinnedBoneLookup].Number;
	Header.Elements.oSkinnedBoneLookup = Elements[EElement_SkinnedBoneLookup].Offset;

	Header.Elements.nTextureLookup = Elements[EElement_TextureLookup].Number;
	Header.Elements.oTextureLookup = Elements[EElement_TextureLookup].Offset;

	Header.Elements.nTextureUnitLookup = Elements[EElement_TextureUnitLookup].Number;
	Header.Elements.oTextureUnitLookup = Elements[EElement_TextureUnitLookup].Offset;

	Header.Elements.nTransparencyLookup = Elements[EElement_TransparencyLookup].Number;
	Header.Elements.oTransparencyLookup = Elements[EElement_TransparencyLookup].Offset;

	Header.Elements.nTextureAnimationLookup = Elements[EElement_TextureAnimationLookup].Number;
	Header.Elements.oTextureAnimationLookup = Elements[EElement_TextureAnimationLookup].Offset;

	Header.Elements.nBoundingTriangle = Elements[EElement_BoundingTriangle].Number;
	Header.Elements.oBoundingTriangle = Elements[EElement_BoundingTriangle].Offset;

	Header.Elements.nBoundingVertex = Elements[EElement_BoundingVertex].Number;
	Header.Elements.oBoundingVertex = Elements[EElement_BoundingVertex].Offset;

	Header.Elements.nBoundingNormal = Elements[EElement_BoundingNormal].Number;
	Header.Elements.oBoundingNormal = Elements[EElement_BoundingNormal].Offset;

	Header.Elements.nAttachment = Elements[EElement_Attachment].Number;
	Header.Elements.oAttachment = Elements[EElement_Attachment].Offset;

	Header.Elements.nAttachmentLookup = Elements[EElement_AttachmentLookup].Number;
	Header.Elements.oAttachmentLookup = Elements[EElement_AttachmentLookup].Offset;

	Header.Elements.nEvent = Elements[EElement_Event].Number;
	Header.Elements.oEvent = Elements[EElement_Event].Offset;

	Header.Elements.nLight = Elements[EElement_Light].Number;
	Header.Elements.oLight = Elements[EElement_Light].Offset;

	Header.Elements.nCamera = Elements[EElement_Camera].Number;
	Header.Elements.oCamera = Elements[EElement_Camera].Offset;

	Header.Elements.nCameraLookup = Elements[EElement_CameraLookup].Number;
	Header.Elements.oCameraLookup = Elements[EElement_CameraLookup].Offset;

	Header.Elements.nRibbonEmitter = Elements[EElement_RibbonEmitter].Number;
	Header.Elements.oRibbonEmitter = Elements[EElement_RibbonEmitter].Offset;

	Header.Elements.nParticleEmitter = Elements[EElement_ParticleEmitter].Number;
	Header.Elements.oParticleEmitter = Elements[EElement_ParticleEmitter].Offset;

	Header.Elements.nUnknown1 = Elements[EElement_Unknown1].Number;
	Header.Elements.oUnknown1 = Elements[EElement_Unknown1].Offset;
}
