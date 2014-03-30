/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#if !defined(AFX_SEMPQWIZARD_H__3679CE16_A05F_482B_A7FE_7EA31433F19B__INCLUDED_)
#define AFX_SEMPQWIZARD_H__3679CE16_A05F_482B_A7FE_7EA31433F19B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*	SEMPQWizard.h : header file
	This file describes the implementation of the MPQDraft SEMPQ wizard, which 
	allows users to create self-executing MPQs based on a specific patch 
	target, patch MPQ, and set of plugins. The first page is unique to the 
	SEMPQ wizard, due to required differences in the data gathered here, and 
	pages 2 and 3 are essentially the same as the corresponding pages in the 
	patch wizard. Those latter two pages are implemented as common base 
	classes, which customized, lightweight wrappers inherit from, here.
	Once all parameters are entered, the SEMPQ wizard displays a progress 
	dialog showing the progress of creation, and allowing cancellation. */

#include "CommonPages.h"

/////////////////////////////////////////////////////////////////////////////
/*	CSEMPQWizardPage1 dialog
	This page manages the majority of different pieces of information 
	required to create an SEMPQ - the name of the mod being packaged, the MPQ 
	to use, and the file name of the SEMPQ to create. From the very 
	beginning of MPQDraft I'd conceived of users being able to give custom 
	icons to their SEMPQs, with the mod logo or some such. At the time this 
	wasn't really feasible. Modifying an executable's icon post-build, while 
	possible, is not particularly easy; Windows NT has an API to accomplish 
	this, but at the time 9x systems were still very common, and MPQDraft had 
	to support them. However, now that MPQDraft no longer supports 9x...
	There isn't anything particularly complicated or difficult about this 
	page to explain. Just simple edit boxes and browse buttons. Apart from 
	registry stuff, about all the code here is in message handlers. */

class CSEMPQWizardPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CSEMPQWizardPage1)

// Construction
public:
	CSEMPQWizardPage1();
	~CSEMPQWizardPage1();

// Dialog Data
	//{{AFX_DATA(CSEMPQWizardPage1)
	enum { IDD = IDD_SEMPQWIZARD1 };
	// The name of this mod
	CEdit	m_ctlCustomName;
	// The path of the MPQ to create the SEMPQ from
	CEdit	m_ctlMPQName;
	// The path of the SEMPQ executable to create
	CEdit	m_ctlEXEName;
	// The selected icon path
	CEdit	m_ctlIconName;
	// The icon preview box
	CStatic	m_ctlIconDisplay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSEMPQWizardPage1)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Operations
public:
	// The usual
	BOOL SaveSettings();
	// LoadSettings must not be called before dialog initialization
	BOOL LoadSettings();

	// Basic accessor functions
	inline void GetCustomName(CString &strCustomName)
	{ m_ctlCustomName.GetWindowText(strCustomName); }
	inline void GetMPQFileName(CString &strFileName)
	{ m_ctlMPQName.GetWindowText(strFileName); }
	inline void GetSEMPQFileName(CString &strFileName)
	{ m_ctlEXEName.GetWindowText(strFileName); }
	inline void GetIconFileName(CString &strFileName)
	{ m_ctlIconName.GetWindowText(strFileName); }

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSEMPQWizardPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBrowseMPQ();
	afx_msg void OnBrowseEXE();
	afx_msg void OnBrowseIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
/*	CSEMPQWizardPage2 dialog
	Lightweight wrapper around CPatchTargetPage. All we have to handle here 
	is interfacing with the wizard itself. */

class CSEMPQWizardPage2 : public CPatchTargetPage
{
	DECLARE_DYNCREATE(CSEMPQWizardPage2)

// Construction
public:
	CSEMPQWizardPage2();
	~CSEMPQWizardPage2();

// Dialog Data
	//{{AFX_DATA(CSEMPQWizardPage2)
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSEMPQWizardPage2)
	public:
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// Operations
public:

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSEMPQWizardPage2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/*	CSEMPQWizardPage3 dialog
	Lightweight wrapper around CPluginPage to handle interfacing with the 
	wizard class. */

class CSEMPQWizardPage3 : public CPluginPage
{
	DECLARE_DYNCREATE(CSEMPQWizardPage3)

// Construction
public:
	CSEMPQWizardPage3();
	~CSEMPQWizardPage3();

// Dialog Data
	//{{AFX_DATA(CSEMPQWizardPage3)
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSEMPQWizardPage3)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	//}}AFX_VIRTUAL

