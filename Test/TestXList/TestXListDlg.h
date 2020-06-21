// TestXListDlg.h : header file
//

#ifndef   TESTXLISTDLG_H
#define   TESTXLISTDLG_H


#include "mfcx.h"

class CTestListCtrl : public CPersistentListCtrl
{
public:
     CTestListCtrl() {}
     ~CTestListCtrl() {}
     virtual DataTypes GetColumnType (int nCol);
};



class CTestXListDlg : public CDialog
{
// Construction
public:
	CTestXListDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestXListDlg)
	enum { IDD = IDD_TESTXLIST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CTestListCtrl	m_lc;



	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestXListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestXListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // TESTXLISTDLG_H
