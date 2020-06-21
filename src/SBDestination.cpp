/*!
	\file	SBDestination.cpp		A folder browsing dialog.
	\author	Copyright 1998 Scott D. Killen

*/

#include "stdafx.h"
#include "../include/SBDestination.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSBDestination::CSBDestination (const HWND hParent /*= NULL*/, 
                                const int nTitleID /*= 0*/)
	: CBrowseForFolder(hParent, NULL, nTitleID)
{

}

CSBDestination::CSBDestination (const HWND hParent, const TCHAR* pszTitle)
     : CBrowseForFolder (hParent, NULL, pszTitle)
{
}
     

CSBDestination::~CSBDestination()
{
}

void CSBDestination::SetInitialSelection(const CString & strPath)
{
	m_strInitialSelection = strPath;
}

void CSBDestination::OnInit() const
{
	SetSelection (m_strInitialSelection);
	SetStatusText (m_strInitialSelection);
}

void CSBDestination::OnSelChanged (const LPITEMIDLIST pidl) const
{
	CString strBuffer;
	if (SHGetPathFromIDList(pidl, strBuffer.GetBuffer(MAX_PATH)))
		strBuffer.ReleaseBuffer();
	else
		strBuffer.ReleaseBuffer(0);
	SetStatusText(strBuffer);
}
