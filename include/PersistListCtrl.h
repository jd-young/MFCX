//
//   PersistListCtrl.h
//

#ifndef   __MFCX_PERSISTLISTCTRL_H
#define   __MFCX_PERSISTLISTCTRL_H

#include "XListCtrl.h"


/**	A list control that saves its status to the registry.

	This is derived from CXListCtrl that allows sorting and has a sort 
	indicator in its header.  All this information is then saved to the 
	registry.
*/
class CPersistentListCtrl : public CXListCtrl
{
// Construction
public:

	/// Constructs a persistent list control and saves / retrieves the 
	/// information from the given registry key.
     CPersistentListCtrl (const TCHAR* pszRegBase = NULL);

// Attributes
public:

// Operations
public:
	/// The registry base for storing persistence stuff (position, columns, 
	/// etc.).
     void SetRegBase (const TCHAR* pszRegBase);
     
	/// Initialises the list control.
	bool Initialise (const TCHAR** ppszHeadings, 
				  const TCHAR* pszRegBase = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPersistentListCtrl)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPersistentListCtrl();

protected:
     CString        m_sRegBase;

	// Generated message map functions
	//{{AFX_MSG(CPersistentListCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif    // __MFCX_PERSISTLISTCTRL_H
