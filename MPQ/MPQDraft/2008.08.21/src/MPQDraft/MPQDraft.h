/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

// MPQDraft.h : main header file for the MPQDRAFT application
//

#if !defined(AFX_MPQDRAFT_H__E554FE73_9CB1_4CFC_8AAF_6299644E02A9__INCLUDED_)
#define AFX_MPQDRAFT_H__E554FE73_9CB1_4CFC_8AAF_6299644E02A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Common.h"

// A couple of functions to derive hash keys from CStrings. No idea why these 
// functions aren't built into MFC.
template<> inline UINT AFXAPI HashKey<CString>(CString key)
{ return HashKey((LPCSTR)key); }
template<> inline UINT AFXAPI HashKey<CString &>(CString &key)
{ return HashKey((LPCSTR)key); }

// Creates a theoretically unique canonical representation for a file path. 
// Ideally, this function can take all possible path representations for a 
// particular file and produce a single unique path for that file.
void SanitizeFilePath(LPCSTR lpszFilePath, CString &strSanitizedFilePath);

// The structures for the two-tier supported program hierarchy. This 
// structure is for individual patch target components.
struct PROGRAMFILEENTRY
{
	// The name displayed in the list view
	LPCSTR szComponentName;

	// The name of the file to launch, located in the game's install directory
	LPCSTR szFileName;
	// The patch target filename, in the game's install directory
	LPCSTR szTargetFileName;
	// The shunt count. See MPQDraftPatcher for more informatin.
	int nShuntCount;

	// The icon resource ID for the list view entry
	DWORD iIcon;

	BOOL bExtendRedir;	// MPQD_EXTENDED_REDIR
	DWORD dwFlags;
};

// And this one is for installed programs, which have one or more supported 
// patch targets.
struct PROGRAMENTRY
{
	// The display name of the program
	LPCSTR szProgramName;

	// The registry key which contains the registry value
	LPCSTR szRegistryKey;
	// The value name which holds the game's install path
	LPCSTR szRegistryValue;

	// The game's icon resource ID
	DWORD iIcon;

	// The list of components. This list is terminated by an entry containing 
	// a NULL szComponentName.
	const PROGRAMFILEENTRY *files;
};

// The list itself
extern const PROGRAMENTRY SupportApps[];

/////////////////////////////////////////////////////////////////////////////
/*	CMPQDraft
	The class representing the MPQDraft application itself. In addition to 
	this role, this class also holds various global data and functions shared 
	by many components of MPQDraft. */

class CMPQDraft : public CWinApp
{
public:
	CMPQDraft();
	~CMPQDraft();

	// Icon list related functions
	inline CImageList &GetLargeIcons()
	{ return m_imglLargeIcons; }
	inline CImageList &GetSmallIcons()
	{ return m_imglSmallIcons; }
	// Gets the entry in the icon image list for a given icon resource ID
	inline int GetIconIndex(DWORD iIcon)
	{
		int index = 0;
		VERIFY(m_mapIconIds.Lookup(iIcon, index));
		return index;
	}

	// Gets the initial startup directory
	inline const CString &GetStartupPath()
	{ return m_strStartupPath; }

	// Extracts the patcher DLL and returns the path to it
	CString *GetPatcherDLLPath();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPQDraft)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Maps an icon's resource ID to its index in the icon image list
	typedef CMap<DWORD, DWORD, int, int> TIconIdMap;

	// Icons not used by programs or components that should be loaded into the
	// icon image list.
	static const DWORD OtherIconsToLoad[];

	CString m_strStartupPath;

	// The two icon image lists for list views and things. They both contain 
	// the same images at the same indices.
	CImageList m_imglLargeIcons;
	CImageList m_imglSmallIcons;
	// Maps icon resource IDs to indices in the image lists
	TIconIdMap m_mapIconIds;

	// The file path of the extracted patcher DLL. Empty if it hasn't been
	// extracted yet.
	CString m_strPatcherPath;

	// Loads all icons from the supported program and file lists, and those 
	// listed in OtherIconsToLoad, into the image lists
	BOOL CreateImageLists();
	// Load a single icon resource into the image lists
	BOOL AddImageListIcon(DWORD iIcon);

	//{{AFX_MSG(CMPQDraft)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
extern CMPQDraft theApp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPQDRAFT_H__E554FE73_9CB1_4CFC_8AAF_6299644E02A9__INCLUDED_)
