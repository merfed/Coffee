/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#include "stdafx.h"
#include "MPQDraft.h"
#include "resource.h"
#include "CommonPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchTargetPage property page

IMPLEMENT_DYNCREATE(CPatchTargetPage, CPropertyPage)

// Different caption for SEMPQ and patch wizards
CPatchTargetPage::CPatchTargetPage(BOOL bSEMPQWizard, UINT nTitleStringId,
	UINT nInfoStringId) 
	: CPropertyPage(CPatchTargetPage::IDD, nTitleStringId), 
	m_nInfoStringId(nInfoStringId)
{
	//{{AFX_DATA_INIT(CPatchTargetPage)
	//}}AFX_DATA_INIT

	m_bSEMPQWizard = bSEMPQWizard;

	m_bWarnOnRedir = TRUE;

	m_bUpdatingList = FALSE;

	m_nSelProgram = -1;
	m_nSelFile = -1;
}

CPatchTargetPage::~CPatchTargetPage()
{
}

void CPatchTargetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchTargetPage)
	DDX_Control(pDX, IDC_INFORMATION, m_ctlInformation);
	DDX_Control(pDX, IDC_PROGRAMS, m_ctlPrograms);
	DDX_Control(pDX, IDC_COMPONENTS, m_ctlComponents);
	DDX_Control(pDX, IDC_FILENAME, m_ctlProgramPath);
	DDX_Control(pDX, IDC_PARAMETERS, m_ctlParameters);
	DDX_Control(pDX, IDC_REDIROPENFILEEX, m_ctlExtendedRedir);
	//}}AFX_DATA_MAP
}

