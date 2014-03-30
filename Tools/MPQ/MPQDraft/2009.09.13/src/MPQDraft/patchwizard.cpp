/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// PatchWizard.cpp : implementation file
//

#include "stdafx.h"
#include "MPQDraft.h"
#include "resource.h"
#include "PatchWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchWizardPage1 property page

IMPLEMENT_DYNCREATE(CPatchWizardPage1, CPatchTargetPage)

CPatchWizardPage1::CPatchWizardPage1() 
	: CPatchTargetPage(FALSE, IDS_PATCHWIZARD1TITLE, IDS_PATCHWIZARD1INFO)
{
	//{{AFX_DATA_INIT(CPatchWizardPage1)
	//}}AFX_DATA_INIT
}

CPatchWizardPage1::~CPatchWizardPage1()
{
}

BOOL CPatchWizardPage1::OnSetActive() 
{
	// Simply set up the button states
	CPatchWizard *pWizard = (CPatchWizard *)GetParent();
	pWizard->SetWizardButtons(PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}

BEGIN_MESSAGE_MAP(CPatchWizardPage1, CPatchTargetPage)
	//{{AFX_MSG_MAP(CPatchWizardPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchWizardPage1 message handlers

/////////////////////////////////////////////////////////////////////////////
// CPatchWizardPage2 property page

IMPLEMENT_DYNCREATE(CPatchWizardPage2, CPropertyPage)

CPatchWizardPage2::CPatchWizardPage2() : CPropertyPage(CPatchWizardPage2::IDD)
{
	//{{AFX_DATA_INIT(CPatchWizardPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bUpdatingList = FALSE;
}

CPatchWizardPage2::~CPatchWizardPage2()
{
}

void CPatchWizardPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchWizardPage2)
	DDX_Control(pDX, IDC_MPQS, m_ctlPatchMPQs);
	//}}AFX_DATA_MAP
}

BOOL CPatchWizardPage2::OnSetActive() 
{
	// Set the wizard buttons
	CPatchWizard *pWizard = (CPatchWizard *)GetParent();
	pWizard->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}

BOOL CPatchWizardPage2::OnKillActive() 
{
	if (!CPropertyPage::OnKillActive())
		return FALSE;

	// As with all the other pages, we do our verification when the page 
	// changes. Here, we need to verify that the user has not selected too 
	// many MPQs. Count them.
	int nMPQs = m_ctlPatchMPQs.GetItemCount(), nNumSelMPQs = 0;
	for (int iMPQ = 0; iMPQ < nMPQs; iMPQ++)
	{
		MPQListMap::CPair *lpMPQEntry = 
			(MPQListMap::CPair *)m_ctlPatchMPQs.GetItemData(iMPQ);
		if (lpMPQEntry->value)
			nNumSelMPQs++;
	}

	if (nNumSelMPQs > MAX_PATCH_MPQS)
	{
		// Too many
		CString strMessage;
		strMessage.Format(IDS_TOOMANYMPQS, MAX_PATCH_MPQS);

		MessageBox(strMessage, NULL, MB_OK | MB_ICONEXCLAMATION);

		return FALSE;
	}
	
	return TRUE;
}

BOOL CPatchWizardPage2::ListDirectoryMPQs()
{
	// Here we can search for whatever file types we can use to patch stuff. 
	// There are a wide variety of things that use MPQs - Starcraft maps, 
	// Warcraft 3 maps, even SEMPQs. However, most of these have very specific
	// uses, and aren't relevant to us. As of right now, the only other 
	// non-MPQ extension I know of that is used for patches is D2M - Diablo II
	// Mod Manager. If there are any others, they can be added here.
	// It doesn't matter what order we list them, as we're going to sort the 
	// entire list afterwards.
	CString strFilter;

	strFilter.LoadString(IDS_MPQFILTER);
	if (!ListMPQsBySpec(strFilter))
		return FALSE;

	strFilter.LoadString(IDS_D2MFILTER);
	if (!ListMPQsBySpec(strFilter))
		return FALSE;

	return TRUE;
}

