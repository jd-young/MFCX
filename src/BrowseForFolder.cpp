/*!
	\file	BrowseForFolder.cpp		A browse for folder dialog.
	\author	Copyright 1998 Scott D. Killen
	http://codeguru.earthweb.com/shell/bff.shtml

*/

#include "stdafx.h"
#include "../include/BrowseForFolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*!	

*/
CBrowseForFolder::CBrowseForFolder (const HWND hParent /*= NULL*/, 
                                    const LPITEMIDLIST pidl /*= NULL*/, 
                                    const int nTitle /*= 0*/)
{
	CString strTitle;
	if ( nTitle > 0 )
	     strTitle.LoadString (nTitle);
     
     CommonConstruct (hParent, pidl, strTitle);
}

CBrowseForFolder::CBrowseForFolder (const HWND hParent, 
                                    const LPITEMIDLIST pidl, 
                                    const CString& strTitle)
{
     CommonConstruct (hParent, pidl, strTitle);
}

CBrowseForFolder::CBrowseForFolder (const HWND hParent, 
                                    const LPITEMIDLIST pidl, 
                                    const TCHAR* pszTitle)
{
     CommonConstruct (hParent, pidl, pszTitle);
}




void CBrowseForFolder::CommonConstruct (const HWND hParent, 
                                        const LPITEMIDLIST pidl, 
                                        const TCHAR* pszTitle)
{
	m_hwnd = NULL;
	SetOwner (hParent);
	SetRoot (pidl);
	SetTitle (pszTitle);
	m_bi.lpfn = BrowseCallbackProc;
	m_bi.lParam = reinterpret_cast<long>(this);
	m_bi.pszDisplayName = m_szSelected;
}

CBrowseForFolder::~CBrowseForFolder()
{

}


void CBrowseForFolder::SetOwner(const HWND hwndOwner)
{
	if (m_hwnd != NULL)
		return;

	m_bi.hwndOwner = hwndOwner;
}

/*!	Sets the root of the heirarchy that will be browsed.  Get pidl from 
	SHGetSpecialFolderLocation.  This can be set to NULL to use the Virtual 
	Folder that represents the Windows Desktop.

\param	pidl		Pointer to an ITEMIDLIST.
*/
void CBrowseForFolder::SetRoot(const LPITEMIDLIST pidl)
{
	if (m_hwnd != NULL)
		return;

	m_bi.pidlRoot = pidl;
}

CString CBrowseForFolder::GetTitle() const
{
	return m_bi.lpszTitle;
}

/*!	Sets the title of the Browse for Folder dialog.  This can be used to give 
	instructions to the user.  
	
\param	sTitle 	The string to display.
*/
void CBrowseForFolder::SetTitle(const CString& sTitle)
{
	SetTitle ((const char*) sTitle);

/*	
	if (m_hwnd != NULL)
		return;

	m_pchTitle = std::auto_ptr<char>(new char [static_cast<size_t>(sTitle.GetLength()) + 1]);
	strcpy(m_pchTitle.get(), sTitle);
	m_bi.lpszTitle = m_pchTitle.get();
*/
}


/*!	Sets the title of the Browse for Folder dialog.  This can be used to give 
	instructions to the user.  
	
\param	pszTitle 		The string to display.
*/
void CBrowseForFolder::SetTitle(const TCHAR* pszTitle)
{
	if (m_hwnd != NULL)
		return;

     int len = strlen (pszTitle);
	m_pchTitle = std::auto_ptr<char>(new char [len + 1]);
	strcpy(m_pchTitle.get(), pszTitle);
	m_bi.lpszTitle = m_pchTitle.get();
}


/*!	Sets the title of the Browse for Folder dialog.  This can be used to give 
	instructions to the user.  
	
\param	nTitle	The index of a string resource to be loaded.  

\return	\b false if the resource could not be loaded, otherwise \b true.
*/
bool CBrowseForFolder::SetTitle (const int nTitle)
{
	if (nTitle <= 0)
		return false;

	CString strTitle;
	if(!strTitle.LoadString(static_cast<size_t>(nTitle)))
	{
		return false;
	}
	SetTitle(strTitle);
	return true;
}



