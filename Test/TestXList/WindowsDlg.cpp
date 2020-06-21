/*!
	\file	WindowsDlg.cpp		Windows | More Windows handler
	\author	John Young
	\date	25 February 1998
	
	Copyright (c) 1998 Young Associates
*/

#include "stdafx.h"
#include "PrEditor.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "WindowsDlg.h"
#include "Workspace.h"
#include "Project/ProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const char szWindowsDlg[] = "WindowPlacement\\WindowsDlg";
     const char szDlg[] = "Dialog";


#define   COL_NAME       0
#define   COL_PATH       1
#define   COL_TYPE       2
#define   COL_ALT        3
static const char *szHeadings[] = { "Name", "Path", "Type", "Alt", NULL };


/*virtual*/ CXListCtrl::DataTypes CWindowsListCtrl::GetColumnType (int iCol)
{
     switch (iCol)
     {
          case COL_NAME:
          case COL_PATH:
          case COL_TYPE:
          default:
               return TypeCaselessText;
          case COL_ALT:
               return TypeInt;
     }
}


CWindowsDlg::CWindowsDlg(CWnd* pParent /*=NULL*/)
	: CNSFlexDialog(CWindowsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindowsDlg)
	//}}AFX_DATA_INIT
	m_hViewWnd = NULL;
	
	// 'Flexi' areas
//     CNSFlexHorizontalConstraint areaList (0.0f,  1.0f);

//     AddFlexConstraint (IDC_VERIFY_ORDERS,  areaList, NSFlexExpandDown);
     AddFlexConstraint (IDC_WINDOWS_LIST,  NSFlexExpandRight, NSFlexExpandDown);
	AddFlexConstraint (IDC_WINDOWS_ACTIVATE, NSFlexShiftRight, NSFlexVerticallyFixed); 
	AddFlexConstraint (IDCANCEL, NSFlexShiftRight, NSFlexVerticallyFixed);
	AddFlexConstraint (IDC_WINDOWS_TOUCH, NSFlexShiftRight, NSFlexVerticallyFixed);
	AddFlexConstraint (IDC_WINDOWS_SAVE, NSFlexShiftRight, NSFlexVerticallyFixed);
	AddFlexConstraint (IDC_WINDOWS_CLOSE, NSFlexShiftRight, NSFlexVerticallyFixed);
	AddFlexConstraint (ID_HELP, NSFlexShiftRight, NSFlexVerticallyFixed);

}


void CWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNSFlexDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowsDlg)
	DDX_Control(pDX, IDC_WINDOWS_LIST, m_lcWindows);
	DDX_Control(pDX, IDC_WINDOWS_TOUCH, m_btnTouch);
	DDX_Control(pDX, IDC_WINDOWS_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_WINDOWS_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_WINDOWS_ACTIVATE, m_btnActivate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindowsDlg, CNSFlexDialog)
	//{{AFX_MSG_MAP(CWindowsDlg)
	ON_BN_CLICKED(IDC_WINDOWS_CLOSE, OnCloseView)
	ON_BN_CLICKED(IDC_WINDOWS_HELP, OnHelp)
	ON_BN_CLICKED(IDC_WINDOWS_SAVE, OnSave)
	ON_BN_CLICKED(IDC_WINDOWS_TOUCH, OnTouch)
	ON_BN_CLICKED(IDC_WINDOWS_ACTIVATE, OnActivate)
	ON_NOTIFY(NM_DBLCLK, IDC_WINDOWS_LIST, OnDblclkWindowsList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WINDOWS_LIST, OnItemChangedWindowsList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




#define MAX_ITEMSIZE     64

BOOL CWindowsDlg::OnInitDialog() 
{
	CNSFlexDialog::OnInitDialog();

	// Using NanoSoft's Flexi-dialog necessitates using the 'resizing' style 
	// for the dialog bordor.  This forces an icon to be displayed in the top 
	// left of the dialog box, which defaults to the 'Windows' icon unless we 
	// do this...
	HICON hIcon = AfxGetApp()->LoadIcon (IDI_WINDOWS);
     SetIcon (hIcon, TRUE);		// Set big icon
	//SetIcon (hIcon, FALSE);		// Set small icon

     m_lcWindows.SetExtendedStyle (LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

     CString sRegBase = theApp.GetRegBase() + "\\" + szWindowsDlg;
     m_lcWindows.SetRegBase (sRegBase);
     m_lcWindows.Initialise (szHeadings);

     CRegKey rkWindows (HKEY_CURRENT_USER, sRegBase);
     if ( rkWindows.IsOpen() )
     {
          CRect rect;
          if ( rkWindows.Query (szDlg, &rect, sizeof CRect) )
               ::MoveWindow (m_hWnd, rect.left, rect.top, 
                             rect.Width(), rect.Height(), TRUE);
     }

	// Add children to the list box
	CWorkspaceMgr& workspaceMgr = CWorkspaceMgr::Instance();
	int iRow = 0;
	int activeIndex = -1;
	CFrameWnd* pActive = theApp.GetMainFrame()->GetActiveFrame();
	for (POSITION pos = workspaceMgr.GetHeadPosition(); pos; iRow++)
	{
		CWorkspaceItem* pItem = workspaceMgr.GetNext (pos);
		ASSERT ( pItem );
		ASSERT ( pItem->GetHWnd() );

          CDocument* pDoc = pItem->GetDocument();

          // Skip the project view...
          if ( pDoc->IsKindOf (RUNTIME_CLASS (CProjectDoc)) )
               continue;

          // Fill up an array of text for this row.  The order is fixed as 
          // 'Name', 'Path', 'Type', and 'Alt', ie the same as the headings.
          CStringArray arrText;

          arrText.Add (pDoc->GetTitle());

          CString sPath = pDoc->GetPathName();
          int n = sPath.ReverseFind ('\\');
          if ( n != -1 )
               sPath = sPath.Left (n+1);
          arrText.Add (sPath);

		// Get the file description from the shell...
		SHFILEINFO fileInfo;
		fileInfo.hIcon = 0;
		fileInfo.iIcon = 0;
          fileInfo.dwAttributes = 0; 
		::SHGetFileInfo (pDoc->GetPathName(), 0, &fileInfo, 
		                 sizeof fileInfo, SHGFI_TYPENAME);
		if ( fileInfo.szTypeName [0] == '\0' )
		{
		     // No registered type.  Use the extension...
               CString sType = CFilename::GetFileExt (pDoc->GetPathName());
               if ( !sType.IsEmpty() )
               {
                    if ( sType [0] == '.' )
                         sType = sType.Right (sType.GetLength() - 1);
                    sType += " file";
               }
               arrText.Add (sType);
          }
          else arrText.Add (fileInfo.szTypeName);

		CString sAltNr;
		sAltNr.Format ("%d", pItem->GetAltNr());
          arrText.Add (sAltNr);
	
	     // Now insert the sub-items into the list...
     	LV_ITEM lvItem;
     
     	lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
     	lvItem.state = 0;      
     	lvItem.stateMask = 0;  
     	lvItem.iImage = 0;
     
     	lvItem.iItem = iRow;
     	lvItem.iSubItem = 0;

     	lvItem.pszText = (char*) (const char*) arrText [m_lcWindows.GetColIndex (0)]; 
     	lvItem.cchTextMax = MAX_ITEMSIZE;
     	lvItem.lParam = (LPARAM) pItem->GetHWnd();
     
     	int iIndex = m_lcWindows.InsertItem (&lvItem);
     	if ( iIndex != -1)
               for (int j = 1; j < m_lcWindows.GetColCount(); j++)
          		m_lcWindows.SetItemText (iIndex, j, 
          		          (char*) (const char*) arrText [m_lcWindows.GetColIndex (j)]);

	     if ( pActive->m_hWnd == pItem->GetHWnd() )
			activeIndex = iIndex;
	}

	if ( activeIndex != -1 )
	     m_lcWindows.SetItemState (activeIndex, LVIS_SELECTED | LVIS_FOCUSED,
	                                            LVIS_SELECTED | LVIS_FOCUSED);
     
     m_lcWindows.Sort();

	EnableButtons();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CWindowsDlg::EnableButtons()
{
	// 'Active'	-> Enable if exactly one selection
	// 'Save'		-> Enable if one or more selection
	// 'Close window'		"
	// 'Touch'			"

	int	nSelected = m_lcWindows.GetSelectedCount();
	m_btnActivate.EnableWindow (nSelected == 1);
	m_btnTouch.EnableWindow (nSelected >= 1);
	m_btnSave.EnableWindow (nSelected >= 1);
	m_btnClose.EnableWindow (nSelected >= 1);
}


void CWindowsDlg::OnItemChangedWindowsList(NMHDR* pNMHDR, LRESULT* pResult)
{
     NMLISTVIEW* pNM = (NMLISTVIEW*) pNMHDR;

     EnableButtons();
     *pResult = 0;
}


void CWindowsDlg::OnCloseView() 
{
	int	iFirstSelected = -1;

	// We do one file at a time, because we delete each file from the
	// list from the first selected.  Since the selection changes each
	// time we close a window we need to get the new selection.
	while ( m_lcWindows.GetSelectedCount() )
	{
          POSITION pos = m_lcWindows.GetFirstSelectedItemPosition();
          ASSERT ( pos );
          int iSelected = m_lcWindows.GetNextSelectedItem (pos);
		HWND hWnd = (HWND) m_lcWindows.GetItemData (iSelected);

		if ( iFirstSelected == -1 )
			iFirstSelected = iSelected;

		CChildFrame* pChild = (CChildFrame*) CWnd::FromHandlePermanent (hWnd);
		ASSERT ( pChild && pChild->IsKindOf (RUNTIME_CLASS (CChildFrame)) );
		CView* pView = pChild->GetView (0);
		ASSERT ( pView && pView->IsKindOf (RUNTIME_CLASS (CView)) );

		CDocument* pDoc = pView->GetDocument();
		ASSERT ( pDoc && pDoc->IsKindOf (RUNTIME_CLASS (CDocument)) );

		if ( pDoc->SaveModified() )
		{
			pDoc->OnCloseDocument();
			// Document (and all it's views) are gone... remove the filename 
			// from the list
			m_lcWindows.DeleteItem (iSelected);
			m_lcWindows.UpdateWindow();
		}
		else break;
	}

	int	nCount = m_lcWindows.GetItemCount();
	if ( nCount )
	{
		if ( iFirstSelected >= nCount )
			iFirstSelected = nCount - 1;
	     m_lcWindows.SetItemState (iFirstSelected, LVIS_SELECTED | LVIS_FOCUSED,
	                                               LVIS_SELECTED | LVIS_FOCUSED);
	}

	EnableButtons();
}


void CWindowsDlg::OnSave() 
{
     for (POSITION pos = m_lcWindows.GetFirstSelectedItemPosition(); pos; )
	{
		int	nIndex = m_lcWindows.GetNextSelectedItem (pos);
		HWND hWnd = (HWND) m_lcWindows.GetItemData (nIndex);
		CChildFrame* pChild = (CChildFrame*) CWnd::FromHandlePermanent (hWnd);
		ASSERT ( pChild && pChild->IsKindOf (RUNTIME_CLASS (CChildFrame)) );
		CView* pView = pChild->GetView (0);
		ASSERT ( pView && pView->IsKindOf (RUNTIME_CLASS (CView)) );

		CDocument* pDoc = pView->GetDocument();
		ASSERT ( pDoc && pDoc->IsKindOf (RUNTIME_CLASS (CDocument)) );

		if ( pDoc->DoFileSave() )
		{
			// Remove the " *" modified flag at the end of the string
			CString sText = m_lcWindows.GetItemText (nIndex, 0);
			if ( sText.Right (2) == " *" )
			{
			     sText = sText.Left (sText.GetLength() - 2);
			     m_lcWindows.SetItemText (nIndex, 0, sText);
			}
		}
	}

	EnableButtons();
}

void CWindowsDlg::OnTouch() 
{
	OnSave();
}


void CWindowsDlg::ActivateView() 
{
	POSITION pos = m_lcWindows.GetFirstSelectedItemPosition();
	if ( pos )
	{
     	int	nIndex = m_lcWindows.GetNextSelectedItem (pos);
     	if ( nIndex != -1 )
     	{
     		m_hViewWnd = (HWND) m_lcWindows.GetItemData (nIndex);
     		CNSFlexDialog::OnOK();
     	}
     }
}


void CWindowsDlg::OnActivate() 
{
	int	nSelected = m_lcWindows.GetSelectedCount();
	ASSERT ( nSelected == 1 );
	if ( nSelected == 1 )
		ActivateView();
}


void CWindowsDlg::OnDblclkWindowsList(NMHDR* pNMHDR, LRESULT* pResult) 
{
     UNREFERENCED_PARAMETER (pNMHDR);

	int	nSelected = m_lcWindows.GetSelectedCount();
	ASSERT ( nSelected == 1 );
	
	ActivateView();
	
	*pResult = 0;
}



void CWindowsDlg::OnHelp() 
{
}


void CWindowsDlg::OnDestroy() 
{
     // Now save the size and position to the registry

     CRect rect;
     GetWindowRect (rect);
     CString sRegBase = theApp.GetRegBase() + "\\" + szWindowsDlg;
     CRegKey rkWindows (HKEY_CURRENT_USER, sRegBase, KEY_ALL_ACCESS);
     if ( rkWindows.IsOpen() )
     {
          //CString sOrder;
          rkWindows.Set (szDlg, &rect, sizeof CRect);
     }

	CNSFlexDialog::OnDestroy();
}
