// TestXList.h : main header file for the TESTXLIST application
//

#if !defined(AFX_TESTXLIST_H__C6DDBCAF_30A7_4AF5_83A4_D428EE104A20__INCLUDED_)
#define AFX_TESTXLIST_H__C6DDBCAF_30A7_4AF5_83A4_D428EE104A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestXListApp:
// See TestXList.cpp for the implementation of this class
//

class CTestXListApp : public CWinApp
{
public:
	CTestXListApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestXListApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestXListApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTXLIST_H__C6DDBCAF_30A7_4AF5_83A4_D428EE104A20__INCLUDED_)
