// 
//   MyTree.h


#ifndef   MYTREE_H
#define   MYTREE_H

//#include  "XTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTree window

class CMyTree : public CXTreeCtrl
{
// Construction
public:
	CMyTree();
	virtual ~CMyTree();

// Attributes
public:

protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTree)
	//}}AFX_VIRTUAL

// Implementation
public:

protected:

     // Drag 'n' drop support...
	virtual void CopyBranch (HTREEITEM hParent, HTREEITEM hFrom); 
	virtual void DeleteBranch (HTREEITEM hItem);
     HTREEITEM AddItem (HTREEITEM hParent, 
                        const char* pszName,
                        HTREEITEM hAfter = TVI_SORT);


	// Generated message map functions
	//{{AFX_MSG(CMyTree)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // MYTREE_H
