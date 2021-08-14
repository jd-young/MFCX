/*!
     \file     XListBox.cpp

     An extended list box that allows creation, editing, deletion and moving 
     position of the entries.

     \author   John Young
     \date     9-Jul-99

     Copyright (c) 1999 Young Associates.
*/

#include "stdafx.h"
#include <afxribbonres.h>          // To get the IDB_AFXBARRES_XXX button ID's.
#include "../include/MFCXres.h"
#include "../include/XListBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT GetUniqueID (const CWnd* pWnd);

#define   BUTTON_NEW          0
#define   BUTTON_DELETE       1
#define   BUTTON_MOVEUP       2
#define   BUTTON_MOVEDOWN     3          

static UINT arrStandardButtonIDs[] =
{
     IDB_AFXBARRES_NEW32,
     IDB_AFXBARRES_DELETE32,
     IDB_AFXBARRES_UP32,
     IDB_AFXBARRES_DOWN32
};

#define  NR_BUTTONS  (sizeof arrStandardButtonIDs / sizeof UINT)

#define   IDC_EDITCTRL        1
#define   IDC_BROWSEBTN       2

// @class Private helper class for the CXListBox class
// @base  public | CWnd
//
class CButtonsWnd : public CWnd
{
friend CXListBox;

// Construction
public:
	CButtonsWnd (CXListBox* pLB);
	virtual ~CButtonsWnd();

// Attributes
public:
     CSize GetSize() const { return m_sizeWnd; }
     CSize GetSize (int i) const;
     CRect GetButtonRect (int i) const { return m_arrButtons [i].rect; }

private:
     CXListBox* m_pBuddy;
     int m_nActive;           // -1 means not active.
     bool m_bPressed;
     
     typedef struct 
     {
          UINT nID;
          CRect rect;
          CBitmap bm;
     }    BUTTONINFO;
     BUTTONINFO m_arrButtons [NR_BUTTONS];
     CSize m_sizeWnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonsWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
private:
     int HitTest (CPoint pt) const;
     void InvalidateButton (int nIndex);
     void CalculateMetrics();
     void LoadBitmaps();

protected:
	//{{AFX_MSG(CButtonsWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CListEdit window

class CListEdit : public CEdit
{
// Construction
public:
	CListEdit (CXListBox* pParent);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEdit)
	public:
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListEdit();
     void EndEdit (bool bKeepChanges = true);
	
	CXListBox* m_pParent;

	// Generated message map functions
protected:
	//{{AFX_MSG(CListEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



ListBoxInfo::ListBoxInfo (const ListBoxInfo& from)
 :   sText (from.sText), dwData (from.dwData)
{
}

ListBoxInfo::ListBoxInfo (CString sText, DWORD data)
 :   sText (sText), dwData (data)
{
}

CXListBox::CXListBox()
{
     m_pButtons = NULL;
     m_pEdit = NULL;
     m_nPrevClicked = -1;
     m_nStyle = 0;
     m_pBrowse = NULL;
}

/*virtual*/ CXListBox::~CXListBox()
{
     delete m_pButtons;
     delete m_pBrowse;
     // We don't delete m_pEdit here, it takes care of itself
}

BEGIN_MESSAGE_MAP(CXListBox, CListBox)
	//{{AFX_MSG_MAP(CXListBox)
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VKEYTOITEM_REFLECT()
	//}}AFX_MSG_MAP
     ON_COMMAND (IDC_BROWSEBTN, OnBrowseClicked)
END_MESSAGE_MAP()


/*virtual*/ bool CXListBox::Initialise (CWnd* pParent, UINT nID, UINT style)
{
     m_nStyle = style;

     // Subclass the list-box
     HWND hLB = ::GetDlgItem (pParent->GetSafeHwnd(), nID);
     if ( !hLB )
          return false;
     VERIFY ( CWnd::SubclassWindow (hLB) );

     // Grow each item height by 2 to give room for the edit control borders.
     int itemHeight = GetItemHeight (0);
     SetItemHeight (0, itemHeight + 2);

     // Create the static window.
     DWORD dwStyle = SS_SUNKEN | WS_CHILD | WS_CLIPSIBLINGS | SS_CENTERIMAGE;
     DWORD dwExStyle = 0;
     UINT nButtonsID = GetUniqueID (pParent);
     
     m_pButtons = new CButtonsWnd (this);
	VERIFY ( m_pButtons->CreateEx (dwExStyle,
	                               NULL,                   // Windows class
               	                _T(""),                 // Window name
               	                dwStyle,
                                    CRect (0, 0, 0, 0),
                                    pParent,
                                    nButtonsID) );

     UINT nStaticID = GetUniqueID (pParent);
	VERIFY ( m_wndStatic.CreateEx (dwExStyle,
	                               _T("static"),           // Windows class
               	                _T(""),                 // Window name
               	                dwStyle,
                                    CRect (0, 0, 0, 0),
                                    pParent,
                                    nStaticID) );
     m_wndStatic.SetFont (pParent->GetFont());

	CRect rectLB;
	GetWindowRect (&rectLB);
	CRect rectStatic (rectLB);
	
	CClientDC dc (&m_wndStatic);
	CFont* pOldFont = dc.SelectObject (GetFont());
	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);
	dc.SelectObject (pOldFont);

