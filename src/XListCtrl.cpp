/**
	@file     XListCtrl.cpp 	A sortable list control with drag-n-drop columns.

	\section xlistsort	SORT (thanks to Zafir Anjum and Tal J. Rosen)

	CXListCtrl provides a Sort method to do away with the need to supply data
	items which the built-in SortItems method requires.  It also provides a
	virtual method to specify how columns should be sorted.  The choices are
	'Text', 'CaselessText', etc...

	\section xlistsortind	SORT INDICATION (thanks to Zafir Anjum)
	
	The list view control does not provide any visual feedback on whether the
	list is sorted. To give feedback to the users of our application, we can
	use the owner draw feature of the header control and display a triangle
	pointing downwards or pointing updwards, indicating whether the list is
	sorted in the ascending or the descending order. This, of course, is
	applicable only if the list view control is in the report view mode. 
 */

#include "stdafx.h"
#include "../include/XListCtrl.h"
#include "../include/XString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXListCtrl

CXListCtrl::CXListCtrl()
{
     m_headerCtrl.SetCallback (this, (void (CWnd::*)(int, int))DragColumn );
}

CXListCtrl::~CXListCtrl()
{
}


BEGIN_MESSAGE_MAP(CXListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
     ON_NOTIFY_REFLECT (LVN_COLUMNCLICK, OnHeaderClick)
END_MESSAGE_MAP()


void CXListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

     // Subclass dialog based lists here...
     HWND hHeader = ::GetDlgItem (m_hWnd, 0);
     if ( hHeader )
          VERIFY ( m_headerCtrl.SubclassWindow (hHeader) );
}


int CXListCtrl::OnCreate(CREATESTRUCT* pCreateStruct) 
{
     if ( CListCtrl::OnCreate (pCreateStruct) == -1 )
          return -1;
     
     // Subclass non-dialog based lists here...
     if ( m_headerCtrl.m_hWnd == NULL )
     {
          HWND hHeader = ::GetWindow (m_hWnd, GW_CHILD);
          VERIFY ( m_headerCtrl.SubclassWindow (hHeader) );
     }    
     return 0;
}



/**  Gets the data type of the column.  
 *
 *   Override this to tell CXListCtrl which algorithm to sort the list with.
 *
 * @param nCol      The index of the column.  This is the original index (prior
 				to any dragging or re-ordering of columns).
 * @return          The default returns TypeCaselessText, so that Sort will sort 
 *                  by text regardless of case.
 */
/*virtual*/ CXListCtrl::DataTypes CXListCtrl::GetColumnType (int nCol)
{
     nCol;
     return TypeCaselessText;
}


int CXListCtrl::GetCurrCol (int nCol) const
{
	for (int i = 0; i < m_arrOrder.GetSize(); i++)
		if ( nCol == m_arrOrder [i] )
			return i;

	return nCol;
}


/**	Initialises the list control with the given heading information.  The 
	array of headings should be NULL terminated, so initialise it like the 
	following...

\verbatim
	const char* arrHeadings[] = 
			{ 
				"Heading 1", 
				"Heading 2", 
				NULL
			};
\endverbatim

	The headings can be formatted to be left, centred, or right justified.  
	Prefix the heading text with one of the following...
	
	. <		Left justified 
	. ^		Centred
	. >		Right justified

\param	ppszHeadings		An NULL terminated array of strings that make up 
						the column headers.

\return	\b true if all was well, \b false otherwise.
*/
bool CXListCtrl::Initialise (const TCHAR** ppszHeadings, 
					    const CWordArray* pWidths /*= NULL*/)
{
	SetHeadings (ppszHeadings);

     CWordArray arrWidths;
     if ( pWidths )
     {
     	// Initialise according to the given widths...
     	int nSize = pWidths->GetSize();
     	arrWidths.SetSize (nSize); 
     	for (int i = 0; i < nSize; i++)
     		arrWidths [i] = pWidths->GetAt (i);
     }
     else SetDefaultWidths (arrWidths);

	return InsertColumns (arrWidths);
}

bool CXListCtrl::InsertColumns (const CWordArray& arrWidths)
{
     // Initialise the windows list control...
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM; 
	lvColumn.fmt = LVCFMT_LEFT;

	for (int iColumn = 0; iColumn < m_arrHeadings.GetSize(); iColumn++)
	{
		lvColumn.iSubItem = iColumn;
		lvColumn.pszText = const_cast<TCHAR*> (
						(const TCHAR*) m_arrHeadings [m_arrOrder [iColumn]]);
     	lvColumn.cx = arrWidths [iColumn]; 
		
		if ( InsertColumn (iColumn, &lvColumn) == -1 )
			return false;
	}
     return true;
}


