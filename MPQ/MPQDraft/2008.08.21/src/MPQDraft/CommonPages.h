/*
	The contents of this file are subject to the Common Development and Distribution License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.sun.com/cddl/cddl.html.

	Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

	The Initial Developer of the Original Code is Justin Olbrantz. The Original Code Copyright (C) 2008 Justin Olbrantz. All Rights Reserved.
*/

#if !defined(COMMONPAGES_H)
#define COMMONPAGES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/*	CPatchTargetPage dialog
	The patch dialog page is the first page of the patch wizard and second
	page of the SEMPQ wizard. It allows selection of a patch target either by
	selection of a supported game component, selection of an executable through
	the Browse button, or typing in a relative path (SEMPQ wizard only).
	There are a couple differences between the versions of this page in the two
	wizards. The patch wizard lists only installed components, and requires
	selection of a patch target that exists, while any target may be chosen for
	the SEMPQ wizard. As well, only the SEMPQ wizard allows you to type in a
	target, by either absolute or relative path. The page titles, info text,
	and save-state registry keys also differ between the two versions. */

class CPatchTargetPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPatchTargetPage)

public:
// Construction
	CPatchTargetPage(BOOL bSEMPQWizard = FALSE, UINT nTitleStringId = 0,
		UINT nInfoStringId = 0);
	~CPatchTargetPage();

// Dialog Data
	//{{AFX_DATA(CPatchTargetPage)
	enum { IDD = IDD_PATCHTARGETPAGE };
	// The informational/instructional text for the page
	CStatic m_ctlInformation;
	// The list of games. The data member of each item is a PROGRAMENTRY *.
	CListCtrl	m_ctlPrograms;
	// The list of game components (individual executables). The data member
	// of each list item is a PROGRAMFILEENTRY * to its component.
	CListCtrl	m_ctlComponents;
	// The actual path of the patch target
	CEdit	m_ctlProgramPath;
	// Command-line parameters to use when launching the target
	CEdit	m_ctlParameters;
	// See MPQD_EXTENDED_REDIR
	CButton	m_ctlExtendedRedir;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPatchTargetPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	virtual BOOL OnKillActive();
	//}}AFX_VIRTUAL

// Operations
public:
	const UINT m_nInfoStringId;

	// Functions to save and load the page's state to the registry between
	// executions of MPQDraft.
	BOOL SaveSettings();
	// LoadSettings must not be called before the dialog is initialized, and 
	// FindSupportedApps must be called before LoadSettings
	void LoadSettings();

	inline const PROGRAMENTRY *GetSelectedProgram()
	{ return GetProgram(m_nSelProgram); }
	const PROGRAMFILEENTRY *GetSelectedFile()
	{ return GetFile(m_nSelFile); }

	// Gets the path of the program to launch with the patcher. Not necessarily
	// the patch target.
	BOOL GetProgramPath(CString &strPath);
	// Gets the path of the patch target
	BOOL GetPatchTargetPath(CString &strPath);

	inline void GetParameters(CString &strParams)
	{ m_ctlParameters.GetWindowText(strParams); }

	inline BOOL RedirOpenFileEx()
	{ return m_ctlExtendedRedir.GetCheck(); }