/*!	Sets the flags which determine which types of folders to be listed in the 
	dialog box amonst other things.

\param	ulFlags 	Value specifying the types of folders to be listed in the 
				dialog box as well as other options.  This member can 
				include zero or more of the following values...
				
				- BIF_BROWSEFORCOMPUTER  Only returns computers. If the 
									user selects anything other than a 
									computer, the OK button is grayed.

				- BIF_BROWSEFORPRINTER   Only returns printers. If the user 
									selects anything other than a 
									printer, the OK button is grayed.

				- BIF_DONTGOBELOWDOMAIN  Does not include network folders 
									below the domain level in the tree 
									view control.

				- BIF_RETURNFSANCESTORS  Only returns file system ancestors. 
									If the user selects anything other 
									than a file system ancestor, the OK
									button is grayed.

				- BIF_RETURNONLYFSDIRS   Only returns file system 
									directories. If the user selects 
									folders that are not part of the 
									file system, the OK button is 
									grayed.
				
				- BIF_STATUSTEXT         Includes a status area in the 
									dialog box. The callback function 
									can set the status text by sending 
									messages to the dialog box. 
*/
void CBrowseForFolder::SetFlags(const UINT ulFlags)
{
	if (m_hwnd != NULL)
		return;

	m_bi.ulFlags = ulFlags;
}

CString CBrowseForFolder::GetSelectedFolder() const
{
	return m_szSelected;
}

bool CBrowseForFolder::SelectFolder()
{
	bool bRet = false;

	LPITEMIDLIST pidl;
	if ((pidl = ::SHBrowseForFolder(&m_bi)) != NULL)
	{
		m_strPath.Empty();
		if (SUCCEEDED(::SHGetPathFromIDList(pidl, m_szSelected)))
		{
			bRet = true;
			m_strPath = m_szSelected;
		}

		LPMALLOC pMalloc;
		//Retrieve a pointer to the shell's IMalloc interface
		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			// free the PIDL that SHBrowseForFolder returned to us.
			pMalloc->Free(pidl);
			// release the shell's IMalloc interface
			(void)pMalloc->Release();
		}
	}
	m_hwnd = NULL;

	return bRet;
}

void CBrowseForFolder::OnInit() const
{

}

void CBrowseForFolder::OnSelChanged(const LPITEMIDLIST pidl) const
{
	(void)pidl;
}

void CBrowseForFolder::EnableOK(const bool bEnable) const
{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_ENABLEOK, static_cast<WPARAM>(bEnable), NULL);
}

void CBrowseForFolder::SetSelection(const LPITEMIDLIST pidl) const
{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_SETSELECTION, FALSE, reinterpret_cast<long>(pidl));
}

void CBrowseForFolder::SetSelection(const CString& strPath) const
{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<long>(LPCTSTR(strPath)));
}

void CBrowseForFolder::SetStatusText(const CString& strText) const
{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage (m_hwnd, BFFM_SETSTATUSTEXT, NULL, 
					reinterpret_cast<long>(LPCTSTR(strText)));
}

int __stdcall CBrowseForFolder::BrowseCallbackProc (HWND hwnd, UINT uMsg, 
										  LPARAM lParam, 
										  LPARAM lpData)
{
	CBrowseForFolder* pbff = reinterpret_cast<CBrowseForFolder*>(lpData);
	pbff->m_hwnd = hwnd;
	if (uMsg == BFFM_INITIALIZED)
		pbff->OnInit();
	else if (uMsg == BFFM_SELCHANGED)
		pbff->OnSelChanged(reinterpret_cast<LPITEMIDLIST>(lParam));
	
	return 0;
}
