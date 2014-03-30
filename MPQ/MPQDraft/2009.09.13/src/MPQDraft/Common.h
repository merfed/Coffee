/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// The Common.h contains portions of the MPQDraft system that are used by multiple components (the GUI, the DLL, and the SEMPQ stub). This is intended to be kept as small as possible, compartmentalizing as much as possible.
#if !defined(COMMON_H)
#define COMMON_H

#include <windows.h>
#include <MPQDraftPlugin.h>

// Structures and functions relating to the MPQDraft patching operation
// Component and module IDs for MPQDraft modules
#define MPQDRAFT_COMPONENT 0x2f0b5f48
#define MPQDRAFTDLL_MODULE 0xa0fcc4e7

// Patching flags
// Redirect file open attempts that explicitely specify an archive to open the file in
#define MPQD_EXTENDED_REDIR 0x10000
// Do not inject the MPQDraft system into child processes create by the patch target
#define MPQD_NO_SPAWNING 0x20000
#define MPQD_USE_D2_STORM 0x40000
#define MPQD_DETECT_STORM 0x80000

/* 
	* CreateAndPatchProcess *
	This is the function that interfaces the high level (the GUI or the SEMPQ) with the low level (the patcher system and patcher DLL). Specifically, this is the only portion of the patching kernel that is directly exposed to the GUI and SEMPQ; all the stuff below this is internal to the DLL. This function itself is in the DLL, and must be called through a pointer obtained with GetProcAddress.
	Creates the process according the specs provided in the NEWPROCESSINFO, then performs injection of the MPQDraft system, and returns output info to the caller in NEWPROCESSINFO. Any cleanup of the NEWPROCESSINFO (e.g. the handles in lpProcessInformation) must be done by the caller. If FALSE is returned, the patching was unsuccessful, and the process should be terminated.
*/
typedef BOOL (WINAPI *MPQDraftPatcherPtr)(
	// These first 10 parameters are nothing other than the parameters passed to CreateProcess, and used for just that.
	IN LPCSTR lpszApplicationName,
	IN OUT LPSTR lpszCommandLine,

	IN OPTIONAL LPSECURITY_ATTRIBUTES lpProcessAttributes,
	IN OPTIONAL LPSECURITY_ATTRIBUTES lpThreadAttributes,

	IN BOOL bInheritHandles,
	IN DWORD dwCreationFlags,
	IN OPTIONAL LPVOID lpszEnvironment,
	IN OPTIONAL LPCSTR lpszCurrentDirectory,

	IN LPSTARTUPINFO lpStartupInfo,
	// Patching flags, which indicate various options for the patching operation
	IN DWORD dwFlags,
	// The directory of the MPQDraft program or the SEMPQ
	IN LPCSTR lpszMPQDraftDir,
	// The path of the patch target. The DLL will be injected into all processes, but only this process will have the MPQs and plugins loaded.
	IN LPCSTR lpszTargetPath,
	// Number of times to execute the patch target before actually loading the MPQs and plugins (see PATCHTARGETEX)
	IN OPTIONAL DWORD nShuntCount,
	// The number of MPQs to load in the target process
	IN OPTIONAL DWORD nPatchMPQs,
	// The number of auxilliary modules for the patching operation. Any modules marked as executable will be loaded as plugins.
	IN OPTIONAL DWORD nAuxModules,
	// The list of MPQ paths, one for each MPQ to be loaded. Later entries have higher priority.
	IN OPTIONAL LPCSTR *lplpszMPQNames, 
	// The list of auxilliary modules. Plugins will be loaded in order.
	IN OPTIONAL const MPQDRAFTPLUGINMODULE *lpAuxModules
);

typedef BOOL (WINAPI *GetMPQDraftPluginPtr)(OUT IMPQDraftPlugin **lppMPQDraftPlugin);

// Functions and data related to SEMPQs
// SEMPQs are fairly complicated. They consist of, in order, the SEMPQ executable stub, and Embedded File System (EFS), and the MPQ itself. Apart from the STUBDATA resource, the SEMPQ portion is invariant. The EFS contains all the plugins and plugin support files, as well as the MPQDraft DLL itself. The MPQ is just the MPQ used when creating the SEMPQ; originally, Storm would not load MPQs which were not at the very end of the disk file, but that restriction has since been removed to support the strong digital signature in Warcraft III.
#define STUBDATASIZE 0x400
#define STUBDATA_KEY 0xD7DCA2D6 // STUBDATA in MPQ hash

// The description of the patch target which is stored in an SEMPQ's STUBDATA structure. Note that all pointers are relative offsets from the start of the PATCHTARGETEX when stored in the SEMPQ, and adjusted when loaded into memory. For the exact meaning of fields, see MPQStub.cpp.
struct PATCHTARGETEX
{
	BOOL bUseRegistry;	// If FALSE, use file path directly

	// Registry key and value name to locate the patch target's directory
	LPCSTR lpszRegistryKey;
	LPCSTR lpszRegistryValue;
	BOOL bValueIsFileName;	// If TRUE, registry value is treated as a full path, else a directory

	LPCSTR lpszTargetPath;	// Directory of patch target; used if bUseRegistry is FALSE
	// File that will be patached; ALWAYS used. If null, target is same as spawn file spec.
	LPCSTR lpszTargetFileName;

	// The filename to combine with the directory in the registry when launching the patch target (may not be the patch target itself). Used if bValueIsFileName is FALSE.
	LPCSTR lpszSpawnFileName;
	DWORD nShuntCount;

	LPCSTR lpszArguments;

	DWORD iIcon;	// Icon ID in internal arrays

	DWORD grfFlags;	// Flags sent to patcher
};

// This STUBDATA is a resource in the SEMPQ used to store info about the patching operation to be performed. This data is overwritten when creating the SEMPQ. Perhaps at some point this should be changed to be a data file embedded in the SEMPQ's EFS, as that would be easier than overwriting the resource. There was a reason for originally using a resource, but it's become obsolete (rather, it was an idea for a feature in the future, which was never actually implemented and ultimately abandoned).
#include <pshpack1.h>
struct STUBDATA
{
	DWORD dwDummy;	// Don't ask what this is; it's a long story

	DWORD cbSize;	// sizeof(STUBDATA)

	// The name of the SEMPQ. Used in error messages and things.
	char szCustomName[32];

	PATCHTARGETEX patchTarget;

	// The actual strings used in PATCHTARGETEX will reside here
};
#include <poppack.h>

// Misc
// Determines whether a game is installed by checking for its registry entries
BOOL LocateGame(IN LPCSTR lpszGameKey, OUT LPCSTR lpszGameValue);
// Locates an installed game component file path through the game's registry entry
BOOL LocateComponent(IN LPCSTR lpszGameKey, IN LPCSTR lpszGameValue, IN LPCSTR lpszFileName, OUT LPSTR lpszFilePath);

#endif