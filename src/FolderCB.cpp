// 
//   FolderCB.cpp        Folders drop-list combo box.
//
//   Modified by John Young, May 2000
//   Young Associates
//
//   Based on code that was found on www.codeguru.com, and was 
//   © 1999 Brigham W. Thorp
//   © 1998 James R. Twine
//   © 1997 Baldvin Hansson
//
//   Portions Copyright (c) 1999 - 2000 by PJ Naughter.  
//   All rights reserved.
//
// HISTORY
//   Ver  Date      By   Description
//   ---------------------------------------------------------------------------
//   1.0   4-May-00 JY   Added support for system images using 

#include "stdafx.h"
#include <direct.h>
#include <shlobj.h>
#include "../include/FolderCB.h"
#include "../include/MFCXres.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma warning(disable : 4701)


CFolderComboBox::CFolderComboBox()
{
	m_bInitialized = false;
}

CFolderComboBox::~CFolderComboBox()
{
}

BEGIN_MESSAGE_MAP(CFolderComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFolderComboBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderComboBox message handlers

int CFolderComboBox::OnCreate( LPCREATESTRUCT pCStruct ) 
{
	if( CComboBox::OnCreate( pCStruct ) == -1 )
		return( -1 );
	
	// Add the drives and images to the combo box
	Initialize();

	// Select First Item By Default
	SetCurSel( 0 );

	return( 0 );
}


void CFolderComboBox::PreSubclassWindow() 
{
	// Add the drives and images to the combo box
	Initialize();
	
	CComboBox::PreSubclassWindow();

	// Select First Item By Default
	SetCurSel( 0 );

	return;
}


void CFolderComboBox::Initialize( void )
{
//	SFOLDER* tmpFolder;
	CString tmpPath;

	int iMainAddedItem = -1;
	int iSubAddedItem = -1;

	// Don't reinitialize if already initialized
	if(m_bInitialized)
		return;

	// Initialize image lists
	m_imgFolders.Create (IDB_MFCX_FOLDERS, 18, 1, RGB(255, 255, 255));
	m_imgDisabledFolders.Create (IDB_MFCX_FOLDERS_DISABLED, 18, 1, RGB(255, 255, 255));

	// Initialize drive data
	CString tmpStr;
	char szPath[_MAX_PATH];

#if  0
	// Get the name of the Personal Documents folder and add it
	tmpStr.LoadString(IDS_MFCX_DOCUMENTFOLDERS);
	iMainAddedItem = AddString(tmpStr);
	if(iMainAddedItem != CB_ERRSPACE)
	{
		tmpPath = afxEmptyString;
		// Create the associated data, but don't insert the
		// path until we have the all of the sub paths
		m_pFolder = new SFOLDER;
		m_pFolder->m_iImageIndex = 1;
		m_pFolder->m_iIndent = 1;
		m_pFolder->m_sFolderName = tmpStr;
		m_pFolder->m_sPathName = tmpPath;

		// Get the name of the "My Documents" folder and add it
		if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_PERSONAL, FALSE)))
		{
			tmpStr.LoadString(IDS_MFCX_PERSONAL);
			iSubAddedItem = AddString(tmpStr);
			if(iSubAddedItem != CB_ERRSPACE)
			{
				tmpFolder = new SFOLDER;
				tmpFolder->m_iImageIndex = 2;
				tmpFolder->m_iIndent = 2;
				tmpFolder->m_sFolderName = tmpStr;
				tmpFolder->m_sPathName = szPath;
				tmpPath += ";";
				tmpPath += szPath;
				SetItemDataPtr(iSubAddedItem, tmpFolder);
			}
		}

		if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_FAVORITES, FALSE)))
		{
			tmpStr.LoadString(IDS_MFCX_FAVORITES);
			iSubAddedItem = AddString(tmpStr);	// Set folder name/text
			if(iSubAddedItem != CB_ERRSPACE)
			{
				tmpFolder = new SFOLDER;
				tmpFolder->m_iImageIndex = 2;
				tmpFolder->m_iIndent = 2;
				tmpFolder->m_sFolderName = tmpStr;
				tmpFolder->m_sPathName = szPath;
				tmpPath += ";";
				tmpPath += szPath;
				SetItemDataPtr(iSubAddedItem, tmpFolder);
			}
		}

		tmpPath.Delete(0,1);
		m_pFolder->m_sPathName = tmpPath;
		SetItemDataPtr(iMainAddedItem, (void*)m_pFolder);
	}

	tmpStr.LoadString(IDS_MFCX_SYSTEMFOLDERS);
	iMainAddedItem = AddString(tmpStr);	// Set folder name/text
	if(iMainAddedItem != CB_ERRSPACE)
	{
		tmpPath = afxEmptyString;

		// Create the associated data, but don't insert the
		// path until we have the all of the sub paths
		m_pFolder = new SFOLDER;
		m_pFolder->m_iImageIndex = 3;
		m_pFolder->m_iIndent = 1;
		m_pFolder->m_sFolderName = tmpStr;
		m_pFolder->m_sPathName = szPath;

		char *libvar = getenv("PATH");
		if (libvar != NULL)
		{
			tmpStr.LoadString(IDS_MFCX_PATH);
			iSubAddedItem = AddString(tmpStr);	// Set folder name/text
			if(iSubAddedItem != CB_ERRSPACE)
			{
				tmpFolder = new SFOLDER;
				tmpFolder->m_iImageIndex = 4;
				tmpFolder->m_iIndent = 2;
				tmpFolder->m_sFolderName = tmpStr;
				tmpFolder->m_sPathName = libvar;
				tmpPath += ";";
				tmpPath += libvar;
				SetItemDataPtr(iSubAddedItem, tmpFolder);
			}
		}

		if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE))) 
		{
			tmpStr.LoadString(IDS_MFCX_DESKTOP);
			iSubAddedItem = AddString(tmpStr);	// Set folder name/text
			if(iSubAddedItem != CB_ERRSPACE)
			{
				tmpFolder = new SFOLDER;
				tmpFolder->m_iImageIndex = 0;
				tmpFolder->m_iIndent = 2;
				tmpFolder->m_sFolderName = tmpStr;
				tmpFolder->m_sPathName = szPath;
				tmpPath += ";";
				tmpPath += szPath;
				SetItemDataPtr(iSubAddedItem, tmpFolder);
			}
		}

		if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_FONTS, FALSE)))
		{
			tmpStr.LoadString(IDS_MFCX_FONTS);
			iSubAddedItem = AddString(tmpStr);	// Set folder name/text
			if(iSubAddedItem != CB_ERRSPACE)
			{
				tmpFolder = new SFOLDER;
				tmpFolder->m_iImageIndex = 4;
				tmpFolder->m_iIndent = 2;
				tmpFolder->m_sFolderName = tmpStr;
				tmpFolder->m_sPathName = szPath;
				tmpPath += ";";
				tmpPath += szPath;
				SetItemDataPtr(iSubAddedItem, tmpFolder);
			}
		}

		tmpPath.Delete(0,1);
		m_pFolder->m_sPathName = tmpPath;
		SetItemDataPtr(iMainAddedItem, (void*)m_pFolder);

	}
