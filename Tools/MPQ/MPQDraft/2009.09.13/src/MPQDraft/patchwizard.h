/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#if !defined(AFX_PATCHWIZARD_H__A9254BF5_26AE_4CD0_932B_C3399E3742DB__INCLUDED_)
#define AFX_PATCHWIZARD_H__A9254BF5_26AE_4CD0_932B_C3399E3742DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatchWizard.h : header file
//

#include "CommonPages.h"

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/*	CPatchWizardPage1 dialog
	This is a very lightweight wrapper around the CPatchTarget page; the only 
	things that must be done here are those that could not be done in the 
	base class due to needing to access the patch wizard itself: namely, the 
	OnSetActive and OnKillActive functions. */

class CPatchWizardPage1 : public CPatchTargetPage
{
	DECLARE_DYNCREATE(CPatchWizardPage1)

// Construction
public:
	CPatchWizardPage1();
	~CPatchWizardPage1();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPatchWizardPage1)
	public:
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPatchWizardPage1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/*	CPatchWizardPage2 dialog
	This page allows selection of zero or more MPQs to use in the patching
	operation. As the nature of the patching method MPQDraft uses allows MPQs 
	with higher priority to mask MPQs with lower priority, this page must not 
	only allow the user to manipulate which MPQs to use, but also their 
	prioritization.
	Initially, all MPQs in the current MPQ directory are listed by filename, 
	in sorted order. Any additional MPQs added through Browse are added to the
	end of the list and their full path is shown (they are also checked).
	For ease of coding (i.e. laziness) with respect to the ordering of MPQs 
	selection states, and MPQs in the current directory, the implementation 
	does not attempt to save most of the information on this page to the 
	registry. It merely saves the current MPQ directory. */

class CPatchWizardPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPatchWizardPage2)

public:
	// The list of selected MPQs returned to the patching wizard by
	// GetSelectedMPQs. This list must be explicitely freed by calling 
	// FreeSelectedMPQs.
	typedef CArray<LPCSTR> MPQPathList;

// Construction
public:
	CPatchWizardPage2();
	~CPatchWizardPage2();

// Dialog Data
	//{{AFX_DATA(CPatchWizardPage2)
	enum { IDD = IDD_PATCHWIZARD2 };
	CListCtrl	m_ctlPatchMPQs;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPatchWizardPage2)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Operations
public:
	BOOL SaveSettings();
	// Must not be called before the dialog is initialized
	BOOL LoadSettings();

	// Functions for the wizard to retrieve and free the list of selected MPQs
	// from the page, in a format most useful for calling the patcher.
	BOOL GetSelectedMPQs(MPQPathList &MPQPaths);
	void FreeSelectedMPQs(MPQPathList &MPQPaths);

// Implementation
protected:
	// The map of MPQ path strings to the MPQ's selection state. The state 
	// should always be in sync with the check state in the list view, save 
	// for the case of when an MPQ is just checked/unchecked. The is necessary
	// because I don't know whether the way Windows implements list view check
	// boxes (using the state image index) is documented; if it's not 
	// documented, this is the only way we can tell that what is changing is 
	// the check state.
	typedef CMap<CString, CString &, BOOL, BOOL> MPQListMap;

	// The current MPQ search directory. Affects the open archive dialog.
	CString m_strMPQDir;
	// The MPQ search directory when the wizard is first opened. This is 
	// needed because we hide the full pathnames of MPQs that are in this 
	// directory, and the current MPQ directory can change.
	CString m_strInitialMPQDir;
	// The set of MPQs to choose from
	MPQListMap m_mapMPQs;

	BOOL m_bUpdatingList;

	// Finds all MPQs in the current MPQ directory and add them to the MPQ 
	// list and list view.
	BOOL ListDirectoryMPQs();
	// Finds all MPQs in the current directory matching the file spec and adds
	// them to the list and list view.
	BOOL ListMPQsBySpec(LPCSTR lpszFileSpec);

	// Adds a single MPQ to the list and list view. If the MPQ already exists,
	// it is selected and highlighted in the list view.
	BOOL AddMPQ(LPCSTR lpszFileName, LPBOOL lpbSelected);
	// Moves an MPQ in the list view. Both indices are positions compared to 
	// the current order. It is permissible to try to move an MPQ past the end
	// of the list (this simply moves it to the end), but attempts to move an 
	// MPQ before the start of the list (iTo < 0) will fail.
	BOOL MoveMPQ(int iFrom, int iTo);

	// Comparison function used to sort the list view.
	static int CALLBACK CompareMPQs(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	// Generated message map functions
	//{{AFX_MSG(CPatchWizardPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnItemChangedMPQs(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
/*	CPatchWizardPage3 dialog
	A lightweight wrapper around CPluginPage. As with CPatchWizardPage1, the 
	only functions we have to implement here are the ones that directly 
	access the patching wizard. */

class CPatchWizardPage3 : public CPluginPage
{
	DECLARE_DYNCREATE(CPatchWizardPage3)

// Construction
public:
	CPatchWizardPage3();
	~CPatchWizardPage3();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPatchWizardPage3)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPatchWizardPage3)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/*	CPatchWizard
	The patch wizard property sheet/wizard. Contains the three 
	CPatchWizardPages. As all data is contained in the page classes 
	themselves, this class has no data of its own. And because the patching 
	process is so simple, only a handful of functions are needed. */

class CPatchWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CPatchWizard)

// Construction
public:
	CPatchWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPatchWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchWizard)
	//}}AFX_VIRTUAL

// Operations
public:
	// Does most required setup and calls MPQDraftPatcher to perform the patch
	BOOL DoPatch();

// Implementation
public:
	// The wizard pages
	CPatchWizardPage1 m_firstPage;
	CPatchWizardPage2 m_secondPage;
	CPatchWizardPage3 m_thirdPage;

	virtual ~CPatchWizard();

protected:
	// If necessary, extracts and loads the patcher DLL, and find the 
	// patching function.
	MPQDraftPatcherPtr GetPatcher();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPatchWizard)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHWIZARD_H__A9254BF5_26AE_4CD0_932B_C3399E3742DB__INCLUDED_)
