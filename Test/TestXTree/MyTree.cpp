// MyTree.cpp : implementation file
//

#include "stdafx.h"
#include "TreeTest.h"
#include "MyTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTree

CMyTree::CMyTree()
{
}

CMyTree::~CMyTree()
{
}


BEGIN_MESSAGE_MAP(CMyTree, CXTreeCtrl)
	//{{AFX_MSG_MAP(CMyTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTree message handlers


/*virtual*/ void CMyTree::CopyBranch (HTREEITEM hParent, HTREEITEM hFrom)
{
	// This makes an exact copy of the branch pointed at by hFrom onto the 
	// branch pointed at by hParent.  It IS recursive :-)

	if ( hFrom == NULL || hParent == hFrom )
		return;

     CString sText = GetItemText (hFrom);
	HTREEITEM hNewItem = AddItem (hParent, sText);
	HTREEITEM hChild = GetChildItem (hFrom);
	while ( hChild != NULL )
	{
		CopyBranch (hNewItem, hChild);
		hChild = GetNextSiblingItem (hChild);
	}
	
	// Expand new item if the old one was
	if ( IsExpanded (hFrom) )
	     Expand (hNewItem, TVE_EXPAND);
}
 
HTREEITEM CMyTree::AddItem (HTREEITEM hParent, 
                               const char* pszName,
                               HTREEITEM hAfter /*= TVI_SORT*/)
{
	TV_ITEM tviNew;
	//tviNew.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tviNew.mask = TVIF_TEXT;
	tviNew.hItem = 0;
	tviNew.state = 0;
	tviNew.stateMask = 0;
	tviNew.pszText = (char*) pszName;		// This may seem unnecessary since
									// we're all set up for TEXTCALLBACK
									// but this gets us tooltips for free.
//	tviNew.pszText = LPSTR_TEXTCALLBACK;
	tviNew.cchTextMax = 0;
	tviNew.iImage = 0;
	tviNew.iSelectedImage = 0;
	tviNew.cChildren = 0;
	tviNew.lParam = (LPARAM) NULL;

	TV_INSERTSTRUCT tvIns;
	tvIns.hParent = hParent;
	tvIns.hInsertAfter = hAfter;
	tvIns.item = tviNew;

	HTREEITEM hNewItem;
	if ( (hNewItem = InsertItem (&tvIns)) == NULL )
	{
		CString sError;
		sError.Format ("Could not add '%s'", pszName);
		AfxMessageBox (sError, MB_OK | MB_ICONEXCLAMATION);
	}
	return hNewItem;
}





/*virtual*/ void CMyTree::DeleteBranch (HTREEITEM hItem)
{
     HTREEITEM hChild = NULL;
	if ( hItem )
	     hChild = GetChildItem (hItem);
	else hChild = GetRootItem();
	
	while ( hChild )
	{
          HTREEITEM hNext = GetNextSiblingItem (hChild);
          DeleteBranch (hChild);
          hChild = hNext;
     }

     if ( hItem )
     {
		DeleteItem (hItem);
	}
}

#if	0
void CMyTree::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;		// Default return value (allow editing)
	if ( pNMHDR->idFrom != IDI_PROJECT_VIEW )
		return;

	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
     TVITEM& item = pTVDispInfo->item;
     TRACE ("Begin edit '%s'\n", item.pszText);

	//*pResult = 1;		// Disallow editing
}
#endif

