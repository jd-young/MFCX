//
//   XTreeCtrl.h         A multi-select, drag-n-drop, tree with context menu
//                       support.
//
//   Copyright (c) 1999 John Young, Young Associates
//   http://www2.gol.com/users/jyoung/YoungAssociates.html
//


#ifndef   __MFXC_XTREECTRL_H
#define   __MFXC_XTREECTRL_H

#include  "DragMultiTree.h"

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl window

class CXTreeCtrl : public CDragMultiTree
{
// Construction
public:
	CXTreeCtrl();
	virtual ~CXTreeCtrl();
     DECLARE_SERIAL (CXTreeCtrl);

// Attributes
public:

protected:
     UINT m_idMenu;

// Operations
public:
     void SetMenuID (UINT id) { m_idMenu = id; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
     virtual void ShowPopupMenu (CPoint& point);

	// Generated message map functions
	//{{AFX_MSG(CXTreeCtrl)
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnContextMenu (CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MFXC_XTREECTRL_H
