// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2BDF3A69_6D53_452A_B5FB_7AE962E08242__INCLUDED_)
#define AFX_STDAFX_H__2BDF3A69_6D53_452A_B5FB_7AE962E08242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x400
#define _WIN32_IE 0x500
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <string.h>
#include <new>
#include <memory>

#include <shlwapi.h>

#include <QDebug.h>
#include <QInjectDLL.h>
#include <QResource.h>
#include "DLL\\Patcher.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2BDF3A69_6D53_452A_B5FB_7AE962E08242__INCLUDED_)
