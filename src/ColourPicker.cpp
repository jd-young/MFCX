// ColourPicker.cpp : implementation file
//
// ColourPicker is a drop-in Colour picker control. Check out the 
// header file or the accompanying HTML doc file for details.
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// Updated 16 May 1998
//         31 May 1998 - added Default text (CJM)
//          9 Jan 1999 - minor vis update
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 2004/07/19 00:25:57 $
 * $Revision: 1.1.1.1 $
 * $Archive: /CodeJock/CJLibrary/ColourPicker.cpp $
 *
 * $History: ColourPicker.cpp $
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 10/31/99   Time: 11:49p
 * Updated in $/CodeJock/CJLibrary
 * Overrode OnEraseBkgnd(...), OnPaint() and made modifications to
 * DrawItem(...) for flicker free drawing.
 * 
 * Modified resource include for static builds.
 * 
 * Added method CheckTextColour(...) which tests if the intensity of the
 * colour is greater as 128. If the intensity < 128 => colour is dark, so
 * the text must be light. Stephane Routelous
 * [routelous@cad-cam-concept.de]
 * 
 * Fixed potential resource and memory leak problems.
 * 
 * Removed un-necessary calls to GetParent(), a call is made only once at
 * initialization, to ensure we are working with a valid handle.
 * 
 * Made class methods virtual for inheritance purposes.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 10/14/99   Time: 12:22p
 * Updated in $/CodeJock/CJLibrary
 * Added source control history to file header.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../include/ColourPopup.h"
#include "../include/ColourPicker.h"
#include "../include/MFCXres.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI DDX_ColourPicker(CDataExchange *pDX, int nIDC, COLORREF& crColour)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);                
    
    CColourPicker* pColourPicker = (CColourPicker*) CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate)
    {
        crColour = pColourPicker->GetColour();
    }
    else // initializing
    {
        pColourPicker->SetColour(crColour);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker

CColourPicker::CColourPicker()
{
    SetBkColour(GetSysColor(COLOR_3DFACE));
    SetTextColour(GetSysColor(COLOR_BTNTEXT));

    m_bTrackSelection = FALSE;
    m_nSelectionMode = CP_MODE_BK;
    m_bActive = FALSE;

// 1999-06-11 begin mods KStowell
    m_strDefaultText.LoadString (IDS_MFCX_COLOUR_AUTO );
    m_strCustomText.LoadString( IDS_MFCX_COLOUR_CUST );
// 1999-06-11 end mods KStowell

// 10-14-1999 - Kirk Stowell
	m_pParentWnd = NULL;
}

CColourPicker::~CColourPicker()
{
}

IMPLEMENT_DYNCREATE(CColourPicker, CButton)

BEGIN_MESSAGE_MAP(CColourPicker, CButton)
    //{{AFX_MSG_MAP(CColourPicker)
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourPicker message handlers

LONG CColourPicker::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
    COLORREF crNewColour = (COLORREF) lParam;
    m_bActive = FALSE;
    SetColour(crNewColour);
	//BEGIN SRO
	CheckTextColour(crNewColour);
	//END SRO
	
	// 10-14-1999 - Kirk Stowell - Removed redundant calls to GetParent().
    if (m_pParentWnd) {
        m_pParentWnd->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        m_pParentWnd->SendMessage(CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
    }

    if (crNewColour != GetColour()) {
        if (m_pParentWnd) {
			m_pParentWnd->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());
		}
	}

    return TRUE;
}

LONG CColourPicker::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
    m_bActive = FALSE;
    SetColour((COLORREF) lParam);
	//BEGIN SRO
	CheckTextColour((COLORREF) lParam);
	//END SRO
	
	// 10-14-1999 - Kirk Stowell - Removed redundant calls to GetParent().
    if (m_pParentWnd) {
        m_pParentWnd->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        m_pParentWnd->SendMessage(CPN_SELENDCANCEL, lParam, (WPARAM) GetDlgCtrlID());
    }

    return TRUE;
}

LONG CColourPicker::OnSelChange(UINT lParam, LONG /*wParam*/)
{
    if (m_bTrackSelection) {
		SetColour((COLORREF) lParam);
		//BEGIN SRO
		CheckTextColour((COLORREF) lParam);
		//END SRO
	}
	
	// 10-14-1999 - Kirk Stowell - Removed redundant calls to GetParent().
    if (m_pParentWnd) {
		m_pParentWnd->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());
	}

    return TRUE;
}

int CColourPicker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    SetWindowSize();    // resize appropriately
    return 0;
}

