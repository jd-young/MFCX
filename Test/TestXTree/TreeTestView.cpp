// TreeTestView.cpp : implementation of the CTreeTestView class
//

#include "stdafx.h"
#include "TreeTest.h"

#include "TreeTestDoc.h"
#include "TreeTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeTestView

IMPLEMENT_DYNCREATE(CTreeTestView, CView)

BEGIN_MESSAGE_MAP(CTreeTestView, CView)
	//{{AFX_MSG_MAP(CTreeTestView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_POPUP_MENU1, OnPopupMenu1)
	ON_COMMAND(ID_POPUP_MENU2, OnPopupMenu2)
	ON_COMMAND(ID_POPUP_MENU3, OnPopupMenu3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeTestView construction/destruction

CTreeTestView::CTreeTestView()
{
	// TODO: add construction code here

}

CTreeTestView::~CTreeTestView()
{
}

BOOL CTreeTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTreeTestView drawing

void CTreeTestView::OnDraw(CDC* pDC)
{
	CTreeTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CTreeTestView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDragMultiTree& tree = GetTreeCtrl();
	HTREEITEM hItem = tree.InsertItem(_T("Item0"));
		tree.InsertItem(_T("Item00"), hItem);
		HTREEITEM h00 = tree.InsertItem(_T("Item01"), hItem);
			tree.InsertItem(_T("Item010"), h00);
		tree.InsertItem(_T("Item02"), hItem);
		HTREEITEM h03 = tree.InsertItem(_T("Item03"), hItem);
			tree.InsertItem(_T("Item030"), h03);
			tree.InsertItem(_T("Item031"), h03);
	hItem = tree.InsertItem(_T("Item1"));
		tree.InsertItem(_T("Item10"), hItem);
	hItem = tree.InsertItem(_T("Item2"));

     tree.SetMultiSelect (true);
     //tree.SetMultiSelect (false);
     
     tree.SetMoveCursor (IDC_DRAGMOVE);
     tree.SetCopyCursor (IDC_DRAGCOPY);
     tree.SetNoDropCursor (IDC_NODROP);
     //tree.SetCursors (IDC_DRAGMOVE, IDC_DRAGCOPY, IDC_NODROP);
     
     // create CImageList
	CBitmap bm;
	VERIFY ( m_imageList.Create (16, 15, TRUE, 0, 1));
	VERIFY ( bm.LoadBitmap (IDB_IMAGES));
	VERIFY ( m_imageList.Add (&bm, RGB (192, 192, 192)) != -1 );
	tree.SetImageList (&m_imageList, TVSIL_NORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CTreeTestView diagnostics

#ifdef _DEBUG
void CTreeTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTreeTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTreeTestDoc* CTreeTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreeTestDoc)));
	return (CTreeTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeTestView message handlers

int CTreeTestView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
     if ( !m_tree.Create (TVS_HASLINES |
                          TVS_LINESATROOT | 
                          TVS_HASBUTTONS | 
                          TVS_EDITLABELS | 
                          TVS_SHOWSELALWAYS |
                          WS_VISIBLE,
					 CRect (0,0,0,0), this, 1) )
     {
          // Couldn't create the tree...
          TRACE ("Couldn't create the tree\n");
     }
	
	m_tree.SetMenuID (IDR_TREE_POPUP);
	
	return 0;
}

void CTreeTestView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	GetTreeCtrl().MoveWindow (0, 0, cx, cy);	
}


void CTreeTestView::OnPopupMenu1() 
{
     AfxMessageBox ("Menu 1");
}

void CTreeTestView::OnPopupMenu2() 
{
     AfxMessageBox ("Menu 2");
}

void CTreeTestView::OnPopupMenu3() 
{
     AfxMessageBox ("Menu 3");
}
