//
//   PersistColourDlg.h       CColorDialog derived class that remembers custom
//                            colours.
//

#ifndef   __MFCX_PERSISTCOLOURDLG_H
#define   __MFCX_PERSISTCOLOURDLG_H

class CPersistColourDlg : public CColorDialog
{
	DECLARE_DYNAMIC(CPersistColourDlg)

public:
	CPersistColourDlg (COLORREF clrInit = 0, 
	                   DWORD dwFlags = 0, 
	                   CWnd* pParentWnd = NULL);

// Statics
protected:
     enum { NCUSTOMCOLOURS = 16 };
     static COLORREF c_custColours [NCUSTOMCOLOURS];
     static COLORREF c_lastCustColours [NCUSTOMCOLOURS];
     static bool c_bNeedToInitCustColours;
     
     static void InitCustomColours();
     static void SaveCustomColours();

public:
     virtual int DoModal();
     
protected:
     virtual void DoDataExchange (CDataExchange* pDX);      // DDX / DDV support

protected:
	//{{AFX_MSG(CPersistColourDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif    // __MFCX_PERSISTCOLOURDLG_H
