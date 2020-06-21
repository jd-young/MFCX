/*!
     \file     GMouse.cpp          The global mouse object.

     \author   John Young
     \date     13 Nov 95

     Modified from ideas by Paul DiLascia's 1993 MFX

     TheMouse contains cursor shape, clipping, and capture features
*/

#include "StdAfx.h"
#include "../include/GMouse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Instantiate global objects
GMouse      theMouse;


GMouse::~GMouse()
{
 	if ( m_bClipped )
 		UnClip();
}


void GMouse::Clip (CWnd *pWnd)
{
	ASSERT (pWnd);
	CRect rc;
	pWnd->GetClientRect (&rc);	// get window's client area..
	pWnd->ClientToScreen (&rc);	// ..convert to screen coords
	Clip (rc);				// call overloaded clip fn
};


HCURSOR GMouse::LoadCursor (LPCSTR name)
{    // Use this instead of CWinApp::LoadCursor() / LoadStandardCursor()
	// It works because we know the Microsoft hack that all IDC_XXX cursors 
	// have a HIWORD of zero, and uses it to do the right thing.
	
     return ::LoadCursor (HIWORD (name) ? AfxGetApp()->m_hInstance : NULL, name);
}

