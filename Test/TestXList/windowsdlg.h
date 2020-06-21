// WindowsDlg.h
//

#ifndef	WINDOWSDLG_H
#define	WINDOWSDLG_H

#include  <NSFlexDialog.h>


class CWindowsListCtrl : public CPersistentListCtrl
{
public:
     CWindowsListCtrl() {}
     ~CWindowsListCtrl() {}
     virtual DataTypes GetColumnType (int nCol);
};


/////////////////////////////////////////////////////////////////////////////
// CWindowsDlg dialog

class CWindowsDlg : public CNSFlexDialog
{
// Construction
public:
	CWindowsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindowsDlg)
	enum { IDD = IDD_WINDOWS };
	CWindowsListCtrl	m_lcWindows;
	CButton	m_btnTouch;
	CButton	m_btnSave;
	CButton	m_btnClose;
	CButton	m_btnActivate;
	//}}AFX_DATA

	HWND m_hViewWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableButtons();
	void ActivateView();

	// Generated message map functions
	//{{AFX_MSG(CWindowsDlg)
	afx_msg void OnCloseView();
	afx_msg void OnHelp();
	afx_msg void OnSave();
	afx_msg void OnTouch();
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate();
	afx_msg void OnDblclkWindowsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedWindowsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif	// WINDOWSDLG_H