BOOL CPatchTargetPage::OnKillActive() 
{
	if (!CPropertyPage::OnKillActive())
		return FALSE;

	// Verify that there is a program selected and that it exists, and 
	// display an error otherwise.
	if (m_nSelProgram != -1 && m_nSelFile != -1)
		return TRUE;	// We've got one selected

	CString strProgramPath, strMessage;
	GetProgramPath(strProgramPath);

	if (m_bSEMPQWizard)
	{
		if (strProgramPath.GetLength())
		{
			// We've got SOMETHING selected. We can work with this, but using 
			// a nonstandard target carries some risks. Warn the user about 
			// it, here and see what they want to do.
			strMessage.LoadString(IDS_ODDSEMPQTARGET);
			return (MessageBox(strMessage, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES);
		}
	}
	else if (PathFileExists(strProgramPath))
		return TRUE;	// Got something valid selected

	// Don't have anything valid selected. Error time.
	strMessage.LoadString(
		m_bSEMPQWizard ? IDS_NOSEMPQTARGET : IDS_NOPATCHTARGET);

	GetParent()->MessageBox(strMessage, NULL, MB_OK | MB_ICONEXCLAMATION);

	return FALSE;
}

void CPatchTargetPage::LoadSettings()
{
	// Load the key strings from resources
	CString strRegHeader, strRegSelApp, strRegSelFile, strRegPath, 
		strRegUsePath, strRegRedirOpenFileEx, strRegParams;
	strRegHeader.LoadString(m_bSEMPQWizard ? IDS_SEMPQKEY : IDS_PATCHKEY);
	strRegSelApp.LoadString(IDS_SELAPPKEY);
	strRegSelFile.LoadString(IDS_SELFILEKEY);
	strRegPath.LoadString(IDS_APPPATHKEY);
	strRegUsePath.LoadString(IDS_USEPATHKEY);
	strRegRedirOpenFileEx.LoadString(IDS_REDIROPENFILEEXKEY);
	strRegParams.LoadString(IDS_PARAMETERSKEY);

	// Load the registry values
	BOOL bUseAppPath = theApp.GetProfileInt(strRegHeader, strRegUsePath, FALSE),
		bExtendRedir = theApp.GetProfileInt(strRegHeader, strRegRedirOpenFileEx, FALSE);
	CString strSelApp = theApp.GetProfileString(strRegHeader, strRegSelApp),
		strSelFile = theApp.GetProfileString(strRegHeader, strRegSelFile),
		strProgPath = theApp.GetProfileString(strRegHeader, strRegPath),
		strParams = theApp.GetProfileString(strRegHeader, strRegParams);

	// Now process them
	SelectProgram(-1);
	SelectFile(-1);

	m_ctlExtendedRedir.SetCheck(bExtendRedir);
	m_ctlParameters.SetWindowText(strParams);

	// Are we using a manually-specified path?
	if (bUseAppPath)
	{
		// Manually-specified - easy
		if (strProgPath.GetLength())
		{
			// If this is the patch page, we need to check that the patch target exists. However, the SEMPQ wizard allows you to select anything you want, even if it doesn't exist. Allow for that here.
			if (m_bSEMPQWizard || PathFileExists(strProgPath))
				m_ctlProgramPath.SetWindowText(strProgPath);
		}

		return;
	}

	// Using the program and component lists
	LVFINDINFO lvfi;
	CPoint pnt;
	int nProgIndex, nFileIndex;

	// Search for the program by name and select it
	lvfi.flags = LVFI_STRING;
	lvfi.psz = strSelApp;

	nProgIndex = m_ctlPrograms.FindItem(&lvfi);
	m_ctlPrograms.SetItemState(nProgIndex, LVIS_SELECTED, LVIS_SELECTED);
	SelectProgram(nProgIndex);

	m_ctlPrograms.SetFocus();

	if (nProgIndex == -1)
		return;

	// Found the program. Now find the file and select it.
	lvfi.psz = strSelFile;
	nFileIndex = m_ctlComponents.FindItem(&lvfi);

	if (nFileIndex == -1)
		return;

	m_ctlComponents.SetItemState(nFileIndex, LVIS_SELECTED, LVIS_SELECTED);
	SelectFile(nFileIndex);

	m_ctlComponents.SetFocus();
}

BOOL CPatchTargetPage::SaveSettings()
{
	// Load the key strings from resources
	CString strRegHeader, strRegSelApp, strRegSelFile, strRegPath, 
		strRegUsePath, strRegRedirOpenFileEx, strRegParams;
	strRegHeader.LoadString(m_bSEMPQWizard ? IDS_SEMPQKEY : IDS_PATCHKEY);
	strRegSelApp.LoadString(IDS_SELAPPKEY);
	strRegSelFile.LoadString(IDS_SELFILEKEY);
	strRegPath.LoadString(IDS_APPPATHKEY);
	strRegUsePath.LoadString(IDS_USEPATHKEY);
	strRegRedirOpenFileEx.LoadString(IDS_REDIROPENFILEEXKEY);
	strRegParams.LoadString(IDS_PARAMETERSKEY);

	BOOL bUseAppPath = FALSE, 
		bExtendRedir;
	CString strSelApp, strSelFile, strProgPath, strParams;

	// Are we using program/component, or manually-specified path?
	if ((m_nSelProgram != -1) && (m_nSelFile != -1))
	{
		// program/component
		strSelApp = m_ctlPrograms.GetItemText(m_nSelProgram, 0);
		strSelFile = m_ctlComponents.GetItemText(m_nSelFile, 0);

		bUseAppPath = FALSE;
	}
	else if (m_ctlProgramPath.GetWindowTextLength())
	{
		// Path
		m_ctlProgramPath.GetWindowText(strProgPath);

		bUseAppPath = TRUE;
	}
	bExtendRedir = m_ctlExtendedRedir.GetCheck();
	m_ctlParameters.GetWindowText(strParams);

	// Finally, write out the values
	theApp.WriteProfileString(strRegHeader, strRegSelApp, strSelApp);
	theApp.WriteProfileString(strRegHeader, strRegSelFile, strSelFile),
	theApp.WriteProfileString(strRegHeader, strRegPath, strProgPath);
	theApp.WriteProfileInt(strRegHeader, strRegUsePath, bUseAppPath);

	theApp.WriteProfileInt(strRegHeader, strRegRedirOpenFileEx, bExtendRedir);
	theApp.WriteProfileString(strRegHeader, strRegParams, strParams);

	return TRUE;
}

void CPatchTargetPage::FindSupportedApps()
{
	const BOOL bListAllApps = m_bSEMPQWizard;

	// Delete all items from the list and enable filters to prevent unwanted
	// notifications.
	m_bUpdatingList = TRUE;
	m_ctlPrograms.SetRedraw(FALSE);
	m_ctlPrograms.DeleteAllItems();

	// Set up the LVITEM for insertion
	LVITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = 10000;
	lvi.iSubItem = 0;

	// List entries
	for (const PROGRAMENTRY *lpProgram = SupportApps; 
		lpProgram->szProgramName != NULL; lpProgram++)
	{
		// If we're in patch mode, only list those that are found on the HD
		if (!bListAllApps && 
			!LocateGame(lpProgram->szRegistryKey, lpProgram->szRegistryValue))
			continue;

		lvi.pszText = (LPSTR)lpProgram->szProgramName;
		lvi.iImage = theApp.GetIconIndex(lpProgram->iIcon);
		lvi.lParam = (LPARAM)lpProgram;

		m_ctlPrograms.InsertItem(&lvi);
	}

	// Enable notifications again
	m_ctlPrograms.SetRedraw(TRUE);
	m_bUpdatingList = FALSE;
	m_ctlPrograms.Invalidate();
}

void CPatchTargetPage::FindSupportedFiles(const PROGRAMENTRY *lpSelProgram)
{
	// Almost identical to FindSupportedApps
	m_ctlComponents.DeleteAllItems();
	if (!lpSelProgram)
		return;	// Can't be any components to list

	const BOOL bListAllFiles = m_bSEMPQWizard;

	m_bUpdatingList = TRUE;
	m_ctlComponents.SetRedraw(FALSE);

	LVITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = 10000;
	lvi.iSubItem = 0;

	for (const PROGRAMFILEENTRY *lpFile = lpSelProgram->files; lpFile->szComponentName != NULL; lpFile++)
	{
		if (!bListAllFiles)
		{
			if (!LocateComponent(lpSelProgram->szRegistryKey, 
				lpSelProgram->szRegistryValue, lpFile->szFileName, NULL))
				continue;
			// If there is also a patch target filename, verify that
			else if (lpFile->szTargetFileName
				&& !LocateComponent(lpSelProgram->szRegistryKey, 
				lpSelProgram->szRegistryValue, lpFile->szTargetFileName, NULL))
				continue;
		}

		lvi.pszText = (LPSTR)lpFile->szComponentName;
		lvi.iImage = theApp.GetIconIndex(lpFile->iIcon);
		lvi.lParam = (LPARAM)lpFile;

		m_ctlComponents.InsertItem(&lvi);
	}

	m_ctlComponents.SetRedraw(TRUE);
	m_bUpdatingList = FALSE;
	m_ctlComponents.Invalidate();
}

BEGIN_MESSAGE_MAP(CPatchTargetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPatchTargetPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROGRAMS, OnItemChangedPrograms)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COMPONENTS, OnItemChangedComponents)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_REDIROPENFILEEX, OnRedirOpenFileEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchTargetPage message handlers

