/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// MainMenu.h : header file
//

#if !defined(AFX_MAINMENU_H__E8081E01_B9C0_4617_B7BE_0C9B1E0D548E__INCLUDED_)
#define AFX_MAINMENU_H__E8081E01_B9C0_4617_B7BE_0C9B1E0D548E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BitmapDialog.h"

/////////////////////////////////////////////////////////////////////////////
/*	CMainMenu dialog
	The main menu dialog. Simply sets up/draws the graphics for the dialog, 
	and responds to user input. Note that because we're implementing the close
	and minimize buttons ourselves (as opposed to drawing our own title bar), 
	we have to have handlers for them as well. */

class CMainMenu : public CBitmapDialog
{
// Construction
public:
	CMainMenu(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMainMenu)
	enum { IDD = IDD_MAINMENU };
	CBitmapButton	m_ctlMinButton;
	CBitmapButton	m_ctlCloseButton;
	CBitmapButton	m_ctlPatchButton;
	CBitmapButton	m_ctlSEMPQButton;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainMenu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// The main menu icon for the task bar
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMainMenu)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPatch();
	afx_msg void OnSEMPQ();
	afx_msg void OnMinimize();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINMENU_H__E8081E01_B9C0_4617_B7BE_0C9B1E0D548E__INCLUDED_)
