//
//   DragMultiTree.cpp     Drag 'n' Drop Multi-select Tree Control
//
//	Written by John Young, 25-Sep-99
//	Copyright (c) 1999 Young Associates
//

#include "stdafx.h"
#include "../include/DragMultiTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDragMultiTree

IMPLEMENT_SERIAL (CDragMultiTree, CMultiTree, 1)

CDragMultiTree::CDragMultiTree()
{
	m_bDragging = FALSE;
	m_pDragImage = NULL;
	m_hDropTarget = NULL;
     m_hDropHighlight = NULL;
	m_curMove = NULL;
	m_curCopy = NULL;
	m_curNoDrop = NULL;
	m_nHoverTimerID = 0;
	m_ptHover = CPoint (0, 0);
}

CDragMultiTree::~CDragMultiTree()
{
}


/*virtual*/ BOOL CDragMultiTree::IsDropSource (HTREEITEM /*hItem*/)
{
     return TRUE;             // Everything can be dragged.
}

/*virtual*/ HTREEITEM CDragMultiTree::GetDropTarget (HTREEITEM hTarget)
{
     // Check all the selected items.
     for (POSITION pos = m_listDragItems.GetHeadPosition(); pos; )
     {
          HTREEITEM hDragItem = m_listDragItems.GetNext (pos);
          if ( hTarget == hDragItem || IsAncestor (hDragItem, hTarget) )
               return NULL;
     }

	return hTarget;
}




BEGIN_MESSAGE_MAP(CDragMultiTree, CMultiTree)
	//{{AFX_MSG_MAP(CDragMultiTree)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
#ifdef    _DEBUG
	ON_WM_CREATE()
#endif
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDragMultiTree message handlers


void CDragMultiTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	/*HTREEITEM hDragItem = */pNMTreeView->itemNew.hItem;
	
	GetSelectedList (m_listDragItems);
	m_hDropTarget = NULL;
     for (POSITION pos = m_listDragItems.GetHeadPosition(); pos; )
     {
          HTREEITEM hDragItem = (HTREEITEM) m_listDragItems.GetNext (pos);
     	if ( !IsDropSource (hDragItem) )
	     	return;
     }

	// get the image list for dragging 
     /*if ( m_listDragItems.GetCount() == 1 )
     {
          HTREEITEM hDragItem = m_listDragItems.GetHead();
     	m_pDragImage = CreateDragImage (hDragItem);  
     }
     else m_pDragImage = NULL;          // Multi-select.*/

     m_pDragImage = NULL;

	m_bDragging = TRUE;
	SetCapture();

	// CreateDragImage() returns NULL if no image list 
	// associated with the tree view control 
	if( !m_pDragImage )
		return;

	m_pDragImage->BeginDrag (0, CPoint(15, 15));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen (&pt);
	m_pDragImage->DragEnter (NULL, pt);
}




void CDragMultiTree::OnTimer (UINT nIDEvent)
{
	if ( nIDEvent == m_nHoverTimerID )
	{
		KillTimer (m_nHoverTimerID);
		m_nHoverTimerID = 0;

		UINT		uFlag = 0;
		HTREEITEM	hItem = HitTest (m_ptHover, &uFlag);

		if ( hItem )
			Expand (hItem, TVE_EXPAND);	
	}
	else CMultiTree::OnTimer (nIDEvent);
}


void CDragMultiTree::OnMouseMove(UINT nFlags, CPoint point) 
{
#if  0
#ifdef    _DEBUG
     {
          static HTREEITEM hPrev = NULL;
          UINT flags;
          HTREEITEM hItem = HitTest(point, &flags);
          if ( hPrev != hItem )
          {
               TraceItem (hItem);
               hPrev = hItem;
          }
     }
#endif
#endif

	if ( m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
	}

	if ( m_bDragging )
	{
		m_nHoverTimerID = SetTimer (2, 750, NULL);
		m_ptHover = point;

		CPoint pt (point);
		ClientToScreen (&pt);
          CImageList::DragMove (pt);
          UINT flags;
          HTREEITEM hItem = HitTest(point, &flags);
      
		if ( m_hDropTarget != hItem )
		{
			CImageList::DragShowNolock (FALSE);

		     if ( hItem )
			     hItem = GetDropTarget (hItem);

			SelectDropTarget (m_hDropTarget = hItem);
          
               // Set the cursor...
               if ( hItem )
               {
                    if ( nFlags & MK_CONTROL )
     			     SetCursor (m_curCopy);
     			else SetCursor (m_curMove);
               }
     		else	SetCursor(m_curNoDrop);

			CImageList::DragShowNolock (TRUE);
          }
	}

	CMultiTree::OnMouseMove(nFlags, point);
}



