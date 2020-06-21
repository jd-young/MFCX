// SBDestination.h: interface for the CSBDestination class.
//
//////////////////////////////////////////////////////////////////////
//
// Copyright 1998 Scott D. Killen
//
//////////////////////////////////////////////////////////////////////

#ifndef __MFCX_SBDESTINATION_H
#define __MFCX_SBDESTINATION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "BrowseForFolder.h"


///	A folder browser dialog.
/*!	A folder browser dialog.

	To use...
\verbatim

	CSBDestination sb (GetSafeHwnd(), "Welcome to the Jungle!");
	sb.SetInitialSelection ("c:\\Jungle\\Mogley");
	sb.SetFlags (BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT);
	if ( sb.SelectFolder() )
	{
		CString sFolder = sb.GetSelectedFolder();
		:
	}

\endverbatim

*/
class CSBDestination : public CBrowseForFolder  
{
public:
	/// Constructs a folder browser dialog and sets its title from a resource 
	/// string.
	CSBDestination (const HWND hParent = NULL, const int nTitleID = 0);

	/// Constructs a folder browser dialog and sets its title from a string.
	CSBDestination (const HWND hParent, const TCHAR* pszTitle);

	/// Destructor - cleans up all memory.
	~CSBDestination();

	/// Sets the initial selection for the dialog.
	void SetInitialSelection (const CString& strPath);

	void OnSelChanged (const LPITEMIDLIST pidl) const;

private:
	CString m_strInitialSelection;

	/// A callback to set the initial selection.
	void OnInit() const;
};

#endif // __MFCX_SBDESTINATION_H