     m_pButtons->LoadBitmaps();
	m_pButtons->CalculateMetrics();
	
     int nBorderHeight = GetSystemMetrics (SM_CYBORDER);

	int nStaticHeight = tm.tmHeight + 2 * nBorderHeight;
     CSize sizeButtons = m_pButtons->GetSize();
     if ( nStaticHeight < sizeButtons.cy )
          nStaticHeight = sizeButtons.cy;
	rectStatic.bottom = rectStatic.top + nStaticHeight + 2 * nBorderHeight;

	rectLB.top = rectStatic.bottom + 2 * nBorderHeight;

	CRect rectButtons;
	rectButtons.right = rectStatic.right - 1;
	rectButtons.left = rectButtons.right - sizeButtons.cx;
	
	int nYOffset = (rectStatic.Height() - sizeButtons.cy) / 2;
	rectButtons.top = rectStatic.top + nYOffset;
	rectButtons.bottom = rectButtons.top + sizeButtons.cy;
	
	pParent->ScreenToClient(&rectLB);
	pParent->ScreenToClient(&rectStatic);
	pParent->ScreenToClient(&rectButtons);
	CWnd* pPrev = GetWindow (GW_HWNDPREV);
	SetWindowPos (pPrev, rectLB.left, rectLB.top, 
					rectLB.Width(), rectLB.Height(), 0);
	m_pButtons->SetWindowPos (this, 
	                          rectButtons.left, rectButtons.top, 
	                          rectButtons.Width(), rectButtons.Height(), 0);
     m_wndStatic.SetWindowPos (m_pButtons, 
     	                     rectStatic.left, rectStatic.top, 
	                          rectStatic.Width(), rectStatic.Height(), 0);

	// Notify parent of selection changing
     NotifyParent (LBN_SELCHANGE);

	ShowWindow (SW_SHOW);
	m_pButtons->ShowWindow (SW_SHOW);
	m_wndStatic.ShowWindow (SW_SHOW);

     return true;
}




void CXListBox::SetWindowText (const TCHAR* pText) 
{
     m_wndStatic.SetWindowText (pText); 
}


void CXListBox::OnNew()
{
     if ( !IsEditing() )
          DoNew();
}

void CXListBox::OnDelete()
{
     if ( !IsEditing() && GetCount() != 0 )
          DoDelete();
}

void CXListBox::OnMoveUp()
{
     if ( !IsEditing() )
          DoMoveUp();
}

void CXListBox::OnMoveDown()
{
     if ( !IsEditing() )
          DoMoveDown();
}


/*virtual*/ void CXListBox::DoNew()
{
     // Creates a new entry at the bottom, and goes into edit mode.
     int nSelected = AddString ("");
     if ( nSelected != LB_ERR )
     {
          SetCurSel (nSelected);
          NotifyParent (LBN_SELCHANGE);
          StartEdit (nSelected);
     }
}


/*virtual*/ void CXListBox::DoDelete()
{
     int nSelected = GetCurSel();
     if ( nSelected != LB_ERR )
     {
     	DeleteString (nSelected);
     
          if ( nSelected >= GetCount() )
               nSelected--;
          
          if ( nSelected >= 0 )
          {
     		SetCurSel (nSelected);
               NotifyParent (LBN_SELCHANGE);
          }
     }
}

