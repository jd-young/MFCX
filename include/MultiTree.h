// MultiTree.h : header file
// Copyright (c) 1999 Richard Hazlewood
// This code is provided as-is.  Use at your own peril.


#ifndef   __MFCX_MULTITREE_H
#define   __MFCX_MULTITREE_H

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

//take the liberty of using 1 for focused item - it used
// to be defined anyway, before MS removed it.
#ifndef TVIS_FOCUSED
#define TVIS_FOCUSED	1
#else
#if TVIS_FOCUSED != 1
#error TVIS_FOCUSED was assumed to be 1
#endif
#endif

//define for custom notification
// sent on check-box change
// (Care: may clash in future)
#ifdef _MST_TVN_CHKCHANGE
#ifndef _MST_MULTI_CHECK
#define _MST_MULTI_CHECK	//implied
#endif
#ifndef TVN_CHKCHANGE
#define TVN_CHKCHANGEA	(TVN_FIRST-49)
#define TVN_CHKCHANGEW	(TVN_FIRST-(49+49))
#ifdef UNICODE
#define TVN_CHKCHANGE	TVN_CHKCHANGEW
#else
#define TVN_CHKCHANGE	TVN_CHKCHANGEA
#endif
#endif //!TVN_CHKCHANGE
#endif //_MST_TVN_CHKCHANGE


typedef CTypedPtrList<CPtrList, HTREEITEM> CTreeItemList;

/////////////////////////////////////////////////////////////////////////////
// CMultiTree window

#ifndef MST_AFX_EXT_CLASS
#define MST_AFX_EXT_CLASS
//define this if building class for DLL export/import
// typically: #define MST_AFX_EXT_CLASS AFX_EXT_CLASS
#endif


class MST_AFX_EXT_CLASS CMultiTree : public CTreeCtrl
{
	DECLARE_DYNAMIC (CMultiTree)

public:
	CMultiTree();
	virtual ~CMultiTree();

// Attributes
public:
protected:
	BOOL		m_bMulti;
	HTREEITEM	m_hSelect;	//for shift selection
	BOOL		m_bEmulated;
	BOOL		m_bBandLabel;


// Operations
public:
	UINT GetSelectedCount() const;
	HTREEITEM GetFirstSelectedItem() const;
	HTREEITEM GetNextSelectedItem (HTREEITEM hItem) const;

	void SelectAll (BOOL bSelect = TRUE);	//doesn't affect focus
	void SelectRange (HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly = TRUE);
	BOOL IsSelected (HTREEITEM hItem) const 
	     {
	          return !!(TVIS_SELECTED & 
	                    CTreeCtrl::GetItemState (hItem, TVIS_SELECTED));
	     };

	//SelectChildren returns TRUE if focus was on a child item
	BOOL SelectChildren (HTREEITEM hParent, 
	                     BOOL bSelect = TRUE, 
	                     BOOL bRecurse = TRUE);

	//Note: not virtual, so must call this class (or derivative)
	BOOL SetItemState (HTREEITEM hItem, UINT nState, UINT nStateMask);
	UINT GetItemState (HTREEITEM hItem, UINT nStateMask) const;
	BOOL SelectItem (HTREEITEM hItem);
     BOOL Select (HTREEITEM hItem, UINT code);

	//Note: GetSelectedItem should be taken as GetFocusedItem
	// when in multi-select mode
	HTREEITEM GetFocusedItem() const 
	     {
	          ASSERT (m_bMulti);
	          return CTreeCtrl::GetSelectedItem();
	     };
	BOOL FocusItem (HTREEITEM hItem);

	BOOL SetMultiSelect (BOOL bMulti);
	BOOL IsMultiSelect() const {return m_bMulti;};
	BOOL IsEmulatedNotify() const {return m_bEmulated;};
	BOOL SetBandingHit (BOOL bLabel) 
	     {
	          BOOL b = m_bBandLabel;
	          m_bBandLabel = bLabel;
	          return b;
	     };

	void GetSelectedList (CTreeItemList& list) const;


protected:
	void OnButtonDown (BOOL bLeft, UINT nFlags, CPoint point);
	void DoPreSelection (HTREEITEM hItem, BOOL bLeft, UINT nFlags);
	void DoAction (HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point);
	void DoBanding (BOOL bLeft, UINT nFlags, CPoint point);
	void UpdateSelectionForRect (LPCRECT pRect, UINT nFlags, CTreeItemList& list);
	void SelectAllIgnore (BOOL bSelect, HTREEITEM hIgnore);

	BOOL _SendNotify (LPNMHDR pNMHDR);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL (CMultiTree)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG (CMultiTree)
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnSetFocus (CWnd* pOldWnd);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg BOOL OnItemExpanding (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MFCX_MULTITREE_H