BOOL CPatchWizardPage2::ListMPQsBySpec(LPCSTR lpszFileSpec)
{
	// Simply make the full wildcard path spec, then use it with CFileFind to 
	// find all files of this type.
	CFileFind find;
	char szFileFilter[MAX_PATH + 1];

	strcpy(szFileFilter, m_strMPQDir);
	PathAddBackslash(szFileFilter);
	strcat(szFileFilter, lpszFileSpec);

	BOOL bFound = find.FindFile(szFileFilter), bFalse = FALSE;
	while (bFound)
	{
		bFound = find.FindNextFile();

		if (!AddMPQ(find.GetFilePath(), &bFalse))
			return FALSE;
	}

	return TRUE;
}

BOOL CPatchWizardPage2::AddMPQ(LPCSTR lpszFileName, LPBOOL lpbSelected)
{
	CString strFileName;
	SanitizeFilePath(lpszFileName, strFileName);

	// Check to see if we already have this MPQ in the list
	MPQListMap::CPair *lpMPQEntry = m_mapMPQs.PLookup(strFileName);
	if (lpMPQEntry)
	{
		// We already do. Find it in the list view so we can update it
		LVFINDINFO findInfo;
		findInfo.flags = LVFI_PARAM;
		findInfo.lParam = (LPARAM)lpMPQEntry;

		int iMPQ = m_ctlPatchMPQs.FindItem(&findInfo);
		ASSERT(iMPQ != -1);

		if (lpbSelected)
			// Set the selection. We can rely on the item changed handler to 
			// update the list, if necessary.
			m_ctlPatchMPQs.SetCheck(iMPQ);

		// Highlight it in the list and bring it into focus
		m_ctlPatchMPQs.SetItemState(iMPQ, LVIS_SELECTED, LVIS_SELECTED);
		m_ctlPatchMPQs.EnsureVisible(iMPQ, TRUE);

		return TRUE;
	}

	// It's not already in the list. Add it to the list. Note that we're not 
	// adding it as checked. This is because we have to call SetCheck anyway, 
	// so we might as well just let that do the work for us.
	m_mapMPQs.SetAt(strFileName, FALSE);
	lpMPQEntry = m_mapMPQs.PLookup(strFileName);

	// Now add it to the list view
	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = 100000;
	lvItem.iSubItem = 0;
	lvItem.iImage = theApp.GetIconIndex(IDI_MPQ);
	lvItem.lParam = (LPARAM)lpMPQEntry;

	// If it's in the initial MPQ directory, display only the filename. If 
	// it's from another directory, display the entire path.
	if (_strnicmp(strFileName, m_strInitialMPQDir, 
		m_strInitialMPQDir.GetLength()) == 0)
		lvItem.pszText = PathFindFileName(strFileName);
	else
		lvItem.pszText = (LPSTR)strFileName.GetString();

	int iMPQ = m_ctlPatchMPQs.InsertItem(&lvItem);
	if (iMPQ == -1)
		return FALSE;

	// Lastly, select the MPQ if desired.
	if (lpbSelected && *lpbSelected)
		m_ctlPatchMPQs.SetCheck(iMPQ, TRUE);

	return TRUE;
}

