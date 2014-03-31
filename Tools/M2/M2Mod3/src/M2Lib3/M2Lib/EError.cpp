#include "EError.h"

// todo: not all of these errors are actually used, spread them out better.
char* M2Lib::GetErrorText( EError Error )
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

	case EError_FailedToLoadSKIN_FileCorrupt:
		return "error: failed to load SKIN, file is corrupt.";

	case EError_FailedToSaveSKIN:
		return "error: faled to save SKIN, unspecified error.";

	case EError_FailedToSaveSKIN_NoFileSpecified:
		return "error: faled to save SKIN, no file was specified.";

	}

	return "error: unrecognized error";
}