BOOL CPatchTargetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Set up the image lists. Both list views are set to shared image lists
	m_ctlPrograms.SetImageList(&theApp.GetLargeIcons(), LVSIL_NORMAL);
	m_ctlComponents.SetImageList(&theApp.GetLargeIcons(), LVSIL_NORMAL);

	// Set the proper informational text for the wizard we're in
	CString strInfo;
	if (m_nInfoStringId)
		strInfo.LoadString(m_nInfoStringId);
	m_ctlInformation.SetWindowText(strInfo);

	// Fill the programs list and load settings from the registry
	FindSupportedApps();

	LoadSettings();

	// FALSE to indicate that we've set focus to a control - either the
	// programs list view or the components list view - in LoadSettings.
	return FALSE;
}


void CPatchTargetPage::OnItemChangedPrograms(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Filter out calls from when we're populating the list or for things
	// other than state changes.
	if (m_bUpdatingList || !(pNMListView->uChanged & LVIF_STATE))
		return;

	// This function will be called once for each item that changes. If there
	// was no previous selection or there is no new selection, it will be once;
	// otherwise, it will be called twice, for selection and deselection.

	if (pNMListView->uNewState & LVIS_SELECTED)
		// This is the new selection
		SelectProgram(pNMListView->iItem);
	// This is the previous selection. There may or may not be another
	// notification coming (to tell us what the new selection is). There
	// may also have already been a new selection. If there has already 
	// been a new selection, there's no need to do anything here.
	else if (m_nSelProgram == pNMListView->iItem)
		SelectProgram(-1);
}

void CPatchTargetPage::OnItemChangedComponents(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	// This is almost identical to OnItemChangedPrograms
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_bUpdatingList || !(pNMListView->uChanged & LVIF_STATE))
		return;

	if (pNMListView->uNewState & LVIS_SELECTED)
		SelectFile(pNMListView->iItem);
	else if (m_nSelFile == pNMListView->iItem)
		SelectFile(-1);
}

