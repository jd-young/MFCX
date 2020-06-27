/*!
     \file     XScrollView.cpp 
     
     \author   John Young
     \date     circa 1997
*/

#include "stdafx.h"
#include "../include/XScrollView.h"
#include	<afxpriv.h>		// To get the MFC message WM_RECALCPARENT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXScrollView


// Special mapping modes just for CXScrollView implementation
#define MM_NONE             0
#define MM_SCALETOFIT       (-1)
	// standard GDI mapping modes are > 0


IMPLEMENT_DYNCREATE (CXScrollView, CView)

CXScrollView::CXScrollView()
{
	Initialise();
}

void CXScrollView::Initialise()
{
	// Init everything to zero
	m_nMapMode = MM_NONE;
	m_sizeScalingFactor.cx = 
	m_sizeScalingFactor.cy = 1;
	m_ptScrollPos.x =
	m_ptScrollPos.y = 0;

	m_sizeMin.cx = 
	m_sizeMin.cy =
	m_sizeMax.cx =
	m_sizeMax.cy = 0;
}

CXScrollView::~CXScrollView()
{
}


BEGIN_MESSAGE_MAP (CXScrollView, CView)
	//{{AFX_MSG_MAP(CXScrollView)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXScrollView drawing

void CXScrollView::OnDraw(CDC* pDC)
{
     UNREFERENCED_PARAMETER (pDC);

	ASSERT ( FALSE );
}


/////////////////////////////////////////////////////////////////////////////
// CXScrollView painting

void CXScrollView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID (pDC);

#ifdef _DEBUG
	if ( m_nMapMode == MM_NONE )
	{
		TRACE0("Error: must call SetScrollSizes() or SetScaleToFitSize()");
		TRACE0("\tbefore painting scroll view.\n");
		ASSERT(FALSE);
		return;
	}
#endif //_DEBUG

	ASSERT (m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	switch (m_nMapMode)
	{
		case MM_SCALETOFIT:
			pDC->SetMapMode(MM_ANISOTROPIC);
			pDC->SetWindowExt(m_totalLog);  // window is in logical coordinates
			pDC->SetViewportExt(m_totalDev);
			if (m_totalDev.cx == 0 || m_totalDev.cy == 0)
				TRACE0("Warning: CXScrollView scaled to nothing.\n");
			break;

		default:
			ASSERT(m_nMapMode > 0);
			pDC->SetMapMode (m_nMapMode);
			break;
	}

	CPoint ptVpOrg (0, 0);       // assume no shift for printing
#if	0
	if ( !pDC->IsPrinting() )
	{
		ASSERT(pDC->GetWindowOrg() == CPoint(0,0));

		// by default shift viewport origin in negative direction of scroll
		ptVpOrg = -GetDeviceScrollPosition();

		if (m_bCenter)
		{
			CRect rect;
			GetClientRect(&rect);

			// if client area is larger than total device size,
			// override scroll positions to place origin such that
			// output is centered in the window
			if (m_totalDev.cx < rect.Width())
				ptVpOrg.x = (rect.Width() - m_totalDev.cx) / 2;
			if (m_totalDev.cy < rect.Height())
				ptVpOrg.y = (rect.Height() - m_totalDev.cy) / 2;
		}
	}
#endif
	pDC->SetViewportOrg(ptVpOrg);

	CView::OnPrepareDC(pDC, pInfo);     // For default Printing behavior
}

/////////////////////////////////////////////////////////////////////////////
// Set mode and scaling/scrolling sizes

void CXScrollView::SetScaleToFitSize(SIZE sizeTotal)
{
	// Note: It is possible to set sizeTotal members to negative values to
	//  effectively invert either the X or Y axis.

	ASSERT(m_hWnd != NULL);
	m_nMapMode = MM_SCALETOFIT;     // special internal value
	m_totalLog = sizeTotal;

	// reset and turn any scroll bars off
	if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL)))
	{
		SetScrollPos(SB_HORZ, 0);
		SetScrollPos(SB_VERT, 0);
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
	}

	CRect rectT;
	GetClientRect(rectT);
	m_totalDev = rectT.Size();

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate
		UpdateBars();
		Invalidate(TRUE);
	}
}

