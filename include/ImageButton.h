// ImageButton.h : header file
//

#ifndef	__MFCX_IMAGEBUTTON_H
#define 	__MFCX_IMAGEBUTTON_H

/////////////////////////////////////////////////////////////////////////////
// CImageButton window

class CImageButton : public CButton
{
// Construction
public:
	CImageButton(UINT up, UINT down = 0, UINT disabled = 0, BOOL fill = FALSE);
	// Default constructor (required for MFC compatibility)
	CImageButton() {up = 0; down = 0; disabled = 0; m_fill = FALSE; }
	BOOL m_fill; // We want to fill the image
	DWORD SetHPos(DWORD style); // Set the horizonatal alignment style
	DWORD SetVPos(DWORD style); // Set the vertical alignment style
	//----------------------
	// LoadBitmaps
	//	UINT up: Resource ID of image to use for up, enabled
	//	UINT down: Resource ID of image to use for down, enabled
	//			if NULL, the up image is used
	//	UINT disabled: Resource ID of image to use for disabled
	//			if NULL, the up image is halftoned gray
	void LoadBitmaps(UINT up, UINT down = 0, UINT disabled = 0)
	      { this->up = up; this->down = down; this->disabled = disabled; } 
	void GetBitmaps(UINT &up, UINT &down, UINT &disabled)
	      { up = this->up; down = this->down; disabled = this->disabled; }
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageButton();

	// Generated message map functions
protected:
        UINT up;
	UINT down;
	UINT disabled;
	//{{AFX_MSG(CImageButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __MFCX_IMAGEBUTTON_H
