// TreeTestView.h : interface of the CTreeTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREETESTVIEW_H__42B36C7A_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_)
#define AFX_TREETESTVIEW_H__42B36C7A_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTree.h"

class CTreeTestView : public CView
{
protected: // create from serialization only
	CTreeTestView();
	DECLARE_DYNCREATE(CTreeTestView)

// Attributes
public:
	CTreeTestDoc* GetDocument();

     CMyTree m_tree;
     CImageList m_imageList;
     
     CXTreeCtrl& GetTreeCtrl() { return m_tree; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTreeTestView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupMenu1();
	afx_msg void OnPopupMenu2();
	afx_msg void OnPopupMenu3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TreeTestView.cpp
inline CTreeTestDoc* CTreeTestView::GetDocument()
   { return (CTreeTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREETESTVIEW_H__42B36C7A_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_)
