//
//	DragMultiTree.h     A multi-select tree with drag 'n' drop support.
//
//   Copyright (c) 1999 John Young, Young Associates
//   http://www2.gol.com/users/jyoung/YoungAssociates.html
//
//   THOSE RESPONSIBLE
//   The CMultiTree class comes from Richard Hazlewood, and the CDragMultiTree
//   is a corrupted (by me :-) version of the drag 'n' drop tree from Miguel 
//   Grinberg and Pål K. Tønder.  The auto-expand while dragging support Pete 
//   Peterson.
//
//   DISCLAIMER
//   You are free to use this class in any way you like.  If you do use it, you 
//   use it at your own risk. You can't blame me or anyone else except yourself 
//   for anything that goes wrong, no matter how horrible.
//
//   USAGE
//   Derive your tree class from CDragMultiTree.  You may want to override the 
//   CopyBranch and DeleteBranch, and set the move, copy, and no-drop cursors by
//   calling SetMoveCursor, SetCopyCursor, SetNoDropCursor (or SetCursors to do
//   the lot).
//


#ifndef	__MFCX_DRAGMULTITREE_H
#define	__MFCX_DRAGMULTITREE_H

#include "MultiTree.h"

/////////////////////////////////////////////////////////////////////////////
// CDragMultiTree window

class CDragMultiTree : public CMultiTree
{
// Construction
public:
	CDragMultiTree();
	virtual ~CDragMultiTree();
	DECLARE_SERIAL (CDragMultiTree);

// Attributes
public:
     void SetMoveCursor (HCURSOR hMove) { m_curMove = hMove; }
     void SetCopyCursor (HCURSOR hCopy) { m_curCopy = hCopy; }
     void SetNoDropCursor (HCURSOR hNoDrop) { m_curNoDrop = hNoDrop; }
     void SetCursors (HCURSOR hMove,
                      HCURSOR hCopy,
                      HCURSOR hNoDrop)
     {
          m_curMove = hMove;
          m_curCopy = hCopy;
          m_curNoDrop = hNoDrop;
     }

     void SetMoveCursor (UINT nMove) 
          { m_curMove = AfxGetApp()->LoadCursor (nMove); }
     void SetCopyCursor (UINT nCopy) 
          { m_curCopy = AfxGetApp()->LoadCursor (nCopy); }
     void SetNoDropCursor (UINT nNoDrop) 
          { m_curNoDrop = AfxGetApp()->LoadCursor (nNoDrop);  }
     void SetCursors (UINT nMove, UINT nCopy, UINT nNoDrop)
          {
               SetMoveCursor (nMove);
               SetCopyCursor (nCopy);
               SetNoDropCursor (nNoDrop);
          }               

     BOOL IsDragging() const { return m_bDragging; }
     BOOL IsDragItem (HTREEITEM hItem) const;

protected:
	CImageList*    m_pDragImage;
	BOOL           m_bDragging;
     CTreeItemList  m_listDragItems;
	HTREEITEM      m_hDropTarget;           // Where the dragged items will be 
	                                        // dropped.
	HTREEITEM      m_hDropHighlight;        // Displayed as highlighted.  Used
	                                        // only in SelectDropTarget().
	HCURSOR        m_curMove;
	HCURSOR        m_curCopy;
	HCURSOR        m_curNoDrop;

     // Auto-expand support
	UINT m_nHoverTimerID;
	POINT m_ptHover;

// Operations
public:
     virtual BOOL IsDropSource (HTREEITEM hItem);
     virtual HTREEITEM GetDropTarget (HTREEITEM hItem);

     // Helpers...
	BOOL Select (HTREEITEM hItem, UINT nCode);
	BOOL SelectItem (HTREEITEM hItem, BOOL bOnly = FALSE);
	BOOL SelectDropTarget (HTREEITEM hItem);
     BOOL IsExpanded (HTREEITEM hItem)
	{
	     return (TVIS_EXPANDED & 
                  CTreeCtrl::GetItemState (hItem, TVIS_EXPANDED)) == 
                    TVIS_EXPANDED;
	};

#if  _DEBUG
     void SetCursor (HCURSOR hCursor)
     {
          ASSERT ( hCursor );           // You might want to set this to something.
          ::SetCursor (hCursor);
     }
     void TraceItem (HTREEITEM hItem);
     void TraceTree (HTREEITEM hItem);
#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragMultiTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	CancelDrag();
	virtual void CopyBranch (HTREEITEM hParent, HTREEITEM hFrom); 
	virtual void DeleteBranch (HTREEITEM hItem);

	/// Copies or moves the given list of items to the target tree item.
	virtual void CopyItems (HTREEITEM hTarget, const CTreeItemList& listItems, 
						bool bDelete = false);
	virtual bool CanDragItem (HTREEITEM hDrag, HTREEITEM hDropTarget) { return true; }

protected:
     void InvalidateItem (HTREEITEM hItem);
     void MakeCopyList (CTreeItemList& listCopy, const CTreeItemList& listDragItems);
     BOOL IsAncestor (HTREEITEM hAncestor, HTREEITEM hChild);

	// Generated message map functions
	//{{AFX_MSG(CDragMultiTree)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
#ifdef    _DEBUG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
#endif
	DECLARE_MESSAGE_MAP()
};


#endif    // __MFCX_DRAGMULTITREE_H
