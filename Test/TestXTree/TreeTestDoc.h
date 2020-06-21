// TreeTestDoc.h : interface of the CTreeTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREETESTDOC_H__42B36C78_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_)
#define AFX_TREETESTDOC_H__42B36C78_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTreeTestDoc : public CDocument
{
protected: // create from serialization only
	CTreeTestDoc();
	DECLARE_DYNCREATE(CTreeTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTreeTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREETESTDOC_H__42B36C78_3137_11D3_BA15_8141F0CF8CC9__INCLUDED_)
