//
//	GMouse.h		GMouse Global mouse cursor
// 

#ifndef	__MFCX_GMOUSE_H
#define	__MFCX_GMOUSE_H


class GMouse
{
private:
	BOOL m_bClipped;
	int m_nID;

	HCURSOR LoadCursor (LPCSTR name);

public:
	GMouse() { m_bClipped = FALSE; m_nID = 0; }
	~GMouse();

	// Cursor shape setting
	HCURSOR operator= (HCURSOR hc)	{ return ::SetCursor(hc); }
	HCURSOR operator= (LPCSTR name)	{ return *this = LoadCursor (name); }
	HCURSOR operator= (int id)
	{ return *this = AfxGetApp()->LoadCursor (m_nID=id); }

	// Cursor shape getting
	operator int()	{ return m_nID; }
	int GetCurID() { return m_nID; }
	
	// Set/Get position
	void     SetPos (int x, int y)	{ ::SetCursorPos(x, y); }
	CPoint   GetPos()				{ CPoint p; ::GetCursorPos(&p); return p; }
	operator CPoint()				{ return GetPos(); }
	CPoint   operator= (CPoint p)		{ ::SetCursorPos (p.x, p.y); return p; }
	
	void Clip (CWnd *pWnd);
	void Clip (CRect &rect)	{ ::ClipCursor(&rect); m_bClipped = TRUE; }
	void UnClip()			{ ::ClipCursor(NULL); m_bClipped = FALSE; }

	operator CWnd*()		{ return CWnd::GetCapture(); }
	CWnd* Capture (CWnd *w)	{ return w->SetCapture(); }
	void Release ()		{ ReleaseCapture(); m_nID = 0; }
};


// Global objects
extern GMouse theMouse;


#endif	// __MFCX_GMOUSE_H