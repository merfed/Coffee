/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// MPQDraft.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MPQDraft.h"
#include "resource.h"
#include "MainMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The lists of components for the supported programs
const PROGRAMFILEENTRY DiabloFiles[] =
{
	"Diablo", "Diablo.exe", "Diablo.exe", 1, IDI_DIABLO, TRUE, 0,
	NULL
};

const PROGRAMFILEENTRY DiabloHellfireFiles[] =
{
	"Diablo: Hellfire", "Hellfire.exe", "Hellfire.exe", 1, IDI_HELLFIRE, TRUE, 0,
	NULL
};

const PROGRAMFILEENTRY Diablo2Files[] =
{
	"Diablo II", "Diablo II.exe", "Game.exe", 0, IDI_DIABLO2, FALSE, 0,
	NULL
};

const PROGRAMFILEENTRY StarcraftFiles[] =
{
	"Starcraft", "Starcraft.exe", "Starcraft.exe", 0, IDI_STARCRAFT, TRUE, 0,
	"Campaign Editor", "StarEdit.exe", "StarEdit.exe", 0, IDI_STAREDIT, TRUE, 0,
	NULL
};

const PROGRAMFILEENTRY Warcraft2Files[] =
{
	"Warcraft II BNE", "Warcraft II BNE.exe", "Warcraft II BNE.exe", 0, IDI_WAR2BNE, TRUE, 0,
	//"Map Editor", "Warcraft II Map Editor.exe", "", IDI_WAR2EDIT, FALSE, 0
	NULL
};

const PROGRAMFILEENTRY Warcraft3Files[] =
{
	"Warcraft III", "Warcraft III.exe", "War3.exe", 0, IDI_WARCRAFT3, TRUE, 0,
	"The Frozen Throne", "Frozen Throne.exe", "War3.exe", 0, IDI_WAR3X, TRUE, 0,
	"World Editor", "World Editor.exe", "WorldEdit.exe", 0, IDI_WAR3EDIT, TRUE, 0,
	NULL
};

const PROGRAMFILEENTRY LordsOfMagicFiles[] =
{
	"Lords of Magic SE", "LOMSE.exe", "LOMSE.exe", 1, IDI_LOMSE, TRUE, 0,
	NULL
};

// The list of supported programs themselves
const PROGRAMENTRY SupportApps[] =
{
	"Diablo", "SOFTWARE\\Blizzard Entertainment\\Archives", "DiabloInstall", IDI_DIABLO, DiabloFiles,
	"Diablo: Hellfire", "SOFTWARE\\Sierra OnLine\\Setup\\HELLFIRE", "Directory", IDI_HELLFIRE, DiabloHellfireFiles,
	"Diablo II", "SOFTWARE\\Blizzard Entertainment\\Diablo II", "InstallPath", IDI_DIABLO2, Diablo2Files,
	"Starcraft", "SOFTWARE\\Blizzard Entertainment\\Starcraft", "InstallPath", IDI_STARCRAFT, StarcraftFiles,
	"Warcraft II BNE", "SOFTWARE\\Blizzard Entertainment\\Warcraft II BNE", "InstallPath", IDI_WAR2BNE, Warcraft2Files,
	"Warcraft III", "SOFTWARE\\Blizzard Entertainment\\Warcraft III", "InstallPath", IDI_WAR3HEADER, Warcraft3Files,
	//"Warcraft III Exp", "SOFTWARE\\Blizzard Entertainment\\Warcraft III", "InstallPathX", IDI_WAR3X, NULL,
	"Lords of Magic SE", "SOFTWARE\\Sierra OnLine\\Setup\\LOMSE", "Directory", IDI_LOMSE, LordsOfMagicFiles,
	NULL
};

void SanitizeFilePath(LPCSTR lpszFilePath, CString &strSanitizedFilePath)
{
	ASSERT(lpszFilePath);

	// As we are going to use the filename as a hash key for things, we have 
	// to worry about variations of the same file path. Thus here we do a 
	// minimal amount of sanitization on the file name by converting it to 
	// lower-case. If this isn't enough, it can be changed to something more 
	// elaborate later.
	strSanitizedFilePath = lpszFilePath;
	strSanitizedFilePath.MakeLower();
}

/////////////////////////////////////////////////////////////////////////////
// CMPQDraft

