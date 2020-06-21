// 
//   DriveCB.h      Header file for the CDriveComboBox class
//
//   Written by Wilfried Roemer <Wilfried.Roemer@bln.siemens.de>
//   and hacked by John Young <jyoung@gol.com>
//
// DESCRIPTION
//   A CDrvListBos is a drop down list box which shows all drives on the local
//   computer with the same icons and names as the explorer. This control is
//   derived from CComboBoxEx. Use it instead of a CComboBoxEx and call
//   Populate in the OnInitDialog() of the parent dialog. By default, small
//   icons are displayed but you may specify ( true ) as parameter to Populate
//   to request the large icons. This will also increase the size of your edit
//   box! GetDataPtr will retrieve a pointer a string containing the root path
//   (e.g. for "Harddisk1 (C:)" this would be "C:\").
//

#ifndef   __MFCX_DRIVECB_H
#define   __MFCX_DRIVECB_H

#include <afxtempl.h>
#include "SysImageList.h"

class CDriveComboBox : public CComboBoxEx
{
public:
	CDriveComboBox();

     // call Populate during OnInitDialog to fill ComboBox with data
	int Populate (const TCHAR* pszPath = NULL,
	              const bool bLargeIcons = false);

// Attributes
public:

     CString GetPath (int index) const;

// Operations
public:

// Overrides
     //{{AFX_VIRTUAL(CDriveComboBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
private:
	CSysImageList m_SysImageList;		// holds the attached system image list

     class CPathInfo
     {
     public:
          CPathInfo() { m_iIndent = m_iIcon = 0; }
          CString   m_sDisplayName;
          CString   m_sPath;
          int       m_iIndent;
          int       m_iIcon;
     };

	class CPaths : public CList<CPathInfo*, CPathInfo*>
	{
	public:
	     CPaths() {};
	     ~CPaths() { MakeEmpty(); }
	     void MakeEmpty() 
	          { while ( !IsEmpty() ) delete RemoveHead(); }
	} m_list;           // List of drive/path strings

public:
	virtual ~CDriveComboBox();

// Generated message map functions

protected:
     void MakeEmpty();

     //{{AFX_MSG(CDriveComboBox)
	//}}AFX_MSG

     DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // __MFCX_DRIVECB_H
