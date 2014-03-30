/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// SEMPQWizard.cpp : implementation file
//

#include "stdafx.h"
#include "MPQDraft.h"
#include "resource.h"
#include "ProgressWnd.h"
#include "SEMPQWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizardPage1 property page

IMPLEMENT_DYNCREATE(CSEMPQWizardPage1, CPropertyPage)

CSEMPQWizardPage1::CSEMPQWizardPage1() : CPropertyPage(CSEMPQWizardPage1::IDD)
{
	//{{AFX_DATA_INIT(CSEMPQWizardPage1)
	//}}AFX_DATA_INIT
}

CSEMPQWizardPage1::~CSEMPQWizardPage1()
{
}

void CSEMPQWizardPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEMPQWizardPage1)
	DDX_Control(pDX, IDC_SEMPQNAME, m_ctlCustomName);
	DDX_Control(pDX, IDC_MPQNAME, m_ctlMPQName);
	DDX_Control(pDX, IDC_EXENAME, m_ctlEXEName);
	DDX_Control(pDX, IDC_ICONNAME, m_ctlIconName);
	DDX_Control(pDX, IDC_ICONDISPLAY, m_ctlIconDisplay);
	//}}AFX_DATA_MAP
}

BOOL CSEMPQWizardPage1::OnSetActive() 
{
	// Set the wizard buttons to show
	CSEMPQWizard *pWizard = (CSEMPQWizard *)GetParent();
	pWizard->SetWizardButtons(PSWIZB_NEXT);
	pWizard->ModifyStyleEx(0, WS_EX_CONTEXTHELP);

	return CPropertyPage::OnSetActive();
}

BOOL CSEMPQWizardPage1::OnKillActive() 
{
	CString strMessage;

	if (!CPropertyPage::OnKillActive())
		return FALSE;

	// Verify each item. If one of them is missing, set the focus to it and 
	// warn the user. Need to get the MPQ file name here, because we need to 
	// check if it exists.
	CString strMPQName;
	m_ctlMPQName.GetWindowText(strMPQName);

	if (!m_ctlCustomName.GetWindowTextLength())
	{
		// No mod name
		strMessage.LoadString(IDS_NOSEMPQNAME);
		MessageBox(strMessage, NULL, MB_OK | MB_ICONEXCLAMATION);

		m_ctlCustomName.SetFocus();
		return FALSE;
	}
	else if (!strMPQName.GetLength() || !PathFileExists(strMPQName))
	{
		// No MPQ
		strMessage.LoadString(IDS_NOSEMPQMPQ);
		MessageBox(strMessage, NULL, MB_OK | MB_ICONEXCLAMATION);

		GetDlgItem(IDC_BROWSEMPQ)->SetFocus();
		return FALSE;
	}
	else if (!m_ctlEXEName.GetWindowTextLength())
	{
		// No output file name
		strMessage.LoadString(IDS_NOSEMPQFILENAME);
		MessageBox(strMessage, NULL, MB_OK | MB_ICONEXCLAMATION);

		GetDlgItem(IDC_BROWSEEXE)->SetFocus();
		return FALSE;
	}
	/*else if (!strIconName.GetLength())
	{
		// No icon. Check if they want to use the default icon.
		strMessage.LoadString(IDS_USEDEFSEMPQICON);
		if (MessageBox(strMessage, NULL, MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			GetDlgItem(IDC_BROWSEICON)->SetFocus;
			return FALSE;
		}
	}*/

	return TRUE;	// Looks good
}

BEGIN_MESSAGE_MAP(CSEMPQWizardPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CSEMPQWizardPage1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BROWSEMPQ, OnBrowseMPQ)
	ON_BN_CLICKED(IDC_BROWSEEXE, OnBrowseEXE)
	ON_BN_CLICKED(IDC_BROWSEICON, OnBrowseIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizardPage1 message handlers

BOOL CSEMPQWizardPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Set the limit on the length of the mod name, and load settings
	m_ctlCustomName.SetLimitText(31);
	m_ctlCustomName.SetFocus();

	LoadSettings();

	// FALSE because we set the focus
	return FALSE;
}

void CSEMPQWizardPage1::OnDestroy() 
{
	CPropertyPage::OnDestroy();

	// If there was an icon selected, we need to destroy it here
	HICON hIcon = m_ctlIconDisplay.GetIcon();
	if (hIcon)
		DestroyIcon(hIcon);
}