BOOL CPatchWizardPage2::MoveMPQ(int nFrom, int nTo)
{
	ASSERT(nFrom >= 0);
	ASSERT(nTo >= 0);
	ASSERT(nFrom < m_ctlPatchMPQs.GetItemCount());
	ASSERT(nFrom != nTo);

	// Just insert a new copy of the item, then delete the original. Easy. The
	// only nontrivial part is making sure you act on the right indices, as 
	// the indices of the ones we're working with may change due to the 
	// addition.
	MPQListMap::CPair *lpFromMPQ = 
		(MPQListMap::CPair *)m_ctlPatchMPQs.GetItemData(nFrom);
	ASSERT(lpFromMPQ);

	// Get the info for the old one
	CString strFileName = m_ctlPatchMPQs.GetItemText(nFrom, 0);
	BOOL bSelected = m_ctlPatchMPQs.GetCheck(nFrom);

	// Insert the item into its new position. If the new position is before 
	// the old one, we can use the new index directly. If it's >= the old 
	// one, however, we need to add one, as the old one is still there for 
	// the moment, and we have to hop over it.
	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = (nFrom < nTo ? nTo + 1 : nTo);
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPSTR)strFileName.GetString();
	lvItem.iImage = theApp.GetIconIndex(IDI_MPQ);
	lvItem.lParam = (LPARAM)lpFromMPQ;

	int iMPQ = m_ctlPatchMPQs.InsertItem(&lvItem);
	if (iMPQ == -1)
		return FALSE;

	// Check the new one if necessary
	if (bSelected)
		m_ctlPatchMPQs.SetCheck(iMPQ);

	// Delete the old item. Here we also have to be careful with the index. If
	// the new position was inserted above the old one, with need to increase 
	// the target index by 1. The case of = also results in the new one being 
	// above the old one.
	m_ctlPatchMPQs.DeleteItem(nTo <= nFrom ? nFrom + 1 : nFrom);

	return TRUE;
}

int CALLBACK CPatchWizardPage2::CompareMPQs(
	LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// Two-level comparison. Sort first by file name (excluding path). If that
	// fails (the two are equal), compare the full file path.
	MPQListMap::CPair *lpMPQEntry1 = (MPQListMap::CPair *)lParam1,
		*lpMPQEntry2 = (MPQListMap::CPair *)lParam2;

	int result = _stricmp(PathFindFileName(lpMPQEntry1->key), 
		PathFindFileName(lpMPQEntry2->key));
	if (result != 0)
		return result;

	return _stricmp(lpMPQEntry1->key, lpMPQEntry2->key);
}

BEGIN_MESSAGE_MAP(CPatchWizardPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CPatchWizardPage2)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveUp)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMoveDown)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MPQS, OnItemChangedMPQs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchWizardPage2 message handlers

BOOL CPatchWizardPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Load the settings from the registry
	LoadSettings();

	// Set up the list view - title, styles, image list, and the column. As 
	// the title is the current MPQ path, we must do this after LoadSettings 
	// has been called.
	CString strTitle;
	strTitle.Format(IDS_MPQLISTHEADER, m_strInitialMPQDir);

	m_ctlPatchMPQs.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_ctlPatchMPQs.InsertColumn(0, strTitle);
	m_ctlPatchMPQs.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ctlPatchMPQs.SetImageList(&theApp.GetSmallIcons(), LVSIL_SMALL);

	// List the MPQs and sort the ones initially listed
	ListDirectoryMPQs();
	m_ctlPatchMPQs.SortItems(CompareMPQs, 0);

	// Return FALSE, as we have not set focus to a control
	return TRUE;
}

void CPatchWizardPage2::OnItemChangedMPQs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	// Filter out notifications sent while one of the update functions is 
	// messing with the list, or due to things other than the state changing.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_bUpdatingList || (pNMListView->uChanged != LVIF_STATE))
		return;

	MPQListMap::CPair *lpMPQEntry = (MPQListMap::CPair *)
		m_ctlPatchMPQs.GetItemData(pNMListView->iItem);

	// Check if there's been a change to the check state
	BOOL bSelect = m_ctlPatchMPQs.GetCheck(pNMListView->iItem);
	if ((bSelect != FALSE) == (lpMPQEntry->value != FALSE))
		// There hasn't. This is either a change not related to check state, 
		// or one of our functions has desynced the list and view. Ignore.
		return;

	// The state has changed. Update the list version.
	lpMPQEntry->value = bSelect;
}