void CPatchTargetPage::OnRedirOpenFileEx() 
{
	if (m_nSelFile == -1 || !m_bWarnOnRedir)
		return; // No need to warn the user

	// Need to warn the user about the potential for problems changing this
	CString strMessage;
	strMessage.LoadString(IDS_REDIRWARNING);

	// Ignore or fail?
	if (MessageBox(strMessage, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
		m_bWarnOnRedir = FALSE;
	else
		m_ctlExtendedRedir.SetCheck(m_ctlExtendedRedir.GetCheck() ? FALSE : TRUE);
}

void CPatchTargetPage::SelectProgram(int nSelProgram)
{
	m_nSelProgram = nSelProgram;
	m_nSelFile = -1;

	FindSupportedFiles(GetProgram(nSelProgram));
	SelectFile(-1);
}

void CPatchTargetPage::SelectFile(int nSelIndex)
{
	const BOOL bAllowCustomPaths = m_bSEMPQWizard;
	m_nSelFile = nSelIndex;

	// Is anything selected?
	if (nSelIndex != -1)
	{
		// Yes. Select it. If it exists on the computer, show its path.
		ASSERT(nSelIndex >= 0 && nSelIndex < m_ctlComponents.GetItemCount());

		CString strProgramPath;
		VERIFY(GetProgramPath(strProgramPath) || bAllowCustomPaths);

		m_ctlProgramPath.SetWindowText(strProgramPath);
		m_ctlProgramPath.EnableWindow(FALSE);

		m_ctlExtendedRedir.SetCheck(GetFile(nSelIndex)->bExtendRedir);
		m_ctlExtendedRedir.EnableWindow(TRUE);
		m_bWarnOnRedir = TRUE;
	}
	else
	{
		// No. Deselect. Either enable or disable the path box and
		// extended redirect box depending on
		// whether we're in the SEMPQ or patch wizard - the SEMPQ wizard 
		// lets you do stuff with no selection, the patch wizard does not.
		m_ctlProgramPath.SetWindowText("");
		m_ctlProgramPath.EnableWindow(bAllowCustomPaths);

		m_ctlExtendedRedir.SetCheck(FALSE);
		m_ctlExtendedRedir.EnableWindow(bAllowCustomPaths);
	}
}

void CPatchTargetPage::OnBrowse() 
{
	CString strFilter, strTitle;
	strFilter.LoadString(IDS_OPENEXESETTINGS);
	strTitle.LoadString(
		m_bSEMPQWizard ? IDS_OPENSEMPQTARGET : IDS_OPENPATCHTARGET);

	// Set up the open dialog and show it
	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | 
		OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);
	dlg.m_ofn.lpstrTitle = strTitle;

	if (dlg.DoModal() != IDOK)
		return;	// The user cancelled

	// If we had a previous selection, unselect it.
	if (m_nSelFile != -1)
	{
		m_ctlComponents.SetItemState(m_nSelFile, FALSE, LVIS_SELECTED);
		SelectFile(-1);
	}

	// Set the new selection
	m_ctlProgramPath.SetWindowText(dlg.GetPathName());
	m_ctlExtendedRedir.EnableWindow(TRUE);
}

const PROGRAMENTRY *CPatchTargetPage::GetProgram(int iProgram)
{
	if (iProgram == -1)
		return NULL;

	ASSERT(iProgram >= 0 && iProgram < m_ctlPrograms.GetItemCount());

	return (const PROGRAMENTRY *)m_ctlPrograms.GetItemData(iProgram);
}

const PROGRAMFILEENTRY *CPatchTargetPage::GetFile(int iFile)
{
	if (iFile == -1)
		return NULL;

	ASSERT(iFile >= 0 && iFile < m_ctlComponents.GetItemCount());

	return (const PROGRAMFILEENTRY *)m_ctlComponents.GetItemData(iFile);
}

BOOL CPatchTargetPage::GetProgramPath(CString &strPath)
{
	const BOOL bAllowCustomPaths = m_bSEMPQWizard;

	strPath.Empty();

	// Do we have a selected component in the list view?
	if (m_nSelFile != -1)
	{
		// Yes. Get its path, if possible
		const PROGRAMENTRY *lpProgram = GetProgram(m_nSelProgram);
		const PROGRAMFILEENTRY *lpFile = GetFile(m_nSelFile);
		char szFilePath[MAX_PATH + 1] = "";

		VERIFY(LocateComponent(lpProgram->szRegistryKey,
			lpProgram->szRegistryValue, lpFile->szFileName, szFilePath)
			|| bAllowCustomPaths);

		strPath = szFilePath;
	}
	else
	{
		// It's a custom path. See if it looks valid, then return it
		if (!m_ctlProgramPath.GetWindowTextLength() && !bAllowCustomPaths)
			return FALSE;	// Not valid

		m_ctlProgramPath.GetWindowText(strPath);
	}

	return TRUE;
}

BOOL CPatchTargetPage::GetPatchTargetPath(CString &strPath)
{
	const BOOL bAllowCustomPaths = m_bSEMPQWizard;

	strPath.Empty();

	// Practically identical to GetProgramPath
	if (m_nSelFile != -1)
	{
		const PROGRAMENTRY *lpProgram = GetProgram(m_nSelProgram);
		const PROGRAMFILEENTRY *lpFile = GetFile(m_nSelFile);
		char szFilePath[MAX_PATH + 1] = "";

		// Is the patch target the same as the program path to spawn?
		if (!strlen(lpFile->szTargetFileName))
			return TRUE;	// Yes

		// Get the patch target filename this time
		VERIFY(LocateComponent(lpProgram->szRegistryKey,
			lpProgram->szRegistryValue, lpFile->szTargetFileName, szFilePath)
			|| bAllowCustomPaths);

		strPath = szFilePath;
	}
	else
	{
		if (!m_ctlProgramPath.GetWindowTextLength() && !bAllowCustomPaths)
			return FALSE;

		m_ctlProgramPath.GetWindowText(strPath);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPluginPage property page

IMPLEMENT_DYNCREATE(CPluginPage, CPropertyPage)

CPluginPage::CPluginPage(BOOL bSEMPQWizard, UINT nTitleStringId)
	: CPropertyPage(CPluginPage::IDD, nTitleStringId)
{
	//{{AFX_DATA_INIT(CPluginPage)
	//}}AFX_DATA_INIT

	m_bSEMPQWizard = bSEMPQWizard;

	m_bUpdatingList = FALSE;

	// Because neither LoadSettings or FindPlugins require the dialog to be
	// created, we can call them on construct.
	LoadSettings();

	FindPlugins();
}

CPluginPage::~CPluginPage()
{
	// Have to clean up the plugin list because it won't automatically destruct
	UnloadPlugins();
}

void CPluginPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginPage)
	DDX_Control(pDX, IDC_PLUGINS, m_ctlPlugins);
	DDX_Control(pDX, IDC_CONFIGURE, m_ctlConfigure);
	DDX_Control(pDX, IDC_BROWSE, m_ctlBrowse);
	//}}AFX_DATA_MAP
}

BOOL CPluginPage::OnKillActive() 
{
	if (!CPropertyPage::OnKillActive())
		return FALSE;

	// Verify that there aren't too many plugins and plugin modules selected
	DWORD nSelectedPlugins, nPluginModules;
	if (!GetNumSelectedPluginsAndModules(nSelectedPlugins, nPluginModules)
		|| nSelectedPlugins > MAX_MPQDRAFT_PLUGINS
		|| nPluginModules > MAX_AUXILIARY_MODULES)
	{
		// Failure
		CString strMessage;
		strMessage.Format(IDS_TOOMANYPLUGINS, MAX_MPQDRAFT_PLUGINS);

		GetParent()->MessageBox(strMessage, NULL, MB_OK | MB_ICONEXCLAMATION);

		return FALSE;
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CPluginPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPluginPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLUGINS, OnItemChangedPlugins)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginPage message handlers

BOOL CPluginPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString strTitle;
	strTitle.Format(IDS_PLUGINLISTHEADER, m_strPluginDir);

	// Set up the list view how we want it
	m_ctlPlugins.SetImageList(&theApp.GetSmallIcons(), LVSIL_SMALL);
	m_ctlPlugins.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_ctlPlugins.InsertColumn(0, strTitle);
	m_ctlPlugins.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	// Since we won't be getting ListPlugins called by the child page in 
	// SEMPQ wizard mode, do that automatically, here.
	if (m_bSEMPQWizard)
		ListPlugins();

	// No focus set, so return TRUE
	return TRUE;
}

BOOL CPluginPage::SaveSettings()
{
	CString strRegHeader, strRegPluginDir;
	strRegHeader.LoadString(m_bSEMPQWizard ? IDS_SEMPQKEY : IDS_PATCHKEY);
	strRegPluginDir.LoadString(IDS_PLUGINDIRKEY);

	theApp.WriteProfileString(strRegHeader, strRegPluginDir, m_strPluginDir);

	const DWORD nMaxPluginsToSave = 50;
	DWORD nNumPlugins = m_lstPlugins.GetCount(), iCurPlugin = 0;
	CString strDataString, strValueString;

	// Write out each entry in the map, up to an arbitrary but reasonable 
	// maximum number of registry entries. For historical reasons, plugin 
	// indices in the registry are 1-based.
	PluginSet::CPair *lpPluginEntry = m_lstPlugins.PGetFirstAssoc();
	for (iCurPlugin = 0; lpPluginEntry && iCurPlugin < nMaxPluginsToSave; 
		iCurPlugin++, 
		lpPluginEntry = m_lstPlugins.PGetNextAssoc(lpPluginEntry))
	{
		PLUGINENTRY &Plugin = *lpPluginEntry->value;

		strValueString.Format(IDS_PLUGINVALUETEMPLATE, iCurPlugin + 1);
		strDataString.Format(IDS_PLUGINDATATEMPLATE, Plugin.strFileName, Plugin.bSelected);

		theApp.WriteProfileString(strRegHeader, strValueString, strDataString);
	}

	theApp.WriteProfileInt(strRegHeader, "Num Plugins", iCurPlugin);

	return TRUE;
}

BOOL CPluginPage::LoadSettings()
{
	CString strRegHeader, strRegPluginDir;
	LPCSTR lpszDefaultDir = theApp.GetStartupPath();
	strRegHeader.LoadString(m_bSEMPQWizard ? IDS_SEMPQKEY : IDS_PATCHKEY);
	strRegPluginDir.LoadString(IDS_PLUGINDIRKEY);

	m_strPluginDir = theApp.GetProfileString(strRegHeader, strRegPluginDir, lpszDefaultDir);

	if (!PathIsDirectory(m_strPluginDir))
		m_strPluginDir = lpszDefaultDir;

	// This default value here should allow this code to work with values that
	// were saved by an older version of MPQDraft, which wrote this many
	// values and didn't save how many it wrote.
	DWORD nPlugins = theApp.GetProfileInt(strRegHeader, "Num Plugins", MAX_MPQDRAFT_PLUGINS * 2);

	// Read each plugin's file path and selected state into the map
	CString strValueString;
	for (DWORD iCurPlugin = 0; iCurPlugin < nPlugins; iCurPlugin++)
	{
		strValueString.Format(IDS_PLUGINVALUETEMPLATE, iCurPlugin + 1);

		CString strRegString = theApp.GetProfileString(strRegHeader, strValueString, "");
		if (strRegString.IsEmpty())
			continue;

		AddPluginFromRegistry(strRegString);
	}

	return TRUE;
}

CPluginPage::PLUGINENTRY *CPluginPage::AddPlugin(LPCSTR lpszFileName, 
	BOOL *lpbSelected, BOOL *lpbAddedNew)
{
	ASSERT(lpszFileName);

	CString strFileName;
	SanitizeFilePath(lpszFileName, strFileName);

	// Check to see if we already have this plugin in the list
	PLUGINENTRY *lpPluginEntry;
	if (m_lstPlugins.Lookup(strFileName, lpPluginEntry))
	{
		// We do. Synchronize the selection state.
		if (lpbAddedNew)
			*lpbAddedNew = FALSE;

		if (lpbSelected)
			lpPluginEntry->bSelected = *lpbSelected;

		return lpPluginEntry;
	}
	else
	{
		// It's not in the list. Add it.
		if (lpbAddedNew)
			*lpbAddedNew = TRUE;

		// Catch exceptions coming from PLUGINENTRY, as they indicate that 
		// the plugin could not be loaded for some reason (failure).
		try
		{
			lpPluginEntry = new PLUGINENTRY(strFileName, 
				(lpbSelected ? *lpbSelected : FALSE));

			m_lstPlugins.SetAt(strFileName, lpPluginEntry);

			return lpPluginEntry;
		}
		catch (...)
		{ return NULL; }
	}
}

CPluginPage::PLUGINENTRY *CPluginPage::AddPluginFromRegistry(LPCSTR lpszRegString)
{
	ASSERT(lpszRegString);

	// The string is in "%s;%d" format

	DWORD dwStrLen = strlen(lpszRegString);
	// Simple sanity check: file path must be at least 4 chars, 2 more for the 
	// selection state and semicolon.
	if (dwStrLen < 6)
		return NULL;

	BOOL bSelected = FALSE;
	char szFileName[MAX_PATH];

	strcpy(szFileName, lpszRegString);

	// This is kind of cheesy, but works
	if (lpszRegString[dwStrLen - 2] == ';')
	{
		szFileName[dwStrLen - 2] = '\0';
		bSelected = (atoi(&szFileName[dwStrLen - 1]) != 0);
	}

	// It's possible the plugin got deleted since MPQDraft was run last.
	// Check for that possibility.
	if (!PathFileExists(szFileName))
		return FALSE;

	// Add it
	return AddPlugin(szFileName, &bSelected);
}

BOOL CPluginPage::FindPlugins()
{
	CFileFind find;
	CString strFilter;
	char szFindSpec[MAX_PATH + 1];

	strFilter.LoadString(IDS_PLUGINFILTER);

	// Form the wildcard spec
	strcpy(szFindSpec, m_strPluginDir);
	PathAddBackslash(szFindSpec);
	strcat(szFindSpec, strFilter);

	// Add all files found that match the spec through CFileFind
	BOOL bFileFound = find.FindFile(szFindSpec);
	while (bFileFound)
	{
		// FindNextFile has strange semantecs: the return value indicates if 
		// there are MORE files, or if this is the last one. Thus it's called 
		// before GetFilePath.
		bFileFound = find.FindNextFile();

		AddPlugin(find.GetFilePath(), NULL, FALSE);
	}

	return TRUE;
}

BOOL CPluginPage::ListPlugins()
{
	// Basic list view item properties
	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = 100000;
	lvItem.iSubItem = 0;
	lvItem.iImage = theApp.GetIconIndex(IDI_PLUGIN);

	m_ctlPlugins.SetRedraw(FALSE);
	m_bUpdatingList = TRUE;

	// Clear the previous contents
	m_ctlPlugins.DeleteAllItems();

	// Add all items in the plugin list
	for (PluginSet::CPair *lpPluginEntry = m_lstPlugins.PGetFirstAssoc();
		lpPluginEntry != NULL; 
		lpPluginEntry = m_lstPlugins.PGetNextAssoc(lpPluginEntry))
	{
		PLUGINENTRY *lpPlugin = lpPluginEntry->value;
		lvItem.lParam = (LPARAM)lpPlugin;

		// If we're in patch wizard mode, only list plugins that are 
		// compatible with the patch target.
		if (!m_bSEMPQWizard 
			&& !lpPlugin->pPlugin->CanPatchExecutable(m_strPatchTarget))
			continue;

		// Add the item and check it if necessary
		CString strItemTitle;
		strItemTitle.Format(IDS_PLUGINLISTTEMPLATE, lpPlugin->strPluginName, PathFindFileName(lpPlugin->strFileName));

		lvItem.pszText = (LPSTR)strItemTitle.GetString();

		int iPlugin = m_ctlPlugins.InsertItem(&lvItem);
		if (iPlugin != -1)
			m_ctlPlugins.SetCheck(iPlugin, lpPlugin->bSelected);
	}

	// Finally, sort the list and make it draw itself
	m_ctlPlugins.SortItems(ComparePlugins, 0);
	m_ctlPlugins.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	m_bUpdatingList = FALSE;
	m_ctlPlugins.SetRedraw(TRUE);
	m_ctlPlugins.Invalidate();

	return TRUE;
}

BOOL CPluginPage::UnloadPlugins()
{
	// Simply delete the PLUGINENTRYs and clear the map
	for (PluginSet::CPair *lpPluginEntry = m_lstPlugins.PGetFirstAssoc();
		lpPluginEntry != NULL; 
		lpPluginEntry = m_lstPlugins.PGetNextAssoc(lpPluginEntry))
		delete lpPluginEntry->value;

	m_lstPlugins.RemoveAll();

	return TRUE;
}

BOOL CPluginPage::SetPatchTarget(CString strPatchTarget)
{
	BOOL bTargetChanged = (_stricmp(strPatchTarget, m_strPatchTarget) != 0);
	if (bTargetChanged)
		m_strPatchTarget = strPatchTarget;

	return bTargetChanged;
}

BOOL CPluginPage::PluginsReady()
{
	CString strMessage;

	// Verify that all compatible selected plugins are ready to patch. Use the
	// list view, as we know those are compatible.
	int nPlugins = m_ctlPlugins.GetItemCount();
	for (int iPlugin = 0; iPlugin < nPlugins; iPlugin++)
	{
		if (!m_ctlPlugins.GetCheck(iPlugin))
			continue;	// Not selected

		// Get the plugin entry
		PLUGINENTRY *lpPlugin = (PLUGINENTRY *)
			m_ctlPlugins.GetItemData(iPlugin);
		ASSERT(lpPlugin);

		if (lpPlugin->pPlugin->ReadyForPatch())
			continue;	// It's ready

		// It's not ready. Ask the user if they want to continue without it.
		strMessage.Format(IDS_PLUGINNOTREADY, lpPlugin->strPluginName);
		if (MessageBox(strMessage, NULL, MB_YESNO | MB_ICONQUESTION) == IDNO)
			return FALSE;

		// Deselect it and continue
		lpPlugin->bSelected = FALSE;

		m_bUpdatingList = TRUE;
		m_ctlPlugins.SetCheck(iPlugin, FALSE);
		m_bUpdatingList = FALSE;
	}

	return TRUE;
}

BOOL CPluginPage::GetSelectedPluginsAndModules(PluginList *lpPlugins,
   PluginModuleList &modules)
{
	if (lpPlugins)
		lpPlugins->RemoveAll();
	modules.RemoveAll();

	// Same principle as in PluginsReady
	int nPlugins = m_ctlPlugins.GetItemCount();
	for (int iPlugin = 0; iPlugin < nPlugins; iPlugin++)
	{
		if (!m_ctlPlugins.GetCheck(iPlugin))
			continue;	// Not selected

		// Get the plugin entry for the selected plugin and add it to the list
		PLUGINENTRY *lpPlugin = (PLUGINENTRY *)
			m_ctlPlugins.GetItemData(iPlugin);
		ASSERT(lpPlugin);

		if (lpPlugins)
			lpPlugins->Add(lpPlugin);

		// Get the modules. Get the number of required modules, grow the 
		// array to make room for the modules, and then fetch the modules 
		// directly into the array. Note that we also have to add the plugin 
		// binary itself to the list.
		DWORD nPluginModules;
		if (!lpPlugin->pPlugin->GetModules(NULL, &nPluginModules))
			return FALSE;

		DWORD nPrevModules = modules.GetSize();
		modules.SetSize(nPrevModules + nPluginModules + 1);

		// Add the info for the plugin binary
		modules[nPrevModules].dwComponentID = lpPlugin->dwPluginID;
		modules[nPrevModules].dwModuleID = 0;
		modules[nPrevModules].bExecute = TRUE;
		strcpy(modules[nPrevModules].szModuleFileName, lpPlugin->strFileName);

		if (!nPluginModules)
			continue;	// No modules to get for this plugin

		if (!lpPlugin->pPlugin->GetModules(
			modules.GetData() + nPrevModules + 1, &nPluginModules))
			return FALSE;

		// Force all modules to have the right component ID
		for (INT_PTR iModule = (INT_PTR)nPrevModules + 1; iModule < modules.GetCount(); iModule++)
			modules.GetAt(iModule).dwComponentID = lpPlugin->dwPluginID;
	}

	return TRUE;
}

BOOL CPluginPage::GetNumSelectedPluginsAndModules(DWORD &nPlugins, DWORD &nModules)
{
	nPlugins = 0;
	nModules = 0;

	// Same as in GetSelectedPluginsAndModules, but we only count them
	int nTotalPlugins = m_ctlPlugins.GetItemCount();
	for (int iPlugin = 0; iPlugin < nTotalPlugins; iPlugin++)
	{
		if (!m_ctlPlugins.GetCheck(iPlugin))
			continue;	// Not selected

		// Get the plugin entry for the selected plugin and add it to the count
		PLUGINENTRY *lpPlugin = (PLUGINENTRY *)
			m_ctlPlugins.GetItemData(iPlugin);
		ASSERT(lpPlugin);

		nPlugins++;

		// Get the module count
		DWORD nPluginModules;
		if (!lpPlugin->pPlugin->GetModules(NULL, &nPluginModules))
			return FALSE;

		nModules += nPluginModules;
	}

	nModules += nPlugins;	// One module for each plugin

	return TRUE;
}

void CPluginPage::OnBrowse() 
{
	CString strFilter, strTitle;
	strFilter.LoadString(IDS_OPENPLUGINSETTINGS);
	strTitle.LoadString(IDS_OPENPLUGIN);

	// Set up the basic properties of the open dialog
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER | 
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = m_strPluginDir;

	if (dlg.DoModal() != IDOK)
		return;	// Cancelled or an error occurred

	// Update the list view by iterating over the returned file paths (yes, 
	// there can be more than one selected).
	m_ctlPlugins.SetRedraw(FALSE);
	m_bUpdatingList = TRUE;

	int iLastPlugin = -1;
	CString strItemTitle;
	POSITION posPlugin = dlg.GetStartPosition();
	while (posPlugin)
	{
		CString strFilePath = dlg.GetNextPathName(posPlugin);

		// Add it to the plugin list
		BOOL bSelected = TRUE, bAddedNew;
		PLUGINENTRY *lpPlugin = AddPlugin(strFilePath, &bSelected, &bAddedNew);
		if (!lpPlugin)
			continue;

		// Now add/update it in the list view
		int iPlugin = -1;
		if (!bAddedNew)
		{
			// The item already exists in the list. Try and find it.
			LVFINDINFO listFindInfo;
			listFindInfo.flags = LVFI_PARAM;
			listFindInfo.lParam = (LPARAM)lpPlugin;

			iPlugin = m_ctlPlugins.FindItem(&listFindInfo);
		}
		// The item didn't exist. If it's compatible, add it now.
		else if (m_bSEMPQWizard 
			|| lpPlugin->pPlugin->CanPatchExecutable(m_strPatchTarget))
		{
			strItemTitle.Format(IDS_PLUGINLISTTEMPLATE, 
				lpPlugin->strPluginName, 
				PathFindFileName(lpPlugin->strFileName));

			LVITEM lvItem;
			lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
			lvItem.iItem = 100000;
			lvItem.iSubItem = 0;
			lvItem.pszText = (LPSTR)(LPCSTR)strItemTitle;
			lvItem.iImage = theApp.GetIconIndex(IDI_PLUGIN);
			lvItem.lParam = (LPARAM)lpPlugin;

			iPlugin = m_ctlPlugins.InsertItem(&lvItem);
		}

		// It's possible for the item to not exist in the list view at this 
		// point. While this could indicate an error adding it, it could also 
		// simply mean that the plugin doesn't support our patch target.
		if (iPlugin != -1)
			m_ctlPlugins.SetCheck(iPlugin);

		iLastPlugin = max(iLastPlugin, iPlugin);
	}

	// Set the list view selection cursor to the last added plugin
	if (iLastPlugin != -1)
		m_ctlPlugins.SetItemState(iLastPlugin, LVIS_SELECTED, LVIS_SELECTED);

	// Redraw it
	m_bUpdatingList = FALSE;
	m_ctlPlugins.SetRedraw(TRUE);
	m_ctlPlugins.Invalidate();
}

void CPluginPage::OnConfigure() 
{
	// Get the selected plugin entry
	POSITION posSelPlugin = m_ctlPlugins.GetFirstSelectedItemPosition();
	if (!posSelPlugin)
		return;	// No item is selected

	DWORD iSelPlugin = m_ctlPlugins.GetNextSelectedItem(posSelPlugin);
	PLUGINENTRY *lpPlugin = (PLUGINENTRY *)m_ctlPlugins.GetItemData(iSelPlugin);
	ASSERT(lpPlugin);

	// Tell it to configure itself
	lpPlugin->pPlugin->Configure(*GetParent());
}

void CPluginPage::OnItemChangedPlugins(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_bUpdatingList || pNMListView->iItem == -1)
		return; // Filter out unwanted notifications
	
	// The checked state is stored in the state image list index for each 
	// item in the list view. However, I haven't found any confirmation that 
	// this is documented and guaranteed, so we're just gonna set the check 
	// state every time we get called.
	PLUGINENTRY *lpPlugin = (PLUGINENTRY *)
		m_ctlPlugins.GetItemData(pNMListView->iItem);
	ASSERT(lpPlugin);

	BOOL bSelected = m_ctlPlugins.GetCheck(pNMListView->iItem);
	lpPlugin->bSelected = bSelected;
}

int CALLBACK CPluginPage::ComparePlugins(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	PLUGINENTRY *lpPlugin1 = (PLUGINENTRY *)lParam1, 
		*lpPlugin2 = (PLUGINENTRY *)lParam2;

	ASSERT(lpPlugin1);
	ASSERT(lpPlugin2);

	// We're going to do a hierarchal comparison of plugin name, plugin file 
	// name (excluding path), and plugin path, in that order. Simply use 
	// strcmp to do the comparisons.
	int result = strcmp(lpPlugin1->strPluginName, lpPlugin2->strPluginName);
	if (result != 0)
		return result;

	result = _stricmp(PathFindFileName(lpPlugin1->strFileName),
		PathFindFileName(lpPlugin2->strFileName));
	if (result != 0)
		return result;

	return _stricmp(lpPlugin1->strFileName, lpPlugin2->strFileName);
}