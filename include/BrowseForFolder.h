//////////////////////////////////////////////////////////////////////
//
// ShellBrowser.h: interface for the CShellBrowser class.
//
// Copyright 1998 Scott D. Killen
// http://codeguru.earthweb.com/shell/bff.shtml
//
//////////////////////////////////////////////////////////////////////

#ifndef __MFCX_SHELLBROWSER_H__
#define __MFCX_SHELLBROWSER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <memory>
#include <shlobj.h>

///	A Browse-for-folder dialog.
/*!	A Browse-for-folder dialog.  


	http://codeguru.earthweb.com/shell/bff.shtml
*/
class CBrowseForFolder
{
public:
	CBrowseForFolder (const HWND hParent = NULL, 
				   const LPITEMIDLIST pidl = NULL, 
				   const int nTitleID = 0);

	CBrowseForFolder (const HWND hParent, 
				   const LPITEMIDLIST pidl, 
				   const CString& strTitle);

	CBrowseForFolder (const HWND hParent, 
				   const LPITEMIDLIST pidl, 
				   const TCHAR* pszTitle);

	virtual ~CBrowseForFolder() = 0;

	/// Sets the handle of the owner window for the dialog box.
	void SetOwner(const HWND hwndOwner);

	/// Sets the root of the heirarchy that will be browsed.  
	void SetRoot(const LPITEMIDLIST pidl);

	/// Accesses the string that is displayed above the tree view control in 
	/// the dialog box. 
	CString GetTitle() const;
	
	/// Set the title of the Browse for Folder dialog.
	void SetTitle(const CString& strTitle);


	/// Set the title of the Browse for Folder dialog.  
	void SetTitle(const TCHAR* pszTitle);

	/// Set the title of the Browse for Folder dialog.  
	bool SetTitle(const int nTitle);


	/// Gets the flags which determine which types of folders to be listed in 
	/// the dialog box amonst other things.  For more information see SetFlags.
	UINT GetFlags() const;

	/// Sets the flags which determine which types of folders to be listed in 
	/// the dialog box amonst other things.
	void SetFlags(const UINT ulFlags);

	/// Call GetSelectedFolder to retrieve the folder selected by the user.
	CString GetSelectedFolder() const;

	/// Retreives the image associated with the selected folder. 
	int GetImage() const;

	/// Call SelectFolder to display the dialog and get a selection from the 
	/// user.  Use GetSelectedFolder and GetImage to get the results of the 
	/// dialog.
	bool SelectFolder();

protected:
     /// The common constructor, called be all constructors.
     void CommonConstruct (const HWND hParent, 
                           const LPITEMIDLIST pidl, 
                           const TCHAR* pszTitle);

	/// OnInit is called before the dialog is displayed on the screen.
	virtual void OnInit() const;

	/// OnSelChanged is called whenever the user selects a different directory.  
	/// pidl is the LPITEMIDLIST of the new selection.  Use SHGetPathFromIDList 
	/// to retrieve the path of the selection.
	virtual void OnSelChanged(const LPITEMIDLIST pidl) const;

	/// Call EnableOK to enable the OK button on the active dialog.  If bEnable 
	/// is true then the button is enabled, otherwise it is disabled.
	/// NOTE -- This function should only be called within overrides of OnInit 
	/// and OnSelChanged.
	void EnableOK(const bool bEnable) const;

	/// Call SetSelection to set the selection in the active dialog.  pidl is 
	/// the LPITEMIDLIST of the path to be selected.  strPath is a CString 
	/// containing the path to be selected.
	/// NOTE -- This function should only be called within overrides of OnInit 
	/// and OnSelChanged.
	void SetSelection(const LPITEMIDLIST pidl) const;
	void SetSelection(const CString& strPath) const;

	/// Call SetStatusText to set the text in the Status area in the active 
	/// dialog.  strText is the text to be displayed.
	/// NOTE -- This function should only be called within overrides of OnInit 
	/// and OnSelChanged.
	void SetStatusText(const CString& strText) const;

private:
	static int __stdcall BrowseCallbackProc (HWND hwnd, UINT uMsg, 
									 LPARAM lParam, LPARAM lpData);

	typedef std::auto_ptr<char> AUTO_STR;
	AUTO_STR m_pchTitle;

	BROWSEINFO m_bi;
	char m_szSelected[MAX_PATH];
	CString m_strPath;
	HWND m_hwnd;
};




inline UINT CBrowseForFolder::GetFlags() const
{
	return m_bi.ulFlags;
}

/** 	Retreives the image associated with the selected folder. The image is 
	specified as an index to the system image list. 
*/
inline int CBrowseForFolder::GetImage() const
{
	return m_bi.iImage;
}

#endif // __MFCX_SHELLBROWSER_H__