void CXListBox::NotifyParent (int nMsg)
{
     CWnd* pWnd = GetParent();
     if ( pWnd )
     	pWnd->PostMessage (WM_COMMAND, 
     		              (WPARAM) MAKELONG (GetDlgCtrlID(), nMsg), 
                             (LPARAM) m_hWnd);
}


void CXListBox::InsertItem (int nIndex, const ListBoxInfo& lbInfo, bool bSelect)
{
	int newIndex = InsertString (nIndex, lbInfo.sText);
	if (newIndex != LB_ERR)
	{
	     ASSERT ( newIndex == nIndex );
	     SetItemData (newIndex, lbInfo.dwData);
          if ( bSelect )
         		SetCurSel (newIndex);
	}
}



ListBoxInfo CXListBox::RemoveItem (int nIndex)
{
	CString sText;
	GetText (nIndex, sText);

	// Now remove current in list
	DWORD dwData = GetItemData (nIndex);
	DeleteString (nIndex);
	
	return ListBoxInfo (sText, dwData);
}



/*virtual*/ void CXListBox::DoMoveUp()
{
     // Moves the current selection up one line.
     int nSelected = GetCurSel();
     if ( nSelected != LB_ERR && nSelected > 0 )
     {
          ListBoxInfo lbInfo = RemoveItem (nSelected);
          nSelected--;   // Move up
          InsertItem (nSelected, lbInfo, true);
     }
}



/*virtual*/ void CXListBox::DoMoveDown()
{
     // Moves the current selection up one line.
     int nSelected = GetCurSel();
     if ( nSelected != LB_ERR && nSelected < GetCount() - 1 )
     {
          ListBoxInfo lbInfo = RemoveItem (nSelected);
          nSelected++;   // Move down
          InsertItem (nSelected, lbInfo, true);
     }
}



/*virtual*/ bool CXListBox::StartEdit (int nIndex, const TCHAR* pszText /*= NULL*/)
{
     CRect rectEdit;
     GetItemRect (nIndex, &rectEdit);

     CRect rectBrowse = rectEdit;
     rectBrowse.left = rectBrowse.right - rectEdit.Height();

     if ( m_nStyle & XLB_BROWSE )
          rectEdit.right -= rectBrowse.Width();

	m_pEdit = new CListEdit (this);
	if ( m_pEdit && 
          !m_pEdit->Create (WS_CHILD  | WS_BORDER | ES_AUTOHSCROLL,
                             rectEdit, this, IDC_EDITCTRL) )
     {
          delete m_pEdit;
          m_pEdit = NULL;
          return false;
     }

     if ( m_nStyle & XLB_BROWSE )
          m_pBrowse = new CButton;

     if ( m_pBrowse &&
          !m_pBrowse->Create ("...", WS_CHILD | BS_PUSHBUTTON, 
                              rectBrowse, this, IDC_BROWSEBTN) )
     {
          delete m_pBrowse;
          m_pBrowse = NULL;
     }

     if ( pszText )
          m_pEdit->SetWindowText (pszText);

	m_pEdit->SetFont (GetFont());
	m_pEdit->SetFocus();
	m_pEdit->ShowWindow (SW_SHOW);
	m_pEdit->UpdateWindow();

     if ( m_pBrowse )
     {
          m_pBrowse->ShowWindow (SW_SHOW);
          m_pBrowse->UpdateWindow();
     }

     return true;
}


/*virtual*/ void CXListBox::OnEndEdit (const TCHAR* pszText /*= NULL*/)
{
     if ( pszText )
     {
          int nSelected = GetCurSel();
          if ( nSelected != LB_ERR )
          {
               SetRedraw (FALSE);
          	DWORD dwData = GetItemData (nSelected);
               DeleteString (nSelected);
               InsertString (nSelected, pszText);
               SetItemData (nSelected, dwData);
               SetCurSel (nSelected);
               SetRedraw (TRUE);
          }
     }
     m_pEdit = NULL;

     if ( m_pBrowse )
     {
          delete m_pBrowse;
          m_pBrowse = NULL;
     }
}


