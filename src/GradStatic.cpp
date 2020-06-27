//
//   GradStatic.cpp      Gradient CStatic 
//
//   Written by John Young, 4-Jun-2000
//   Copyright (c) 2000 Young Associates.
//
//

#include "stdafx.h"
#include "../include/GradStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CGradStatic

CGradStatic::CGradStatic()
{
     m_crText = RGB (0, 0, 0);          // Black
     m_bHorz = true;
     m_bActive = true;
     m_crActiveStart = ::GetSysColor (COLOR_ACTIVECAPTION);
     m_crActiveFinish = ::GetSysColor (COLOR_BTNFACE);
     m_crInactiveStart = ::GetSysColor (COLOR_INACTIVECAPTION);
     m_crInactiveFinish = ::GetSysColor (COLOR_BTNFACE);
     
     m_pFont = NULL;
}

CGradStatic::~CGradStatic()
{
     delete m_pFont;
}


void CGradStatic::SetGradientColours (COLORREF crActiveStart, COLORREF crActiveFinish,
                                      COLORREF crInactiveStart /*= 0xffffffff*/, 
                                      COLORREF crInactiveFinish /*= 0xffffffff*/)
{
     m_crActiveStart = crActiveStart;
     m_crActiveFinish = crActiveFinish;
     
     if ( crInactiveStart == 0xffffffff )
          m_crInactiveStart = m_crActiveStart;
     else m_crInactiveStart = crInactiveStart;
     
     if ( crInactiveFinish == 0xffffffff )
          m_crInactiveFinish = m_crActiveFinish;
     else m_crInactiveFinish = crInactiveFinish;
     Invalidate();
}

void CGradStatic::SetHorzGradient (bool f /*= true*/)
{
     if ( m_bHorz != f )
     {
          m_bHorz = f;
          Invalidate();
     }
}


void CGradStatic::SetTextColour (COLORREF cr)
{
     if ( m_crText != cr )
     {
          m_crText = cr;
          Invalidate();
     }
}


void CGradStatic::SetFont (CFont* pFont, BOOL bRedraw /*= TRUE*/)
{
     if ( m_pFont )
          m_pFont->DeleteObject();
     else m_pFont = new CFont();
     
     if ( m_pFont )
     {
          LOGFONT lf;
          pFont->GetLogFont (&lf);
          m_pFont->CreateFontIndirect (&lf);
          CStatic::SetFont (m_pFont);
     }
}


void CGradStatic::DrawGradient (CDC* pDC)
{
     // draw the caption background

     CRect rectClient;
     GetClientRect (rectClient);
     
     COLORREF clrCptn = m_bActive ? m_crActiveStart : m_crInactiveStart;
     COLORREF clrCptnRight = m_bActive ? m_crActiveFinish : m_crInactiveFinish;

     // this will make 2^6 = 64 fountain steps
     int nShift = 6;
     int nSteps = 1 << nShift;
     
     for (int i = 0; i < nSteps; i++)
     {
     	// do a little alpha blending
     	BYTE bR = (BYTE) ((GetRValue(clrCptn) * (nSteps - i) +
     						 GetRValue(clrCptnRight) * i) >> nShift);
     	BYTE bG = (BYTE) ((GetGValue(clrCptn) * (nSteps - i) +
     						 GetGValue(clrCptnRight) * i) >> nShift);
     	BYTE bB = (BYTE) ((GetBValue(clrCptn) * (nSteps - i) +
     						 GetBValue(clrCptnRight) * i) >> nShift);
     
     	CBrush br (RGB(bR, bG, bB));
     
     	// then paint with the resulting colour
     	CRect r2 = rectClient;
     	if ( m_bHorz )
     	{
			r2.left = rectClient.left + ((i * rectClient.Width()) >> nShift);
			r2.right = rectClient.left + (((i + 1) * rectClient.Width()) >> nShift);
			if ( r2.Width() > 0 )
			     pDC->FillRect (r2, &br);
     	}
     	else
     	{
			r2.bottom = rectClient.bottom - ((i * rectClient.Height()) >> nShift);
			r2.top = rectClient.bottom - (((i + 1) * rectClient.Height()) >> nShift);
			if ( r2.Height() > 0 )
                    pDC->FillRect(r2, &br);

     	}
     }
}

#if  0
     // Create the palette
     UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
	LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];		
	pLP->palVersion = 0x300;
	pLP->palNumEntries = nColors;		
	for( int i=0; i < nColors; i++)		
	{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
	}
	pal.CreatePalette( pLP );		
	delete[] pLP;
	delete[] pRGB;



     CDC memDC;
     memDC.CreateCompatibleDC (&dc);
     CBitmap bitmap;	
     CPalette palette;
	GetBitmapAndPalette( IDB_BITMAP, bitmap, palette );
	
	memDC.SelectObject( &bitmap );	// Select and realize the palette
	if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && palette.m_hObject != NULL )	
	{
		dc.SelectPalette( &palette, FALSE );		
		dc.RealizePalette();	
	}
	dc.BitBlt(0, 0, 180, 180, &memDC, 0, 0,SRCCOPY);
#endif

BEGIN_MESSAGE_MAP(CGradStatic, CStatic)
	//{{AFX_MSG_MAP(CGradStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
     ON_MESSAGE( WM_SETTEXT, OnSetText )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradStatic message handlers

void CGradStatic::OnPaint() 
{
     CPaintDC dc (this);           // device context for painting

     CFont* pOldFont = dc.SelectObject (GetFont());

	// Draw text
	CString sText;
	GetWindowText (sText);

     dc.SetTextColor (m_crText);
     dc.SetBkMode (TRANSPARENT);

     CRect rect;
     GetClientRect (rect); 
	rect.left += 5;
	dc.DrawText (sText, rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	if ( pOldFont )
		dc.SelectObject (pOldFont);
}

BOOL CGradStatic::OnEraseBkgnd(CDC* pDC) 
{
     DrawGradient (pDC);
     return TRUE;
}


LRESULT CGradStatic::OnSetText(WPARAM wParam, LPARAM lParam)
{
	DefWindowProc (WM_SETTEXT, wParam, lParam);
	Invalidate (TRUE);
	return (TRUE);
}