void CXListCtrl::SetDefaultWidths (CWordArray& arrWidths)
{
     arrWidths.SetSize (m_arrHeadings.GetSize());
     m_arrOrder.SetSize (m_arrHeadings.GetSize());

	CRect rect;
	GetClientRect (&rect);
     for (int i = 0; i < m_arrHeadings.GetSize(); i++)
     {
          arrWidths [i] = rect.right / m_arrHeadings.GetSize(); 
          m_arrOrder [i] = i;
     }
}


void CXListCtrl::SetHeadings (const TCHAR** ppszHeadings)
{
     CWordArray arrAlignment;
     
     // How many headings are there?
     const TCHAR** pszHeading = ppszHeadings;
     for ( ; *pszHeading; pszHeading++)
     {
          int align = LVCFMT_LEFT;
          CString sHeading = *pszHeading;
          int len = sHeading.GetLength();
          if ( len > 0 )
          {
               TCHAR cAlign = sHeading [0];
               switch ( cAlign )
               {
                    case '<':
                         align = LVCFMT_LEFT;
                         sHeading = sHeading.Right (len-1);
                         break;
                    case '>':
                         align = LVCFMT_RIGHT;
                         sHeading = sHeading.Right (len-1);
                         break;
                    case '^':
                         align = LVCFMT_CENTER;
                         sHeading = sHeading.Right (len-1);
                         break;
               }
          }
          arrAlignment.Add (align);
          m_arrHeadings.Add (sHeading);
     }
}




/*!	Inserts a row to the list control, with the given text string array 
	corresponding to the columns (prior to any re-ordering).
	
\param	arrText		The text strings to add.
\param	lParam		The data item to associate with the row (or 0 if 
					omitted).

\return	\b true if successfully inserted, \b false otherwise.
*/
bool CXListCtrl::Insert (const CStringArray& arrText, LPARAM lParam /*= 0*/)
{
     const int MAX_ITEMSIZE = 64;

     // Now insert the sub-items into the list...
	LV_ITEM lvItem;

	lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lvItem.state = 0;      
	lvItem.stateMask = 0;  
	lvItem.iImage = 0;

	lvItem.iItem = GetItemCount();
	lvItem.iSubItem = 0;

	lvItem.pszText = (char*) (const char*) arrText [m_arrOrder [0]]; 
	lvItem.cchTextMax = MAX_ITEMSIZE;
	lvItem.lParam = (LPARAM) lParam;

	int iIndex = InsertItem (&lvItem);
	if ( iIndex != -1)
	{
          for (int j = 1; j < m_arrHeadings.GetSize(); j++)
     		SetItemText (iIndex, j, 
     		          (char*) (const char*) arrText [m_arrOrder [j]]);
          return true;
     }
     return false;
}






/*virtual*/ bool CXListCtrl::Sort()
{
     int sortCol = m_headerCtrl.GetSortCol();
     if ( sortCol != -1 )
     {
          bool asc = m_headerCtrl.GetSortAsc();
          return Sort (sortCol, asc);
     }
     return false;
}


bool CXListCtrl::Sort (int nCol, bool bAscending)
{
     DoSort (nCol, bAscending);
     m_headerCtrl.SetSortCol (nCol, bAscending);
     return true;
}



  
/**  Called to drag the column.
 *
 * @param source    The index of the source column.
 * @param dest      The index of the destination column.
 */
