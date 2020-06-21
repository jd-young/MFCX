// TreeTestDoc.cpp : implementation of the CTreeTestDoc class
//

#include "stdafx.h"
#include "TreeTest.h"

#include "TreeTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeTestDoc

IMPLEMENT_DYNCREATE(CTreeTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CTreeTestDoc, CDocument)
	//{{AFX_MSG_MAP(CTreeTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeTestDoc construction/destruction

CTreeTestDoc::CTreeTestDoc()
{
	// TODO: add one-time construction code here

}

CTreeTestDoc::~CTreeTestDoc()
{
}

BOOL CTreeTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTreeTestDoc serialization

void CTreeTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTreeTestDoc diagnostics

#ifdef _DEBUG
void CTreeTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTreeTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeTestDoc commands
