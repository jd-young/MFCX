// MFCX_DLL.h : main header file for the MFCX_DLL DLL
//

#if !defined(AFX_MFCX_DLL_H__6DCEDFF2_9818_11D5_A667_0000E87CAD39__INCLUDED_)
#define AFX_MFCX_DLL_H__6DCEDFF2_9818_11D5_A667_0000E87CAD39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "../include/MFCXres.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFCX_DLLApp
// See MFCX_DLL.cpp for the implementation of this class
//

class CMFCX_DLLApp : public CWinApp
{
public:
	CMFCX_DLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCX_DLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMFCX_DLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCX_DLL_H__6DCEDFF2_9818_11D5_A667_0000E87CAD39__INCLUDED_)
