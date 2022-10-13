//
//   DriveCB.cpp    A combobox that displays the drives & folders.
// 
//   Written by Wilfried Roemer, and mangled by John Young.
//
// HISTORY
//   Ver  Date      By   Description
//   ---------------------------------------------------------------------------
//   2.0   4-May-00 JY   Modified from original
//

#include "stdafx.h"
#include "../include/DriveCB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Error return codes for Populate
#define   DCB_OUTOFMEMORY          -1
#define   DCB_BADDRIVESTRINGS      -2

CDriveComboBox::CDriveComboBox()
{
}

CDriveComboBox::~CDriveComboBox()
{
}

void CDriveComboBox::MakeEmpty() 
{
     ResetContent();
     m_list.MakeEmpty();
}



BEGIN_MESSAGE_MAP(CDriveComboBox, CComboBoxEx)
	//{{AFX_MSG_MAP(CDriveComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Load Items - collects all drive information and place it into the listbox,
// return number of items added to the list: a negative value is an error;
// 
int CDriveComboBox::Populate (const TCHAR* pszPath /*= NULL*/, 
                              const bool bLargeIcons /*= false*/)
{
     // Populuates the drop-down with the local and mapped network drives + the
     // passed in path.  For example if the passed in path is 
     //
     //   "d:\projects\rugby\stadiums\MurrayField"
     //
     // then the drop-down would look something like...
     //
     //   + jyoung on PURPLE_HAZE
     //     + BOOTDISC (C:)
     //     - WORK (D:)
     //       - projects
     //         - rugby
     //           - stadiums
     //             - MurrayField
     //     + INTERNET (E:)
     //     + Audio CD (F:)
     //     + D on 'SANDMAN' (S:)
     //
     
     MakeEmpty();

	DWORD dIconSize = bLargeIcons ? SHGFI_LARGEICON : SHGFI_SMALLICON;

     // Get the 'My Computer' info and add it first.
     LPITEMIDLIST pidl;
     SHGetSpecialFolderLocation (this->m_hWnd, CSIDL_DRIVES, &pidl);

	SHFILEINFO fileInfo;
	DWORD r = SHGetFileInfo ((LPCTSTR) pidl, 0, &fileInfo, sizeof fileInfo,
			               SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX | 
                              SHGFI_PIDL | dIconSize);

	LPMALLOC pMalloc;
	//Retrieve a pointer to the shell's IMalloc interface
	if ( SUCCEEDED (SHGetMalloc (&pMalloc)) )
	{
		// free the PIDL that SHBrowseForFolder returned to us.
		pMalloc->Free (pidl);
		// release the shell's IMalloc interface
		(void) pMalloc->Release();
	}
	CPathInfo* pInfo = new CPathInfo;
	if ( pInfo )
	{
          m_list.AddTail (pInfo);
          pInfo->m_sPath = "";
          pInfo->m_sDisplayName = fileInfo.szDisplayName;
          pInfo->m_iIcon = fileInfo.iIcon;   // index into system image list
          pInfo->m_iIndent = 0;
     }

	// allocate buffer for the drive strings: GetLogicalDriveStrings will tell
	// me how much is needed (minus the trailing zero-byte)
	DWORD len = GetLogicalDriveStrings (0, NULL);
	int size = sizeof (_T(""));
	TCHAR* pDrives = new TCHAR [len + sizeof ( _T(""))]; // + for trailer
	if ( pDrives == NULL )
	     return DCB_OUTOFMEMORY;

     if ( GetLogicalDriveStrings (len, pDrives) != len - 1 )
     {
          delete [] pDrives;
		return DCB_BADDRIVESTRINGS;
	}
	
	ASSERT ( pDrives != NULL );

     // Now that we have the drives, make the list of drive/path strings, 
     // including the passed in path (if given).  This list is persistent.
     //
     CString sPath = pszPath;
     CString sDrive = sPath.Left (3);
     CString sDir = sPath.Right (sPath.GetLength() - 2);

	TCHAR *pszDrive = pDrives;
	size_t lenDrive = strlen (pszDrive);
     int nSelected = 0;       // Default to My Computer
	while( lenDrive > 0 )
	{
		SHFILEINFO fileInfo;
		if ( SHGetFileInfo (pszDrive, 0, &fileInfo, sizeof fileInfo,
			                    SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX | 
			                    dIconSize ) )
          {
     	     CPathInfo* pInfo = new CPathInfo;
     	     if ( pInfo )
     	     {
                    m_list.AddTail (pInfo);
                    pInfo->m_sPath = pszDrive;
                    pInfo->m_sDisplayName = fileInfo.szDisplayName;
                    pInfo->m_iIcon = fileInfo.iIcon;
                    pInfo->m_iIndent = 1;
               }
          }

          // Now deal with the passed in path...               
          if ( sDrive.CompareNoCase (pszDrive) == 0 )
          {
               // Add the passed in directories...
               CString sPath = pszDrive;
               int iIndent = 2;
               TCHAR* s = strtok (sDir.GetBuffer (0), "\\/");
               while ( s )
               {
                    CPathInfo* pInfo = new CPathInfo;
                    if ( pInfo )
                    {
                         m_list.AddTail (pInfo);
                         if ( !sPath.IsEmpty() &&
                              sPath [sPath.GetLength() - 1] != '\\' )
                              sPath += '\\';
                         sPath += s;
                         pInfo->m_sPath = sPath;
                         pInfo->m_iIndent = iIndent++;

               		SHGetFileInfo (sPath, 0, &fileInfo, sizeof fileInfo,
               		               SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX | 
               			          dIconSize);

                         pInfo->m_sDisplayName = fileInfo.szDisplayName;
                         pInfo->m_iIcon = fileInfo.iIcon;
                    }
                    s = strtok (NULL, "\\/");
               }
               nSelected = m_list.GetCount() - 1;
          }
	
          // setup for next drive string (next round in loop)
          pszDrive += lenDrive + 1;
		lenDrive = strlen (pszDrive);
	}
     delete [] pDrives;

     // this structure is used to enter items into the CComboBoxEx, preset
	// some parts before loop to fill box starts; I need to set both 
	// standard and selected images or I won't see an icon when an entry
	// is selected; the lParam is set so I can have direct access to the
	// selected root path.

	COMBOBOXEXITEM cbItem;
	cbItem.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT |
	              CBEIF_TEXT | CBEIF_LPARAM;
	cbItem.cchTextMax = 0;	     // is ignored
	cbItem.iItem = -1;            // insert at end

	for (POSITION pos = m_list.GetHeadPosition(); pos; )
	{
          CPathInfo* pInfo = m_list.GetNext (pos);

		// insert icon and string into list box

		cbItem.pszText = (TCHAR*) (const TCHAR*) pInfo->m_sDisplayName;
		cbItem.lParam  = (LPARAM) (const TCHAR*) pInfo;

		cbItem.iSelectedImage =
          cbItem.iImage = pInfo->m_iIcon;    // index into system image list
          cbItem.iIndent = pInfo->m_iIndent;

		int index = InsertItem (&cbItem);
          ASSERT ( index >= 0 );
     }

     // have items in list, now provide image list: it seems I cannot just
	// pass the system image list (see notes on System Image List by Marc
	// Otway) so I create a copy on the fly (code based also on Marc's)


     if ( GetImageList() == NULL )
     {
          // make the background colour transparent, works better for lists etc.
          CImageList* pImgList = m_SysImageList.GetImageList();
	     pImgList->SetBkColor (CLR_NONE);

	     // don't forget to set it up!
	     VERIFY ( SetImageList (pImgList) == NULL );
     }

     if ( nSelected != -1 )
          SetCurSel (nSelected);

	return GetCount();
}


CString CDriveComboBox::GetPath (int index) const
{
     ASSERT ( index >= 0 && index < GetCount() );
     
     CPathInfo* pInfo = (CPathInfo*) GetItemDataPtr (index);
     return pInfo->m_sPath;
}
