/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// MainMenu.cpp : implementation file
//

#include "stdafx.h"
#include "MPQDraft.h"
#include "resource.h"
#include "MainMenu.h"
#include "PatchWizard.h"
#include "ProgressWnd.h"
#include "SEMPQWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainMenu dialog

CMainMenu::CMainMenu(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CMainMenu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainMenu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// Load the dialog icon for display in the taskbar
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainMenu::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainMenu)
	DDX_Control(pDX, IDC_MINIMIZE, m_ctlMinButton);
	DDX_Control(pDX, IDC_CLOSE, m_ctlCloseButton);
	DDX_Control(pDX, IDC_LOADMPQS, m_ctlPatchButton);
	DDX_Control(pDX, IDC_CREATESEMPQ, m_ctlSEMPQButton);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainMenu, CBitmapDialog)
	//{{AFX_MSG_MAP(CMainMenu)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_LOADMPQS, OnPatch)
	ON_BN_CLICKED(IDC_CREATESEMPQ, OnSEMPQ)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_MINIMIZE, OnMinimize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainMenu message handlers

BOOL CMainMenu::OnInitDialog()
{
	CBitmapDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	ModifyStyle(0, WS_SYSMENU);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set up the window properties, and sizes for the dialog and buttons. 
	// We set this up manually here, because we need pixel accuracy to ensure 
	// the bitmaps line up properly.
	SetWindowText(AfxGetAppName());
	SetWindowPos(NULL, 0, 0, 420, 273, SWP_NOZORDER | SWP_NOMOVE);

	m_ctlSEMPQButton.SetWindowPos(NULL, 18, 226, 162, 33, SWP_NOZORDER);
	m_ctlPatchButton.SetWindowPos(NULL, 240, 226, 162, 33, SWP_NOZORDER);
	m_ctlMinButton.SetWindowPos(NULL, 385, 6, 12, 12, SWP_NOZORDER);
	m_ctlCloseButton.SetWindowPos(NULL, 400, 6, 12, 12, SWP_NOZORDER);

	// Load the bitmaps
	SetBitmap(IDB_MAINMENU, CBitmapDialog::BITMAP_STRETCH);

	m_ctlSEMPQButton.LoadBitmaps(IDB_SEMPQBUTTONUP, IDB_SEMPQBUTTONDOWN, IDB_SEMPQBUTTONDOWN);
	m_ctlPatchButton.LoadBitmaps(IDB_PATCHBUTTONUP, IDB_PATCHBUTTONDOWN, IDB_PATCHBUTTONDOWN);

	m_ctlMinButton.LoadBitmaps(IDB_MINBUTTONUP, IDB_MINBUTTONDOWN, IDB_MINBUTTONDOWN);
	m_ctlCloseButton.LoadBitmaps(IDB_XBUTTONUP, IDB_XBUTTONDOWN, IDB_XBUTTONDOWN);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainMenu::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBitmapDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainMenu::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CMainMenu::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBitmapDialog::OnLButtonDown(nFlags, point);

	// Translate it into a title bar click, to allow whole window dragging
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

void CMainMenu::OnOK()
{
	// This handles the enter key. Find the button that currently has focus 
	// and click it. Dunno if there's a better way to do this.
	CWnd *lpFocusButton = GetFocus();
	if (!lpFocusButton)
		return;

	// Get the control ID and HWND needed to send the notification message
	DWORD nButtonID = lpFocusButton->GetDlgCtrlID();
	HWND hButton;
	GetDlgItem(nButtonID, &hButton);
	if (!hButton)
		return;	// Not a window

	// Send the button clicked notification to ourselves and let MFC call the 
	// proper click handler.
	SendMessage(WM_COMMAND, MAKEWPARAM(nButtonID, BN_CLICKED), (LPARAM)hButton);
}

void CMainMenu::OnPatch() 
{
	// Run the patch wizard
	CPatchWizard wizard(IDS_PATCHWIZARD, this);
	wizard.DoModal();

	SetForegroundWindow();
}

void CMainMenu::OnSEMPQ() 
{
	// Run the SEMPQ wizard
	CSEMPQWizard wizard(IDS_SEMPQWIZARD, this);
	wizard.DoModal();

	SetForegroundWindow();
}

void CMainMenu::OnMinimize() 
{
	ShowWindow(SW_MINIMIZE);
}

void CMainMenu::OnClose() 
{
	OnCancel();
}