// Operations
public:

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSEMPQWizardPage3)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/*	CSEMPQWizard
	The SEMPQ wizard itself. As the pages are mostly autonomous and 
	object-oriented, this page has no data, apart from the instances of the 
	pages themselves, and MFC takes care of all the property sheet dialog 
	details, it is composed entirely of code for actually creating the SEMPQ 
	itself, begun by the single public function CreateSEMPQ (which is called 
	by CSEMPQWizardPage3::OnWizardFinish). */

class CSEMPQWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CSEMPQWizard)

// Construction
public:
	CSEMPQWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSEMPQWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:
	// Creates the SEMPQ based on the parameters specified in the three pages
	BOOL CreateSEMPQ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEMPQWizard)
	//}}AFX_VIRTUAL

// Implementation
public:
	// The wizard pages
	CSEMPQWizardPage1 m_firstPage;
	CSEMPQWizardPage2 m_secondPage;
	CSEMPQWizardPage3 m_thirdPage;

	virtual ~CSEMPQWizard();

protected:
	// These constants define the range of progress bar values that can occur 
	// at each step of the SEMPQ creation. This is just a rough estimate 
	// of how long each section is expected to take. All are in %.
	enum
	{
		WRITE_STUB_INITIAL_PROGRESS = 0,
		WRITE_PLUGINS_INITIAL_PROGRESS = 5,
		WRITE_PLUGINS_PROGRESS_SIZE = 15,
		WRITE_MPQ_INITIAL_PROGRESS = 20,
		WRITE_MPQ_PROGRESS_SIZE = 80
	};

	// Generates the STUBDATA block that is to be written to the SEMPQ file, 
	// containing various things from the first and second pages, which are 
	// necessary to perform the patch. See Common.h for more information 
	// about the STUBDATA structure.
	STUBDATA *CreateStubData();
	// Frees the allocated STUBDATA structure. Must be used to free the data 
	// returned by CreateStubData.
	void FreeStubData(STUBDATA *lpStubData);

	// Gets the full list of modules that need to be written to the SEMPQ. 
	// This function calls CSEMPQWizardPage3::GetSelectedPluginModules, then 
	// adds on any modules required by MPQDraft itself.
	BOOL GetPluginModules(CPluginPage::PluginModuleList &modules);

	// The stub data, which includes basic parameters for the patch, is 
	// stored in a resource named "STUBDATA", of type "BIN". For historical 
	// reasons, overwriting this could not simply be done by writing to the 
	// module resources, as there was no easy way to do that in 9x. 
	// Consequently, we have to write directly to the SEMPQ, overwriting the 
	// stub data resource. Finding where in the EXE the resource is is the job
	// of this function. In the future this archaic method could be replaced 
	// by a version that uses the NT resource editing function; this would be 
	// especially appropriate if we also have to edit the resources to write 
	// an icon. Or heck, the stub data itself could be made into a plugin 
	// module, and taken out of the resources completely, as the motivation 
	// for using a resource (yes, there was one) is now gone.
	// A return value of 0 indicates failure.
	DWORD GetStubDataWriteOffset(const CString &strStubFileName);

	// Extracts the SEMPQ stub to the SEMPQ file name from the MPQDraft 
	// resources, and then updates the stub data block with the stub data 
	// produced by GetStubData.
	BOOL WriteStubToSEMPQ(CProgressWnd &wndProgress, 
		const CString &strEXEName, const STUBDATA &dataSEMPQ);

	// Appends the list of modules compiled by GetPluginModules to the end of 
	// the newly written SEMPQ (which is just the stub at this point). Updates
	// the progress dialog with the current progress, and watches for 
	// cancellation.
	BOOL WritePluginsToSEMPQ(CProgressWnd &wndProgress, 
		const CString &strEXEName, 
		const CPluginPage::PluginModuleList &modules);
	// Appends the selected MPQ to the end of the SEMPQ. Updates
	// the progress dialog with the current progress, and watches for 
	// cancellation.
	BOOL WriteMPQToSEMPQ(CProgressWnd &wndProgress, const CString &strEXEName, const CString &strMPQName);
	BOOL WriteMPQToSEMPQ(CProgressWnd &wndProgress, const CString &strMPQName, const CString &strEXEName, HANDLE hSEMPQ, HANDLE hMPQ);

// Generated message map functions
protected:
	//{{AFX_MSG(CSEMPQWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMPQWIZARD_H__3679CE16_A05F_482B_A7FE_7EA31433F19B__INCLUDED_)