CEdit* CXListBox::GetEditCtrl() const
{
     return m_pEdit;
}


void CXListBox::OnSetFocus(CWnd* pOldWnd) 
{
     if ( m_pEdit )
          m_pEdit->SetFocus();
     else CListBox::OnSetFocus(pOldWnd);
}


void CXListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
     int nIndex;
	if ( isprint (nChar) &&
          (nIndex = GetCurSel()) != LB_ERR )
     {
          ASSERT ( m_pEdit == NULL );

          CString str = (TCHAR) nChar;
          StartEdit (nIndex, str);
          if ( m_pEdit )
               m_pEdit->SetSel (str.GetLength(), -1);
     }
	else CListBox::OnChar (nChar, nRepCnt, nFlags);
}

int CXListBox::VKeyToItem (UINT nKey, UINT nIndex) 
{
     if ( nKey == VK_F2 )
     {
          ASSERT ( m_pEdit == NULL );

          CString str;
          GetText (nIndex, str);
          StartEdit (nIndex, str);
          if ( m_pEdit )
               m_pEdit->SetSel (0, -1);
          return -2;               // handled
     }
     
	return -1;                    // not handled - do default
}


void CXListBox::OnLButtonDown (UINT nFlags, CPoint point) 
{
     m_nPrevClicked = GetCurSel();
	CListBox::OnLButtonDown(nFlags, point);
}

void CXListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Handle drag 'n' drop of items to new positions.  Maybe make the
	// list box owner draw so that we can draw the 'drop' line.
	
	CListBox::OnMouseMove(nFlags, point);
}

void CXListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
     if ( IsEditing() )
          return;

     int nSelected = GetCurSel();
     if ( nSelected != LB_ERR )
     {
          CRect rect;
          GetItemRect (nSelected, &rect);
          if ( m_nPrevClicked == nSelected )
          {
               if ( rect.PtInRect (point) )
               {
                    ASSERT ( m_pEdit == NULL );

                    CString sText;
          	     GetText (nSelected, sText);
                    StartEdit (nSelected, sText);
                    if ( m_pEdit )
                         m_pEdit->SetSel (0, -1);
                    return;        // Don't do the default
               }
               else
               {
                    // Must have clicked in the area below the list items...
                    DoNew();
                    return;
               }
          }
     }
	CListBox::OnLButtonUp(nFlags, point);
}


static UINT GetUniqueID (const CWnd* pWnd)
{
     const UINT max = 32767;
     const UINT min = 1;
     HWND hWnd = pWnd->GetSafeHwnd();
     UINT nID = min;
     for (; nID <= max; nID++)
          if ( ::GetDlgItem (hWnd, nID) == NULL )
               break;
     return nID;
}


void CXListBox::OnBrowseClicked()
{
     OnBrowse();
}



/////////////////////////////////////////////////////////////////////////////
// CButtonsWnd


CButtonsWnd::CButtonsWnd (CXListBox* pLB)
{
     m_nActive = -1;                    // -1 means not active.
     m_bPressed = false;
     for (int i = 0; i < NR_BUTTONS; i++)
     {
          BUTTONINFO& bi = m_arrButtons [i];
          bi.nID = arrStandardButtonIDs [i];
          bi.rect = CRect (0, 0, 0, 0);
     }
     m_pBuddy = pLB;
}

CButtonsWnd::~CButtonsWnd()
{
}


BEGIN_MESSAGE_MAP(CButtonsWnd, CWnd)
	//{{AFX_MSG_MAP(CButtonsWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define   BUTTONSWND_CLASSNAME     "jyoung@gol.com/ButtonsWnd"
#define   BUTTONSWND_CLASSSTYLE    0         // Specifically NOT CS_DBLCLICK

