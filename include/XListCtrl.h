// XListCtrl.h : header file 
// 
// DRAGGING COLUMNS (thanks to Zafir Anjum)
//   The CMyHeaderCtrl is derived from CHeaderCtrl. You will notice that the non
//   default constructor is somewhat unusual. It takes a pointer to the 
//   CListCtrl or CListView derived class and also a pointer to a member 
//   function to call when the user has completed dragging the column. A member 
//   function SetCallback() is also defined. This function can be used when you 
//   want to use the default constructor for the CMyListCtrl class. 
//
//   Here’s a brief description of what the protected member variables are used
//   for. The m_bCheckForDrag flag is set true by the WM_LBUTTONDOWN handler
//   only when the user presses the left mouse button over a column header. Its
//   used by the WM_MOUSEMOVE handler to decide whether it should check for a
//   column drag situation. This is important since we want to drag the column
//   only if the user initially pressed the mouse button over a column header. 
//
//   The m_bDragging flag indicates that a column drag is in progress. The
//   m_pWidth variable holds an array of column widths. This is used to
//   determine the column that is the drop target. The array is dynamically
//   allocated using the new operator. The m_nDragCol variable holds the column
//   index of the column being dragged and m_nDropPos holds the column index of
//   the new position. The marker_rect holds the enclosing rectangle for the
//   marker used for visual feedback to the user. The marker is a triangle
//   indicating the new position where the column will be dragged to. The
//   marker_rect is used to erase the previous marker when the marker position
//   changes. 
//
//   The m_fpDragCol variable holds a pointer to the CListCtrl or CListView
//   member function that gets called when the user finishes the drag operation.
//   The m_pOwnerWnd variable holds the object for which the m_fpDragCol member
//   function is called. Normally, this would be the parent window. 

#ifndef   __MFCX_XLISTCTRL_H
#define   __MFCX_XLISTCTRL_H

class CXListCtrl;

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl window

class CXHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CXHeaderCtrl();
	CXHeaderCtrl (CXListCtrl* pWnd, void (CWnd::*fpDragCol)(int, int));

// Attributes
public:
     int GetSortCol() const { return m_nSortCol; }
     bool GetSortAsc() const { return m_bSortAsc; }

     bool IsDragColumnsAllowed() const { return m_bAllowDragColumns; }
     void AllowDragColumns (bool f = true) { m_bAllowDragColumns = f; }

protected:
     int  m_nSortCol;
     bool m_bSortAsc;

// Operations
public:
     int SetSortCol (int nCol, bool bAsc = true);
     void DrawItem (DRAWITEMSTRUCT* pDrawItemStruct);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXHeaderCtrl();
//     void SetCallback(CWnd* pWnd, void (CWnd::*fpDragCol)(int, int));
     void SetCallback(CXListCtrl* pWnd, void (CXListCtrl::*fpDragCol)(int, int));

protected:
     void CommonConstruct();

     // Dragging headers...
     bool	m_bCheckForDrag;
	bool	m_bDragging;
	int	*m_pWidth;
	int	m_nDragCol;
	int	m_nDropPos;
	CRect	marker_rect;
//     void	(CWnd::*m_fpDragCol)(int, int);
     void	(CXListCtrl::*m_fpDragCol)(int, int);
	CXListCtrl *m_pOwnerWnd;
	bool m_bAllowDragColumns;


	// Generated message map functions
protected:
	//{{AFX_MSG(CXHeaderCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



///	An sortable, draggable column list control.
/*!  CXListCtrl provides a Sort method to do away with the need to supply data
	items which the built-in SortItems method requires.  It also provides a
 	virtual method to specify how columns should be sorted.  The choices are
	'Text', 'CaselessText', etc...

	SORT INDICATION (thanks to Zafir Anjum)

	The list view control does not provide any visual feedback on whether the
	list is sorted. To give feedback to the users of our application, we can
	use the owner draw feature of the header control and display a triangle
	pointing downwards or pointing updwards, indicating whether the list is
	sorted in the ascending or the descending order. This, of course, is
	applicable only if the list view control is in the report view mode. 
*/
class CXListCtrl : public CListCtrl
{
// Construction
public:
	/// This constructs a new list control item.
	CXListCtrl();