void CSEMPQWizardPage1::OnBrowseMPQ() 
{
	// Pick the default directory to use in the open dialog.
	char szMPQDir[MAX_PATH + 1];

	// If there's an MPQ selected already, derive the path from that. If that 
	// doesn't work, use the MPQDraft startup directory.
	if (m_ctlMPQName.GetWindowText(szMPQDir, MAX_PATH))
	{
		PathRemoveFileSpec(szMPQDir);

		if (!PathIsDirectory(szMPQDir))
			strcpy(szMPQDir, theApp.GetStartupPath());
	}
	else
		strcpy(szMPQDir, theApp.GetStartupPath());

	// Set up all the other parameters of the open dialog and run it
	CString strFilter, strTitle;
	strFilter.LoadString(IDS_OPENMPQSETTINGS);
	strTitle.LoadString(IDS_OPENSEMPQMPQ);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | 
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, 
		strFilter);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = szMPQDir;

	// Update the MPQ path if we have a new one
	if (dlg.DoModal() == IDOK)
		m_ctlMPQName.SetWindowText(dlg.GetPathName());
}

void CSEMPQWizardPage1::OnBrowseEXE() 
{
	// Get the initial directory from the previous path
	char szSEMPQDir[MAX_PATH + 1];
	if (m_ctlEXEName.GetWindowText(szSEMPQDir, MAX_PATH))
	{
		PathRemoveFileSpec(szSEMPQDir);

		if (!PathIsDirectory(szSEMPQDir))
			strcpy(szSEMPQDir, theApp.GetStartupPath());
	}
	else
		strcpy(szSEMPQDir, theApp.GetStartupPath());

	// Do the save dialog
	CString strDefExt, strFilter, strTitle;
	strDefExt.LoadString(IDS_DEFEXEEXT);
	strFilter.LoadString(IDS_OPENEXESETTINGS);
	strTitle.LoadString(IDS_SAVESEMPQ);

	CFileDialog dlg(FALSE, strDefExt, NULL, 
		OFN_EXPLORER | OFN_OVERWRITEPROMPT, strFilter);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = szSEMPQDir;

	// Update the EXE path if successful
	if (dlg.DoModal() == IDOK)
		m_ctlEXEName.SetWindowText(dlg.GetPathName());
}

void CSEMPQWizardPage1::OnBrowseIcon() 
{
	/*
	// One more time for good measure
	// Get the initial directory
	CString strIconDir;
	m_ctlIconName.GetWindowText(strIconDir);
	if (strIconDir.GetLength())
	{
		int nDirLength = strIconDir.ReverseFind('\\');
		if (nDirLength != -1)
			strIconDir = strIconDir.Left(nDirLength);

		if (!PathIsDirectory(strIconDir))
			strIconDir = theApp.GetStartupPath();
	}
	else
		strIconDir = theApp.GetStartupPath();

	// Do the open dialog
	CString strFilter, strTitle;
	strFilter.LoadString(IDS_OPENICONSETTINGS);
	strTitle.LoadString(IDS_SELECTSEMPQICON);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | 
		OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = strIconDir;

	if (dlg.DoModal() == IDOK)
	{
		// Fetch the icon to use using windows shell functions
		SHFILEINFO shfi;
		HICON hOldIcon;
		CString strIconName = dlg.GetPathName();

		m_ctlIconName.SetWindowText(dlg.GetPathName());

		ZeroMemory(&shfi, sizeof(SHFILEINFO));
		SHGetFileInfo(strIconName, 0, &shfi, sizeof(shfi), SHGFI_ICON  | SHGFI_LARGEICON);

		hOldIcon = m_ctlIconDisplay.SetIcon(shfi.hIcon);
		if (hOldIcon)
			DestroyIcon(hOldIcon);
	}
*/	
}

