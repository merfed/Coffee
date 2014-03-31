#pragma once

namespace M2Lib
{


enum EError
{
	EError_OK,
	EError_FAIL,

	EError_PathTooLong,

	EError_FailedToLoadM2_NoFileSpecified,
	EError_FailedToLoadM2_CouldNotOpenFile,
	EError_FailedToLoadM2_FileCorrupt,
	EError_FailedToLoadM2_VersionNotSupported,

	EError_FailedToExportM2I_NoFileSpecified,
	EError_FailedToExportM2I_CouldNotOpenFile,
	EError_FailedToExportM2I_M2NotLoaded,

	EError_FailedToImportM2I_NoFileSpecified,
	EError_FailedToImportM2I_CouldNotOpenFile,
	EError_FailedToImportM2I_FileCorrupt,
	EError_FailedToImportM2I_UnsupportedVersion,
	EError_FailedToImportM2I_TooManyVertices,
	EError_FailedToImportM2I_SkinHasTooManyIndices,

	EError_FailedToSaveM2,
	EError_FailedToSaveM2_NoFileSpecified,

	EError_FailedToLoadSKIN_NoFileSpecified,
	EError_FailedToLoadSKIN_CouldNotOpenFile,
	EError_FailedToLoadSKIN_FileCorrupt,

	EError_FailedToSaveSKIN,
	EError_FailedToSaveSKIN_NoFileSpecified,
};


char* GetErrorText( EError Error );


}