// Implementation
protected:
	// Which mode we're operating in
	BOOL m_bSEMPQWizard;

	// Because it's hazardous to mess with the extended redirect option when
	// you don't know what you're doing, we warn the user the first time they
	// use it. This specifies whether changes to it should cause a warning.
	BOOL m_bWarnOnRedir;

	// Indicator whether the program and file lists are currently being
	// propagated. This is necessary to filter out the various list item
	// notifications sent when items are added.
	BOOL m_bUpdatingList;

	// The indices of the selected program and component. Note that these are
	// indices for the items in their respective list views. To get the
	// PROGRAMENTRY and PROGRAMFILEENTRY for the current selection, you need
	// to get the list item's data member.
	int m_nSelProgram;
	int m_nSelFile;

	// Functions to populate the programs and files lists. Only supported
	// items (installed games/components for the patch wizard, all for the
	// SEMPQ wizard) are listed.
	void FindSupportedApps();
	// NULL indicates that there is no program currently selected
	void FindSupportedFiles(const PROGRAMENTRY *lpSelProgram);

	// Updates the state and UI after a program or component is selected in
	// their respective lists. -1 indicates no program/component is selected.
	void SelectProgram(int nSelProgram);
	void SelectFile(int nSelFile);

	const PROGRAMENTRY *GetProgram(int iProgram);
	const PROGRAMFILEENTRY *GetFile(int iFile);

	// Generated message map functions
	//{{AFX_MSG(CPatchTargetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemChangedPrograms(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedComponents(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBrowse();
	afx_msg void OnRedirOpenFileEx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/*	CPluginPage dialog
	The plugin page is the last page of both wizards, and allows selection and
	configuration of plugins to use in the patching process.
	Apart from the different title string, there is one difference between the
	patch and SEMPQ wizard versions of this page: the patch wizard version
	requires that all plugins be compatible with the selected patch target,
	and does not list those that aren't; this selectivity is not possible for
	the SEMPQ wizard, because the actual patch target may be different than
	the file selected on the computer creating the SEMPQ (if the file exists
	at all). 
	The plugin page maintains two copies of the plugins at all times: an
	in-memory version stored in a map, and a list view copy stored in the 
	list view control. This is necessary because there may be some plugins we 
	found but do not want to display; that is, the in-memory copy contains 
	all plugins found, while the list view version only lists ones that are 
	able to patch the selected patch target. These two versions must 
	manually be kept in-sync. 
	While not extremely important, the behavior of the list view is designed 
	a certain way. Plugins found in the registry or by searching the plugin 
	directory (and thus initially listed) are sorted to make it easier for the 
	user to find the plugins they want. New plugin added through browse are 
	added to the end of the list to indicate that they are ones the user 
	manually added. This order is not maintained if the patch target changes, 
	however - all plugins are sorted equally. */

class CPluginPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPluginPage)

public:
	// This is sort of a light class that holds information about plugins in 
	// the map. The plugins are loaded and unloaded by constructor/destructor,
	// but apart from that members are read directly.
	class PLUGINENTRY
	{
	public:
		BOOL bSelected;

		// The path of the plugin
		DWORD dwFileNameHash;
		CString strFileName;

		// The plugin's information from IMPQDraftPlugin
		DWORD dwPluginID;
		CString strPluginName;

		// The plugin's DLL and IMPQDraftPlugin interface
		HMODULE hDLLModule;
		IMPQDraftPlugin *pPlugin;

		PLUGINENTRY(LPCSTR lpszFileName, BOOL bSelected)
		{
			// Load the plugin's module and get the plugin's interface
			hDLLModule = LoadLibrary(lpszFileName);
			if (!hDLLModule)
				throw std::exception("Unable to load library");

			GetMPQDraftPluginPtr pGetMPQDraftPlugin = (GetMPQDraftPluginPtr)
				GetProcAddress(hDLLModule, "GetMPQDraftPlugin");
			if (!pGetMPQDraftPlugin || !pGetMPQDraftPlugin(&pPlugin))
			{
				FreeLibrary(hDLLModule);
				throw std::exception("Unable to get plugin interface");
			}

			// Simply set all the other member variables
			this->bSelected = bSelected;

			dwFileNameHash = HashKey(lpszFileName);
			strFileName = lpszFileName;

			pPlugin->Identify(&dwPluginID);
			char szPluginName[255];
			pPlugin->GetPluginName(szPluginName, sizeof(szPluginName));
			strPluginName = szPluginName;
		}

		~PLUGINENTRY()
		{
			// Unload the plugin's module if necessary. No other cleanup is 
			// required.
			if (hDLLModule)
				FreeLibrary(hDLLModule);
		}

		bool operator ==(const PLUGINENTRY &Other)
		{
			return (dwFileNameHash == Other.dwFileNameHash 
				&& strFileName == Other.strFileName);
		}
	};

	// These are types that will be used in this page to return the plugin 
	// and module lists to the wizard.
	// Holds pointers directly to the entries in the plugin page list. Does 
	// not need to be explicitely freed, but will go boom if the page's 
	// plugin list is freed before these pointers are used.
	typedef CArray<PLUGINENTRY *> PluginList;
	// Self-contained - does not need to be explicitely freed.
	typedef CArray<MPQDRAFTPLUGINMODULE> PluginModuleList;

