//   GradStatic.h        Gradient Static


#ifndef   __MFCX_GRADSTATIC_H
#define   __MFCX_GRADSTATIC_H


/// A gradient background CStatic class.
/**	A gradient background static text window.  This is usually used in a 
	dialog box.  Create a CStatic class in your dialog resource with, say, a 
	resource ID of IDC_GRADSTATIC.
	
\verbatim
	class CMyDialog : public CDialog
	{
		:
		CGradStatic m_gradStatic;
		:
	};
	
	DDX_Control(pDX, IDC_GRADSTATIC, m_gradStatic);



     m_gradStatic.SetTextColour (RGB (255, 255, 255));  // White
     m_edFilename.SetWindowText (m_sInitFilename);

     CFont font;
     font.CreateFont (18, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
				 OUT_DEFAULT_PRECIS, 
				 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				 FF_MODERN, "Arial");
     
     m_gradStatic.SetFont (&font);
     m_gradStatic.SetActive (false);
     m_gradStatic.SetWindowText ("");


\endverbatim

*/
class CGradStatic : public CStatic
{
// Construction
public:
	CGradStatic();

// Attributes
public:

	/// Sets the active and inactive background colours.
     void SetGradientColours (COLORREF crActiveStart, COLORREF crActiveFinish,
                              COLORREF crInactiveStart = 0xffffffff, 
                              COLORREF crInactiveFinish = 0xffffffff);
     
     /// Sets the gradient orientation.
     void SetHorzGradient (bool f = true);
     
     /// Sets the text colour for the control.
     void SetTextColour (COLORREF cr);
     
     /// Sets the font in which to display the text.
     void SetFont (CFont* pFont, BOOL bRedraw = TRUE);

	/// Sets whether to display the control as active or inactive.
     void SetActive (bool f = true) { m_bActive = f; Invalidate(); }

protected:
	/// The orientation of the gradient.
     bool m_bHorz;
     bool m_bActive;
     COLORREF m_crText;            // The text colour
     COLORREF m_crActiveStart,     // Gradient start and stop colours
              m_crActiveFinish;
     COLORREF m_crInactiveStart,
              m_crInactiveFinish;

     CFont*   m_pFont;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradStatic();

protected:
     void DrawGradient (CDC* pDC);

	// Generated message map functions
protected:
	//{{AFX_MSG(CGradStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetText (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif    // __MFCX_GRADSTATIC_H