void CPatchWizardPage2::OnBrowse() 
{
	// Load the settings for the dialog
	CString strFilter, strTitle;
	strFilter.LoadString(IDS_OPENMPQSETTINGS);
	strTitle.LoadString(IDS_OPENPATCHMPQ);

	// Set up the dialog and run it
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER | 
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = m_strMPQDir;

	if (dlg.DoModal() != IDOK)
		return;	// Cancelled

	// Add each selected MPQ
	CString strFilePath;
	POSITION pos = dlg.GetStartPosition();
	while (pos != NULL)
	{
		strFilePath = dlg.GetNextPathName(pos);

		BOOL bTrue = TRUE;
		AddMPQ(strFilePath, &bTrue);
	}

	// Update the current MPQ directory if the user changed the directory
	char szMPQDir[MAX_PATH] = "";
	strcpy(szMPQDir, dlg.GetPathName());
	PathRemoveFileSpec(szMPQDir);
	m_strMPQDir = szMPQDir;
}

BOOL CPatchWizardPage2::SaveSettings()
{
	// Just one setting to write: the MPQ directory
	CString strRegPatchHdr, strRegMPQDir;
	strRegPatchHdr.LoadString(IDS_PATCHKEY);
	strRegMPQDir.LoadString(IDS_MPQDIRKEY);

	theApp.WriteProfileString(strRegPatchHdr, strRegMPQDir, m_strMPQDir);

	return TRUE;
}

BOOL CPatchWizardPage2::LoadSettings()
{
	// Same as with saving: just one setting
	CString strRegPatchHdr, strRegMPQDir;
	strRegPatchHdr.LoadString(IDS_PATCHKEY);
	strRegMPQDir.LoadString(IDS_MPQDIRKEY);
	LPCSTR lpszStartupPath = theApp.GetStartupPath();

	m_strMPQDir = theApp.GetProfileString(strRegPatchHdr, 
		strRegMPQDir, lpszStartupPath);
	if (!PathIsDirectory(m_strMPQDir))
		m_strMPQDir = lpszStartupPath;
	// Make sure we sanitize the path, as we're going to be using strcmp on it
	SanitizeFilePath(m_strMPQDir, m_strInitialMPQDir);

	return TRUE;
}

void CPatchWizardPage2::OnMoveUp() 
{
	// Get the MPQ to move
	POSITION pos = m_ctlPatchMPQs.GetFirstSelectedItemPosition();
	if (!pos)
		return;	// No highlighted MPQ

	int nItemPos = m_ctlPatchMPQs.GetNextSelectedItem(pos),
		nNewItemPos = nItemPos - 1;
	if (nNewItemPos < 0)
		return;	// We're at the top of the list view. Ignore.

	// Move it. Highlight it after the move, as movement actually creates a 
	// new list entry, which obviously won't be highlighted.
	MoveMPQ(nItemPos, nNewItemPos);
	m_ctlPatchMPQs.SetItemState(nNewItemPos, 
		LVIS_SELECTED, LVIS_SELECTED);
}

void CPatchWizardPage2::OnMoveDown() 
{
	// Exactly the same as with OnMoveUp
	POSITION pos = m_ctlPatchMPQs.GetFirstSelectedItemPosition();
	if (!pos)
		return;

	int nItemPos = m_ctlPatchMPQs.GetNextSelectedItem(pos),
		nNewItemPos = nItemPos + 1;
	if (nNewItemPos >= m_ctlPatchMPQs.GetItemCount())
		return;	// It's the last item in the list view

	MoveMPQ(nItemPos, nNewItemPos);
	m_ctlPatchMPQs.SetItemState(nNewItemPos, 
		LVIS_SELECTED, LVIS_SELECTED);
}

