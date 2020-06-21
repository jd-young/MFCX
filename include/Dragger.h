//	Dragger.h


#ifndef   __MFCX_DRAGGER_H
#define   __MFCX_DRAGGER_H

class CDragger
{
public:
	CDragger();
	virtual ~CDragger() {}
     CPoint GetOrigPt() const { return m_ptOrig; }

// Operations...
	virtual void Begin (CPoint pt, int dragop, CWnd* pWnd = NULL, CRect* pRect = NULL);
	virtual void End();

	virtual void BeginMove (CPoint pt);
	virtual void EndMove();

	void Clip (CRect& rcScreen);
	BOOL operator== (int) const;
	BOOL operator!= (int) const;

     void Set (int dragOp) { m_dragOp = dragOp; }
     void operator= (int dragOp) { Set (dragOp); }

	void Set (CPoint pt) { m_ptCur; }

protected:
	// Attributes...
	int	    m_dragOp;			// Current dragging operation
 	CPoint   m_ptOrig;            // original clicked point

	CPoint   m_ptPrev;            // previous point
	CPoint   m_ptCur;             // current point

	CRect    m_rcOrg;             // original rectangle
	CRect    m_rcPrev;            // previous rectangle
	CRect    m_rcCur;             // current rectangle

	CRect    m_rcClip;            // cursor clipping rectangle if any
	CWnd*    m_pCapWnd;           // window with capture, if any
};

#endif    // __MFCX_DRAGGER_H