void CXListCtrl::DragColumn (int source, int dest)
{
     int nPrevSort = m_headerCtrl.GetSortCol();
     bool bPrevAsc = m_headerCtrl.GetSortAsc();

	TCHAR sColText[160];

	// Insert a column at dest
	LV_COLUMN  lv_col;
	lv_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv_col.pszText = sColText;
	lv_col.cchTextMax = 159;
	GetColumn( source, &lv_col );
	lv_col.iSubItem = dest;
	InsertColumn (dest, &lv_col);

	// Adjust source col number since it might have changed 
	// because a new column was inserted
	if ( source > dest ) 
		source++;

	// Moving a col to position 0 is a special case
	if ( dest == 0 )
		for (int i = GetItemCount()-1; i > -1 ; i--)
			SetItemText (i, 1, GetItemText( i, 0));
	
	// Copy sub item from source to dest
	for (int i = GetItemCount()-1; i > -1 ; i--)
		SetItemText (i, dest, GetItemText (i, source ));

	// Delete the source column, but not if it is the first
	if( source != 0 )
		DeleteColumn (source);
	else
	{
		// If source col is 0, then copy col# 1 to col#0
		// and then delete col# 1
		GetColumn (1, &lv_col);
		lv_col.iSubItem = 0;
		SetColumn (0, &lv_col);
		for (int i = GetItemCount()-1; i > -1 ; i--)
			SetItemText (i, 0, GetItemText (i, 1));
		DeleteColumn (1);
	}

	// Sort out the m_arrOrder array...
     int nColCount = m_headerCtrl.GetItemCount();
     for (i = 0; i < nColCount; i++)
     {
          LVCOLUMN lvCol;
          TCHAR szBuf [256];
          lvCol.pszText = szBuf;
          lvCol.cchTextMax = sizeof szBuf - 1;
          lvCol.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
          VERIFY (GetColumn (i, &lvCol));

	     int index = -1;
	     for (int j = 0; j < nColCount; j++)
	          if ( strcmp (m_arrHeadings [j], lvCol.pszText) == 0 )
	               index = j;
	     ASSERT ( index != -1 );

	     TRACE ("Column %d is '%s'\n", i, m_arrHeadings [index]);
		m_arrOrder [i] = index;
	}

     // Resort if necessary...
     if ( nPrevSort == source )
          Sort (dest, bPrevAsc);

	Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// CXListCtrl message handlers


/**  Called when the user clicks on the header.  
 *
 *   We check our 'm_bAllowSort' to see if we are to sort the data by column.
 *
 * @param pNMHDR    A notification item.  It has the following members...
 *   
 *        - HWND hwndFrom  - the window handle from where the notification came
 *        - UINT idFrom    - the window ID from where the notification came
 *        - UINT code      - 
 *
 * @param pResult        The result.  This is set to 0 if the message was handled here.
 */
void CXListCtrl::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult)
{
     TRACE ("CXListCtrl::OnHeaderClick()\n");
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iCol = pNMListView->iSubItem;
	bool bAsc = true;
	if ( iCol == m_headerCtrl.GetSortCol() )
	     bAsc = !m_headerCtrl.GetSortAsc();
	Sort (iCol, bAsc);
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl

CXHeaderCtrl::CXHeaderCtrl()
     : marker_rect (0, 0, 0, 0)
{
     CommonConstruct();
}

CXHeaderCtrl::CXHeaderCtrl (CWnd *pWnd, void (CWnd::*fpDragCol)(int, int)) 
	: marker_rect (0, 0, 0, 0)
{
     CommonConstruct();
	m_fpDragCol = fpDragCol;
	m_pOwnerWnd = pWnd;
}


void CXHeaderCtrl::CommonConstruct()
{
     m_nSortCol = -1;
     m_bSortAsc = true;
     
     m_pWidth = NULL;
	m_bDragging = FALSE;
	m_bCheckForDrag = FALSE;
	m_fpDragCol = NULL;
	m_pOwnerWnd = NULL;

     m_bAllowDragColumns = true;
}

CXHeaderCtrl::~CXHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CXHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXHeaderCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



int CXHeaderCtrl::SetSortCol (int nCol, bool bAsc /*= true*/)
{
     int nPrevCol = m_nSortCol;
     
     m_nSortCol = nCol;
     m_bSortAsc = bAsc;
     
     // Change the item to ownder drawn
     HD_ITEM hditem;
     
     hditem.mask = HDI_FORMAT;
     GetItem (nCol, &hditem);
     hditem.fmt |= HDF_OWNERDRAW;
     SetItem (nCol, &hditem);
     
     // Invalidate header control so that it gets redrawn
     Invalidate();
     return nPrevCol;
}



/**  Owner draw method.
 *
 *   The DrawItem() is where the sort indicator actually gets drawn. Besides
 *   drawing the sort triangle, this function is now also responsible for
 *   drawing the column label itself. The DrawItem() function is called for each
 *   item in the header control that has the HDF_OWNERDRAW format. 
 *   
 *   These are the steps we take in the DrawItem() function to draw the column
 *   label and the triangular image to indicate the sort order: 
 *   
 *   Attach the device context handle passed in through the argument to a CDC
 *   object for easier device context handling. The handle is detached from the
 *   CDC object before the function returns. If we did not detach the handle
 *   then the DC would be released when the CDC object is destroyed. 
 *   
 *   We save the DC and change the clipping region so that all the updates are
 *   contrained within the header item for which the DrawItem() function is
 *   called. The device context is restored before the function returns. 
 *   
 *   We compute the offset used when drawing the label and the sort triangle.
 *   The offset is used to leave a margin around the label and is equal to twice
 *   the width of a space character. 
 *   
 *   We determine the format to be used when drawing the column label. Since the
 *   column label can be aligned left, center or right, we have to choose an
 *   appropriate format for the DrawText() function. You will also notice the
 *   flag DT_END_ELLIPSIS. This tells the DrawText() function that if the text
 *   doesn't fit with the rectangle specified, then the text should be shortened
 *   and three dots appended to the text so that the result fits within the
 *   rectangle. 
 *   
 *   We next adjust the rectangle within which the label will be drawn and then
 *   draw the lable using DrawText(). 
 *   
 *   Finally we draw the triangle to indicate the sort order. We use two
 *   different color to draw the triangle so that it matches the other GUI
 *   elements in Widnows. The COLOR_3DHILIGHT color is used for edges facing the
 *   light source, and the COLOR_3DSHADOW color is used for the shadow. 
 */
void CXHeaderCtrl::DrawItem (DRAWITEMSTRUCT* pDrawItemStruct)
{
	CDC dc;
	dc.Attach (pDrawItemStruct->hDC);

	// Get the column rect
	CRect rcLabel (pDrawItemStruct->rcItem);

	// Save DC
	int nSavedDC = dc.SaveDC();

	// Set clipping region to limit drawing within column
	CRgn rgn;
	rgn.CreateRectRgnIndirect (&rcLabel);
	dc.SelectObject (&rgn);
	rgn.DeleteObject();

     // Draw the background
     CBrush brush3DFace (::GetSysColor(COLOR_3DFACE));
     dc.FillRect(rcLabel, &brush3DFace);
	
	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int charWidth = dc.GetTextExtent (_T(" "), 1).cx;
	int offset = charWidth * 2;

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;
	
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;

	GetItem (pDrawItemStruct->itemID, &hditem);

	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | 
	               DT_VCENTER | DT_END_ELLIPSIS ;

	if ( hditem.fmt & HDF_CENTER )
		uFormat |= DT_CENTER;
	else if ( hditem.fmt & HDF_RIGHT )
		uFormat |= DT_RIGHT;
	else uFormat |= DT_LEFT;

	// Adjust the rect if the mouse button is pressed on it
	if ( pDrawItemStruct->itemState == ODS_SELECTED )
	{
		rcLabel.left++;
		rcLabel.top += 2;
		rcLabel.right++;
	}

	// Adjust the rect further if Sort arrow is to be displayed
	if ( pDrawItemStruct->itemID == (UINT)m_nSortCol )
	{
		rcLabel.right -= 3 * offset;
	}

	rcLabel.left += offset;
	rcLabel.right -= offset;

	// Draw column label
	if ( rcLabel.left < rcLabel.right )
		dc.DrawText (buf, -1, rcLabel, uFormat);

	// Draw the Sort arrow
	if ( pDrawItemStruct->itemID == (UINT) m_nSortCol )
	{
		CRect rcIcon (pDrawItemStruct->rcItem);

		// Set up pens to use for drawing the triangle
		CPen penLight (PS_SOLID, 1, GetSysColor (COLOR_3DHILIGHT));
		CPen penShadow (PS_SOLID, 1, GetSysColor (COLOR_3DSHADOW));
		CPen *pOldPen = dc.SelectObject (&penLight);

		if ( m_bSortAsc )
		{
			// Draw triangle pointing upwards
			dc.MoveTo (rcIcon.right - 2*offset, offset - 1);
			dc.LineTo (rcIcon.right - 3*offset/2, rcIcon.bottom - offset);
			dc.LineTo (rcIcon.right - 5*offset/2-2, rcIcon.bottom - offset);
			dc.MoveTo (rcIcon.right - 5*offset/2-1, rcIcon.bottom - offset - 1);

			dc.SelectObject (&penShadow);
			dc.LineTo (rcIcon.right - 2*offset, offset-2);
		}
		else
		{
			// Draw triangle pointing downwords
			dc.MoveTo (rcIcon.right - 3*offset/2, offset-1);
			dc.LineTo (rcIcon.right - 2*offset-1, rcIcon.bottom - offset + 1);
			dc.MoveTo (rcIcon.right - 2*offset-1, rcIcon.bottom - offset);

			dc.SelectObject (&penShadow);
			dc.LineTo (rcIcon.right - 5*offset/2-1, offset -1);
			dc.LineTo( rcIcon.right - 3*offset/2, offset -1);
		}

		// Restore the pen
		dc.SelectObject (pOldPen);
	}

	// Restore dc
	dc.RestoreDC (nSavedDC);

	// Detach the dc before returning
	dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl message handlers

void CXHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( !IsDragColumnsAllowed() || (MK_LBUTTON & nFlags) == 0 )
	{
		// The left mouse button is not pressed - so reset flags
		m_bCheckForDrag = FALSE;
		m_bDragging = FALSE;
	}
	else if ( m_bDragging )
	{
		// Get column number that falls under the mouse
		int i = 0, cx = 0;
		if ( point.x > 0 )
			for( i = 0; i < GetItemCount(); i++ )
			{
				if ( point.x >= cx && point.x < cx + m_pWidth [i] )
					break;
				cx += m_pWidth [i];
			}

		if ( i != m_nDropPos )
		{
			m_nDropPos = i;

			CRect rect;
			GetWindowRect (&rect);

			// Invalidate area occupied by previous marker
			InvalidateRect (&marker_rect);

			// Draw a new marker
			CClientDC dc(this);
			POINT pts[3];
			pts[0].x = cx; pts[1].x = cx -3; pts[2].x = cx +3;
			pts[0].y = rect.Height(); pts[1].y = pts[2].y = rect.Height() -7; 
			dc.Polygon( pts, 3 );

			// save marker information
			marker_rect.left = cx - 4;
			marker_rect.top = rect.Height() -8;
			marker_rect.right = cx + 4;
			marker_rect.bottom = rect.Height();
		}
		return;
	}
	else if( m_bCheckForDrag )
	{
		// The mouse button was pressed over a column header
		// and now the mouse has moved - so start drag
		m_bCheckForDrag = FALSE;

		m_bDragging = TRUE;
		m_nDropPos = m_nDragCol;

		SetCapture();

		// Store information for later use
		int iCount = GetItemCount();
		HD_ITEM hd_item;
		m_pWidth = new int[iCount];
		for( int i = 0; i < iCount; i++ )
		{
			hd_item.mask = HDI_WIDTH;
			GetItem( i, &hd_item );
			m_pWidth[i] = hd_item.cxy;

		}
		return;
	}
	
	CHeaderCtrl::OnMouseMove(nFlags, point);
}

void CXHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ASSERT( m_pOwnerWnd != NULL && m_fpDragCol != NULL );

	if( m_bDragging )
	{
		m_bDragging = FALSE;
		delete[] m_pWidth;
		ReleaseCapture();
		Invalidate();

		// Call the callback function.
		if( m_nDragCol != m_nDropPos && m_nDragCol != m_nDropPos -1 )

			(m_pOwnerWnd->*m_fpDragCol)( m_nDragCol, m_nDropPos );
	}
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}