BOOL CPatchWizardPage2::GetSelectedMPQs(MPQPathList &MPQPaths)
{
	// MPQDraftPatch takes MPQs as an array of LPCSTRs, so we have to build it
	MPQPaths.RemoveAll();

	// Scan through the list looking for selected MPQs
	DWORD nMPQs = m_ctlPatchMPQs.GetItemCount();
	for (DWORD iMPQ = 0; iMPQ < nMPQs; iMPQ++)
	{
		MPQListMap::CPair *lpMPQEntry = 
			(MPQListMap::CPair *)m_ctlPatchMPQs.GetItemData(iMPQ);
		if (!lpMPQEntry->value)
			continue;	// It's not selected

		// I get the feeling it's a bad idea to go calling GetString on all 
		// of the CStrings in the MPQ list. So, we're going to allocate 
		// buffers for each path, and add it.
		LPSTR lpszMPQPath = NULL;
		try
		{
			lpszMPQPath = new char[lpMPQEntry->key.GetLength() + 1];
			strcpy(lpszMPQPath, lpMPQEntry->key);

			MPQPaths.Add(lpszMPQPath);
		}
		catch (...)
		{
			// Out of memory. Free the buffer, if there is one, and all the 
			// buffers allocated previously.
			if (lpszMPQPath)
				delete [] lpszMPQPath;
			FreeSelectedMPQs(MPQPaths);

			return FALSE;
		}
	}

	return TRUE;
}

void CPatchWizardPage2::FreeSelectedMPQs(MPQPathList &MPQPaths)
{
	// Free all the buffers and clear the list
	for (int iMPQ = 0; iMPQ < MPQPaths.GetSize(); iMPQ++)
		delete [] (LPSTR)MPQPaths[iMPQ];

	MPQPaths.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CPatchWizardPage3 property page

IMPLEMENT_DYNCREATE(CPatchWizardPage3, CPropertyPage)

CPatchWizardPage3::CPatchWizardPage3() 
: CPluginPage(FALSE, IDS_PATCHWIZARD3TITLE)
{
	//{{AFX_DATA_INIT(CPatchWizardPage3)
	//}}AFX_DATA_INIT
}

CPatchWizardPage3::~CPatchWizardPage3()
{
}

BEGIN_MESSAGE_MAP(CPatchWizardPage3, CPluginPage)
	//{{AFX_MSG_MAP(CPatchWizardPage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchWizardPage3 message handlers

BOOL CPatchWizardPage3::OnSetActive() 
{
	// Set the buttons like usual
	CPatchWizard *pWizard = (CPatchWizard *)GetParent();
	pWizard->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	
	// This page is special because it's the only page that depends on data 
	// from a previous page (namely CPatchTargetPage). Here we need to get 
	// the selected program from that page and update ourself with it.
	CPatchWizardPage1 *pPage1 = &pWizard->m_firstPage;

	CString strProgramPath;
	pPage1->m_ctlProgramPath.GetWindowText(strProgramPath);
	if (SetPatchTarget(strProgramPath))
		ListPlugins();	// The target has changed. Refresh the plugin list.

	return CPropertyPage::OnSetActive();
}

BOOL CPatchWizardPage3::OnWizardFinish() 
{
	// Use OnKillActive to verify that there aren't too many selected plugins 
	// or modules.
	if (!OnKillActive())
		return FALSE;

	// Verify that all plugins are configured and ready to rock
	if (!PluginsReady())
		return FALSE;

	// Do it
	CPatchWizard *pWizard = (CPatchWizard *)GetParent();
	if (!pWizard->DoPatch())
		return FALSE;

	// Close the wizard
	return CPropertyPage::OnWizardFinish();
}

/////////////////////////////////////////////////////////////////////////////
// CPatchWizard

IMPLEMENT_DYNAMIC(CPatchWizard, CPropertySheet)

CPatchWizard::CPatchWizard(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_firstPage);
	AddPage(&m_secondPage);
	AddPage(&m_thirdPage);

	SetWizardMode();
}

CPatchWizard::CPatchWizard(LPCTSTR pszCaption, 
	CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_firstPage);
	AddPage(&m_secondPage);
	AddPage(&m_thirdPage);

	SetWizardMode();
}

CPatchWizard::~CPatchWizard()
{
}