/*!	Called when the user releases the left mouse button.  We're only interested 
	in this when the user is dragging a file (or files).   If this is the case, 
	then we need to move the files to a new position.

\param	nFlags		Indicates whether various virtual keys are down. This 
					parameter can be any combination of the following 
					values: 

					- MK_CONTROL   Set if the CTRL key is down.
					- MK_MBUTTON   Set if the middle mouse button is down.
					- MK_RBUTTON   Set if the right mouse button is down.
					- MK_SHIFT   	Set if the SHIFT key is down. 

\param	point		Specifies the x- and y-coordinate of the cursor. These 
					coordinates are always relative to the upper-left 
					corner of the window.
*/
void CDragMultiTree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CMultiTree::OnLButtonUp(nFlags, point);
	if ( m_bDragging )
	{
          CancelDrag();

          if ( m_hDropTarget )
               m_hDropTarget = GetDropTarget (m_hDropTarget);
	
		if ( m_hDropTarget == NULL )
			return;

		CopyItems (m_hDropTarget, m_listDragItems, (nFlags & MK_CONTROL) == 0);

          SelectDropTarget (NULL);

          SelectItem (m_hDropTarget, TRUE);
		Expand (m_hDropTarget, TVE_EXPAND);
          m_hDropTarget = NULL;
	}
}


/*!	Does a copy of the given list of tree items to the given target.  Once the
	copy is complete, the source list is deleted, if the bDelete flag is set.

\param	hTarget		The target tree item.  This is where the list of items
					are copied to.
\param	listOrig 		The list of items to be copied.
\param	bDelete		Set this to \b true if you want to move the items (i.e. 
					delete them after the copy).  This defaults to \b 
					false if omitted.

*/
/*virtual*/ void CDragMultiTree::CopyItems (HTREEITEM hTarget, 
								    const CTreeItemList& listOrig, 
								    bool bDelete /*= false*/)
{ 
     CTreeItemList listItems;
     MakeCopyList (listItems, listOrig);

	for (POSITION pos = listItems.GetHeadPosition(); pos; )
	{
	     HTREEITEM hDragItem = (HTREEITEM) listItems.GetNext (pos);
		CopyBranch (hTarget, hDragItem); 
	     if ( bDelete )       // Move op?
	 		DeleteBranch (hDragItem);
	}
}


void CDragMultiTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if ( m_bDragging )
		CancelDrag();
     else CMultiTree::OnRButtonDown (nFlags, point);
}


void CDragMultiTree::CancelDrag()
{
	if ( m_bDragging )
	{
		VERIFY (::ReleaseCapture());
		m_bDragging = FALSE;
		SelectDropTarget (NULL);

		CImageList::DragLeave (this);
		CImageList::EndDrag();

		delete m_pDragImage;
		m_pDragImage = NULL;
	}
}

BOOL CDragMultiTree::IsDragItem (HTREEITEM hItem) const
{
     for (POSITION pos = m_listDragItems.GetHeadPosition(); pos; )
     {
          HTREEITEM hDragItem = m_listDragItems.GetNext (pos);
          
          if ( hDragItem == hItem )
               return TRUE;
     }
     return FALSE;
}


/*!	Make a copy list from the selected drag list.  The rule is to ignore any 
	item that has its parent in the draglist.

	Note that this method relies on the fact that the listDragItems is in tree 
	order, that is the first item on the list is the highest item.

\param	listItems		The generated list of copied items.
\param	listOrig		The list of items to copy.
*/
void CDragMultiTree::MakeCopyList (CTreeItemList& listItems, 
                                   const CTreeItemList& listOrig)
{
     HTREEITEM hPrev = NULL;
     for (POSITION pos = listOrig.GetHeadPosition(); pos; )
     {
          HTREEITEM hDrag = listOrig.GetNext (pos);
          
          if ( hPrev == NULL || !IsAncestor (hPrev, hDrag) || 
          	!CanDragItem (hDrag, m_hDropTarget) )
          {
               listItems.AddTail (hDrag);
               hPrev = hDrag;
          }
     }
}