void CXHeaderCtrl::SetCallback( CWnd* pWnd, void (CWnd::*fpDragCol)(int, int) )
{
	m_fpDragCol = fpDragCol;
	m_pOwnerWnd = pWnd;
}

void CXHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Determine if mouse was pressed over a column header
	HD_HITTESTINFO hd_hittestinfo;
	hd_hittestinfo.pt = point;
	SendMessage(HDM_HITTEST, 0, (LPARAM)(&hd_hittestinfo));
	if( hd_hittestinfo.flags == HHT_ONHEADER )
	{
		m_nDragCol = hd_hittestinfo.iItem;
		m_bCheckForDrag = TRUE;
	}
	
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}




void CXListCtrl::DoSort (int nSortCol, bool bAscending)
{
	int NumItems = GetItemCount();	

	// replace Item data with pointer to SortItem structure
	for (int i = 0; i < NumItems; i++)	
	{
		DWORD dwData = GetItemData(i); // save current data to restore it later
		CString sText = GetItemText (i, nSortCol);
		SetItemData (i, (DWORD)new SortItem (i, sText, dwData));
	}

	SortData data (this, nSortCol, bAscending);
	SortItems (Compare, (LPARAM) &data);

     // Now put the old data back to the way it was...
	for (i = 0; i < NumItems; i++) 
	{
		SortItem* pData = (SortItem*) GetItemData (i);
		SetItemData (i, pData->_dwData);
		delete pData;
	}
}