#endif

	if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_DRIVES, FALSE)))
	{
		SFOLDER* tmpMainFolder;
		tmpPath = afxEmptyString;
		int iTmpAddedItem = -1;
		tmpStr.LoadString (IDS_MFCX_MYCOMPUTER);
		iMainAddedItem = AddString(tmpStr);	// Set folder name/text
		if(iMainAddedItem != CB_ERRSPACE)
		{
			CString tmpStr, sLocalHardDrives;
			SFOLDER* tmpFolder;

			// Finish with the "My Computer stuff"
			tmpMainFolder = new SFOLDER;
			tmpMainFolder->m_iImageIndex = 6;
			tmpMainFolder->m_iIndent = 1;
			tmpMainFolder->m_sFolderName = tmpStr;
			tmpMainFolder->m_sPathName = szPath;
			SetItemDataPtr(iMainAddedItem, tmpMainFolder);

			// Add the local hard drive stuff
			tmpStr.LoadString (IDS_MFCX_LOCALHARDRIVES);
			iTmpAddedItem = AddString(tmpStr);	// Set folder name/text
			if(iTmpAddedItem != CB_ERRSPACE)
			{
				tmpFolder = new SFOLDER;
				tmpFolder->m_iImageIndex = 4;
				tmpFolder->m_iIndent = 2;
				tmpFolder->m_sFolderName = tmpStr;
				tmpFolder->m_sPathName = szPath;
			}

			// Since we have "My Computer" add the drive list underneath.
			int drive, curdrive;
			curdrive = _getdrive();

			// If we can switch to the drive, it exists.
			for(drive = 1; drive <= 26; drive++)
			{
				if(!_chdrive(drive))
				{
					CString sDriveLetter;
					sDriveLetter.Format("%c:", drive + 'A' - 1);
					iSubAddedItem = AddString(sDriveLetter);	// Set folder name/text
					if(iSubAddedItem != CB_ERRSPACE)
					{
						m_pFolder = new SFOLDER;
						CString sTmpDrvLetter;
						sTmpDrvLetter = sDriveLetter;
						sTmpDrvLetter += "\\";
						tmpPath += ";";
						tmpPath += sTmpDrvLetter;
						UINT uiRet = GetDriveType(sTmpDrvLetter);
						switch(uiRet)
						{
							case DRIVE_REMOVABLE:
								m_pFolder->m_iImageIndex = 5;
								break;
							case DRIVE_REMOTE:
								m_pFolder->m_iImageIndex = 7;
								break;
							case DRIVE_CDROM:
								m_pFolder->m_iImageIndex = 6;
								break;
							case DRIVE_FIXED:
								m_pFolder->m_iImageIndex = 4;
								sLocalHardDrives += ";";
								sLocalHardDrives += sDriveLetter;
								break;
							default:
								m_pFolder->m_iImageIndex = 4;
						}
						m_pFolder->m_iIndent = 2;
						m_pFolder->m_sFolderName = sDriveLetter;
						m_pFolder->m_sPathName = sDriveLetter;
						SetItemDataPtr(iSubAddedItem, m_pFolder);
					}
				}
			}
			_chdrive(curdrive);

			if(iTmpAddedItem != CB_ERRSPACE)
			{
				// Remove the beginning colon
				sLocalHardDrives.Delete(0, 1);
				tmpFolder->m_sPathName = sLocalHardDrives;
				tmpFolder->m_sFolderName += " (";
				tmpFolder->m_sFolderName += sLocalHardDrives;
				tmpFolder->m_sFolderName += ")";
				SetItemDataPtr(iTmpAddedItem, tmpFolder);
			}
			if (iMainAddedItem != CB_ERRSPACE)
			{
				tmpPath.Delete(0,1);
				tmpMainFolder->m_sPathName = tmpPath;
				SetItemDataPtr(iMainAddedItem, (void*)tmpMainFolder);
			}
		}
	}

	// Set initialized flag
	m_bInitialized = true;
}