BOOL CButtonsWnd::PreCreateWindow (CREATESTRUCT& cs) 
{
     // The whole point of this is to get rid of the CS_DBLCLICK style for the
     // Buttons Window.  This is so that if the user clicks like a dervish on 
     // the buttons, then the double click timer doesn't kick in and cause the
     // button to 'miss' (like number three cylinder on a cold winter's morn).

     WNDCLASS wndClass;
     if ( !::GetClassInfo (AfxGetInstanceHandle(), 
                           BUTTONSWND_CLASSNAME, 
                           &wndClass))
     {
     	wndClass.style = BUTTONSWND_CLASSSTYLE;
     	wndClass.lpfnWndProc = ::DefWindowProc;
     	wndClass.cbClsExtra = 0;
          wndClass.cbWndExtra = 0;
     	wndClass.hInstance = AfxGetInstanceHandle();
     	wndClass.hIcon = 0;
     	wndClass.hCursor = 0;
     	wndClass.hbrBackground = 0;
     	wndClass.lpszMenuName = NULL;
     	wndClass.lpszClassName = BUTTONSWND_CLASSNAME;
     	if ( ::RegisterClass (&wndClass) )
     	     cs.lpszClass = BUTTONSWND_CLASSNAME;
     }
     ASSERT ( wndClass.style == BUTTONSWND_CLASSSTYLE );
    
	return CWnd::PreCreateWindow(cs);
}



CSize CButtonsWnd::GetSize (int i) const
{
     const CRect& rect = m_arrButtons [i].rect;
     return CSize (rect.Width(), rect.Height());
}



void CButtonsWnd::LoadBitmaps()
{
     for (int i = 0; i < NR_BUTTONS; i++)
     {
          BUTTONINFO& bi = m_arrButtons [i];
     	VERIFY ( bi.bm.LoadBitmap (bi.nID) );
     }
} 



void CButtonsWnd::CalculateMetrics()
{
     // Fills in the m_arrButtons array.
     m_sizeWnd = CSize (0, 0);
     for (int i = 0; i < NR_BUTTONS; i++)
     {
          BUTTONINFO& bi = m_arrButtons [i];

     	CBitmap& bm = bi.bm;
     	BITMAP bmInfo;
     	bm.GetObject (sizeof bmInfo, &bmInfo);
     	
     	CRect& rect = bi.rect;
     	rect.left = m_sizeWnd.cx;
     	rect.right = rect.left + bmInfo.bmWidth + 3;
     	rect.top = 0;
     	rect.bottom = rect.top + bmInfo.bmHeight + 3;

		if ( m_sizeWnd.cy < rect.Height() )
			m_sizeWnd.cy = rect.Height();
		m_sizeWnd.cx += rect.Width();
     }
} 


// @mfunc Called when a WM_PAINT message is received.  Do all painting in here.
//
//        Each button has a one pixel border, and a one pixel SE movement if
//        it is pressed.
//
void CButtonsWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
     CRect rectClip;
     CRect rectVisible;
     dc.GetClipBox (&rectClip);
     dc.RectVisible (&rectVisible);

     // Erase everything first
	dc.FillSolidRect (&rectClip, ::GetSysColor (COLOR_BTNFACE));

	int xPos = 0;
	int yPos = 0;

     CPen penLightSource, penShadow;
     VERIFY ( penLightSource.CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DHILIGHT)) );
     VERIFY ( penShadow.CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DSHADOW)) );
     CPen* pOldPen = dc.GetCurrentPen();
	
	// Draw the bitmap buttons.
     for (int i = 0; i < NR_BUTTONS; i++)
     {
          CBitmap& bm = m_arrButtons [i].bm;

		BITMAP bmInfo;
		bm.GetObject (sizeof bmInfo, &bmInfo);

		CDC dcMem;
		dcMem.CreateCompatibleDC (&dc);
		CBitmap* pOldBitmap = dcMem.SelectObject (&bm);

          int xBM = xPos + 1;      // To allow room for the borders
          int yBM = yPos + 1;  
          if ( m_nActive == i && m_bPressed )
          {
               // Move the bitmap south-east to make it look like it is pushed
               xBM++;
               yBM++;
          }
          VERIFY ( dc.BitBlt (xBM, yBM, bmInfo.bmWidth, bmInfo.bmHeight, 
                              &dcMem, 0, 0, SRCCOPY) );

          if ( m_nActive == i )
          {
               // Draw the 3D lines round it.
               dc.SelectObject (m_bPressed ? penShadow : penLightSource);
               dc.MoveTo (xPos, yPos + bmInfo.bmHeight + 2);
               dc.LineTo (xPos, yPos);
               dc.LineTo (xPos + bmInfo.bmWidth + 2, yPos);
               
               dc.SelectObject (m_bPressed ? penLightSource : penShadow);
               dc.LineTo (xPos + bmInfo.bmWidth + 2, yPos + bmInfo.bmHeight + 2);
               dc.LineTo (xPos, yPos + bmInfo.bmHeight + 2);
          }
		dcMem.SelectObject (pOldBitmap);
          xPos += m_arrButtons [i].rect.Width();
     }
     dc.SelectObject (pOldPen);
     
	// Do not call CStatic::OnPaint() for painting messages
}

