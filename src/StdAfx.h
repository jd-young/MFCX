// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BA3E3809_170E_11D4_A60D_0000E87CAD39__INCLUDED_)
#define AFX_STDAFX_H__BA3E3809_170E_11D4_A60D_0000E87CAD39__INCLUDED_

// This will remove warnings for deprecated standard C functions.
#define _CRT_SECURE_NO_WARNINGS    1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifndef   _WIN32_WINNT
#define _WIN32_WINNT     _WIN32_WINNT_MAXVER
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxadv.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <Afxmt.h>            // Multi-thread support

#define	SIZEOF_ARRAY(a)	(sizeof (a) / sizeof (a[0]))


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BA3E3809_170E_11D4_A60D_0000E87CAD39__INCLUDED_)
