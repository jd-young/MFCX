//
//   XListBox.h
//

#ifndef   __MFXC_XLISTBOX_H
#define   __MFXC_XLISTBOX_H

// CXListBox style flags
#define   XLB_BROWSE     1


class CButtonsWnd;
class CListEdit;
class CBrowseButton;

struct ListBoxInfo
{
     ListBoxInfo (const ListBoxInfo& from);
     ListBoxInfo (CString sText, DWORD data);

     const CString sText;
     const DWORD dwData;
};


///  An extended list box that allows creation, editing, deletion and moving 
///  position of the entries.
class CXListBox : public CListBox
{
friend CListEdit;
friend CButtonsWnd;

public:
     CXListBox();
     virtual ~CXListBox();

     virtual bool Initialise (CWnd* pwndParent, 
                              UINT nID, 
                              UINT flags = XLB_BROWSE);
     void SetWindowText (const TCHAR* pText);

     CEdit* GetEditCtrl() const;

// Overrides
     virtual void DoNew();
     virtual void DoDelete();
     virtual void DoMoveUp();
     virtual void DoMoveDown();


protected:
     virtual bool StartEdit (int nIndex, const TCHAR* pszText = NULL);
     virtual void OnEndEdit (const TCHAR* pszText = NULL);
     virtual void OnBrowse() {};

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListBox)
	public:
	//}}AFX_VIRTUAL

private:
     int GetStaticHeight();
     void OnNew();
     void OnDelete();
     void OnMoveUp();
     void OnMoveDown();
     void NotifyParent (int nMsg);

     bool IsEditing() const { return m_pEdit != NULL; }

     void InsertItem (int nIndex, const ListBoxInfo& lbInfo, bool bSelect);
     ListBoxInfo RemoveItem (int nIndex);

     UINT m_nStyle;                // Refer to the XLB_... flags above

     CStatic m_wndStatic;          // The static area to put the title
     CListEdit* m_pEdit;           // For in-place editing
     CButton* m_pBrowse;
     CButtonsWnd* m_pButtons;      // The buttons to add, delete, and move.
     int m_nPrevClicked;

     afx_msg void OnBrowseClicked();
	
     // Generated message map functions
protected:
	//{{AFX_MSG(CXListBox)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int VKeyToItem(UINT nKey, UINT nIndex);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif    // __MFXC_XLISTBOX_H