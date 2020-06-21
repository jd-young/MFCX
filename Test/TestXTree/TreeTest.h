// TreeTest.h : main header file for the TREETEST application
//

#if !defined(AFX_TREETEST_H__42B36C72_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_)
#define AFX_TREETEST_H__42B36C72_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTreeTestApp:
// See TreeTest.cpp for the implementation of this class
//

class CTreeTestApp : public CWinApp
{
public:
	CTreeTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTreeTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREETEST_H__42B36C72_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_)