// Construction
public:
	CPluginPage(BOOL bSEMPQWizard = FALSE, UINT nTitleStringId = 0);
	~CPluginPage();

// Dialog Data
	//{{AFX_DATA(CPluginPage)
	enum { IDD = IDD_PLUGINPAGE };
	// The list of plugins. The data member is a pointer to its PLUGINENTRY.
	CListCtrl	m_ctlPlugins;
	// The button to configure the currently selected plugin
	CButton	m_ctlConfigure;
	// The button to browse for plugins not currently in the list
	CButton	m_ctlBrowse;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPluginPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	virtual BOOL OnKillActive();
	//}}AFX_VIRTUAL

// Operations
public:
	BOOL SaveSettings();
	// LoadSettings may be called at any time
	BOOL LoadSettings();

	// Sets the current patch target, as the plugins listed in the list view
	// depend on this. Returns whether ListPlugins needs to subsequently be 
	// called to list the plugins for the new patch target. This does not 
	// need to be called for SEMPQ wizard mode.
	BOOL SetPatchTarget(CString strPatchTarget);

	// Determine whether all selected plugins have been configured to run and
	// are ready for the target program to be launched.
	BOOL PluginsReady();

	// Functions to get the plugin and module lists
	BOOL GetNumSelectedPluginsAndModules(DWORD &nPlugins, DWORD &nModules);
	BOOL GetSelectedPluginsAndModules(PluginList *lpPlugins, 
		PluginModuleList &modules);

// Implementation
protected:
	// The map of plugin path names to PLUGINENTRYs, which contains the 
	// in-memory copy of the plugin information.
	typedef CMap<CString, CString &, PLUGINENTRY *, PLUGINENTRY *> PluginSet;

	// Which mode we're operating in
	BOOL m_bSEMPQWizard;

	// The plugin directory, either loaded from the registry or set to 
	// MPQDraft's startup directory.
	CString m_strPluginDir;
	// The patch target to be used to check plugins for compatibility
	CString m_strPatchTarget;

	// The list of all plugins found, regardless of whether they are
	// compatible with the patch target or not. The key is the file path 
	// itself, standardized to lower case.
	PluginSet m_lstPlugins;

	// Same function as in CPatchTargetPage
	BOOL m_bUpdatingList;

	// Find and load all plugins into the plugin list
	BOOL FindPlugins();
	// Unloads all plugins and empties the plugin list
	BOOL UnloadPlugins();
	// Populate the plugin list view with compatible plugins from the list
	BOOL ListPlugins();

	// Adds a plugin by path to the plugin list - does not add it to the 
	// list view control. Optionally indicates whether the plugin was added
	// to the list or it already existed. When lpbSelected is NULL, new 
	// plugins are set to unselected, while existing plugins are unchanged. 
	// lpszFileName must be a full file path.
	PLUGINENTRY *AddPlugin(LPCSTR lpszFileName, BOOL *lpbSelected, 
		BOOL *lpbAddedNew = NULL);
	// Adds a plugin to the list by its registry string, which includes both
	// the path and whether the plugin is selected.
	PLUGINENTRY *AddPluginFromRegistry(LPCSTR lpszRegString);

	// Comparison function used to sort the list of plugins
	static int CALLBACK ComparePlugins(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	// Generated message map functions
	//{{AFX_MSG(CPluginPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemChangedPlugins(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnConfigure();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(COMMONPAGES_H)