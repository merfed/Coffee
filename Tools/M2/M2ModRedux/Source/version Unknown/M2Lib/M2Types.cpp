#include "M2Types.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

M2Lib::CTriangle::CTriangle()
{
	Index = 0;

	Vertices[0] = 0;
	Vertices[1] = 0;
	Vertices[2] = 0;
}

M2Lib::CTriangle::CTriangle( const CTriangle& Other )
{
	*this = Other;
}

M2Lib::CTriangle& M2Lib::CTriangle::operator = ( const CTriangle& Other )
{
	Index = Other.Index;

	Vertices[0] = Other.Vertices[0];
	Vertices[1] = Other.Vertices[1];
	Vertices[2] = Other.Vertices[2];

	return *this;
}

M2Lib::CVertex::CVertex()
{
	Position[0] = 0.0f;
	Position[1] = 0.0f;
	Position[2] = 0.0f;

	BoneWeights[0] = 0;
	BoneWeights[1] = 0;
	BoneWeights[2] = 0;
	BoneWeights[3] = 0;

	BoneIndices[0] = 0;
	BoneIndices[1] = 0;
	BoneIndices[2] = 0;
	BoneIndices[3] = 0;

	Normal[0] = 0.0f;
	Normal[1] = 0.0f;
	Normal[2] = 0.0f;

	Texture[0] = 0.0f;
	Texture[1] = 0.0f;

	Padding[0] = 0.0f;
	Padding[1] = 0.0f;
}

M2Lib::CVertex::CVertex( const CVertex& Other )
{
	*this = Other;
}

M2Lib::CVertex& M2Lib::CVertex::operator = ( const CVertex& Other )
{
	Position[0] = Other.Position[0];
	Position[1] = Other.Position[1];
	Position[2] = Other.Position[2];

	BoneWeights[0] = Other.BoneWeights[0];
	BoneWeights[1] = Other.BoneWeights[1];
	BoneWeights[2] = Other.BoneWeights[2];
	BoneWeights[3] = Other.BoneWeights[3];

	BoneIndices[0] = Other.BoneIndices[0];
	BoneIndices[1] = Other.BoneIndices[1];
	BoneIndices[2] = Other.BoneIndices[2];
	BoneIndices[3] = Other.BoneIndices[3];

	Normal[0] = Other.Normal[0];
	Normal[1] = Other.Normal[1];
	Normal[2] = Other.Normal[2];

	Texture[0] = Other.Texture[0];
	Texture[1] = Other.Texture[1];

	Padding[0] = Other.Padding[0];
	Padding[1] = Other.Padding[1];

	return *this;
}

// compares 2 vertices to see if they have the same position, bones, and texture coordinates. vertices between subsets that pass this test are most likely duplicates.
bool M2Lib::CVertex::CompareSimilar( CVertex& A, CVertex& B, bool CompareTextures, bool CompareBones, Float32 PositionalTolerance, Float32 AngularTolerance )
{
	// compare position
	PositionalTolerance = PositionalTolerance * PositionalTolerance;
	if ( PositionalTolerance > 0.0f )
	{
		Float32 Delta[3];
		Delta[0] = A.Position[0] - B.Position[0];
		Delta[1] = A.Position[1] - B.Position[1];
		Delta[2] = A.Position[2] - B.Position[2];
		Float32 Distance = ( ( Delta[0] * Delta[0] ) + ( Delta[1] * Delta[1] ) + ( Delta[2] * Delta[2] ) );
		if ( Distance > PositionalTolerance )
		{
			return false;
		}
	}
	else
	{
		if ( ( A.Position[0] != B.Position[0] ) || ( A.Position[1] != B.Position[1] ) || ( A.Position[2] != B.Position[2] ) )
		{
			return false;
		}
	}

	// compare texture coordinates
	if ( CompareTextures )
	{
		if ( ( A.Texture[0] != B.Texture[0] ) || ( A.Texture[1] != B.Texture[1] ) )
		{
			return false;
		}
	}

	// compare bones
	if ( CompareBones )
	{
		// order independent comparison
		bool SameBones[4]; SameBones[0] = false; SameBones[1] = false; SameBones[2] = false; SameBones[3] = false;
		for ( UInt32 i = 0; i < 4; i++ )
		{
			bool HasSameBone = false;
			for ( UInt32 j = 0; j < 4; j++ )
			{
				if ( A.BoneIndices[i] == B.BoneIndices[j] && SameBones[j] == false )
				{
					SameBones[j] = true;
					break;
				}
			}
		}
		if ( !( SameBones[0] && SameBones[1] && SameBones[2] && SameBones[3] ) )
		{
			return false;
		}
	}

	// compare normals
	// one way to visualize this is that NormalA defines a plane and NormalB defines a point
	// we want to determine what side of the PlaneA that the PointB lies on
	// it's just as simple as getting the dot product of the two vectors and checking the sign of the result
	// arc cosine the dot product of the vectors to get the angle between them
	if ( AngularTolerance > 0.0f )
	{
		Float32 Dot = ( A.Normal[0] * B.Normal[0] ) + ( A.Normal[1] * B.Normal[1] ) + ( A.Normal[2] * B.Normal[2] );
		if ( acos( Dot ) > AngularTolerance )	// units are radians
		{
			return false;
		}
	}
	else
	{
		if ( ( A.Normal[0] != B.Normal[0] ) || ( A.Normal[1] != B.Normal[1] ) || ( A.Normal[2] != B.Normal[2] ) )
		{
			return false;
		}
	}

	return true;
}

