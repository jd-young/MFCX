/*!
     \file     XTreeCtrl.cpp  - A flashy tree control
     \author   John Young
     \date     6 May 2001
     
     Copyright (c) 2004 Thistle Software

     THOSE RESPONSIBLE
     The CDragMultiTree comes from me, sort of (see DragMultiTree.h for details) 
     and the context menu support is from ideas from ?

     DISCLAIMER
     You are free to use this class in any way you like.  If you do use it, you 
     use it at your own risk. You can't blame me or anyone else except yourself 
     for anything that goes wrong, no matter how horrible.

     USAGE
     Derive your tree class from CXTreeCtrl.  You may want to override the 
     CopyBranch and DeleteBranch, and set the move, copy, and no-drop cursors by
     calling SetMoveCursor, SetCopyCursor, SetNoDropCursor (or SetCursors to do
     the lot).  Set the context menu and provide message handlers for them.

*/


#include "stdafx.h"
#include "../include/XTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl
IMPLEMENT_SERIAL (CXTreeCtrl, CDragMultiTree, 1);

CXTreeCtrl::CXTreeCtrl()
{
     m_idMenu = NULL;
}

CXTreeCtrl::~CXTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXTreeCtrl, CDragMultiTree)
	//{{AFX_MSG_MAP(CXTreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl message handlers


void CXTreeCtrl::OnRButtonDown (UINT nFlags, CPoint point) 
{
     CDragMultiTree::OnRButtonDown (nFlags, point);
     
     // We cheat here and call OnContextMenu directly.
     //ClientToScreen (&point);
     //OnContextMenu (NULL, point);
}

void CXTreeCtrl::OnContextMenu (CWnd* pWnd, CPoint point) 
{
     if ( m_idMenu )
     {
          UINT uFlags;
          CPoint ptTree = point;
          ScreenToClient (&ptTree);
          HTREEITEM htItem = HitTest (ptTree, &uFlags);

          if ( (htItem != NULL) && (uFlags & TVHT_ONITEM) )
          {
                ShowPopupMenu( point );
                SetItemState(htItem, 0, TVIS_DROPHILITED);
                return;
          }
     }
     CDragMultiTree::OnContextMenu (pWnd, point);
}

/*virtual*/ void CXTreeCtrl::ShowPopupMenu( CPoint& point )
{
     if ( m_idMenu == 0 )
          return;

     if ( point.x == -1 && point.y == -1 )
     {
          //keystroke invocation
          CRect rect;
          GetClientRect (rect);
          ClientToScreen (rect);

          point = rect.TopLeft();
          point.Offset (5, 5);
     }

     CMenu menu;
     VERIFY ( menu.LoadMenu (m_idMenu) );
     CMenu* pPopup = menu.GetSubMenu (0);
     ASSERT ( pPopup != NULL );
     CWnd* pWndPopupOwner = this;

     while ( pWndPopupOwner->GetStyle() & WS_CHILD )
          pWndPopupOwner = pWndPopupOwner->GetParent();

     pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
                             point.x, point.y, pWndPopupOwner);
}

