/*!
     \file     Dragger.cpp		Generic dragger class

     \author   John Young
     \date     21-Dec-97

     Copyright (c) 1997 Thistle Software
*/

#include "stdafx.h"
#include "../include/Dragger.h"
#include "../include/GMouse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDragger::CDragger()
{
	m_dragOp = 0;
	m_ptPrev =
	m_ptCur  = CPoint (0, 0);

	m_rcOrg = 
	m_rcPrev =
	m_rcCur = CRect (0, 0, 0, 0);

	m_rcClip = CRect (0, 0, 0, 0);
	m_pCapWnd = NULL;

}


void CDragger::Begin (CPoint pt, int dragop, CWnd *pWnd /*=NULL*/, CRect* pRc /*=NULL*/)
{
	// Called when dragging begins.
	// pt     = current mouse location
	// Optional:
	//	dragop = app-specific drag mode (0=no dragging, dflt = 1)
	//	pWnd   = ptr to window to capture mouse, or NULL
	//	pRc    = current rectangle (optional)
	//

	m_dragOp = dragop;
	m_ptOrig = pt;

	if ( pRc )
		m_rcPrev = m_rcOrg = m_rcCur = *pRc;
	else m_rcPrev = m_rcOrg = m_rcCur = CRect (0, 0, 0, 0);

	m_rcClip.SetRectEmpty();

	// Maybe capture mouse
	if ( pWnd ) 
		pWnd->SetCapture();
	m_pCapWnd = pWnd;
}


void CDragger::End()
{
	// Called to end dragging
	m_dragOp = FALSE;             // no longer dragging
	if ( m_pCapWnd)
		ReleaseCapture();
	if ( !m_rcClip.IsRectEmpty() )
		theMouse.UnClip();
 }

void CDragger::BeginMove (CPoint pt)
{
	// Start of two-step move operation
	// move current location to new point
	m_ptCur = pt;
	m_rcCur += m_ptCur;
	m_rcCur -= m_ptPrev;
}

void CDragger::EndMove()
{
	// End two-step move operation
	// update previous location to current 
	m_ptPrev = m_ptCur;
	m_rcPrev = m_rcCur;
}

void CDragger::Clip (CRect& rcScreen)
{
	// Set cursor clipping rectangle
	m_rcClip = rcScreen;
	theMouse.Clip (m_rcClip);
}


BOOL CDragger::operator==(int drag) const
{
	return drag == m_dragOp;
}

BOOL CDragger::operator!=(int drag) const
{
	return drag != m_dragOp;
}

