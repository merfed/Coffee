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

	// load header
	FileStream.read( (Char8*)&Header, 20 );
	if ( Header.Type & 0x08 )
	{
		// special long header
		FileStream.read( (Char8*)&Header.nGlobalSequences, sizeof( CM2Header ) - 20 );
	}
	else
	{
		// normal header
		FileStream.read( (Char8*)&Header.nGlobalSequences, sizeof( CM2Header ) - 28 );
		Header.nUnknown1 = 0;
		Header.oUnknown1 = 0;
	}

	// check header
	if ( Header.ID[0] != 'M' || Header.ID[1] != 'D' || Header.ID[2] != '2' || Header.ID[3] != '0' )
		return M2Lib::EError_FailedToLoadM2_FileCorrupt;
	if ( Header.Version[0] != 8 )
		return M2Lib::EError_FailedToLoadM2_VersionNotSupported;

	// fill elements header data
	m_LoadElements_CopyHeaderToElements();
	m_LoadElements_FindSizes( FileSize );

	// load elements
	UInt32 ElementCount = EElement__Count__;
	if ( !( Header.Type & 0x08 ) )
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
	if ( ( Header.nViews == 0 ) || ( Header.nViews > 4 ) )
		return M2Lib::EError_FailedToLoadM2_FileCorrupt;
	for ( UInt32 i = 0; i != Header.nViews; i++ )
	{
		Char8 szFileSkin[1024];
		GetFileSkin( szFileSkin, szFile, i );

		Skins[i] = new M2Lib::CM2Skin( this );
		if ( M2Lib::EError Error = Skins[i]->Load( szFileSkin ) )
			return Error;
	}

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
	FileStream.write( (Char8*)&Header, 20 );
	if ( Header.Type & 0x08 )
	{
		// long header
		FileStream.write( (Char8*)&Header.nGlobalSequences, sizeof( CM2Header ) - 20 );
	}
	else
	{
		// normal header
		FileStream.write( (Char8*)&Header.nGlobalSequences, sizeof( CM2Header ) - 28 );
	}

	// save elements
	UInt32 ElementCount = EElement__Count__;
	if ( !( Header.Type & 0x08 ) )
	{
		ElementCount--;
	}
	for ( UInt32 i = 0; i != ElementCount; i++ )
	{
		if ( !Elements[i].Save( FileStream ) )
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
	if ( ( Header.nViews == 0 ) || ( Header.nViews > 4 ) )
		return M2Lib::EError_FailedToSaveM2;
	for ( UInt32 i = 0; i < Header.nViews; i++ )
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

	UInt32 SubsetCount = pSkin->Elements[CM2Skin::EElement_Subsets].Number;;
	CM2Skin::CSubset* Subsets = (CM2Skin::CSubset*)pSkin->Elements[CM2Skin::EElement_Subsets].Data;

	Elements[EElement_Vertices].Number;
	M2Lib::CVertex* Vertices = (CVertex*)Elements[EElement_Vertices].Data;
	UInt16* Triangles = (UInt16*)pSkin->Elements[CM2Skin::EElement_Triangles].Data;
	UInt16* Indices = (UInt16*)pSkin->Elements[CM2Skin::EElement_Indices].Data;

	UInt32 BonesCount = Elements[EElement_Bones].Number;
	CBone* Bones = (CBone*)Elements[EElement_Bones].Data;

	UInt32 AttachmentsCount = Elements[EElement_Attachments].Number;
	CAttachment* Attachments = (CAttachment*)Elements[EElement_Attachments].Data;

	UInt32 CamerasCount = Elements[EElement_Cameras].Number;
	CCamera* Cameras = (CCamera*)Elements[EElement_Cameras].Data;

	// write signature
	UInt32 OutSignature = 1;
	BWriter.Write( &OutSignature, sizeof( UInt32 ) );

	// write subsets
	UInt32 TotalVertexCount = 0;
	UInt32 TotalTriangleCount = 0;
	BWriter.Write( &SubsetCount, sizeof( UInt32 ) );
	for ( UInt32 i = 0; i < SubsetCount; i++ )
	{
		M2Lib::CM2Skin::CSubset* pSubsetOut = &Subsets[i];

		// write subset ID
		UInt32 SubsetID = pSubsetOut->ID;
		BWriter.Write( &SubsetID, sizeof( UInt32 ) );

		std::vector< UInt32 > MaterialsIndexes;

		// write root bone
		BWriter.Write( &pSubsetOut->RootBone, sizeof( UInt16 ) );

		// find materials for this subset
		UInt32 MaterialCount = pSkin->Elements[M2Lib::CM2Skin::EElement_Materials].Number;
		M2Lib::CM2Skin::CMaterial* Materials = (M2Lib::CM2Skin::CMaterial*)pSkin->Elements[M2Lib::CM2Skin::EElement_Materials].Data;
		for ( UInt32 j = 0; j < MaterialCount; j++ )
		{
			if ( Materials[j].iSubset == i )
			{
				MaterialsIndexes.push_back( j );
			}
		}
		assert( MaterialsIndexes.size() );
		// write materials
		MaterialCount = MaterialsIndexes.size();
		if ( OutSignature == 1 )
		{
			BWriter.Write( &MaterialCount, sizeof( UInt32 ) );
		}
		for ( UInt32 j = 0; j < MaterialCount; j++ )
		{
			M2Lib::CM2Skin::CMaterial* pMaterial = &Materials[MaterialsIndexes[j]];

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

			if ( OutSignature == 0 )
				break;
		}

		// get first vertex
		UInt32 SubsetVertexStart = pSubsetOut->IndexStart;

		// pre count vertices and triangles, we need to write these numbers
		UInt32 SubsetIndexCountOut = pSubsetOut->IndexCount;
		UInt32 SubsetTriangleCountOut = pSubsetOut->TriangleCount / 3;

		// write vertices
		BWriter.Write( &SubsetIndexCountOut, sizeof( UInt32 ) );
		UInt32 VertexEnd = pSubsetOut->IndexStart + pSubsetOut->IndexCount;
		for ( UInt32 k = pSubsetOut->IndexStart; k < VertexEnd; k++ )
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
		TotalVertexCount += pSubsetOut->IndexCount;

		// write triangles
		BWriter.Write( &SubsetTriangleCountOut, sizeof( UInt32 ) );
		UInt32 TriangleEnd = pSubsetOut->TriangleStart + pSubsetOut->TriangleCount;
		for ( UInt32 k = pSubsetOut->TriangleStart; k < TriangleEnd; k++ )
		{
			UInt16 IndexOut = Triangles[k] - SubsetVertexStart;
			assert( IndexOut < SubsetIndexCountOut );
			BWriter.Write( &IndexOut, sizeof( UInt16 ) );
		}
		TotalTriangleCount += pSubsetOut->TriangleCount;
	}

	// write bones
	BWriter.Write( &BonesCount, sizeof( UInt32 ) );
	for ( UInt16 i = 0; i < BonesCount; i++ )
	{
		CBone& Bone = Bones[i];

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
		CAttachment& Attachment = Attachments[i];

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
		CCamera& Camera = Cameras[i];

		BWriter.Write( &Camera.Type, sizeof( SInt32 ) );
		BWriter.Write( &Camera.FieldOfView, sizeof( Float32 ) );
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


M2Lib::EError M2Lib::CM2::ImportM2Intermediate( Char8* szFile, bool IgnoreExtras, Float32 SubmeshPositionalTolerance, Float32 SubmeshAngularTolerance, Float32 BodyPositionalTolerance, Float32 BodyAngularTolerance, Float32 ClothingPositionalTolerance, Float32 ClothingAngularTolerance )
{
	if ( !szFile )
		return EError_FailedToImportM2I_NoFileSpecified;

	// check that we have an M2 already loaded and ready to be injected
	if ( Elements[EElement_Skins].Number == 0 )
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

	// read signature
	UInt32 InSignature = 0;
	BReader.Read( &InSignature, sizeof( UInt32 ) );
	if ( InSignature != 0 && InSignature != 1 )
		return M2Lib::EError_FailedToImportM2I_FileCorrupt;

	// read subsets, build new global vertex list
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

		// read root bone
		BReader.Read( &pNewSubset->RootBone, sizeof( UInt16 ) );

		// read materials
		if ( InSignature == 0 )
		{
			// version 0, one material

			M2Lib::CM2Skin::CMaterial NewMaterial;

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
		else if ( InSignature == 1 )
		{
			// version 1, multiple materials

			UInt32 InMaterialCount;
			BReader.Read( &InMaterialCount, sizeof( UInt32 ) );
			for ( UInt32 j = 0; j < InMaterialCount; j++ )
			{
				M2Lib::CM2Skin::CMaterial NewMaterial;

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

		InM2I.Subsets.push_back( pNewSubset );
	}

	if ( !IgnoreExtras )
	{
		// read bones, overwrite existing
		if ( g_Verbose >= 2 )
		{
			std::cout << "\t\tloading bones..." << std::endl;
		}
		UInt32 BoneCount = Elements[EElement_Bones].Number;
		CBone* Bones = (CBone*)Elements[EElement_Bones].Data;
		UInt32 BoneCountIn;
		BReader.Read( &BoneCountIn, sizeof( UInt32 ) );
		for ( UInt32 i = 0; i < BoneCountIn; i++ )
		{
			UInt16 InBoneIndex = 0;
			BReader.Read( &InBoneIndex, sizeof( UInt16 ) );
			assert( InBoneIndex < BoneCount );
			CBone& BoneToMod = Bones[InBoneIndex];

			BReader.Read( &BoneToMod.ParentBone, sizeof( SInt16 ) );

			BReader.Read( &BoneToMod.Position[0], sizeof( Float32 ) );
			BReader.Read( &BoneToMod.Position[1], sizeof( Float32 ) );
			BReader.Read( &BoneToMod.Position[2], sizeof( Float32 ) );
		}

		// read attachments, overwrite existing
		if ( g_Verbose >= 2 )
		{
			std::cout << "\t\tloading attachments..." << std::endl;
		}
		UInt32 AttachmentsCount = Elements[EElement_Attachments].Number;
		CAttachment* Attachments = (CAttachment*)Elements[EElement_Attachments].Data;
		UInt32 AttachmentCountIn;
		BReader.Read( &AttachmentCountIn, sizeof( UInt32 ) );
		for ( UInt32 i = 0; i < AttachmentCountIn; i++ )
		{
			UInt32 InAttachmentID = 0;
			BReader.Read( &InAttachmentID, sizeof( UInt32 ) );
			CAttachment* pAttachmentToMod = 0;
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

		// read cameras, overwrite existing
		if ( g_Verbose >= 2 )
		{
			std::cout << "\t\tloading cameras..." << std::endl;
		}
		UInt32 CameraCount = Elements[EElement_Cameras].Number;
		CCamera* Cameras = (CCamera*)Elements[EElement_Cameras].Data;
		UInt32 CameraCountIn;
		BReader.Read( &CameraCountIn, sizeof( UInt32 ) );
		for ( UInt32 i = 0; i < CameraCountIn; i++ )
		{
			SInt32 InType;
			BReader.Read( &InType, sizeof( SInt32 ) );

			CCamera* pCameraToMod = 0;
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

				BReader.Read( &pCameraToMod->FieldOfView, sizeof( Float32 ) );
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
	Elements[EElement_Skins].Number = 0;
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
	Elements[EElement_Skins].Number = iSkin;
	if ( iSkin == 0 )
	{
		int pause = 1;
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

	void* pIn;
	void* pOut;

	// copy new global vertex list
	Elements[EElement_Vertices].SetDataSize( NewVertexList.size(), NewVertexList.size() * sizeof( M2Lib::CVertex ), false );
	pIn = &NewVertexList[0];
	pOut = Elements[EElement_Vertices].Data;
	memcpy( pOut, pIn, NewVertexList.size() * sizeof( M2Lib::CVertex ) );

	// copy new global bone lookup
	Elements[EElement_BonesLookup].SetDataSize( NewBoneLookup.size(), NewBoneLookup.size() * sizeof( UInt16 ), false );
	pIn = &NewBoneLookup[0];
	pOut = Elements[EElement_BonesLookup].Data;
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

	// done, ready to be saved
	return M2Lib::EError_OK;
}


void M2Lib::CM2::PrintInfo()
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

	FileStream << "Number Global Sequences: " << Header.nGlobalSequences << std::endl;
	FileStream << "Offset Global Sequences: " << Header.oGlobalSequences << std::endl << std::endl;

	FileStream << "Number Animations: " << Header.nAnimations << std::endl;
	FileStream << "Offset Animations: " << Header.oAnimations << std::endl << std::endl;

	FileStream << "Number Animations Lookup: " << Header.nAnimationsLookup << std::endl;
	FileStream << "Offset Animations Lookup: " << Header.oAnimationsLookup << std::endl << std::endl;

	FileStream << "Number Bones: " << Header.nBones << std::endl;
	FileStream << "Offset Bones: " << Header.oBones << std::endl << std::endl;

	FileStream << "Number Bones Lookup: " << Header.nBonesLookup << std::endl;
	FileStream << "Offset Bones Lookup: " << Header.oBonesLookup << std::endl << std::endl;

	FileStream << "Number Vertices: " << Header.nVertices << std::endl;
	FileStream << "Offset Vertices: " << Header.oVertices << std::endl << std::endl;

	FileStream << "Number Views: " << Header.nViews << std::endl << std::endl;

	FileStream << "Number Colors: " << Header.nColors << std::endl;
	FileStream << "Offset Colors: " << Header.oColors << std::endl << std::endl;

	FileStream << "Number Textures: " << Header.nTextures << std::endl;
	FileStream << "Offset Textures: " << Header.oTextures << std::endl << std::endl;

	FileStream << "Number Transparencies: " << Header.nTransparencies << std::endl;
	FileStream << "Offset Transparencies: " << Header.oTransparencies << std::endl << std::endl << std::endl << std::endl;

	//CAnimation* Animations = (CAnimation*)Elements[EElement_Animations].Data;
	//Count = Elements[EElement_Animations].Number;
	//FileStream << "Animations: " << Count << std::endl << std::endl;
	//for ( UInt32 i = 0; i < Count; i++ )
	//{
	//	CAnimation& Animation = Animations[i];
	//	FileStream << "[" << i << "]" << std::endl;
	//	FileStream << "AnimationID: " << Animation.AnimationID << std::endl;
	//	FileStream << "AnimationSubID: " << Animation.AnimationSubID << std::endl;
	//	FileStream << "Lenth: " << Animation.Length << std::endl;
	//	FileStream << "Move Speed: " << Animation.MoveSpeed << std::endl;
	//	FileStream << "Loop Type: " << Animation.LoopType << std::endl;
	//	FileStream << "Flags: " << Animation.Flags << std::endl;
	//	FileStream << "Unknown0: " << Animation.Unknown0 << std::endl;
	//	FileStream << "Unknown1: " << Animation.Unknown1 << std::endl;
	//	FileStream << "Next Animation: " << Animation.NextAnimation<< std::endl;
	//	FileStream << "Index: " << Animation.Index << std::endl << std::endl;
	//}
	//FileStream << std::endl << std::endl;

	//UInt16* TextureUnits = (UInt16*)Elements[EElement_TextureUnitsLookup].Data;
	//Count = Elements[EElement_TextureUnitsLookup].Number;
	//FileStream << "TextureUnits: " << Count << std::endl << std::endl;
	//for ( UInt32 i = 0; i < Count; i++ )
	//{
	//	FileStream << TextureUnits[i] << std::endl;
	//}
	//FileStream << std::endl << std::endl;

	CVertex* Vertices = (CVertex*)Elements[EElement_Vertices].Data;

	CRenderFlag* iRenderFlags = (CRenderFlag*)Elements[EElement_TextureFlags].Data;
	Count = Elements[EElement_TextureFlags].Number;
	FileStream << "Render Flags: " << Count << std::endl << std::endl;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CRenderFlag& RenderFlag = iRenderFlags[i];
		FileStream << "[" << i << "]" << std::endl;
		FileStream << "A: " << RenderFlag.A << std::endl;
		FileStream << "A: " << RenderFlag.B << std::endl << std::endl;
	}

	CTexture* Textures = (CTexture*)Elements[EElement_Textures].Data;
	Count = Elements[EElement_Textures].Number;
	FileStream << "Textures: " << Count << std::endl << std::endl;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CTexture& Texture = Textures[i];
		FileStream << "[" << i << "]" << std::endl;
		FileStream << "Type: " << Texture.Type << std::endl;
		FileStream << "Flags: " << Texture.Flags << std::endl;
		FileStream << "File: " << (Char8*)Elements[EElement_Textures].GetLocalPointer( Texture.oName ) << std::endl << std::endl;
	}
	FileStream << std::endl << std::endl;

	CCamera* Cameras = (CCamera*)Elements[EElement_Cameras].Data;
	Count = Elements[EElement_Cameras].Number;
	FileStream << "Cameras: " << Count << std::endl << std::endl;
	for ( UInt32 i = 0; i < Count; i++ )
	{
		CCamera& Camera = Cameras[i];
		FileStream << "[" << i << "]" << std::endl;
		FileStream << "Type: " << Camera.Type << std::endl;
		FileStream << "FieldOfView: " << Camera.FieldOfView << std::endl;
		FileStream << "ClipFar: " << Camera.ClipFar << std::endl;
		FileStream << "ClipNear: " << Camera.ClipNear << std::endl;
		FileStream << "Position: (" << Camera.Position[0] << ", " << Camera.Position[1] << ", " << Camera.Position[2] << ")" << std::endl;
		FileStream << "Target: (" << Camera.Target[0] << ", " << Camera.Target[1] << ", " << Camera.Target[2] << ")" << std::endl << std::endl;
	}

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
	for ( UInt32 i = 0; i != Header.nViews; i++ )
	{
		if ( Skins[i] )
		{
			Skins[i]->BuildVertexBoneIndices();
		}
	}
}


void M2Lib::CM2::BuildMaxBones()
{
	for ( UInt32 i = 0; i != Header.nViews; i++ )
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

	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)Elements[EElement_Vertices].Data;
	UInt16* Indices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Data;
	M2Lib::CM2Skin::CSubset* Subsets = (M2Lib::CM2Skin::CSubset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Number;

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
	std::vector< std::vector< M2Lib::CM2Skin::CSubset* > > SubMeshes;

	M2Lib::CM2Skin::CSubset* Subsets = (M2Lib::CM2Skin::CSubset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Number;
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
			std::vector< M2Lib::CM2Skin::CSubset* > NewSubmeshSubsetList;
			NewSubmeshSubsetList.push_back( &Subsets[i] );
			SubMeshes.push_back( NewSubmeshSubsetList );
		}
	}


	UInt32 VerticesCount = Elements[EElement_Vertices].Number;
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)Elements[EElement_Vertices].Data;

	UInt32 IndicesCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Number;
	UInt16* Indices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Data;

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
			M2Lib::CM2Skin::CSubset* pSubSet1 = SubMeshes[iSubMesh1][iSubSet1];

			UInt32 IndexEnd1 = pSubSet1->IndexStart + pSubSet1->IndexCount;
			for ( UInt32 iIndex1 = pSubSet1->IndexStart; iIndex1 < IndexEnd1; iIndex1++ )
			{
				UInt32 iVertex1 = Indices[iIndex1];
				if ( ProcessedVertices[iVertex1] )
				{
					continue;
				}
				M2Lib::CVertex& Vertex1 = Vertices[iVertex1];

				bool AddedVertex1 = false;

				for ( UInt32 iSubSet2 = 0; iSubSet2 < SubMeshes[iSubMesh1].size(); iSubSet2++ )
				{
					M2Lib::CM2Skin::CSubset* pSubSet2 = SubMeshes[iSubMesh1][iSubSet2];

					UInt32 IndexEnd2 = pSubSet2->IndexStart + pSubSet2->IndexCount;
					for ( UInt32 iIndex2 = pSubSet2->IndexStart; iIndex2 < IndexEnd2; iIndex2++ )
					{
						UInt32 iVertex2 = Indices[iIndex2];
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
	std::vector< std::vector< M2Lib::CM2Skin::CSubset* > > SubMeshes;

	// gather up the body submeshes
	M2Lib::CM2Skin::CSubset* Subsets = (M2Lib::CM2Skin::CSubset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Number;
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
				std::vector< M2Lib::CM2Skin::CSubset* > NewSubmeshSubsetList;
				NewSubmeshSubsetList.push_back( &Subsets[i] );
				SubMeshes.push_back( NewSubmeshSubsetList );
			}
		}
	}


	// now that all the body submeshes are gathered up in a list, we can average normals of duplicate vertices between them
	// but make sure not to compare against vertices that are in the same submesh
	UInt32 VerticesCount = Elements[EElement_Vertices].Number;
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)Elements[EElement_Vertices].Data;

	UInt32 IndicesCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Number;
	UInt16* Indices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Data;


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
			UInt32 IndexStart1 = SubMeshes[iSubMesh1][iSubSet1]->IndexStart;
			UInt32 IndexEnd1 = IndexStart1 + SubMeshes[iSubMesh1][iSubSet1]->IndexCount;
			for ( UInt32 iIndex1 = IndexStart1; iIndex1 < IndexEnd1; iIndex1++ )
			{
				// get this vertex
				UInt32 iVertex1 = Indices[iIndex1];
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
						UInt32 IndexStart2 = SubMeshes[iSubMesh2][iSubSet2]->IndexStart;
						UInt32 IndexEnd2 = IndexStart2 + SubMeshes[iSubMesh2][iSubSet2]->IndexCount;
						for ( UInt32 iIndex2 = IndexStart2; iIndex2 < IndexEnd2; iIndex2++ )
						{
							UInt32 iVertex2 = Indices[iIndex2];
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
	M2Lib::CVertex* Vertices = (M2Lib::CVertex*)Elements[EElement_Vertices].Data;
	UInt16* Indices = (UInt16*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Indices].Data;
	M2Lib::CM2Skin::CSubset* Subsets = (M2Lib::CM2Skin::CSubset*)Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Data;
	UInt32 SubsetCount = Skins[0]->Elements[M2Lib::CM2Skin::EElement_Subsets].Number;

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

	// copy vertex properties from main body vertex to duplicate clothing vertices
	for ( UInt32 iSubMeshOther = 0; iSubMeshOther < SubMeshesOther.size(); iSubMeshOther++ )
	{
		// for each subset vertex
		std::vector< M2Lib::CM2Skin::CSubset* >& SubMeshOther = SubMeshesOther[iSubMeshOther];
		for ( UInt32 iSubset = 0; iSubset < SubMeshOther.size(); iSubset++ )
		{
			M2Lib::CM2Skin::CSubset* pSubset = SubMeshOther[iSubset];

			// compare with body vertices
			for ( UInt32 iSubsetBody = 0; iSubsetBody < SubMeshBody.size(); iSubsetBody++ )
			{
				M2Lib::CM2Skin::CSubset* pSubsetBody = SubMeshBody[iSubsetBody];
				for ( SInt32 iIndex = pSubset->IndexStart; iIndex < pSubset->IndexStart + pSubset->IndexCount; iIndex++ )
				{
					UInt32 VertexA = Indices[iIndex];
					for ( SInt32 iIndexBody = pSubsetBody->IndexStart; iIndexBody < pSubsetBody->IndexStart + pSubsetBody->IndexCount; iIndexBody++ )
					{
						UInt32 VertexB = Indices[iIndexBody];
						if ( M2Lib::CVertex::CompareSimilar( Vertices[VertexA], Vertices[VertexB], false, false, PositionalTolerance, AngularTolerance ) )
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
		UInt32 VertexListLength = Elements[EElement_Vertices].Number;
		CVertex* VertexList = (CVertex*)Elements[EElement_Vertices].Data;
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
		UInt32 BoneListLength = Elements[EElement_Bones].Number;
		CBone* BoneList = (CBone*)Elements[EElement_Bones].Data;
		for ( UInt32 i = 0; i < BoneListLength; i++ )
		{
			CBone& Bone = BoneList[i];
			Bone.Position[0] *= Scale;
			Bone.Position[1] *= Scale;
			Bone.Position[2] *= Scale;
		}
	}

	// attachments
	{
		UInt32 AttachmentListLength = Elements[EElement_Attachments].Number;
		CAttachment* AttachmentList = (CAttachment*)Elements[EElement_Attachments].Data;
		for ( UInt32 i = 0; i < AttachmentListLength; i++ )
		{
			CAttachment& Attachment = AttachmentList[i];
			Attachment.Position[0] *= Scale;
			Attachment.Position[1] *= Scale;
			Attachment.Position[2] *= Scale;
		}
	}

	// events
	{
		UInt32 EventListLength = Elements[EElement_Events].Number;
		CEvent* EventList = (CEvent*)Elements[EElement_Events].Data;
		for ( UInt32 i = 0; i < EventListLength; i++ )
		{
			CEvent& Event = EventList[i];
			Event.Position[0] *= Scale;
			Event.Position[1] *= Scale;
			Event.Position[2] *= Scale;
		}
	}

	// lights
	{
		UInt32 LightListLength = Elements[EElement_Lights].Number;
		CLight* LightList = (CLight*)Elements[EElement_Lights].Data;
		for ( UInt32 i = 0; i < LightListLength; i++ )
		{
			CLight& Light = LightList[i];
			Light.Position[0] *= Scale;
			Light.Position[1] *= Scale;
			Light.Position[2] *= Scale;
		}
	}

	// cameras
	{
		UInt32 CameraListLength = Elements[EElement_Cameras].Number;
		CCamera* CameraList = (CCamera*)Elements[EElement_Cameras].Data;
		for ( UInt32 i = 0; i < CameraListLength; i++ )
		{
			CCamera& Camera = CameraList[i];
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
		UInt32 RibbonEmitterListLength = Elements[EElement_RibbonEmitters].Number;
		CRibbonEmitter* RibbonEmitterList = (CRibbonEmitter*)Elements[EElement_RibbonEmitters].Data;
		for ( UInt32 i = 0; i < RibbonEmitterListLength; i++ )
		{
			CRibbonEmitter& RibbonEmitter = RibbonEmitterList[i];
			RibbonEmitter.Position[0] *= Scale;
			RibbonEmitter.Position[1] *= Scale;
			RibbonEmitter.Position[2] *= Scale;
		}
	}

	// particle emitters
	{
		UInt32 ParticleEmitterListLength = Elements[EElement_ParticleEmitters].Number;
		CParticleEmitter* ParticleEmitterList = (CParticleEmitter*)Elements[EElement_ParticleEmitters].Data;
		for ( UInt32 i = 0; i < ParticleEmitterListLength; i++ )
		{
			CParticleEmitter& ParticleEmitter = ParticleEmitterList[i];
			ParticleEmitter.Position[0] *= Scale;
			ParticleEmitter.Position[1] *= Scale;
			ParticleEmitter.Position[2] *= Scale;
		}
	}
}


void M2Lib::CM2::ScaleAnimation( M2Lib::CM2Element* pElement, CAnimationBlock* pAnimationBlock, Float32 Scale )
{
	UInt32 TimeLinesCount = pAnimationBlock->nKeys;
	CAnimationBlock::CIndirectPair* TimeLines = (CAnimationBlock::CIndirectPair*)pElement->GetLocalPointer( pAnimationBlock->oKeys );

	for ( UInt32 i = 0; i < TimeLinesCount; i++ )
	{
		for ( UInt32 j = 0; j < TimeLines[i].n; j++ )
		{
			if ( ( TimeLines[i].o > pElement->Offset ) && TimeLines[i].o < ( pElement->Offset + pElement->DataSize ) )
			{
				CAnimationBlock::SKeyFloat3* Keys = (CAnimationBlock::SKeyFloat3*)pElement->GetLocalPointer( TimeLines[i].o );
				Keys[j].Values[0] *= Scale;
				Keys[j].Values[1] *= Scale;
				Keys[j].Values[2] *= Scale;
			}
		}
	}
}


void M2Lib::CM2::MirrorCamera()
{
	CCamera* Cameras = (CCamera*)Elements[EElement_Cameras].Data;
	UInt32 CameraCount = Elements[EElement_Cameras].Number;
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
	Elements[EElement_Name].Number = Header.nName;
	Elements[EElement_Name].Offset = Header.oName;

	Elements[EElement_GlobalSequences].Number = Header.nGlobalSequences;
	Elements[EElement_GlobalSequences].Offset = Header.oGlobalSequences;

	Elements[EElement_Animations].Number = Header.nAnimations;
	Elements[EElement_Animations].Offset = Header.oAnimations;

	Elements[EElement_AnimationsLookup].Number = Header.nAnimationsLookup;
	Elements[EElement_AnimationsLookup].Offset = Header.oAnimationsLookup;

	Elements[EElement_Bones].Number = Header.nBones;
	Elements[EElement_Bones].Offset = Header.oBones;

	Elements[EElement_KeyBonesLookup].Number = Header.nKeyBonesLookup;
	Elements[EElement_KeyBonesLookup].Offset = Header.oKeyBonesLookup;

	Elements[EElement_Vertices].Number = Header.nVertices;
	Elements[EElement_Vertices].Offset = Header.oVertices;

	Elements[EElement_Skins].Number = Header.nViews;
	Elements[EElement_Skins].Offset = 0;

	Elements[EElement_Colors].Number = Header.nColors;
	Elements[EElement_Colors].Offset = Header.oColors;

	Elements[EElement_Textures].Number = Header.nTextures;
	Elements[EElement_Textures].Offset = Header.oTextures;

	Elements[EElement_Transparencies].Number = Header.nTransparencies;
	Elements[EElement_Transparencies].Offset = Header.oTransparencies;

	Elements[EElement_TextureAnimations].Number = Header.nTextureAnimations;
	Elements[EElement_TextureAnimations].Offset = Header.oTextureAnimations;

	Elements[EElement_TextureReplace].Number = Header.nTextureReplace;
	Elements[EElement_TextureReplace].Offset = Header.oTextureReplace;

	Elements[EElement_TextureFlags].Number = Header.nTextureFlags;
	Elements[EElement_TextureFlags].Offset = Header.oTextureFlags;

	Elements[EElement_BonesLookup].Number = Header.nBonesLookup;
	Elements[EElement_BonesLookup].Offset = Header.oBonesLookup;

	Elements[EElement_TexturesLookup].Number = Header.nTexturesLookup;
	Elements[EElement_TexturesLookup].Offset = Header.oTexturesLookup;

	Elements[EElement_TextureUnitsLookup].Number = Header.nTextureUnitsLookup;
	Elements[EElement_TextureUnitsLookup].Offset = Header.oTextureUnitsLookup;

	Elements[EElement_TransparenciesLookup].Number = Header.nTransparenciesLookup;
	Elements[EElement_TransparenciesLookup].Offset = Header.oTransparenciesLookup;

	Elements[EElement_TextureAnimationsLookup].Number = Header.nTextureAnimationsLookup;
	Elements[EElement_TextureAnimationsLookup].Offset = Header.oTextureAnimationsLookup;

	Elements[EElement_BoundingTriangles].Number = Header.nBoundingTriangles;
	Elements[EElement_BoundingTriangles].Offset = Header.oBoundingTriangles;

	Elements[EElement_BoundingVertices].Number = Header.nBoundingVertices;
	Elements[EElement_BoundingVertices].Offset = Header.oBoundingVertices;

	Elements[EElement_BoundingNormals].Number = Header.nBoundingNormals;
	Elements[EElement_BoundingNormals].Offset = Header.oBoundingNormals;

	Elements[EElement_Attachments].Number = Header.nAttachments;
	Elements[EElement_Attachments].Offset = Header.oAttachments;

	Elements[EElement_AttachmentsLookup].Number = Header.nAttachmentsLookup;
	Elements[EElement_AttachmentsLookup].Offset = Header.oAttachmentsLookup;

	Elements[EElement_Events].Number = Header.bEvents;
	Elements[EElement_Events].Offset = Header.oEvents;

	Elements[EElement_Lights].Number = Header.nLights;
	Elements[EElement_Lights].Offset = Header.oLights;

	Elements[EElement_Cameras].Number = Header.nCameras;
	Elements[EElement_Cameras].Offset = Header.oCameras;

	Elements[EElement_CamerasLookup].Number = Header.nCamerasLookup;
	Elements[EElement_CamerasLookup].Offset = Header.oCamersaLookup;

	Elements[EElement_RibbonEmitters].Number = Header.nRibbonEmitters;
	Elements[EElement_RibbonEmitters].Offset = Header.oRibbonEmitters;

	Elements[EElement_ParticleEmitters].Number = Header.nParticleEmitters;
	Elements[EElement_ParticleEmitters].Offset = Header.oParticleEmitters;

	Elements[EElement_Unknown1].Number = Header.nUnknown1;
	Elements[EElement_Unknown1].Offset = Header.oUnknown1;

	for ( UInt32 i = 0; i < EElement__Count__; i++ )
	{
		Elements[i].OffsetOriginal = Elements[i].Offset;
	}
}


void M2Lib::CM2::m_LoadElements_FindSizes( UInt32 FileSize )
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
				if ( Elements[j].Offset != 0 )
				{
					NextOffset = Elements[j].Offset;
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
	if ( !( Header.Type & 0x08 ) )
	{
		NewSize -= 8;
	}
	for ( UInt32 i = 0; i < EElement__Count__; i++ )
	{
		NewSize += Elements[i].DataSize;
	}
	SInt32 OffsetDeltaEnd = NewSize - m_OriginalSize;				// the delta to apply to animation offsets that lie past m_OriginalSize. if the file grew, this will be positive. if the file shrank this will be negative.
	UInt32 OriginalEnd = Elements[EElement__Count__ - 4].Offset;	// animation pointers that lie past this point are considered to be external
	//OriginalEnd = m_OriginalSize;

	// fix animation offsets and find element offsets
	UInt32 CurrentOffset = 0;
	if ( Header.Type & 0x08 )
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
					case EElement_Bones:
					{
						CBone* Bones = (CBone*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Position, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Rotation, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Scale, iElement, false );
						}
						break;
					}

					case EElement_Colors:
					{
						CColor* Colors = (CColor*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Color, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Opacity, iElement, false );
						}
						break;
					}

					case EElement_Textures:
					{
						CTexture* Textures = (CTexture*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( Textures[j].oName );
							Textures[j].oName += OffsetDelta;
						}
						break;
					}

					case EElement_Transparencies:
					{
						CTransparency* Transparencies = (CTransparency*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Transparencies[j].AnimationBlock_Transparency, iElement, false );
						}
						break;
					}

					case EElement_Attachments:
					{
						CAttachment* Attachments = (CAttachment*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Attachments[j].AnimationBlock_Animation, iElement, false );
						}
						break;
					}

					case EElement_Events:
					{
						CEvent* Events = (CEvent*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( Events[j].oTimeLines );
							Events[j].oTimeLines += OffsetDelta;
						}
						break;
					}

					case EElement_Lights:
					{
						CLight* Lights = (CLight*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientColor, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientIntensity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseColor, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseIntensity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationStart, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationEnd, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_Unknown, iElement, false );
						}
						break;
					}

					case EElement_Cameras:
					{
						CCamera* Cameras = (CCamera*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Position, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Target, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Roll, iElement, false );
						}
						break;
					}

					case EElement_RibbonEmitters:  
					{
						// untested!
						CRibbonEmitter* RibbonEmitters = (CRibbonEmitter*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( RibbonEmitters[j].oTextures );
							RibbonEmitters[j].oTextures += OffsetDelta;
							VERIFY_OFFSET_LOCAL( RibbonEmitters[j].oBlend );
							RibbonEmitters[j].oBlend += OffsetDelta;

							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Color, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Opacity, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Above, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Below, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Unknown1, iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Unknown2, iElement, false );
						}
						break;
					}

					case EElement_ParticleEmitters: 
					{
						// untested!
						CParticleEmitter* ParticleEmitters = (CParticleEmitter*)Elements[iElement].Data;
						for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
						{
							VERIFY_OFFSET_LOCAL( ParticleEmitters[j].oFileNameModel );
							ParticleEmitters[j].oFileNameModel += OffsetDelta;
							VERIFY_OFFSET_LOCAL( ParticleEmitters[j].oZero2 );
							ParticleEmitters[j].oZero2 += OffsetDelta;

							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[0], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[1], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[2], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[3], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[4], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[5], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[6], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[7], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[8], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[9], iElement, false );
							m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Enable, iElement, false );
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


	if ( OffsetDelta != 0 )
	{
		for ( UInt32 iElement = 0; iElement != EElement__Count__; iElement++ )
		{
			switch ( iElement )
			{
				case EElement_Bones:
				{
					CBone* Bones = (CBone*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Position, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Rotation, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Bones[j].AnimationBlock_Scale, iElement, true );
					}
					break;
				}

				case EElement_Colors:
				{
					CColor* Colors = (CColor*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Color, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Colors[j].AnimationBlock_Opacity, iElement, true );
					}
					break;
				}

				case EElement_Transparencies:
				{
					CTransparency* Transparencies = (CTransparency*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Transparencies[j].AnimationBlock_Transparency, iElement, true );
					}
					break;
				}

				case EElement_Attachments:
				{
					CAttachment* Attachments = (CAttachment*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Attachments[j].AnimationBlock_Animation, iElement, true );
					}
					break;
				}

				case EElement_Events:
				{
					CEvent* Events = (CEvent*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						CEvent::CTimeLine* TimeLines = (CEvent::CTimeLine*)Elements[iElement].GetLocalPointer( Events[j].oTimeLines );
						for ( UInt32 k = 0; k < Events[j].nTimeLines; k++ )
						{
							CEvent::CTimeLine& TimeLine = TimeLines[k];
							if ( TimeLine.nTimeStamps && ( TimeLine.oTimeStamps > Elements[EElement_CamerasLookup].Offset ) )
							{
								VERIFY_OFFSET_NOTLOCAL( TimeLine.oTimeStamps );
								TimeLine.oTimeStamps += OffsetDelta;
							}
						}
					}
					break;
				}

				case EElement_Lights:
				{
					CLight* Lights = (CLight*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientColor, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AmbientIntensity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseColor, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_DiffuseIntensity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationStart, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_AttenuationEnd, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Lights[j].AnimationBlock_Unknown, iElement, true );
					}
					break;
				}

				case EElement_Cameras:
				{
					CCamera* Cameras = (CCamera*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Position, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Target, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, Cameras[j].AnimationBlock_Roll, iElement, true );
					}
					break;
				}

				case EElement_RibbonEmitters:
				{
					// untested!
					CRibbonEmitter* RibbonEmitters = (CRibbonEmitter*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Color, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Opacity, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Above, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Below, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Unknown1, iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, RibbonEmitters[j].AnimationBlock_Unknown2, iElement, true );
					}
					break;
				}

				case EElement_ParticleEmitters:
				{
					// untested!
					CParticleEmitter* ParticleEmitters = (CParticleEmitter*)Elements[iElement].Data;
					for ( UInt32 j = 0; j < Elements[iElement].Number; j++ )
					{
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[0], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[1], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[2], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[3], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[4], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[5], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[6], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[7], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[8], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Blocks[9], iElement, true );
						m_FixAnimationOffsets( OriginalEnd, OffsetDelta, ParticleEmitters[j].AnimationBlock_Enable, iElement, true );
					}
					break;
				}
			}
		}
	}
}