/*static*/ int CALLBACK CXListCtrl::Compare (LPARAM lParam1, LPARAM lParam2, 
                                             LPARAM pParam3)
{
	const SortItem* pLeft = ((SortItem*) lParam1);
	const SortItem* pRight = ((SortItem*) lParam2);
     SortData* pData = (SortData*) pParam3;
	return pData->_pList->Compare (pLeft, pRight, pData->_nSortCol, 
							 pData->_bAscending);
}




/*virtual*/ int CXListCtrl::Compare (const SortItem* pLeft, 
							  const SortItem* pRight, 
							  int nCol, 
							  bool bAscending)
{
	return Compare (pLeft->_sText, pRight->_sText, nCol, bAscending);
}


/**  Compare two string by in the given column.  
 *
 *   This uses the virtual function 'GetColumnType' to get the type of data 
 *   stored in the column. CXListCtrl's default is 'Text'.
 *
 * @param cstL      Left hand string
 * @param cstR      Right hand string
 * @return          - Zero if the strings are identical,
 *                  - < 0 if cstL is less than cstR,
 *                  - > 0 if cstL is greater than cstR.
 */
/*virtual*/ int CXListCtrl::Compare (const CString& cstL, const CString& cstR, 
                                     int nCol, bool bAscending)
{
     DataTypes type = GetColumnType (GetColIndex (nCol));
     int result = 0;
     switch ( type )
     {
          case TypeCaselessText:   result = cstL.CompareNoCase (cstR);    break;
          case TypeInt:            result = atoi (cstL) - atoi (cstR);    break;
          case TypeFloat:          result = atof (cstL) > atof (cstR) ? 1 : -1;
          					break;

		case TypeNumber:		// Sort numerically (ignoring any commas).
		{
			CXString sL (cstL);
			CXString sR (cstR);
			sL.Decommatise();
			sR.Decommatise();
			result = atoi (sL) - atoi (sR);
			break;
		}

		case TypeCurrency:		// Sort numerically (skipping '$' or '£' sings)
		{
			CString sL = CvtCurrency (cstL);
			CString sR = CvtCurrency (cstR);
			result = atoi (sL) - atoi (sR);
			break;
		}

          case TypeDate:           
          case TypeTime:
          case TypeDateTime: 
          {
               COleDateTime odtL;
               COleDateTime odtR;
               odtL.ParseDateTime (cstL);
               odtR.ParseDateTime (cstR);
               COleDateTimeSpan spanElapsed = odtL - odtR;
               if (spanElapsed.GetStatus() == COleDateTimeSpan::invalid ) 
               {
                    // Invalid Date time, compare the given strings instead...
                    result = cstL.CompareNoCase (cstR);
               }
               else result = (int) spanElapsed.GetTotalSeconds();
               break;
          }

          default:
          case TypeText:
               result = cstL.Compare (cstR);
               break;
      }
     
     return bAscending ? result : -result;
}


/**	Converts a currency string into a number and returns it.  The currency 
	string could be any of the following formats...

	. $123.45
	. +$123.34
	. -$123.45
	. ($123.45)	(accountants use this form to denote 'in-the-red')
	
	Some currencies may have the currency denoter at the end of the string...
	
	. 123.45GBP
	. 123.45USD
	
	...but we don't worry about processing them since the atof method will stop
	automatically when it hits 'GBP' or 'USD'.

*/
CString CXListCtrl::CvtCurrency (const CString& sOrig) const
{
	CString sCurr (sOrig);
	TCHAR* s = sCurr.GetBuffer (0);

	bool bNeg = false;	

	while ( isspace (*s) ) s++;
	
	// Skip past +ve / -ve sign (if any)
	if ( *s == '-' || *s == '+' || *s == '(' )
	{
		bNeg = *s != '+';
		s++;
	}
	
	// Now skip past the first non numerics (i.e. the '$', or '£' sign)...
	while ( !isdigit (*s) && *s != '.' ) s++;
	
	CXString::Decommatise (s);
	CString sRes;
	if ( bNeg )
		sRes = '-';
	sRes += s;
	sCurr.ReleaseBuffer();		// after accessing 's'
	return sRes;
}