// On mouse click, create and show a ColourPopup window for Colour selection
BOOL CColourPicker::OnClicked()
{
    m_bActive = TRUE;

    CRect rect;
    GetWindowRect(rect);
    new CColourPopup (CPoint(rect.left, rect.bottom),  // Point to display popup
                      GetColour(),                     // Selected Colour
                      this,                            // parent
                      m_strDefaultText,                // "Default" text area
                      m_strCustomText);                // Custom Text

	// 10-14-1999 - Kirk Stowell - Removed redundant calls to GetParent().
    if (m_pParentWnd) {
        m_pParentWnd->SendMessage(CPN_DROPDOWN, (LPARAM)GetColour(), (WPARAM) GetDlgCtrlID());
	}

    // Docs say I should return FALSE to stop the parent also getting the message.
    // HA! What a joke.

    return TRUE;
}

// 1999-06-11 begin mods KStowell
void CColourPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	// define some temporary variables.
	CDC*	pDC			= CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect	rcItem		= lpDrawItemStruct->rcItem;
    int 	nState		= lpDrawItemStruct->itemState;
	DWORD	dwState		= EDGE_RAISED;
	DWORD	dwArrow		= DFCS_SCROLLDOWN;

	// Paint the background.
	pDC->FillSolidRect(rcItem, ::GetSysColor(COLOR_3DFACE));

	// set display flags based on state.
	if((nState & ODS_SELECTED) || (m_bActive == TRUE)) {
		dwState = EDGE_SUNKEN;
		dwArrow = DFCS_SCROLLDOWN|DFCS_PUSHED;
	}
	if(nState & ODS_DISABLED) {
		dwArrow = DFCS_SCROLLDOWN|DFCS_INACTIVE;
	}
	
	// Draw the drop arrow.
	CRect rcArrow( rcItem );
	rcArrow.left = rcArrow.Width()-::GetSystemMetrics( SM_CXHTHUMB );
    pDC->DrawFrameControl( &rcArrow, DFC_SCROLL, dwArrow );
	pDC->Draw3dRect( rcArrow, ::GetSysColor( COLOR_3DFACE ), ::GetSysColor( COLOR_3DFACE ));
	rcArrow.DeflateRect( 1,1 );
	pDC->Draw3dRect( rcArrow, ::GetSysColor( COLOR_3DFACE ), ::GetSysColor( COLOR_3DFACE ));
	
	if((nState & ODS_SELECTED) || (m_bActive == TRUE))
		rcArrow.OffsetRect( 1,1 );
	
	// draw the seperator line.
	CPen penShadow( PS_SOLID, 1, ::GetSysColor (COLOR_3DSHADOW));
	CPen* pOldPen = pDC->SelectObject( &penShadow );
	pDC->MoveTo( rcArrow.left-1, rcArrow.top+2 );
	pDC->LineTo( rcArrow.left-1, rcArrow.bottom-2 );

	CPen penHilite( PS_SOLID, 1, ::GetSysColor (COLOR_3DHILIGHT));
	pDC->SelectObject( &penHilite );
	pDC->MoveTo( rcArrow.left, rcArrow.top+2 );
	pDC->LineTo( rcArrow.left, rcArrow.bottom-2 );
	
	// draw the control border.
    pDC->DrawEdge( rcItem, dwState, BF_RECT );
	
	// draw the focus rect.
	rcItem.DeflateRect( 2, 2 );
	pDC->Draw3dRect( rcItem, ::GetSysColor( COLOR_3DFACE ), ::GetSysColor( COLOR_3DFACE ) );
	if( nState & ODS_FOCUS ) {
		pDC->DrawFocusRect( rcItem );
	}
	
	// draw the colour box.
	if((nState & ODS_SELECTED) || (m_bActive == TRUE))
		rcItem.OffsetRect( 1,1 );
	
	rcItem.DeflateRect( 2, 2 );
	rcItem.right = rcArrow.left-4;
	
    CBrush brush( ((nState & ODS_DISABLED) || m_crColourBk == CLR_DEFAULT)? 
		::GetSysColor(COLOR_3DFACE) : m_crColourBk);
    CBrush* pOldBrush = (CBrush*)pDC->SelectObject( &brush );
	pDC->SelectStockObject(( nState & ODS_DISABLED )?WHITE_PEN:BLACK_PEN );
	pDC->Rectangle( rcItem );
	
    // Draw the window text (if any)
	CString strText;
    GetWindowText(strText);
    if (strText.GetLength())
    {
        pDC->SetBkMode(TRANSPARENT);
        if (nState & ODS_DISABLED)
        {
            rcItem.OffsetRect(1,1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(strText, rcItem, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
            rcItem.OffsetRect(-1,-1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(strText, rcItem, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
        else
        {
            pDC->SetTextColor((m_crColourText == CLR_DEFAULT)? 0 : m_crColourText);
            pDC->DrawText(strText, rcItem, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
    }

	// fix potential resource leak - KStowell - 10-21-99
    pDC->SelectObject( pOldPen );
    pDC->SelectObject( pOldBrush );
	brush.DeleteObject();
	penHilite.DeleteObject();
	penHilite.DeleteObject();
}
// 1999-06-11 end mods KStowell

/////////////////////////////////////////////////////////////////////////////
// CColourPicker overrides

void CColourPicker::PreSubclassWindow() 
{
    ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
    CButton::PreSubclassWindow();
    SetWindowSize();                     // resize appropriately
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker attributes

COLORREF CColourPicker::GetColour()
{ 
    return (m_nSelectionMode == CP_MODE_TEXT)? 
        GetTextColour(): GetBkColour(); 
}

void CColourPicker::SetColour(COLORREF crColour)
{ 
    (m_nSelectionMode == CP_MODE_TEXT)? 
        SetTextColour(crColour): SetBkColour(crColour); 
}

void CColourPicker::SetBkColour(COLORREF crColourBk)
{
    m_crColourBk = crColourBk;
	
	//BEGIN SRO
	CheckTextColour(crColourBk);
	//END SRO
	
    if (IsWindow(m_hWnd)) RedrawWindow();
}

void CColourPicker::SetTextColour(COLORREF crColourText)
{
    m_crColourText = crColourText;
    if (IsWindow(m_hWnd)) RedrawWindow();
}

void CColourPicker::SetDefaultText(LPCTSTR szDefaultText)
{
    m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
}

void CColourPicker::SetCustomText(LPCTSTR szCustomText)
{
    m_strCustomText = (szCustomText)? szCustomText : _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker implementation

void CColourPicker::SetWindowSize()
{
	// 10-14-1999 - Kirk Stowell - Added to help reduce flicker, we only need to make
	// a call to GetParent() once at initialization.
	m_pParentWnd = GetParent();

    // Get size dimensions of edges
    CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Get size of dropdown arrow
    int nArrowWidth = max(::GetSystemMetrics(SM_CXHTHUMB), 5*MarginSize.cx);
    int nArrowHeight = max(::GetSystemMetrics(SM_CYVTHUMB), 5*MarginSize.cy);
    CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));

    // Get window size
    CRect rect;
    GetWindowRect(rect);

    if (m_pParentWnd) {
        m_pParentWnd->ScreenToClient(rect);
	}

    // Set window size at least as wide as 2 arrows, and as high as arrow + margins
    int nWidth = max(rect.Width(), 2*ArrowSize.cx + 2*MarginSize.cx);
    MoveWindow(rect.left, rect.top, nWidth, ArrowSize.cy+2*MarginSize.cy, TRUE);

    // Get the new coords of this window
    GetWindowRect(rect);
    ScreenToClient(rect);

    // Get the rect where the arrow goes, and convert to client coords.
    m_ArrowRect.SetRect(rect.right - ArrowSize.cx - MarginSize.cx, 
                        rect.top + MarginSize.cy, rect.right - MarginSize.cx,
                        rect.bottom - MarginSize.cy);
}

void CColourPicker::CheckTextColour(COLORREF crColour)
{
	int R = GetRValue(crColour);
	int G = GetGValue(crColour);
	int B = GetBValue(crColour);
	
	double Intensity = .299 * R + .587 * G + .114 * B ;
	
	if ( Intensity >= 127 ) {
		SetTextColour(RGB(0,0,0));
	} else {
		SetTextColour(RGB(255,255,255));
	}
}

BOOL CColourPicker::OnEraseBkgnd(CDC* pDC) 
{
	// overridden for flicker-free drawing.
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CColourPicker::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// KStowell - Get the client rect.
	CRect rcClient, rcClip;
	dc.GetClipBox( &rcClip );
	GetClientRect( &rcClient );

	// KStowell - Create a memory device-context. This is done to help reduce
	// screen flicker, since we will paint the entire control to the
	// off screen device context first.
	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// KStowell - Repaint the background.
	memDC.FillSolidRect( rcClient, ::GetSysColor(COLOR_WINDOW) );

	// let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// KStowell - Copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
		rcClip.left, rcClip.top, SRCCOPY );

	// KStowell - Cleanup resources.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}