	/// The sort type enum.
     typedef enum 
     {
     	TypeText, 			///< Sort alphabetically (case sensitive).
     	TypeCaselessText,  		///< Sort alphabetically (case insensitive).
		TypeInt, 				///< Sort numerically.
		TypeFloat, 			///< Sort numerically (with floats).
		TypeDate, 			///< Sort by date.
		TypeTime, 			///< Sort by time (date is irrelevant).
		TypeDateTime, 			///< Sort by date / time.
		TypeNumber,			///< Sort numerically (ignoring any commas)
		TypeCurrency			///< Sort as currency (ignore leading '$', or 
							///< '£' signs).
	}	DataTypes;

// Attributes
public:
     CXHeaderCtrl m_headerCtrl;
     CXHeaderCtrl* GetHeaderCtrl()
          { return (CXHeaderCtrl*) CListCtrl::GetHeaderCtrl(); }

     int GetColCount() const { return m_headerCtrl.GetItemCount(); }
     bool IsDragColumnsAllowed() const 
     	{ return m_headerCtrl.IsDragColumnsAllowed(); }
     void AllowDragColumns (bool f = true) 
     	{ m_headerCtrl.AllowDragColumns (f); }
     
// Operations
public:
     /// Initialises the list control with the given heading information.
     bool Initialise (const TCHAR** ppszHeadings, 
     			  const CWordArray* pWidths = NULL);

	/// Sort the list according to the current sort column and direction.
     virtual bool Sort();
     
     /// Sort the list by the given column (the current index).
     virtual bool Sort (int nCol, bool bAscending);

	/// Gets the type of the given column.
     virtual DataTypes GetColumnType (int nColIndx);

	/// Gets the original index of the given column.
     int GetColIndex (int i) const { return m_arrOrder [i]; }

	/// Gets the current column of the given original column index.
	int GetCurrCol (int nCol) const;


	/// Inserts a row into the list.
	bool Insert (const CStringArray& arrText, LPARAM lParam = 0);

protected:
     class SortItem
     {
     public:
          SortItem (int nIndex, const CString& sText, LPARAM dwData)
          {
     		_dwData = dwData;
     		_sText = sText;
     		_nIndex = nIndex;
          }
		int _nIndex;
		CString _sText;
		DWORD _dwData;
	};

// Compare functions.  This can be overridden.
     virtual int Compare (const SortItem* pLeft, const SortItem* pRight, int nCol, 
     				 bool bAscending);
     virtual int Compare (const CString& cstL, const CString& cstR, int nCol, 
     				 bool bAscending);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrl)
	public:
     int OnCreate(CREATESTRUCT* pCreateStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXListCtrl();

protected:
	/// The headings.
	CStringArray   m_arrHeadings;

	/// The order in which the columns are now arranged.
     CWordArray m_arrOrder;

	/** 	An object of this class is used to pass information to the Compare 
		method as the LPARAM parameter.
	*/
     class SortData
     {
     public:
          SortData (CXListCtrl* pList, int nSortCol, bool bAscending)
               { _pList = pList; _nSortCol = nSortCol; _bAscending = bAscending; }

          CXListCtrl* _pList;
          int         _nSortCol;
          bool        _bAscending;
     };

	/// Sets the headings for the columns.
	void SetHeadings (const TCHAR** ppszHeadings);

	/// Inserts the columns.
	bool InsertColumns (const CWordArray& arrWidths);

	/// Sets the default widths for the columns.
     void SetDefaultWidths (CWordArray& arrWidths);


     void DragColumn (int source, int dest);
     void DoSort (int nCol, bool bAscending);


     static int CALLBACK Compare (LPARAM lParam1, LPARAM lParam2, LPARAM lSortCrit);
     static int Compare (const CString& cstL, const CString& cstR, 
                         DataTypes type, bool bAscending);

	CString CvtCurrency (const CString& sOrig) const;

	// Generated message map functions
	//{{AFX_MSG(CXListCtrl)
     afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MFCX_XLISTCTRL_H