const DWORD CMPQDraft::OtherIconsToLoad[] =
{
	IDI_MPQ,
	IDI_PLUGIN,
	IDR_MAINFRAME,
	0
};

BEGIN_MESSAGE_MAP(CMPQDraft, CWinApp)
	//{{AFX_MSG_MAP(CMPQDraft)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPQDraft construction

CMPQDraft::CMPQDraft() : CWinApp("MPQDraft for Windows")
{
	// Place all significant initialization in InitInstance
}

CMPQDraft::~CMPQDraft()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMPQDraft object

CMPQDraft theApp;

/////////////////////////////////////////////////////////////////////////////
// CMPQDraft initialization

BOOL CMPQDraft::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	QResourceInitialize();

	// Set the registry key MFC will use for saving/loading settings
	CString strRegKey;
	strRegKey.LoadString(IDS_REGKEY);
	SetRegistryKey(strRegKey);

	// Get the startup path
	char szStartupPath[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, szStartupPath);
	m_strStartupPath = szStartupPath;

	// Load the image lists
	if (!CreateImageLists())
	{
		CString strWindowsError;
		strWindowsError.LoadString(IDS_WINDOWSERROR);

		MessageBox(NULL, strWindowsError, AfxGetAppName(), MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Run the main menu, which is the entire program
	CMainMenu dlg;

	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


int CMPQDraft::ExitInstance() 
{
	// Clean up QResource and any files we may have extracted
	DeleteTemporaryFiles();
	QResourceDestroy();

	return CWinApp::ExitInstance();
}

CString *CMPQDraft::GetPatcherDLLPath()
{
	if (m_strPatcherPath.GetLength())
		return &m_strPatcherPath;	// Already loaded

	// Extract it
	BOOL bSuccess = ExtractTempResource(NULL, MAKEINTRESOURCE(IDR_PATCHERDLL),
		"DLL", m_strPatcherPath.GetBuffer(MAX_PATH + 1));
	m_strPatcherPath.ReleaseBuffer();

	if (bSuccess)
		return &m_strPatcherPath;
	else
	{
		m_strPatcherPath.Empty();

		return NULL;
	}
}

BOOL CMPQDraft::AddImageListIcon(DWORD iIcon)
{
	// See if we already have it in our image lists
	int iImageListEntry;
	if (m_mapIconIds.Lookup(iIcon, iImageListEntry))
		return TRUE;	// No problem

	// It's not in the list. Load the icon and add it to the image lists.
	HICON hIcon = LoadIcon(iIcon);
	if (!hIcon)
		return FALSE;

	int iLargeIcon = m_imglLargeIcons.Add(hIcon);
	if (iLargeIcon == -1 || iLargeIcon != m_imglSmallIcons.Add(hIcon))
		return FALSE;

	m_mapIconIds.SetAt(iIcon, iLargeIcon);
	return TRUE;
}

BOOL CMPQDraft::CreateImageLists()
{
	// Create the two images lists: large icon and small icon, both using 256 
	// colors with transparency masks
	if (!m_imglLargeIcons.Create(32, 32, ILC_COLOR8 | ILC_MASK, 0, 8) ||
		!m_imglSmallIcons.Create(16, 16, ILC_COLOR8 | ILC_MASK, 0, 8))
		return FALSE;

	m_imglLargeIcons.SetBkColor(CLR_NONE);
	m_imglSmallIcons.SetBkColor(CLR_NONE);

	// Scan through the list of supported programs and files, looking for 
	// needed icons
	for (const PROGRAMENTRY *lpProgram = SupportApps; 
		lpProgram->szProgramName != NULL; lpProgram++)
	{
		if (!AddImageListIcon(lpProgram->iIcon))
			return FALSE;

		ASSERT(lpProgram->files);

		for (const PROGRAMFILEENTRY *lpFile = lpProgram->files; 
			lpFile->szComponentName != NULL; lpFile++)
		{
			if (!AddImageListIcon(lpFile->iIcon))
				return FALSE;
		}
	}

	// Load any icons that aren't in the supported programs and files lists
	for (DWORD iIcon = 0; OtherIconsToLoad[iIcon] != 0; iIcon++)
	{
		if (!AddImageListIcon(OtherIconsToLoad[iIcon]))
			return FALSE;
	}

	return TRUE;
}
