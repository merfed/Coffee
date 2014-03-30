/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// The core of MPQDraft is extremely simple and effective, which is what makes it so beautiful. In addition to opening files directly out of specific MPQs, Storm maintains a sorted list of all open MPQs which we'll refer to as the search list; each archive's position in the list is determined by a priority value passed to SFileOpenArchive. When one of the SFileOpenFile functions is called with NULL for the archive handle, this list is searched in order for the file. This makes it trivially easy for us to override content: we simply need to insert our patch MPQs at the head of the list, and Storm itself will handle everything else.
// There are, however, a few odds and ends that need to be dealt with. There are a bunch of file-loading functions that take an archive handle, meaning they don't need to use the search list; if desired (the Extended Redirect option), we need to hook those and force Storm to use the search list. Similarly, we need to hook functions that could be used to escape from our primitive sandbox. As there are a couple different versions of Storm with different function ordinals, we also need to make sure not to get confused. Finally, actually opening our patch MPQs is deferred until the first game MPQ is opened, for reasons explained in PatchOpenArchive.
// Other than those things mentioned, that just leaves plugin loading and operation for us to deal with.

#include <windows.h>
#include <QHookAPI.h>
#include "..\\Common.h"
#include "Patcher.h"

extern "C" BOOL WINAPI MPQDraftInitialize(IN MPQDRAFTPATCHERDATA *pData, IN DWORD nDataSize);
  BOOL LoadContext();
  BOOL IsPatchTarget();
  BOOL PatchFunctions();
    BOOL LocateStorm();
	 BOOL GetStormVersion();
    BOOL GetFunctionPointers();
    BOOL PatchModuleFunctions(HMODULE hModule);
  BOOL LoadPatchMPQs();
  BOOL LoadPlugins();

BOOL MPQDraftTerminate();
  BOOL UnloadPlugins();
  BOOL UnloadPatchMPQs();

void MPQDraftAbort();

// The MPQDraft plugin server which hosts any plugins loaded in this patch. See IMPQDraftServer for more info about plugins and the way they interact with the plugin server..
class CPluginServer : public IMPQDraftServer
{
private:
	BOOL m_bReady;	// Whether we've been initialized yet

	// The modules that have been loaded
	DWORD m_nModules;
	MPQDRAFTPLUGINMODULE *m_pModules;

public:
	CPluginServer();
	~CPluginServer();

	// Set the modules for this process
	BOOL SetModules(IN MPQDRAFTPLUGINMODULE *pModules, IN DWORD nModules);

	virtual BOOL WINAPI GetPluginModule(IN DWORD dwPluginID, IN DWORD dwModuleID, OUT LPSTR lpszFileName);
};