BOOL CDragMultiTree::IsAncestor (HTREEITEM hAncestor, HTREEITEM hChild)
{
     while ( hChild )
     {
          if ( hAncestor == hChild )
               return TRUE;
          
          hChild = GetParentItem (hChild);
     }
     return FALSE;
}


/*virtual*/ void CDragMultiTree::CopyBranch (HTREEITEM hParent, HTREEITEM hFrom)
{
     hParent;
     hFrom;
     // Implement this in your derived class.
     ASSERT (false);
}


/*virtual*/ void CDragMultiTree::DeleteBranch (HTREEITEM hItem)
{
     hItem;
     // Implement this in your derived class.
     ASSERT (false);
}


BOOL CDragMultiTree::Select (HTREEITEM hItem, UINT nCode) 
{ 
     if ( IsMultiSelect() )
     {
     	SelectRange (hItem, hItem);
          return TRUE;
     }
     else return CMultiTree::Select (hItem, nCode);
}

BOOL CDragMultiTree::SelectItem (HTREEITEM hItem, BOOL bOnly /*= FALSE*/) 
{ 
     if ( IsMultiSelect() )
     {
     	SelectRange (hItem, hItem, bOnly);
          FocusItem (hItem);
          return TRUE;
     }
     else return CMultiTree::SelectItem (hItem);
}

BOOL CDragMultiTree::SelectDropTarget (HTREEITEM hItem)
{
     if ( IsMultiSelect() )
     {
          if ( m_hDropHighlight )
          {
               SetItemState (m_hDropHighlight, 0, TVIS_DROPHILITED);
               m_hDropHighlight = NULL;
          }
          
          if ( hItem )
               SetItemState (m_hDropHighlight = hItem, 
                                    TVIS_DROPHILITED, 
                                    TVIS_DROPHILITED);
          return m_hDropHighlight != NULL;
     }
     else return CMultiTree::SelectDropTarget (hItem);
}

void CDragMultiTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
     if ( nChar == VK_ESCAPE )
          CancelDrag();
     CMultiTree::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDragMultiTree::InvalidateItem (HTREEITEM hItem)
{
     CRect rect;
     GetItemRect (hItem, &rect, TRUE);
     InvalidateRect (rect);
}



#ifdef    _DEBUG
void CDragMultiTree::TraceTree (HTREEITEM hItem)
{
     if ( hItem )
          TraceItem (hItem);

     HTREEITEM hChild = NULL;
     if ( hItem == NULL )
          hChild = GetRootItem();
     else hChild = GetChildItem (hItem);
     
     while ( hChild )
     {
          TraceTree (hChild);
          hChild = GetNextSiblingItem (hChild);
     }          
}

void CDragMultiTree::TraceItem (HTREEITEM hItem)
{
     if ( hItem )
     {
          CString sText = GetItemText (hItem);
          UINT state = GetItemState (hItem, TVIS_SELECTED | TVIS_CUT |
                                     TVIS_DROPHILITED | TVIS_BOLD |
                                     TVIS_EXPANDED | TVIS_EXPANDEDONCE);
          TRACE ("\"%s\" ", sText);
          if ( state & TVIS_SELECTED )
               TRACE ("SELECTED ");
          if ( state & TVIS_CUT )
               TRACE ("CUT ");
          if ( state & TVIS_DROPHILITED )
               TRACE ("DROPHILITED ");
          if ( state & TVIS_BOLD )
               TRACE ("BOLD ");
          if ( state & TVIS_EXPANDED )
               TRACE ("EXPANDED ");
          TRACE ("\n");
     }
     else TRACE ("(null)\n");
}

int CDragMultiTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
     // Check that the tree has the drag 'n' drop style bit set.
     DWORD style = lpCreateStruct->style;
     ASSERT ( (style & TVS_DISABLEDRAGDROP) == 0 );

	if (CMultiTree::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
#endif