BOOL CSEMPQWizardPage1::SaveSettings()
{
	// Load all the value name strings
	CString strRegSEMPQHdr, strRegCustomName, strRegMPQPath, 
		strRegSEMPQPath, strRegIconPath;
	strRegSEMPQHdr.LoadString(IDS_SEMPQKEY);
	strRegCustomName.LoadString(IDS_SEMPQNAMEKEY);
	strRegMPQPath.LoadString(IDS_SRCPATHKEY);
	strRegSEMPQPath.LoadString(IDS_SEMPQPATHKEY);
	strRegIconPath.LoadString(IDS_ICONPATHKEY);

	CString strCustomName, strMPQPath, strSEMPQPath, strIconPath;

	// Save it all
	m_ctlCustomName.GetWindowText(strCustomName);
	m_ctlMPQName.GetWindowText(strMPQPath);
	m_ctlEXEName.GetWindowText(strSEMPQPath);
	m_ctlIconName.GetWindowText(strIconPath);

	theApp.WriteProfileString(strRegSEMPQHdr, strRegCustomName, strCustomName);
	theApp.WriteProfileString(strRegSEMPQHdr, strRegMPQPath, strMPQPath);
	theApp.WriteProfileString(strRegSEMPQHdr, strRegSEMPQPath, strSEMPQPath);
	//theApp.WriteProfileString(strRegSEMPQHdr, strRegIconPath, strIconPath);

	return TRUE;
}

