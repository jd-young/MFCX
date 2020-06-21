// ColourPicker.h : header file
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
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 2004/07/19 00:25:55 $
 * $Revision: 1.1.1.1 $
 * $Archive: /CodeJock/Include/ColourPicker.h $
 *
 * $History: ColourPicker.h $
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 10/31/99   Time: 11:50p
 * Updated in $/CodeJock/Include
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
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 10/14/99   Time: 12:22p
 * Updated in $/CodeJock/Include
 * Added source control history to file header.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __MFCX_COLOURPICKER_H
#define __MFCX_COLOURPICKER_H

#include "ColourPopup.h"

/////////////////////////////////////////////////////////////////////////////
// CColourPicker window

void AFXAPI DDX_ColourPicker(CDataExchange *pDX, int nIDC, COLORREF& crColour);

/////////////////////////////////////////////////////////////////////////////
// CColourPicker window

#define CP_MODE_TEXT 1  // edit text Colour
#define CP_MODE_BK   2  // edit background Colour (default)

class CColourPicker : public CButton
{
// Construction
public:
    CColourPicker();
    DECLARE_DYNCREATE(CColourPicker);

// Attributes
public:
    virtual COLORREF GetColour();
    virtual void     SetColour(COLORREF crColour); 

    virtual void     SetDefaultText(LPCTSTR szDefaultText);
    virtual void     SetCustomText(LPCTSTR szCustomText);

    virtual void     SetTrackSelection(BOOL bTracking = TRUE)  { m_bTrackSelection = bTracking; }
    virtual BOOL     GetTrackSelection()                       { return m_bTrackSelection; }

    virtual void     SetSelectionMode(UINT nMode)              { m_nSelectionMode = nMode; }
    virtual UINT     GetSelectionMode()                        { return m_nSelectionMode; };

    virtual void     SetBkColour(COLORREF crColourBk);
    virtual COLORREF GetBkColour()                             { return m_crColourBk; }
    
    virtual void     SetTextColour(COLORREF crColourText);
    virtual COLORREF GetTextColour()                           { return m_crColourText;}

	virtual void	 CheckTextColour(COLORREF crColour);

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColourPicker)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CColourPicker();

protected:
    void SetWindowSize();

// protected attributes
protected:
    BOOL     m_bActive,                // Is the dropdown active?
             m_bTrackSelection;        // track Colour changes?
    COLORREF m_crColourBk;
    COLORREF m_crColourText;
    UINT     m_nSelectionMode;
    CRect    m_ArrowRect;
    CString  m_strDefaultText;
    CString  m_strCustomText;
	bool	 m_bLBtnDown;
	CWnd*	 m_pParentWnd;				// holds a pointer to the parent window.

    // Generated message map functions
protected:
    //{{AFX_MSG(CColourPicker)
    afx_msg BOOL OnClicked();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MFCX__COLOURPICKER_H