int CButtonsWnd::HitTest (CPoint pt) const
{
	for (int i = 0; i < NR_BUTTONS; i++)
	{
          CRect rect = GetButtonRect (i);
          if ( rect.PtInRect (pt) )
               return i;
	}
	return -1;
}




void CButtonsWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
     int nIndex;
     if ( (nIndex = HitTest (point)) != -1 )
     {
          m_bPressed = true;
          InvalidateButton (m_nActive = nIndex);
     }
	CWnd::OnLButtonDown(nFlags, point);
}

void CButtonsWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
     int nIndex;
     if ( (nIndex = HitTest (point)) != -1 )
     {
          if ( nIndex != m_nActive || 
               (!m_bPressed && (nFlags & MK_LBUTTON)) )
          {
               if ( m_nActive != -1 )
                    InvalidateButton (m_nActive);
               SetCapture();
               InvalidateButton (m_nActive = nIndex);
          }
          m_bPressed = nFlags & MK_LBUTTON;
     }
     else
     {
          if ( m_nActive != -1 )
          {
               if ( nFlags & MK_LBUTTON )
               {
                    // Display it active but with the button unpressed.
                    if ( m_bPressed )
                         InvalidateButton (m_nActive);
                    m_bPressed = false;
               }
               else
               {
                    InvalidateButton (m_nActive);
                    m_nActive = -1;
                    ::ReleaseCapture();
                    m_bPressed = false;
               }
          }
     }

	CWnd::OnMouseMove(nFlags, point);
}


void CButtonsWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
     int nIndex;
     if ( (nIndex = HitTest (point)) != -1 )
     {
          if ( m_nActive != nIndex )
               InvalidateButton (m_nActive);
          if ( m_bPressed )
               InvalidateButton (m_nActive = nIndex);

          m_bPressed = false;

          if ( m_pBuddy )
               switch ( m_nActive )
               {
                    case BUTTON_NEW:         m_pBuddy->OnNew();       break;
                    case BUTTON_DELETE:      m_pBuddy->OnDelete();    break;
                    case BUTTON_MOVEUP:      m_pBuddy->OnMoveUp();    break;
                    case BUTTON_MOVEDOWN:    m_pBuddy->OnMoveDown();  break;
               }
     }
	CWnd::OnLButtonUp(nFlags, point);
}



void CButtonsWnd::InvalidateButton (int nIndex)
{
     CRect rect = GetButtonRect (nIndex);
     InvalidateRect (rect);
}


/////////////////////////////////////////////////////////////////////////////
// CListEdit

CListEdit::CListEdit (CXListBox* pParent)
{
     m_pParent = pParent;
}

CListEdit::~CListEdit()
{
}


BEGIN_MESSAGE_MAP(CListEdit, CEdit)
	//{{AFX_MSG_MAP(CListEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers

void CListEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch ( nChar )
	{
	     case VK_ESCAPE:
     		EndEdit (false);
     		return;
	
          case VK_RETURN:
     		EndEdit (true);
     		return;
     }

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CListEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// Focus has changed to something else.  
     if ( m_pParent == NULL || 
          pNewWnd != m_pParent->m_pBrowse )
          EndEdit (true);
}



void CListEdit::EndEdit (bool bKeepChanges)
{
	// Notify the parent that we've finished editing...

     if ( m_pParent )
	     if ( bKeepChanges )
	     {
		     CString sText;
		     GetWindowText (sText);
		     m_pParent->OnEndEdit (sText);
	     }
	     else m_pParent->OnEndEdit();

     m_pParent = NULL;
	DestroyWindow();
}

void CListEdit::PostNcDestroy() 
{
	// Clean up the C++ object
	CEdit::PostNcDestroy();
	delete this;
}

UINT CListEdit::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}