BOOL CPatchWizard::DoPatch()
{
	CString strMessage;

	// Save the settings of all the pages to the registry
	m_firstPage.SaveSettings();
	m_secondPage.SaveSettings();
	m_thirdPage.SaveSettings();

	// Load the DLL and find the patcher function
	MPQDraftPatcherPtr MPQDraftPatcher = GetPatcher();
	if (!MPQDraftPatcher)
	{
		strMessage.LoadString(IDS_PATCHFAILED);
		MessageBox(strMessage, NULL, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Get the program to patch and the command line parameters
	CString strProgramPath, strSpawnPath, strParameters;
	m_firstPage.GetParameters(strParameters);
	if (!m_firstPage.GetProgramPath(strProgramPath) ||
		!m_firstPage.GetPatchTargetPath(strSpawnPath))
	{
		strMessage.LoadString(IDS_PATCHFAILED);
		MessageBox(strMessage, NULL, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Compile the flags
	DWORD dwFlags = 0;

	if (m_firstPage.RedirOpenFileEx())
		dwFlags |= MPQD_EXTENDED_REDIR;
	if (!strSpawnPath.GetLength())
		m_firstPage.GetProgramPath(strSpawnPath);

	// Get the MPQs to use for patching
	CPatchWizardPage2::MPQPathList MPQs;
	CPluginPage::PluginModuleList modules;

	BOOL bPatchSuccess = FALSE;

	if (m_secondPage.GetSelectedMPQs(MPQs)
		&& m_thirdPage.GetSelectedPluginsAndModules(NULL, modules))
	{
		// Build the command line from the program and parameters
		char szCommandLine[MAX_PATH * 2], szCurDir[MAX_PATH + 1],
			szStartDir[MAX_PATH + 1];

		strcpy(szStartDir, strProgramPath);
		PathRemoveFileSpec(szStartDir);

		GetModuleFileName(NULL, szCurDir, MAX_PATH);
		PathRemoveFileSpec(szCurDir);

		wsprintf(szCommandLine, "\"%s\" %s", strProgramPath, strParameters);

		// Hide our windows during the patch
		CWnd *pMainMenu = AfxGetMainWnd();
		ASSERT(pMainMenu);
		
		ShowWindow(SW_HIDE);
		pMainMenu->ShowWindow(SW_HIDE);

		// Use the same environment as for MPQDraft
		STARTUPINFO si;
		GetStartupInfo(&si);

		// Do it
		bPatchSuccess = MPQDraftPatcher(strProgramPath, szCommandLine, NULL, 
			NULL, FALSE, 0, NULL, szStartDir, &si, dwFlags, szCurDir, 
			strSpawnPath, 0, MPQs.GetSize(), modules.GetSize(), 
			MPQs.GetData(), modules.GetData());
			bPatchSuccess = TRUE;

		// Restore our windows
		pMainMenu->ShowWindow(SW_RESTORE);
		ShowWindow(SW_RESTORE);
	}

	m_secondPage.FreeSelectedMPQs(MPQs);

	if (!bPatchSuccess)
	{
		strMessage.LoadString(IDS_PATCHFAILED);
		MessageBox(strMessage, NULL, MB_OK | MB_ICONSTOP);
	}

	return bPatchSuccess;
}

MPQDraftPatcherPtr CPatchWizard::GetPatcher()
{
	// Get patcher DLL path
	CString *lpstrPatcherDLL = theApp.GetPatcherDLLPath();
	if (!lpstrPatcherDLL)
		return NULL;

	// Load it
	HMODULE hDLL = GetModuleHandle(*lpstrPatcherDLL);
	if (!hDLL)
	{
		hDLL = LoadLibrary(*lpstrPatcherDLL);
		if (!hDLL)
			return NULL;
	}

	// Get the pointer to the patcher
	return (MPQDraftPatcherPtr)GetProcAddress(hDLL, "MPQDraftPatcher");
}

BEGIN_MESSAGE_MAP(CPatchWizard, CPropertySheet)
	//{{AFX_MSG_MAP(CPatchWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchWizard message handlers