void M2Lib::CM2::m_FixAnimationOffsets( UInt32 OriginalEnd, SInt32 OffsetDelta, CAnimationBlock& AnimationBlock, SInt32 iElement, bool Post )
{
	// TP is the best
	if ( AnimationBlock.nTimes )
	{
		VERIFY_OFFSET_LOCAL( AnimationBlock.oTimes );
		CAnimationBlock::CIndirectPair* IndirectPairs = (CAnimationBlock::CIndirectPair*)Elements[iElement].GetLocalPointer( AnimationBlock.oTimes );
		for ( UInt32 i = 0; i < AnimationBlock.nTimes; i++ )
		{
			CAnimationBlock::CIndirectPair& IndirectPair = IndirectPairs[i];
			if ( IndirectPair.o )
			{
				bool bLastBlock = ( IndirectPair.o > OriginalEnd );
				if ( bLastBlock && Post )
				{
					IndirectPair.o += OffsetDelta;
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
		CAnimationBlock::CIndirectPair* IndirectPairs = (CAnimationBlock::CIndirectPair*)Elements[iElement].GetLocalPointer( AnimationBlock.oKeys );
		for ( UInt32 i = 0; i < AnimationBlock.nKeys; i++ )
		{
			CAnimationBlock::CIndirectPair& IndirectPair = IndirectPairs[i];
			if (IndirectPair.o)
			{
				bool bLastBlock = IndirectPair.o > OriginalEnd;
				if ( bLastBlock && Post )
				{
					IndirectPair.o += OffsetDelta;
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
	Header.nName = Elements[EElement_Name].Number;
	Header.oName = Elements[EElement_Name].Offset;

	Header.nGlobalSequences = Elements[EElement_GlobalSequences].Number;
	Header.oGlobalSequences = Elements[EElement_GlobalSequences].Offset;

	Header.nAnimations = Elements[EElement_Animations].Number;
	Header.oAnimations = Elements[EElement_Animations].Offset;

	Header.nAnimationsLookup = Elements[EElement_AnimationsLookup].Number;
	Header.oAnimationsLookup = Elements[EElement_AnimationsLookup].Offset;

	Header.nBones = Elements[EElement_Bones].Number;
	Header.oBones = Elements[EElement_Bones].Offset;

	Header.nKeyBonesLookup = Elements[EElement_KeyBonesLookup].Number;
	Header.oKeyBonesLookup = Elements[EElement_KeyBonesLookup].Offset;

	Header.nVertices = Elements[EElement_Vertices].Number;
	Header.oVertices = Elements[EElement_Vertices].Offset;

	Header.nViews = Elements[EElement_Skins].Number;
	//Header.oViews = 0;

	Header.nColors = Elements[EElement_Colors].Number;
	Header.oColors = Elements[EElement_Colors].Offset;

	Header.nTextures = Elements[EElement_Textures].Number;
	Header.oTextures = Elements[EElement_Textures].Offset;

	Header.nTransparencies = Elements[EElement_Transparencies].Number;
	Header.oTransparencies = Elements[EElement_Transparencies].Offset;

	Header.nTextureAnimations = Elements[EElement_TextureAnimations].Number;
	Header.oTextureAnimations = Elements[EElement_TextureAnimations].Offset;

	Header.nTextureReplace = Elements[EElement_TextureReplace].Number;
	Header.oTextureReplace = Elements[EElement_TextureReplace].Offset;

	Header.nTextureFlags = Elements[EElement_TextureFlags].Number;
	Header.oTextureFlags = Elements[EElement_TextureFlags].Offset;

	Header.nBonesLookup = Elements[EElement_BonesLookup].Number;
	Header.oBonesLookup = Elements[EElement_BonesLookup].Offset;

	Header.nTexturesLookup = Elements[EElement_TexturesLookup].Number;
	Header.oTexturesLookup = Elements[EElement_TexturesLookup].Offset;

	Header.nTextureUnitsLookup = Elements[EElement_TextureUnitsLookup].Number;
	Header.oTextureUnitsLookup = Elements[EElement_TextureUnitsLookup].Offset;

	Header.nTransparenciesLookup = Elements[EElement_TransparenciesLookup].Number;
	Header.oTransparenciesLookup = Elements[EElement_TransparenciesLookup].Offset;

	Header.nTextureAnimationsLookup = Elements[EElement_TextureAnimationsLookup].Number;
	Header.oTextureAnimationsLookup = Elements[EElement_TextureAnimationsLookup].Offset;

	Header.nBoundingTriangles = Elements[EElement_BoundingTriangles].Number;
	Header.oBoundingTriangles = Elements[EElement_BoundingTriangles].Offset;

	Header.nBoundingVertices = Elements[EElement_BoundingVertices].Number;
	Header.oBoundingVertices = Elements[EElement_BoundingVertices].Offset;

	Header.nBoundingNormals = Elements[EElement_BoundingNormals].Number;
	Header.oBoundingNormals = Elements[EElement_BoundingNormals].Offset;

	Header.nAttachments = Elements[EElement_Attachments].Number;
	Header.oAttachments = Elements[EElement_Attachments].Offset;

	Header.nAttachmentsLookup = Elements[EElement_AttachmentsLookup].Number;
	Header.oAttachmentsLookup = Elements[EElement_AttachmentsLookup].Offset;

	Header.bEvents = Elements[EElement_Events].Number;
	Header.oEvents = Elements[EElement_Events].Offset;

	Header.nLights = Elements[EElement_Lights].Number;
	Header.oLights = Elements[EElement_Lights].Offset;

	Header.nCameras = Elements[EElement_Cameras].Number;
	Header.oCameras = Elements[EElement_Cameras].Offset;

	Header.nCamerasLookup = Elements[EElement_CamerasLookup].Number;
	Header.oCamersaLookup = Elements[EElement_CamerasLookup].Offset;

	Header.nRibbonEmitters = Elements[EElement_RibbonEmitters].Number;
	Header.oRibbonEmitters = Elements[EElement_RibbonEmitters].Offset;

	Header.nParticleEmitters = Elements[EElement_ParticleEmitters].Number;
	Header.oParticleEmitters = Elements[EElement_ParticleEmitters].Offset;

	Header.nUnknown1 = Elements[EElement_Unknown1].Number;
	Header.oUnknown1 = Elements[EElement_Unknown1].Offset;
}
