/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#include "stdafx.h"
#include "MPQDraftDLL.h"

// Trivial constructor and destructor
CPluginServer::CPluginServer()
{
	m_bReady = FALSE;

	m_nModules = 0;
	m_pModules = NULL;
}

CPluginServer::~CPluginServer()
{
}

BOOL CPluginServer::SetModules(IN MPQDRAFTPLUGINMODULE *pModules, IN DWORD nModules)
{
	QDebugWriteEntry("CPluginServer(0x%X)::SetModules(0x%X, %d)", this, pModules, nModules);

	assert(pModules);
	assert(nModules);
	assert(!m_bReady);

	m_nModules = nModules;
	m_pModules = pModules;

	// For debugging, list all modules present
	for (DWORD iCurModule = 0; iCurModule < m_nModules; iCurModule++)
		QDebugWriteEntry("CPluginServer(0x%X)::SetModules : Module slot %d has plugin ID 0x%X, module ID 0x%X, filename \"%s\"", 
			this, iCurModule, m_pModules[iCurModule].dwComponentID, m_pModules[iCurModule].dwModuleID, m_pModules[iCurModule].szModuleFileName);

	m_bReady = TRUE;

	return TRUE;
}

// Assumes that the buffer pointed to by lpszFileName is large enough to hold the file name
BOOL CPluginServer::GetPluginModule(IN DWORD dwPluginID, IN DWORD dwModuleID, OUT LPSTR lpszFileName)
{
	QDebugWriteEntry("CPluginServer(0x%X)::GetPluginModule(0x%X, 0x%X, 0x%X)", this, dwPluginID, dwModuleID, lpszFileName);

	if (!lpszFileName)
		return FALSE;

	if (!m_bReady)
		return FALSE;

	// Linear search. This could easily be replaced with a hash table, but I didn't really envision the table of modules being so large that linear search would be significant.
	for (DWORD iCurModule = 0; iCurModule < m_nModules; iCurModule++)
	{
		if ((m_pModules[iCurModule].dwComponentID == dwPluginID) &&
			(m_pModules[iCurModule].dwModuleID == dwModuleID))
		{
			QDebugWriteEntry("CPluginServer(0x%X)::GetPluginModule : Module found at slot %d, \"%s\"", this, iCurModule, m_pModules[iCurModule].szModuleFileName);

			strcpy(lpszFileName, m_pModules[iCurModule].szModuleFileName);

			return TRUE;
		}
	}

	QDebugWriteEntry("CPluginServer(0x%X)::GetPluginModule : Module not found", this);

	return FALSE;
}