const char* M2Lib::GetErrorText( EError Error )
{
	switch ( Error )
	{
	case EError_OK:
		return "ok";

	case EError_FAIL:
		return "error: unspecified error.";

	case EError_PathTooLong:
		return "error: file path is too long.";

	case EError_FailedToLoadM2_NoFileSpecified:
		return "error: failed to load M2, no file was specified.";

	case EError_FailedToLoadM2_CouldNotOpenFile:
		return "error: failed to load M2, could not open file.";

	case EError_FailedToLoadM2_FileCorrupt:
		return "error: failed to load M2, file is corrupt.";

	case EError_FailedToLoadM2_VersionNotSupported:
		return "error: failed to load M2, format version is not supported.";

	case EError_FailedToExportM2I_NoFileSpecified:
		return "error: failed to export M2I, no file was specified.";

	case EError_FailedToExportM2I_CouldNotOpenFile:
		return "error: failed to export M2I, could not open file.";

	case EError_FailedToExportM2I_M2NotLoaded:
		return "error: failed to export M2I, no M2 was loaded.";

	case EError_FailedToImportM2I_NoFileSpecified:
		return "error: failed to import M2I, no file was specified.";

	case EError_FailedToImportM2I_CouldNotOpenFile:
		return "error: failed to import M2I, could not open file.";

	case EError_FailedToImportM2I_FileCorrupt:
		return "error: failed to import M2I, file is corrupt.";

	case EError_FailedToImportM2I_UnsupportedVersion:
		return "error: failed to import M2I, version number unsupported.";

	case EError_FailedToImportM2I_TooManyVertices:
		return "error: failed to import M2I, it has too many vertices (more than 65535).";

	case EError_FailedToImportM2I_SkinHasTooManyIndices:
		return "error: failed to import M2I, generated skin has too many indices (more than 65535).";

	case EError_FailedToSaveM2:
		return "error: failed to save M2, unspecified error.";

	case EError_FailedToSaveM2_NoFileSpecified:
		return "error: failed to save M2, no file was specified.";

	case EError_FailedToLoadSKIN_NoFileSpecified:
		return "error: failed to load SKIN, file not specified.";

	case EError_FailedToLoadSKIN_CouldNotOpenFile:
		return "error: failed to load SKIN, could not open file. make sure you put the M2 and skins in the same folder.";

	case EError_FailedToLoadSKIN_FileMissingOrCorrupt:
		return "error: failed to load SKIN, file is missing or corrupt.";

	case EError_FailedToSaveSKIN:
		return "error: faled to save SKIN, unspecified error.";

	case EError_FailedToSaveSKIN_NoFileSpecified:
		return "error: faled to save SKIN, no file was specified.";

	}

	return "error: unrecognized error";
}