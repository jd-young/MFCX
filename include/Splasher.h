/*
	\file	Splasher.h

	A splash screen component for MFC 4.x which uses a DIB bitmap instead of a 
	resource. Palette management code is also included so that the bitmap will 
	display using its own optimized palette.

	\author PJ Naughter
	\author John Young 
	\date   15-11-1996

Copyright (c) 1996 - 2000 by PJ Naughter.  
All rights reserved.

HISTORY:
	circa Jun 2002	Corrupted beyond belief by John Young.

*/


#ifndef __MFCX_SPLASHER_H__
#define __MFCX_SPLASHER_H__

class CSplash;			// The splasher class.
class CSplashThread;	// The thread on which the splash screen is displayed.
class CSplashWnd;		// The splash window itself.


/*!	An application splash screen.  It is a 'fire-and-forget' style of class, it
	creates a separate thread to take care of drawing the splash bitmap, 
	look after mouse clicks, all behind the scenes.
	
	To use this simply insert the following code into your code (usually in the 
	InitInstance() method of your MFC app class)...
	
\verbatim
	BOOL CMyApp::InitInstance()
	{
		:
		CSplash splash (IDB_SPLASH, 5000, RUNTIME_CLASS (CMySplashWnd));
		:

	}
\endverbatim
	
	where
	- IDB_SPLASH		is the resource ID of the bitmap to display.
	- 5000			means display for 5 seconds.
	- CMySplashWnd		is your own CSplashWnd derived class.  In this class
					you can do stuff like override the OnDraw() method to
					add stuff like the version / build number, copyright
					message (with the current year), or any other 'dynamic'
					text or bitmaps.  If you omit this, then CSplashWnd is
					used, which does a pretty good job on it's own.
	
	This class will take care of all the rest.  Note that the class object 
	itself can disappear straight after creating the splash thread and window
	(as in the example above).
*/
class CSplash
{
public:
     CSplash (UINT nResourceID, int nDuration = -1, 
     	    CRuntimeClass* pSplashClass = NULL);
     CSplash (const CString& sFilename, int nDuration = -1, 
     	    CRuntimeClass* pSplashClass = NULL);
     ~CSplash();

protected:
	void Create (CRuntimeClass* pSplashClass, UINT nResourceID, 
			   const char* pszFile, int nDuration);
     CSplashThread* CreateSplashThread (CRuntimeClass* pSplashClass, 
     							int nDuration);
};


/*!	The splash window.
	
	You can override the OnDraw() method to do extra stuff (after calling the 
	base class version).
*/
class CSplashWnd : public CWnd
{
	DECLARE_DYNCREATE (CSplashWnd);

public:
     CSplashWnd();
     ~CSplashWnd();

// Operations
     void SetBitmapToUse(const CString& sFilename);
     void SetBitmapToUse(UINT nResourceID);
     void SetBitmapToUse(LPCTSTR pszResourceName); 
     BOOL Create();
     
	virtual void OnDraw (CDC* pDC);
	void SetDuration (int nDuration) { m_nDuration = nDuration; }
	void Close();

protected:
     //{{AFX_VIRTUAL(CSplashWnd)
     //}}AFX_VIRTUAL
     
     //{{AFX_MSG(CSplashWnd)
     afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
     afx_msg void OnPaint();
     afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
     afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
     afx_msg BOOL OnQueryNewPalette();
	//}}AFX_MSG
     
     DECLARE_MESSAGE_MAP()
     
     BOOL SelRelPal(BOOL bForceBkgnd);
     BOOL LoadBitmap();
     void CreatePaletteFromBitmap();
     
     CBitmap   m_Bitmap;
     CPalette  m_Palette;
     int       m_nHeight;
     int       m_nWidth;
     CWnd      m_wndOwner;                   
     BOOL      m_bUseFile;
     LPCTSTR   m_pszResourceName;
     CString   m_sFilename;
     int		m_nDuration;
};


class CSplashThread : public CWinThread
{
public:
     void HideSplash();
     void SetBitmapToUse(const CString& sFilename);
     void SetBitmapToUse(UINT nResourceID);
     void SetBitmapToUse(LPCTSTR pszResourceName); 

	CSplashWnd* CreateWnd (CRuntimeClass* pClass, int nDuration);

protected:
     CSplashThread();
     virtual ~CSplashThread();

	DECLARE_DYNCREATE(CSplashThread)

	//{{AFX_VIRTUAL(CSplashThread)
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSplashThread)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

     CSplashWnd*  m_pSplashWnd;
};


#endif //	__MFCX_SPLASHER_H__