void CFolderComboBox::DrawItem( LPDRAWITEMSTRUCT pDIStruct )
{
	static CString sFolder, tmpStr;									// No Need To Reallocate Each Time

	CDC dcContext;
	CRect rItemRect(pDIStruct->rcItem);
	CRect rBlockRect(rItemRect);
	CRect rTextRect(rBlockRect);
	CBrush brFrameBrush;

	int iItem = pDIStruct -> itemID;
	int	iState = pDIStruct -> itemState;
	int iImageIndex = 0;
	int iIndent;

	COLORREF crColor = GetSysColor(COLOR_WINDOW);
	COLORREF crNormal = GetSysColor(COLOR_WINDOW);
	COLORREF crSelected = GetSysColor(COLOR_HIGHLIGHT);
	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);

	// Attach the CDC object
	if(!dcContext.Attach(pDIStruct -> hDC ))
		return;

	brFrameBrush.CreateStockObject(BLACK_BRUSH);

	// If it's selected...
	if(iState & ODS_SELECTED)
	{
		// Set inverted text color (with mask)
		dcContext.SetTextColor((0x00FFFFFF & ~(crText)));
		// Set background to highlight color
		dcContext.SetBkColor( crSelected );
		dcContext.FillSolidRect( &rBlockRect, crSelected );
	}
	else
	{
		// not selected - set standard attributes
		dcContext.SetTextColor( crText );
		// Set background to normal color
		dcContext.SetBkColor( crNormal );
		dcContext.FillSolidRect( &rBlockRect, crNormal );
	}

	// If item has the focus...draw the focus rectangle
	if( iState & ODS_FOCUS )
		dcContext.DrawFocusRect( &rItemRect );

	// Draw folder text and icon

	// If an item is selected...	
	if( iItem != -1 )
	{
		// Get text for normal and disabled
		CString sFolder;
		GetLBText(iItem, sFolder);
		SFOLDER* tmpFolder = (SFOLDER*)GetItemDataPtr(iItem);
		iIndent = tmpFolder->m_iIndent;

		// Calculate Text Area
		// Set start of text offset a bit
		rTextRect.left += (ICONWIDTH * iIndent) + 2;
		rTextRect.top += 2;

		// Calculate icon display area and set the width
		rBlockRect.DeflateRect(CSize(2, 2));
		rBlockRect.right = ICONWIDTH * iIndent;

		iImageIndex = tmpFolder->m_iImageIndex;
		if( iState & ODS_DISABLED )
		{
			crColor = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
			dcContext.SetTextColor(crColor);
			CPoint pt(pDIStruct->rcItem.left, pDIStruct->rcItem.top);
			pt.x = pDIStruct->rcItem.left + ((iIndent - 1) * ICONWIDTH);
			m_imgDisabledFolders.Draw(&dcContext, iImageIndex, pt, ILD_TRANSPARENT);
		}
		else
		{
			CPoint pt(pDIStruct->rcItem.left, pDIStruct->rcItem.top);
			pt.x = pDIStruct->rcItem.left + ((iIndent - 1) * ICONWIDTH);
			m_imgFolders.Draw(&dcContext, iImageIndex, pt, ILD_NORMAL);
		}

		dcContext.SetBkMode(TRANSPARENT);
		dcContext.TextOut(rTextRect.left, rTextRect.top, sFolder);
	}

	// Detach DC
	dcContext.Detach();
}

CString CFolderComboBox::GetSelectedPathName( void )
{
	CString tmpStr;

	// Get current selection
	int	iSelectedItem = GetCurSel();

	// If nothing selected, return an empty string
	if(iSelectedItem == CB_ERR)
		return(afxEmptyString);

	// Get pathname
	SFOLDER* tmpFolder = (SFOLDER*)GetItemDataPtr(iSelectedItem);

	return(tmpFolder->m_sPathName);
}

void CFolderComboBox::OnDestroy() 
{
	SFOLDER* tmpFolder;
	// Iterate through the items destroying all of the
	// associated data

	int iListCount = GetCount();
	for (int x = 0; x < iListCount; x++)
	{
		tmpFolder = (SFOLDER*)GetItemDataPtr(x);
		delete tmpFolder;
	}

	CComboBox::OnDestroy();
	
	
}
