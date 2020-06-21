//
//   CFolderComboBox.cpp      Combobox with a drive/folder drop-down.
//
//   Modified by John Young, May 2000, Young Associates.
//
//   Based on code that was found on www.codeguru.com, and was 
//   © 1999 Brigham W. Thorp
//   © 1998 James R. Twine
//   © 1997 Baldvin Hansson
//
//   Portions Copyright (c) 1999 - 2000 by PJ Naughter.  
//        CSystemImageList
//
//   Create a combobox and set the "Owner Draw Fixed" and "Has Strings" 
//   styles/checkboxes, and map this class to it.
//
//   See FolderCB.cpp for history


#ifndef   __MFCX_FOLDERCB_H
#define   __MFCX_FOLDERCB_H

#define ICONWIDTH   18


// Class which encapsulates access to the System image list which contains all 
// the icons used by the shell to represent the file system.
class CSystemImageList
{
public:
     CSystemImageList();
     ~CSystemImageList();
     
     CImageList& GetImageList();

protected:
     CImageList m_ImageList;
     static int m_nRefCount;
};





// Internal structure for folder storage...
//
typedef struct SFolder
{
     int m_iImageIndex;       // Index of picture in image list
     int m_iIndent;           // Indentation index
     CString m_sFolderName;   // Descriptive name of folder
     CString m_sPathName;     // Actual full pathname of folder or folders

}    SFOLDER;


class CFolderComboBox : public CComboBox
{
public:
    CFolderComboBox();

// Attributes
private:
    bool       m_bInitialized;
    SFOLDER*   m_pFolder;
    CImageList m_imgFolders;
    CImageList m_imgDisabledFolders;

public:
    CString GetSelectedPathName( void );

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFolderComboBox)
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implementation
public:
    virtual ~CFolderComboBox();

private:
    void  Initialize( void );

    // Generated message map functions
protected:
    //{{AFX_MSG(CFolderComboBox)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MFCX_FOLDERCOMBO_H
