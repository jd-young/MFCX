//	XScrollView.h : header file


#ifndef	__MFXC_XSCROLLVIEW_H
#define	__MFXC_XSCROLLVIEW_H

/*!  A 32-bit scrolling class.
     
     Almost identical to the MFC CScrollView except you can forget about the 
     32k limit on the Win95 scrollbars.
*/
class CXScrollView : public CView
{
protected:
	CXScrollView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CXScrollView)

	void Initialise();

	static AFX_DATA const SIZE sizeDefault;
	// used to specify default calculated page and line sizes

	// in logical units - call one of the following Set routines
	void SetScaleToFitSize(SIZE sizeTotal);
	void SetScrollSizes(int nMapMode, SIZE sizeTotal,
				const SIZE& sizePage = sizeDefault,
				const SIZE& sizeLine = sizeDefault);

// Attributes
public:
	CPoint GetScrollPosition() const;       // upper corner of scrolling
	CSize GetTotalSize() const			// logical size
	{ ASSERT(this != NULL); return m_totalLog; }

	// for device units
	CPoint GetDeviceScrollPosition() const;
	void GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
			SIZE& sizePage, SIZE& sizeLine) const;

	CSize  GetScrollScale() const { return m_sizeScalingFactor; }

// Operations
public:
	void ScrollToPosition(POINT pt);    // set upper left position
	void FillOutsideRect(CDC* pDC, CBrush* pBrush);
	void ResizeParentToFit(BOOL bShrinkOnly = TRUE);

// CWnd Overrides - for 32k scroll bars
	int  SetScrollPos (int nBar, int nPos, BOOL bRedraw = TRUE);
	int  GetScrollPos (int nBar) const;
	int	GetScrollLimit (int nBar);
	void SetScrollRange (int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
	BOOL SetScrollInfo (int nBar, SCROLLINFO* pScrollInfo, BOOL bRedraw = TRUE);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXScrollView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nMapMode;
	CSize m_totalLog;           // total size in logical units (no rounding)
	CSize m_totalDev;           // total size in device units
	CSize m_pageDev;            // per page scroll size in device units
	CSize m_lineDev;            // per line scroll size in device units

	CSize  m_sizeScalingFactor;	// When > 32k
	CPoint m_ptScrollPos;		// Tracking pos
	CSize  m_sizePage;			// Page size - usually same as m_pageDev
	CSize  m_sizeMin;			// min positions - this is usually (0, 0)
	CSize  m_sizeMax;			// max - usually same as m_totalLog

	BOOL m_bCenter;             // Center output if larger than total size
	BOOL m_bInsideUpdate;       // internal state for OnSize callback
	void CenterOnPoint(CPoint ptCenter);
	virtual void ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking

protected:
	void UpdateBars();          // adjust scrollbars etc
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
		// size with no bars
	void GetScrollBarSizes(CSize& sizeSb);
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
		CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);


protected:
	virtual ~CXScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	// scrolling implementation support for OLE
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);


	// Generated message map functions
protected:
	//{{AFX_MSG(CXScrollView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif	// __MFXC_XSCROLLVIEW_H