const AFX_DATADEF SIZE CXScrollView::sizeDefault = {0,0};

void CXScrollView::SetScrollSizes (int nMapMode, 
							SIZE sizeTotal,
							const SIZE& sizePage, 
							const SIZE& sizeLine)
{
	ASSERT (sizeTotal.cx >= 0 && sizeTotal.cy >= 0);
	ASSERT (nMapMode > 0);
	ASSERT (nMapMode != MM_ISOTROPIC && nMapMode != MM_ANISOTROPIC);

	int nOldMapMode = m_nMapMode;
	m_nMapMode = nMapMode;
	m_totalLog = sizeTotal;

	m_sizeScalingFactor.cx = sizeTotal.cx / 32678 + 1;
	m_sizeScalingFactor.cy = sizeTotal.cy / 32678 + 1;

	//BLOCK: convert logical coordinate space to device coordinates
	{
		CWindowDC dc(NULL);
		ASSERT(m_nMapMode > 0);
		dc.SetMapMode (m_nMapMode);

		// total size
		m_totalDev = m_totalLog;
		//dc.LPtoDP ((LPPOINT) &m_totalDev);
		m_pageDev = sizePage;
		//dc.LPtoDP ((LPPOINT)&m_pageDev);
		m_lineDev = sizeLine;
		//dc.LPtoDP ((LPPOINT)&m_lineDev);
		if ( m_totalDev.cy < 0 )
			m_totalDev.cy = -m_totalDev.cy;
		if ( m_pageDev.cy < 0 )
			m_pageDev.cy = -m_pageDev.cy;
		if ( m_lineDev.cy < 0 )
			m_lineDev.cy = -m_lineDev.cy;
	} // release DC here

	// now adjust device specific sizes
	ASSERT (m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	if ( m_pageDev.cx == 0)
		m_pageDev.cx = m_totalDev.cx / 10;
	if ( m_pageDev.cy == 0)
		m_pageDev.cy = m_totalDev.cy / 10;
	if ( m_lineDev.cx == 0)
		m_lineDev.cx = m_pageDev.cx / 10;
	if ( m_lineDev.cy == 0)
		m_lineDev.cy = m_pageDev.cy / 10;

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate now
		UpdateBars();
		if (nOldMapMode != m_nMapMode)
			Invalidate(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Getting information

CPoint CXScrollView::GetScrollPosition() const   // logical coordinates
{
	if (m_nMapMode == MM_SCALETOFIT)
	{
		return CPoint(0, 0);    // must be 0,0
	}

	CPoint pt = GetDeviceScrollPosition();
	// pt may be negative if m_bCenter is set

	if (m_nMapMode != MM_TEXT)
	{
		ASSERT(m_nMapMode > 0); // must be set
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
		dc.DPtoLP((LPPOINT)&pt);
	}
	return pt;
}

void CXScrollView::ScrollToPosition (POINT pt)    // logical coordinates
{
	ASSERT(m_nMapMode > 0);     // not allowed for shrink to fit
	if (m_nMapMode != MM_TEXT)
	{
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
		dc.LPtoDP((LPPOINT)&pt);
	}

	// now in device coordinates - limit if out of range
	int xMax = GetScrollLimit (SB_HORZ);
	int yMax = GetScrollLimit (SB_VERT);
	if (pt.x < 0)
		pt.x = 0;
	else if (pt.x > xMax)
		pt.x = xMax;
	if (pt.y < 0)
		pt.y = 0;
	else if (pt.y > yMax)
		pt.y = yMax;

	ScrollToDevicePosition(pt);
}

CPoint CXScrollView::GetDeviceScrollPosition() const
{
	CPoint pt (GetScrollPos (SB_HORZ), GetScrollPos (SB_VERT));
	ASSERT (pt.x >= 0 && pt.y >= 0);

	if ( m_bCenter )
	{
		CRect rect;
		GetClientRect (&rect);

		// if client area is larger than total device size,
		// the scroll positions are overridden to place origin such that
		// output is centered in the window
		// GetDeviceScrollPosition() must reflect this

		if ( m_totalDev.cx < rect.Width() )
			pt.x = -((rect.Width() - m_totalDev.cx) / 2);
		if ( m_totalDev.cy < rect.Height() )
			pt.y = -((rect.Height() - m_totalDev.cy) / 2);
	}

	return pt;
}

void CXScrollView::GetDeviceScrollSizes (int& nMapMode, 
								 SIZE& sizeTotal,
								 SIZE& sizePage, 
								 SIZE& sizeLine) const
{
	if ( m_nMapMode <= 0 )
		TRACE0("Warning: CXScrollView::GetDeviceScrollSizes returning invalid mapping mode.\n");

	nMapMode = m_nMapMode;
	sizeTotal = m_totalDev;
	sizePage = m_pageDev;
	sizeLine = m_lineDev;
}

void CXScrollView::ScrollToDevicePosition (POINT ptDev)
{
	ASSERT (ptDev.x >= 0);
	ASSERT (ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as CXScrollView is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos (SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos (SB_VERT);
	SetScrollPos (SB_VERT, ptDev.y);
	ScrollWindow (xOrig - ptDev.x, yOrig - ptDev.y);
}

/////////////////////////////////////////////////////////////////////////////
// Other helpers
#if	0
void CXScrollView::FillOutsideRect (CDC* pDC, CBrush* pBrush)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBrush);

	// fill rect outside the image
	CRect rect;
	GetClientRect (rect);
	ASSERT (rect.left == 0 && rect.top == 0);
	rect.left = m_totalDev.cx;
	if ( !rect.IsRectEmpty() )
		pDC->FillRect (rect, pBrush);    // vertical strip along the side
	rect.left = 0;
	rect.right = m_totalDev.cx;
	rect.top = m_totalDev.cy;
	if ( !rect.IsRectEmpty() )
		pDC->FillRect (rect, pBrush);    // horizontal strip along the bottom
}
#endif

void CXScrollView::ResizeParentToFit (BOOL bShrinkOnly)
{
	// adjust parent rect so client rect is appropriate size
	ASSERT (m_nMapMode != MM_NONE);  // mapping mode must be known

	// determine current size of the client area as if no scrollbars present
	CRect rectClient;
	GetWindowRect (rectClient);
	CRect rect = rectClient;
	CalcWindowRect (rect);
	rectClient.left += rectClient.left - rect.left;
	rectClient.top += rectClient.top - rect.top;
	rectClient.right -= rect.right - rectClient.right;
	rectClient.bottom -= rect.bottom - rectClient.bottom;
	rectClient.OffsetRect(-rectClient.left, -rectClient.top);
	ASSERT(rectClient.left == 0 && rectClient.top == 0);

	// determine desired size of the view
	CRect rectView (0, 0, m_totalDev.cx, m_totalDev.cy);
	if ( bShrinkOnly )
	{
		if ( rectClient.right <= m_totalDev.cx )
			rectView.right = rectClient.right;
		if ( rectClient.bottom <= m_totalDev.cy )
			rectView.bottom = rectClient.bottom;
	}

	CalcWindowRect (rectView, CWnd::adjustOutside);
	rectView.OffsetRect (-rectView.left, -rectView.top);
	ASSERT (rectView.left == 0 && rectView.top == 0);
	if ( bShrinkOnly )
	{
		if ( rectClient.right <= m_totalDev.cx )
			rectView.right = rectClient.right;
		if ( rectClient.bottom <= m_totalDev.cy )
			rectView.bottom = rectClient.bottom;
	}

	// dermine and set size of frame based on desired size of view
	CRect rectFrame;
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID (pFrame);
	pFrame->GetWindowRect (rectFrame);
	CSize size = rectFrame.Size();
	size.cx += rectView.right - rectClient.right;
	size.cy += rectView.bottom - rectClient.bottom;
	pFrame->SetWindowPos (NULL, 0, 0, size.cx, size.cy,
					  SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}

/////////////////////////////////////////////////////////////////////////////
// CXScrollView message handlers

void CXScrollView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize (nType, cx, cy);
	if (m_nMapMode == MM_SCALETOFIT)
	{
		// force recalculation of scale to fit parameters
		SetScaleToFitSize (m_totalLog);
	}
	else
	{
		// UpdateBars() handles locking out recursion
		UpdateBars();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Scrolling Helpers
#if	0
void CXScrollView::CenterOnPoint(CPoint ptCenter) // center in device coords
{
	CRect rect;
	GetClientRect(&rect);           // find size of client window

	int xDesired = ptCenter.x - rect.Width() / 2;
	int yDesired = ptCenter.y - rect.Height() / 2;

	DWORD dwStyle = GetStyle();

	if ((dwStyle & WS_HSCROLL) == 0 || xDesired < 0)
	{
		xDesired = 0;
	}
	else
	{
		int xMax = GetScrollLimit(SB_HORZ);
		if (xDesired > xMax)
			xDesired = xMax;
	}

	if ((dwStyle & WS_VSCROLL) == 0 || yDesired < 0)
	{
		yDesired = 0;
	}
	else
	{
		int yMax = GetScrollLimit(SB_VERT);
		if (yDesired > yMax)
			yDesired = yMax;
	}

	ASSERT(xDesired >= 0);
	ASSERT(yDesired >= 0);

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, xDesired);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, yDesired);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behaviour

void CXScrollView::GetScrollBarSizes (CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if ( GetScrollBarCtrl (SB_VERT) == NULL )
	{
		// vert scrollbars will impact client area of this window
		
		sizeSb.cx = // afxData.cxVScroll;	// This works with MFC v4.0
				  ::GetSystemMetrics (SM_CXVSCROLL) + 1;	//CX_BORDER;	
		if ( dwStyle & WS_BORDER )
			sizeSb.cx -= 1;		// CX_BORDER;
	}
	if ( GetScrollBarCtrl (SB_HORZ) == NULL )
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = // afxData.cyHScroll;
				  ::GetSystemMetrics (SM_CYHSCROLL) + 1;	// CY_SCROLL
		if ( dwStyle & WS_BORDER )
			sizeSb.cy -= 1;		//CY_BORDER;
	}
}

BOOL CXScrollView::GetTrueClientSize (CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes (sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if ( sizeSb.cx != 0 && (dwStyle & WS_VSCROLL) )
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if ( sizeSb.cy != 0 && (dwStyle & WS_HSCROLL) )
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

/**
     Helper to return the state of the scrollbars without actually changing
     the state of the scrollbars.

\param    sizeCLient          The size of the client area.
\param    needSb              Filled in if the client area needs scrollbars or
                              not.
\param    sizeRange           ?
\param    ptMove              ?
\param    bInsideClient       ?
*/
void CXScrollView::GetScrollBarState (CSize sizeClient, 
							   CSize& needSb,
							   CSize& sizeRange, 
							   CPoint& ptMove, 
							   BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_totalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

void CXScrollView::UpdateBars()
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT (m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	// allow parent to do inside-out layout first
	CWnd* pParentWnd = GetParent();
	if ( pParentWnd != NULL )
	{
		// if parent window responds to this message, use just
		//  client area for scroll bar calc -- not "true" client area
		if ( (BOOL) pParentWnd->SendMessage (WM_RECALCPARENT, 0,
									 (LPARAM)(LPCRECT)&rectClient) != 0 )
		{
			// use rectClient instead of GetTrueClientSize for
			//  client size calculation.
			bCalcClient = FALSE;
		}
	}

	CSize sizeClient;
	CSize sizeSb;

	if ( bCalcClient )
	{
		// get client rect
		if ( !GetTrueClientSize(sizeClient, sizeSb) )
		{
			// no room for scroll bars (common for zero sized elements)
			CRect rect;
			GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// if entire client area is not invisible, assume we have
				//  control over our scrollbars
				EnableScrollBarCtrl (SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// let parent window determine the "client" rect
		sizeClient.cx = rectClient.right - rectClient.left;
		sizeClient.cy = rectClient.bottom - rectClient.top;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState (sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if ( needSb.cx )
		sizeClient.cy -= sizeSb.cy;
	if ( needSb.cy )
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition (ptMove);	// will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if ( needSb.cx )
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_totalDev.cx-1;
		if ( !SetScrollInfo (SB_HORZ, &info, TRUE) )
			SetScrollRange (SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl (SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_totalDev.cy-1;
		if ( !SetScrollInfo (SB_VERT, &info, TRUE) )
			SetScrollRange (SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}


void CXScrollView::CalcWindowRect (LPRECT lpClientRect, UINT nAdjustType)
{
	if ( nAdjustType == adjustOutside )
	{
		// allow for special client-edge style
		::AdjustWindowRectEx (lpClientRect, 0, FALSE, GetExStyle());

		if (m_nMapMode != MM_SCALETOFIT)
		{
			// if the view is being used in-place, add scrollbar sizes
			//  (scollbars should appear on the outside when in-place editing)
			CSize sizeClient(
				lpClientRect->right - lpClientRect->left,
				lpClientRect->bottom - lpClientRect->top);

			CSize sizeRange = m_totalDev - sizeClient;
				// > 0 => need to scroll

			// get scroll bar sizes (used to adjust the window)
			CSize sizeSb;
			GetScrollBarSizes(sizeSb);

			// adjust the window size based on the state
			if ( sizeRange.cy > 0 )
			{   // vertical scroll bars take up horizontal space
				lpClientRect->right += sizeSb.cx;
			}
			if ( sizeRange.cx > 0 )
			{   // horizontal scroll bars take up vertical space
				lpClientRect->bottom += sizeSb.cy;
			}
		}
	}
	else
	{
		// call default to handle other non-client areas
		::AdjustWindowRectEx (lpClientRect, GetStyle(), FALSE,
			GetExStyle() & ~(WS_EX_CLIENTEDGE));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXScrollView scrolling

void CXScrollView::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if ( pScrollBar != GetScrollBarCtrl (SB_HORZ) )
		return;

	if ( nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK )
		nPos *= m_sizeScalingFactor.cx;

	OnScroll (MAKEWORD(nSBCode, -1), nPos);
}

void CXScrollView::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if ( pScrollBar != GetScrollBarCtrl(SB_VERT) )
		return;

	if ( nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK )
		nPos *= m_sizeScalingFactor.cy;

	OnScroll (MAKEWORD (-1, nSBCode), nPos);
}

BOOL CXScrollView::OnScroll (UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// calc new x position
	int x = GetScrollPos (SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
		case SB_TOP:
			x = 0;
			break;
		case SB_BOTTOM:
			x = INT_MAX;
			break;
		case SB_LINEUP:
			x -= m_lineDev.cx;
			break;
		case SB_LINEDOWN:
			x += m_lineDev.cx;
			break;
		case SB_PAGEUP:
			x -= m_pageDev.cx;
			break;
		case SB_PAGEDOWN:
			x += m_pageDev.cx;
			break;
		case SB_THUMBTRACK:
			x = nPos;
			break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
		case SB_TOP:
			y = 0;
			break;
		case SB_BOTTOM:
			y = INT_MAX;
			break;
		case SB_LINEUP:
			y -= m_lineDev.cy;
			break;
		case SB_LINEDOWN:
			y += m_lineDev.cy;
			break;
		case SB_PAGEUP:
			y -= m_pageDev.cy;
			break;
		case SB_PAGEDOWN:
			y += m_pageDev.cy;
			break;
		case SB_THUMBTRACK:
			y = nPos;
			break;
	}

	BOOL bResult = OnScrollBy (CSize(x - xOrig, y - yOrig), bDoScroll);
	if ( bResult && bDoScroll )
		UpdateWindow();

	return bResult;
}

BOOL CXScrollView::OnScrollBy (CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl (SB_VERT);
	if ( (pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)) )
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl (SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos (SB_HORZ);
	int xMax = GetScrollLimit (SB_HORZ);
	x += sizeScroll.cx;
	if ( x < 0 )
		x = 0;
	else if ( x > xMax )
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos (SB_VERT);
	int yMax = GetScrollLimit (SB_VERT);
	y += sizeScroll.cy;
	if ( y < 0 )
		y = 0;
	else if ( y > yMax )
		y = yMax;

	// did anything change?
	if ( x == xOrig && y == yOrig )
		return FALSE;

	if ( bDoScroll )
	{
		// do scroll and update scroll positions
          ASSERT ( x == xOrig );   // For PrEditor only
		ScrollWindow (-(x-xOrig), -(y-yOrig));
		if ( x != xOrig )
			SetScrollPos (SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos (SB_VERT, y);
	}
	return TRUE;
}



int CXScrollView::SetScrollPos (int nBar, int nPos, BOOL bRedraw /*= TRUE*/)
{
	int	nFactor;
	if ( nBar == SB_HORZ )
	{
		m_ptScrollPos.x = nPos;
		nFactor = m_sizeScalingFactor.cx;
	}
	else
	{
		m_ptScrollPos.y = nPos;
		nFactor = m_sizeScalingFactor.cy;
	}
	nPos /= nFactor;
	return CView::SetScrollPos (nBar, nPos, bRedraw) * nFactor;
}

int CXScrollView::GetScrollPos (int nBar) const
{
	int  nPos;
	if ( nBar == SB_HORZ )
		nPos = m_ptScrollPos.x;
	else nPos = m_ptScrollPos.y;
	return nPos;
}


int	CXScrollView::GetScrollLimit (int nBar)
{
	if ( nBar == SB_HORZ )
		return m_sizeMax.cx;
	else return m_sizeMax.cy;
}

void CXScrollView::SetScrollRange (int nBar,
							 int nMinPos, 
							 int nMaxPos, 
							 BOOL bRedraw/* = TRUE*/)
{
	ASSERT ( nMinPos >= nMaxPos );

	int	nFactor = nMaxPos / 32678 + 1;
	if ( nBar == SB_HORZ )
	{
		m_sizeMin.cx = nMinPos;
		m_sizeMax.cx = nMaxPos;
		m_sizeScalingFactor.cx = nFactor;
	}
	else
	{
		m_sizeMin.cy = nMinPos;
		m_sizeMax.cy = nMaxPos;
		m_sizeScalingFactor.cy = nFactor;
	}
	nMinPos /= nFactor;
	nMaxPos /= nFactor;

	CScrollBar* pScrollBar;
	if ( (pScrollBar = GetScrollBarCtrl (nBar)) != NULL )
		pScrollBar->SetScrollRange (nMinPos, nMaxPos, bRedraw);
	else ::SetScrollRange (m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
}


BOOL CXScrollView::SetScrollInfo (int nBar, SCROLLINFO* pScrollInfo, BOOL bRedraw /*= TRUE*/)
{
	ASSERT (pScrollInfo != NULL);
	DWORD dwVersion = ::GetVersion();
	int	nWinVer = (LOBYTE(dwVersion) << 8) + HIBYTE(dwVersion);
	if ( nWinVer < 0x333 )
		return FALSE;

	if ( pScrollInfo->fMask & SIF_RANGE )
	{
		int nFactor = pScrollInfo->nMax / 32678 + 1;
		if ( nBar == SB_HORZ )
		{
			m_sizeMin.cx = pScrollInfo->nMin;
			m_sizeMax.cx = pScrollInfo->nMax;
			m_sizeScalingFactor.cx = nFactor;
		}
		else
		{
			m_sizeMin.cy = pScrollInfo->nMin;
			m_sizeMax.cy = pScrollInfo->nMax;
			m_sizeScalingFactor.cy = nFactor;
		}

		pScrollInfo->nMin /= nFactor;
		pScrollInfo->nMax /= nFactor;
	}

	int	nFactor;
	if ( nBar == SB_HORZ )
		nFactor = m_sizeScalingFactor.cx;
	else	nFactor = m_sizeScalingFactor.cy;
		
	if ( pScrollInfo->fMask & SIF_PAGE )
	{
		if ( nBar == SB_HORZ )
			m_sizePage.cx = pScrollInfo->nPage;
		else m_sizePage.cy = pScrollInfo->nPage;
		pScrollInfo->nPage /= nFactor;
	}
	
	if ( pScrollInfo->fMask & SIF_POS )
	{
		if ( nBar == SB_HORZ )
			m_ptScrollPos.x = pScrollInfo->nPos;
		else m_ptScrollPos.y = pScrollInfo->nPos;
		pScrollInfo->nPos /= nFactor;
	}
		
	HWND hWnd = m_hWnd;
	CScrollBar* pScrollBar;
	if ( nBar != SB_CTL && (pScrollBar = GetScrollBarCtrl (nBar)) != NULL )
	{
		hWnd = pScrollBar->m_hWnd;
		nBar = SB_CTL;
	}

	pScrollInfo->cbSize = sizeof SCROLLINFO;
	::SetScrollInfo (hWnd, nBar, pScrollInfo, bRedraw);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CXScrollView diagnostics

#ifdef _DEBUG
void CXScrollView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);

	dc << "m_totalLog = " << m_totalLog;
	dc << "\nm_totalDev = " << m_totalDev;
	dc << "\nm_pageDev = " << m_pageDev;
	dc << "\nm_lineDev = " << m_lineDev;
	dc << "\nm_bCenter = " << m_bCenter;
	dc << "\nm_bInsideUpdate = " << m_bInsideUpdate;
	dc << "\nm_nMapMode = ";
	switch (m_nMapMode)
	{
		case MM_NONE:
			dc << "MM_NONE";
			break;
		case MM_SCALETOFIT:
			dc << "MM_SCALETOFIT";
			break;
		case MM_TEXT:
			dc << "MM_TEXT";
			break;
		case MM_LOMETRIC:
			dc << "MM_LOMETRIC";
			break;
		case MM_HIMETRIC:
			dc << "MM_HIMETRIC";
			break;
		case MM_LOENGLISH:
			dc << "MM_LOENGLISH";
			break;
		case MM_HIENGLISH:
			dc << "MM_HIENGLISH";
			break;
		case MM_TWIPS:
			dc << "MM_TWIPS";
			break;
		default:
			dc << "*unknown*";
			break;
	}

	dc << "\n";
}

void CXScrollView::AssertValid() const
{
	CView::AssertValid();

	switch (m_nMapMode)
	{
		case MM_NONE:
		case MM_SCALETOFIT:
		case MM_TEXT:
		case MM_LOMETRIC:
		case MM_HIMETRIC:
		case MM_LOENGLISH:
		case MM_HIENGLISH:
		case MM_TWIPS:
			break;
		case MM_ISOTROPIC:
		case MM_ANISOTROPIC:
			ASSERT(FALSE); // illegal mapping mode
		default:
			ASSERT(FALSE); // unknown mapping mode
	}
}
#endif //_DEBUG


