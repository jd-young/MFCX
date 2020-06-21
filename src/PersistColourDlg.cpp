// @doc   UTILITIES
//
// @module     PersistColourDlg.cpp | Auto load / save of custom colours
//
// @end
//
//   Originally from Roger Onslow's CMyColorDialog class
//   Modified by John Young circa 20 Nov 1999.
//
//   Copyright (c) 1997 Roger Onslow
//
// HISTORY
//   Ver  Date      By   Description
//   ---------------------------------------------------------------------------
//   1.0  20-Nov-99 JY   Created from Roger Onslow's original version.
//

#include "stdafx.h"
#include "../include/PersistColourDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF crWhite = RGB (255, 255, 255);
const TCHAR szColourFmt[] = _T("Colour%d");
#define   SECTION   _T("Custom Colours")


/////////////////////////////////////////////////////////////////////////////
// CPersistColourDlg

IMPLEMENT_DYNAMIC(CPersistColourDlg, CColorDialog)

bool CPersistColourDlg::c_bNeedToInitCustColours = true;
COLORREF CPersistColourDlg::c_custColours[];
COLORREF CPersistColourDlg::c_lastCustColours[];


void CPersistColourDlg::InitCustomColours()
{
     CWinApp* pApp = ::AfxGetApp();

     for (int i = 0; i < NCUSTOMCOLOURS; i++)
     {
          CString sColour;
          sColour.Format (szColourFmt, i);
          c_lastCustColours [i] = 
          c_custColours [i] = pApp->GetProfileInt (SECTION, sColour, crWhite);
     }
}

void CPersistColourDlg::SaveCustomColours()
{
     CWinApp* pApp = ::AfxGetApp();

     for (int i = 0; i < NCUSTOMCOLOURS; i++)
     {
          if ( c_lastCustColours [i] != c_custColours [i] )
          {
               CString sColour;
               sColour.Format (szColourFmt, i);
               if ( c_custColours [i] == crWhite )
                    pApp->WriteProfileInt (SECTION, sColour, NULL);
               else pApp->WriteProfileInt (SECTION, sColour, c_custColours [i]);
               c_lastCustColours [i] = c_custColours [i];
          }
     }
}

CPersistColourDlg::CPersistColourDlg(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
     if ( c_bNeedToInitCustColours )
          InitCustomColours();
     
     m_cc.lpCustColors = c_custColours;
}

int CPersistColourDlg::DoModal()
{
     int code = CColorDialog::DoModal();
     if ( code == IDOK )
          SaveCustomColours();          // Don't save if cancelled
     return code;
}

void CPersistColourDlg::DoDataExchange (CDataExchange* pDX)
{
     CColorDialog::DoDataExchange (pDX);
}


BEGIN_MESSAGE_MAP(CPersistColourDlg, CColorDialog)
	//{{AFX_MSG_MAP(CPersistColourDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