BOOL CSEMPQWizardPage1::LoadSettings()
{
	// Load all the value name strings
	CString strRegSEMPQHdr, strRegCustomName, strRegMPQPath, 
		strRegSEMPQPath, strRegIconPath;
	strRegSEMPQHdr.LoadString(IDS_SEMPQKEY);
	strRegCustomName.LoadString(IDS_SEMPQNAMEKEY);
	strRegMPQPath.LoadString(IDS_SRCPATHKEY);
	strRegSEMPQPath.LoadString(IDS_SEMPQPATHKEY);
	strRegIconPath.LoadString(IDS_ICONPATHKEY);

	// Load the values
	CString strCustomName = theApp.GetProfileString(strRegSEMPQHdr, strRegCustomName),
		strMPQPath = theApp.GetProfileString(strRegSEMPQHdr, strRegMPQPath),
		strSEMPQPath = theApp.GetProfileString(strRegSEMPQHdr, strRegSEMPQPath),
		strIconPath = theApp.GetProfileString(strRegSEMPQHdr, strRegIconPath);

	// Do a sanity check on the values we got from the registry, then set them
	if (strCustomName.GetLength() < 32)
		m_ctlCustomName.SetWindowText(strCustomName);
	if (PathFileExists(strMPQPath))
		m_ctlMPQName.SetWindowText(strMPQPath);
	m_ctlEXEName.SetWindowText(strSEMPQPath);
	/*if (PathFileExists(strIconPath))
		m_ctlIconName = strIconPath;*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizardPage2 property page

IMPLEMENT_DYNCREATE(CSEMPQWizardPage2, CPatchTargetPage)

CSEMPQWizardPage2::CSEMPQWizardPage2()
	: CPatchTargetPage(TRUE, IDS_SEMPQWIZARD2TITLE, IDS_SEMPQWIZARD2INFO)
{
	//{{AFX_DATA_INIT(CSEMPQWizardPage2)
	//}}AFX_DATA_INIT
}

CSEMPQWizardPage2::~CSEMPQWizardPage2()
{
}

BOOL CSEMPQWizardPage2::OnSetActive() 
{
	// Set the buttons how we want them and remove the help button
	CSEMPQWizard *pWizard = (CSEMPQWizard *)GetParent();

	pWizard->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	pWizard->ModifyStyleEx(0, WS_EX_CONTEXTHELP);

	return CPropertyPage::OnSetActive();
}

BEGIN_MESSAGE_MAP(CSEMPQWizardPage2, CPatchTargetPage)
	//{{AFX_MSG_MAP(CSEMPQWizardPage2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizardPage2 message handlers

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizardPage3 property page

IMPLEMENT_DYNCREATE(CSEMPQWizardPage3, CPropertyPage)

CSEMPQWizardPage3::CSEMPQWizardPage3() 
	: CPluginPage(TRUE, IDS_SEMPQWIZARD3TITLE)
{
	//{{AFX_DATA_INIT(CSEMPQWizardPage3)
	//}}AFX_DATA_INIT
}

CSEMPQWizardPage3::~CSEMPQWizardPage3()
{
}

BEGIN_MESSAGE_MAP(CSEMPQWizardPage3, CPluginPage)
	//{{AFX_MSG_MAP(CSEMPQWizardPage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizardPage3 message handlers

BOOL CSEMPQWizardPage3::OnSetActive() 
{
	// Set the wizard buttons
	CSEMPQWizard *pWizard = (CSEMPQWizard *)GetParent();
	pWizard->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	
	// Set the patch target and list plugins if necessary
	if (SetPatchTarget(""))
		ListPlugins();

	return CPropertyPage::OnSetActive();
}

BOOL CSEMPQWizardPage3::OnWizardFinish() 
{
	// Verify that this page has valid data
	if (!OnKillActive())
		return FALSE;

	// Verify that all plugins are ready
	if (!PluginsReady())
		return FALSE;

	// Do it
	if (!((CSEMPQWizard *)GetParent())->CreateSEMPQ())
		return FALSE;

	return CPropertyPage::OnWizardFinish();
}

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizard

IMPLEMENT_DYNAMIC(CSEMPQWizard, CPropertySheet)

CSEMPQWizard::CSEMPQWizard(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_firstPage);
	AddPage(&m_secondPage);
	AddPage(&m_thirdPage);

	SetWizardMode();
}

CSEMPQWizard::CSEMPQWizard(LPCTSTR pszCaption, 
	CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_firstPage);
	AddPage(&m_secondPage);
	AddPage(&m_thirdPage);

	SetWizardMode();
}

CSEMPQWizard::~CSEMPQWizard()
{
}

BOOL CSEMPQWizard::CreateSEMPQ()
{
	// First of all, save the settings
	m_firstPage.SaveSettings();
	m_secondPage.SaveSettings();
	m_thirdPage.SaveSettings();

	CString strEXEName, strMPQName, strSEMPQError, strMessage;

	// Load the error string just in case
	strSEMPQError.LoadString(IDS_SEMPQFAILED);

	// Get the data from the first page
	//HICON hIcon = m_firstPage.m_ctlIconDisplay.GetIcon();
	/*if (!hIcon)
	{
		hIcon = theApp.LoadIcon(IDR_MAINFRAME);
		if (!hIcon)
		{
			strMessage.LoadString(IDS_CANTLOADDEFICON);
			MessageBox(strMessage, szSEMPQError, MB_OK | MB_ICONSTOP);

			return;
		}
	}*/

	m_firstPage.GetSEMPQFileName(strEXEName);
	m_firstPage.GetMPQFileName(strMPQName);

	// Verify the MPQ path is correct
	if (!PathFileExists(strMPQName))
	{
		strMessage.Format(IDS_BADMPQ, strMPQName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Create the progress dialog and set the basic settings
	CProgressWnd wndProgress;
	CString strSEMPQCaption;

	strSEMPQCaption.LoadString(IDS_SEMPQCAPTION);
	if (!wndProgress.Create(this, strSEMPQCaption))
	{
		strMessage.LoadString(IDS_WINDOWSERROR);
		MessageBox(strMessage, strSEMPQError, MB_ICONSTOP | MB_OK);

		return FALSE;
	}
	
	wndProgress.SetWindowSize(2);
	wndProgress.SetRange(0, 100);

	// Get the list of plugin modules to include (from the third page)
	CPluginPage::PluginModuleList modules;
	if (!GetPluginModules(modules))
	{
		// Failed to get them
		strMessage = strSEMPQError;
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Create the SEMPQ stub data (which loads info from the second page)
	STUBDATA *pDataSEMPQ = CreateStubData();
	if (!pDataSEMPQ)
		return FALSE;	// Something's weird with the first or second page

	// Now the real fun, in 3 calls
	BOOL bSuccess = (WriteStubToSEMPQ(wndProgress, strEXEName, *pDataSEMPQ)
		&& WritePluginsToSEMPQ(wndProgress, strEXEName, modules)
		&& WriteMPQToSEMPQ(wndProgress, strEXEName, strMPQName));

	::MessageBeep(MB_ICONEXCLAMATION);
	if (!bSuccess)
		::DeleteFile(strEXEName);

	FreeStubData(pDataSEMPQ);

	return bSuccess;
}

DWORD CSEMPQWizard::GetStubDataWriteOffset(const CString &strStubFileName)
{
	// Manually finding the address of the stub data in an executable is to 
	// be avoided, where possible. Fortunately, there's a clever way to get 
	// this information through Windows: the LOAD_LIBRARY_AS_DATAFILE flag.
	// When this flag is used to load a module, the file is mapped into 
	// memory as a single chunk, in exactly the same format as the file 
	// itself. Some Windows functions (including the resource functions) are 
	// able to operate on a file loaded in this way. In this case, the RVA of 
	// the resource corresponds exactly to the file offset.
	ASSERT(strStubFileName.GetLength());

	// Open the stub binary
	HMODULE hStub = LoadLibraryEx(strStubFileName, 
		NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (!hStub)
		return 0;

	DWORD dwRetVal = 0;

	// The three steps to getting a resource pointer: find, load, and lock
	HRSRC hStubData = FindResource(hStub, "STUBDATA", "BIN");
	if (hStubData)
	{
		HGLOBAL hStubDataGlobal = LoadResource(hStub, hStubData);
		if (hStubDataGlobal)
		{
			LPVOID lpvStubData = LockResource(hStubDataGlobal);
			DWORD dwStubDataSize = SizeofResource(hStub, hStubData);

			if (lpvStubData && dwStubDataSize >= STUBDATASIZE)
				// Got it
				// Note that in Windows modules always begin at 64 KB offsets,
				// which leaves these 16 bits in the HMODULE free for Windows 
				// to use for flags (though I only know of one bit that is 
				// used in that way); consequently, we must filter out these 
				// flags before we subtract.
				dwRetVal = (DWORD)((UINT_PTR)lpvStubData 
					- ((UINT_PTR)hStub & ~(UINT_PTR)0xFFFF));
		}
	}

	FreeLibrary(hStub);

	return dwRetVal;
}

BOOL CSEMPQWizard::WriteStubToSEMPQ(CProgressWnd &wndProgress, 
	const CString &strEXEName, const STUBDATA &dataSEMPQ)
{
	// Load the strings we'll need
	CString strSEMPQError, strMessage;
	strSEMPQError.LoadString(IDS_SEMPQFAILED);
	strMessage.LoadString(IDS_WRITINGEXE);

	wndProgress.SetText(strMessage);
	wndProgress.SetPos(WRITE_STUB_INITIAL_PROGRESS);

	// We've got a couple tasks to do here. First, we need to create the SEMPQ
	// file and write the unmodified version of the stub.
	if (!ExtractResource(NULL, MAKEINTRESOURCE(IDR_SEMPQSTUB), 
		"EXE", strEXEName))
	{
		strMessage.Format(IDS_CANTCREATEFILE, strEXEName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Next, create the new stub data that contains the info for our mod
	DWORD dwStubDataOffset = GetStubDataWriteOffset(strEXEName);
	if (!dwStubDataOffset)
	{
		strMessage.Format(IDS_INTERNALERROR);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Open the file, and...
	HANDLE hSEMPQ = ::CreateFile(strEXEName, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (!hSEMPQ)
	{				
		strMessage.Format(IDS_CANTOPENFILE, strEXEName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Write the stub data
	BOOL bRetVal = FALSE;
	DWORD dwBytesWritten;
	if (::SetFilePointer(hSEMPQ, dwStubDataOffset, NULL, FILE_BEGIN) == dwStubDataOffset
		&& ::WriteFile(hSEMPQ, &dataSEMPQ, dataSEMPQ.cbSize, &dwBytesWritten, NULL)
		&& (dwBytesWritten == dataSEMPQ.cbSize))
		bRetVal = TRUE;	// Success
	else
	{
		strMessage.Format(IDS_CANTWRITE, strEXEName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);
	}

	::CloseHandle(hSEMPQ);

	return bRetVal;
}

BOOL CSEMPQWizard::WritePluginsToSEMPQ(CProgressWnd &wndProgress, 
	const CString &strEXEName, const CPluginPage::PluginModuleList &modules)
{
	// Load the strings and set the text/progress of the dialog
	CString strSEMPQError, strMessage;
	strSEMPQError.LoadString(IDS_SEMPQFAILED);
	strMessage.LoadString(IDS_WRITINGPLUGINS);

	wndProgress.SetText(strMessage);
	wndProgress.SetPos(WRITE_PLUGINS_INITIAL_PROGRESS);

	// This is pretty straightforward: open, write the modules, close
	EFSHANDLEFORWRITE hEFSFile = OpenEFSFileForWrite(strEXEName, 0);
	if (!hEFSFile)
		return FALSE;

	BOOL bRetVal = FALSE, bCancel = FALSE;
	int iCurModule;
	for (iCurModule = 0; iCurModule < modules.GetCount(); iCurModule++)
	{
		const MPQDRAFTPLUGINMODULE &module = modules[iCurModule];
		if (!AddToEFSFile(hEFSFile, module.szModuleFileName, 
			module.dwComponentID,
			module.dwModuleID, 
			module.bExecute, 0))
			break;

		// Update the progress bar
		wndProgress.SetPos((int)(((float)iCurModule 
			* (float)WRITE_PLUGINS_PROGRESS_SIZE / (float)modules.GetCount()) 
			+ (float)WRITE_PLUGINS_INITIAL_PROGRESS));

		// Now, this is a bit different from the function to write the stub. 
		// Because both this step and the next have the potential to take a 
		// while (imagine an assload of plugin modules), we need to dispatch 
		// messages, or the UI is going to be completely unresponsive, 
		// including the cancel button. Pump messages and check the cancel 
		// button in between files, here.
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled())
		{
			bCancel = TRUE;
			break;	// Abort
		}
	}

	// Success is whether we were able to write all modules
	if (iCurModule == modules.GetCount())
		bRetVal = TRUE;
	else if (!bCancel)
	{
		strMessage.Format(IDS_CANTWRITE, strEXEName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);
	}
	// Else bRetVal remains FALSE, in the case of cancellation

	CloseEFSFileForWrite(hEFSFile);

	return bRetVal;
}

BOOL CSEMPQWizard::WriteMPQToSEMPQ(CProgressWnd &wndProgress, 
	const CString &strEXEName, const CString &strMPQName)
{
	CString strSEMPQError, strMessage;
	strSEMPQError.LoadString(IDS_SEMPQFAILED);

	// This is a trivial function that simply opens both files and passes 
	// things on to the function that actually does the copying. There is no 
	// strategic reason for this division, only an aesthetic one: the code 
	// becomes really ugly if we do all of this in one function.
	HANDLE hSEMPQ = ::CreateFile(strEXEName, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hSEMPQ == INVALID_HANDLE_VALUE)
	{
		strMessage.Format(IDS_CANTCREATEFILE, strEXEName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	BOOL bRetVal = FALSE;
	HANDLE hMPQ = ::CreateFile(strMPQName, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hMPQ != INVALID_HANDLE_VALUE)
	{
		bRetVal = WriteMPQToSEMPQ(wndProgress, strMPQName, 
			strEXEName, hSEMPQ, hMPQ);

		::CloseHandle(hMPQ);
	}
	else
	{
		strMessage.Format(IDS_CANTOPENMPQ, strMPQName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);
	}

	::CloseHandle(hSEMPQ);

	return bRetVal;
}

BOOL CSEMPQWizard::WriteMPQToSEMPQ(CProgressWnd &wndProgress, 
	const CString &strMPQName, const CString &strEXEName, 
	HANDLE hSEMPQ, HANDLE hMPQ)
{
	ASSERT(hSEMPQ != INVALID_HANDLE_VALUE);
	ASSERT(hMPQ != INVALID_HANDLE_VALUE);

	// Load the strings and set up the progress dialog
	CString strSEMPQError, strMessage;
	strSEMPQError.LoadString(IDS_SEMPQFAILED);
	strMessage.LoadString(IDS_WRITINGMPQ);

	wndProgress.SetText(strMessage);
	wndProgress.SetPos(WRITE_MPQ_INITIAL_PROGRESS);

	DWORD dwMPQOffset = ::GetFileSize(hSEMPQ, NULL),
		dwMPQSize = ::GetFileSize(hMPQ, NULL), 
		dwRemaining = dwMPQSize, dwTransferred = 0;

	// Storm searches for MPQs in a file one sector (512 bytes) at a time, so 
	// our archive must be written on a sector boundary. Under anything but 
	// FUBAR conditions, this condition should automatically be met, as 
	// executables must have sizes that are multiples of either 512 or 4096 
	// bytes, and the EFS code is also smart enough to ensure this.
	ASSERT((dwMPQOffset % 512) == 0);

	// 96 is the size of an empty MPQ with a 4-entry hash table (I can't 
	// recall if the minimum hash table size is 4 or 16, off the top of my 
	// head.
	if (dwMPQSize < 96)
	{
		strMessage.Format(IDS_BADMPQ, strMPQName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// Allocate the read buffer
	const DWORD dwMaxBufferSize = 256 * 1024;
	DWORD dwBufferSize = min(dwRemaining, dwMaxBufferSize);

	LPBYTE lpbyReadBuffer = NULL;
	try
	{ lpbyReadBuffer = new BYTE[dwBufferSize]; }
	catch (...)
	{ }

	if (!lpbyReadBuffer)
	{
		strMessage.Format(IDS_CANTMALLOC, dwBufferSize);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	// This is a very simple pump in, pump out function: we read in a buffer 
	// full from the MPQ, write it out to the SEMPQ.
	::SetFilePointer(hSEMPQ, 0, NULL, FILE_END);
	::SetFilePointer(hMPQ, 0, NULL, FILE_BEGIN);

	BOOL bRetVal = FALSE, bCancel = FALSE;
	while (dwRemaining)
	{
		DWORD dwBlockSize = min(dwRemaining, dwBufferSize), dwBytesRead;

		// In and out
		if (!::ReadFile(hMPQ, lpbyReadBuffer, dwBlockSize, &dwBytesRead, NULL)
			|| (dwBytesRead < dwBlockSize)
			|| !::WriteFile(hSEMPQ, lpbyReadBuffer, dwBlockSize, &dwBytesRead, NULL)
			|| (dwBytesRead < dwBlockSize))
			break;

		// Same as before: make sure we process UI messages. We have to do 
		// this before modifying the counters, because we rely on the counters
		// to determine whether we completed successfully.
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled())
		{
			bCancel = TRUE;
			break;
		}

		dwTransferred += dwBlockSize;
		dwRemaining -= dwBlockSize;

		// Finally, update the progress bar
		wndProgress.SetPos((int)(((float)dwTransferred 
			* WRITE_MPQ_PROGRESS_SIZE 
			/ (float)dwMPQSize) + WRITE_MPQ_INITIAL_PROGRESS));
	}

	// Did we finish, or was there an error?
	if (!dwRemaining)
	{
		::SetEndOfFile(hSEMPQ);

		bRetVal = TRUE;
	}
	else if (!bCancel)
	{
		strMessage.Format(IDS_CANTWRITE, strEXEName);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);
	}
	// Else the user cancelled

	delete [] lpbyReadBuffer;

	return bRetVal;
}

STUBDATA *CSEMPQWizard::CreateStubData()
{
	CString strCustomName, strProgramPath, strParameters, 
		strSEMPQError, strMessage;
	strSEMPQError.LoadString(IDS_SEMPQFAILED);

	// Get the first page info we need
	m_firstPage.GetCustomName(strCustomName);

	// Get the second page info we need
	const PROGRAMENTRY *lpProgram = m_secondPage.GetSelectedProgram();
	const PROGRAMFILEENTRY *lpFile = m_secondPage.GetSelectedFile();

	m_secondPage.GetProgramPath(strProgramPath);
	m_secondPage.GetParameters(strParameters);

	// Compile the flags (trivial, as we have only one flag ATM)
	DWORD dwFlags = 0;
	if (m_secondPage.RedirOpenFileEx())
		dwFlags |= MPQD_EXTENDED_REDIR;

	// Compute the size for the stub data. We need this for both verifying 
	// that it will all fit, and so we can get all the offsets correct.
	DWORD cbRegKeyName = 0, cbRegValueName = 0, 
		cbTargetPathName = 0, cbTargetFileName = 0,
		cbSpawnFileName = 0, cbArgs = strParameters.GetLength() + 1;
	char szTargetPath[MAX_PATH];
	LPCSTR lpszTargetFileName = NULL;

	// Are we using a supported app, or a custom one?
	if (lpProgram && lpFile)
	{
		// A built-in one
		cbRegKeyName = strlen(lpProgram->szRegistryKey) + 1;
		cbRegValueName = strlen(lpProgram->szRegistryValue) + 1;

		cbTargetFileName = strlen(lpFile->szTargetFileName) + 1;
		cbSpawnFileName = strlen(lpFile->szFileName) + 1;
	}
	else
	{
		// Custom one
		// Create the target path
		strcpy(szTargetPath, strProgramPath);
		PathRemoveFileSpec(szTargetPath);
		lpszTargetFileName = PathFindFileName(strProgramPath);

		cbTargetPathName = strlen(szTargetPath) + 1;
		cbTargetFileName = cbSpawnFileName = strlen(lpszTargetFileName) + 1;
	}

	// Compute the offsets of the strings
	DWORD nRegKeyOffset = sizeof(PATCHTARGETEX),
		nRegValueOffset = nRegKeyOffset + cbRegKeyName,
		nTargetPathOffset = nRegValueOffset + cbRegValueName,
		nTargetFileOffset = nTargetPathOffset + cbTargetPathName,
		nSpawnFileOffset = nTargetFileOffset + cbTargetFileName,
		nArgsOffset = nSpawnFileOffset + cbSpawnFileName,
		nStubSize = sizeof(STUBDATA) + nArgsOffset + cbArgs - nRegKeyOffset;

	// It shouldn't be possible for this to happen, but who knows
	ASSERT(nStubSize <= STUBDATASIZE);

	// Allocate space for all the data
	STUBDATA *pDataSEMPQ = (STUBDATA *)new BYTE[nStubSize];
	if (!pDataSEMPQ)
	{
		strMessage.Format(IDS_CANTMALLOC, nStubSize);
		MessageBox(strMessage, strSEMPQError, MB_OK | MB_ICONSTOP);

		return NULL;
	}

	// Set up the basic stub data fields
	pDataSEMPQ->dwDummy = ::GetTickCount();
	pDataSEMPQ->cbSize = nStubSize;
	pDataSEMPQ->patchTarget.grfFlags = dwFlags;
	strcpy(pDataSEMPQ->szCustomName, strCustomName);

	// Set up the string pointers for the patch target
	PATCHTARGETEX &patchTarget = pDataSEMPQ->patchTarget;

	patchTarget.lpszRegistryKey = (LPCSTR)nRegKeyOffset;
	patchTarget.lpszRegistryValue = (LPCSTR)nRegValueOffset;

	patchTarget.lpszTargetPath = (LPCSTR)nTargetPathOffset;
	patchTarget.lpszTargetFileName = (LPCSTR)nTargetFileOffset;
	patchTarget.lpszSpawnFileName = (LPCSTR)nSpawnFileOffset;

	patchTarget.lpszArguments = (LPCSTR)nArgsOffset;

	// Actually create the patch target strings and other data
	if (lpProgram && lpFile)
	{
		pDataSEMPQ->patchTarget.bUseRegistry = TRUE;

		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nRegKeyOffset, 
			lpProgram->szRegistryKey);
		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nRegValueOffset, 
			lpProgram->szRegistryValue);
		pDataSEMPQ->patchTarget.bValueIsFileName = FALSE;

		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nSpawnFileOffset, 
			lpFile->szFileName);
		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nTargetFileOffset, 
			lpFile->szTargetFileName);

		pDataSEMPQ->patchTarget.nShuntCount = lpFile->nShuntCount;
	}
	else
	{
		pDataSEMPQ->patchTarget.bUseRegistry = FALSE;

		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nTargetPathOffset, 
			szTargetPath);

		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nTargetFileOffset, 
			lpszTargetFileName);
		strcpy((LPSTR)&pDataSEMPQ->patchTarget + nSpawnFileOffset, 
			lpszTargetFileName);

		pDataSEMPQ->patchTarget.nShuntCount = 0;
	}

	strcpy((LPSTR)&pDataSEMPQ->patchTarget + nArgsOffset, strParameters);

	return pDataSEMPQ;
}

void CSEMPQWizard::FreeStubData(STUBDATA *lpStubData)
{
	ASSERT(lpStubData);

	delete [] (LPBYTE)lpStubData;
}

BOOL CSEMPQWizard::GetPluginModules(CPluginPage::PluginModuleList &modules)
{
	// This is pretty straightforward, as most of the work is done by 
	// the plugin page. All we need to do is add the patcher DLL to the list 
	// so that it will be packed into the SEMPQ.
	modules.RemoveAll();

	// First, get the plugin modules from the plugin page
	if (!m_thirdPage.GetSelectedPluginsAndModules(NULL, modules))
		return FALSE;	// Seems like a plugin wasn't ready

	// Get patcher DLL path
	CString *lpstrPatcherDLL = theApp.GetPatcherDLLPath();
	if (!lpstrPatcherDLL)
		return FALSE;

	// And add it to the list
	MPQDRAFTPLUGINMODULE dllModule;
	dllModule.dwComponentID = MPQDRAFT_COMPONENT;
	dllModule.dwModuleID = MPQDRAFTDLL_MODULE;
	dllModule.bExecute = FALSE;
	strcpy(dllModule.szModuleFileName, *lpstrPatcherDLL);

	modules.Add(dllModule);

	// Done
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSEMPQWizard, CPropertySheet)
	//{{AFX_MSG_MAP(CSEMPQWizard)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMPQWizard message handlers
