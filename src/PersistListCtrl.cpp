/*!
	\file	PersistListCtrl.cpp      Persistent List Control

	\author	John Young
	\date	18 June 2000

	Copyright (c) 2000 Thistle Software
*/


#include "stdafx.h"
#include "../include/PersistListCtrl.h"
#include "../include/RegKey.h"

// Registry keys
const TCHAR szOrder[] = "Order";
const TCHAR szCol[] = "Col";
const TCHAR szSortCol[] = "SortCol";
const TCHAR szAsc[] = "Ascending";


CPersistentListCtrl::CPersistentListCtrl (const TCHAR* pszRegBase /*= NULL*/)
{
     if ( pszRegBase )
          m_sRegBase = pszRegBase;
}

/*virtual*/ CPersistentListCtrl::~CPersistentListCtrl()
{
}


void CPersistentListCtrl::SetRegBase (const TCHAR* pszRegBase)
{
     m_sRegBase = pszRegBase;
}




bool CPersistentListCtrl::Initialise (const TCHAR** ppszHeadings, 
                                      const TCHAR* pszRegBase /*= NULL*/)
{
     if ( pszRegBase )
          SetRegBase (pszRegBase);

	SetHeadings (ppszHeadings);

     CWordArray arrWidths;
     SetDefaultWidths (arrWidths);

     // Get the list column order and widths from the registry if at all 
     // possible...
     if ( !m_sRegBase.IsEmpty() )
     {
          MFCX::CRegKey reg (HKEY_CURRENT_USER, m_sRegBase);
          if ( reg.IsOpen() )
          {
               CRect rect;
               DWORD dwSortCol;
               int nSortCol = -1;
               bool bAscending = true;
          
               if ( reg.Query (szSortCol, dwSortCol) )
                    nSortCol = dwSortCol;
               
               DWORD dwAsc;
               if ( reg.Query (szAsc, dwAsc) )
                    bAscending = dwAsc != 0;

               if ( nSortCol != -1 )
                    m_headerCtrl.SetSortCol (nSortCol, bAscending);
     
               for (int i = 0; i < m_arrHeadings.GetSize(); i++)
               {
                    DWORD dwWidth;
                    CString sCol;
                    sCol.Format ("%s%d", szCol, i);
                    if ( reg.Query (sCol, dwWidth) )
                    	if ( dwWidth > 0 )		// Defend against silly values.
	                         arrWidths [i] = (WORD) dwWidth; 
               }
               
               CString sOrder;
               if ( reg.Query (szOrder, sOrder) )
               {
                    // Reads the order as a space separated list...
                    int index = 0;
                    TCHAR* s = sOrder.GetBuffer (0);
                    s = strtok (s, " ");
                    while ( index < m_arrHeadings.GetSize() && s )
                    {
                         m_arrOrder [index++] = atoi (s);
                         s = strtok (NULL, " ");
                    }
               }
          }
     }

	return InsertColumns (arrWidths);
}



BEGIN_MESSAGE_MAP(CPersistentListCtrl, CXListCtrl)
	//{{AFX_MSG_MAP(CPersistentListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CPersistentListCtrl::OnDestroy()
{
     // Now save the size and position to the registry
     if ( !m_sRegBase.IsEmpty() )
     {
          CRect rect;
          GetWindowRect (rect);
          MFCX::CRegKey reg (HKEY_CURRENT_USER, m_sRegBase, KEY_ALL_ACCESS);
          if ( reg.IsOpen() )
          {
               CString sOrder;
               int nColCount = m_headerCtrl.GetItemCount();
               for (int i = 0; i < nColCount; i++)
               {
                    LVCOLUMN lvCol;
                    TCHAR szBuf [256];
                    lvCol.pszText = szBuf;
                    lvCol.cchTextMax = sizeof szBuf - 1;
                    lvCol.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
                    VERIFY (GetColumn (i, &lvCol));
     
                    CString sCol;
                    sCol.Format ("%s%d", szCol, i);
                    reg.Set (sCol, lvCol.cx);
     
                    int index = -1;
                    for (int j = 0; j < nColCount; j++)
                         if ( strcmp (m_arrHeadings [j], lvCol.pszText) == 0 )
                              index = j;
                    ASSERT ( index != -1 );
                    TRACE ("Column %d is '%s'\n", i, m_arrHeadings [index]);
                    CString sIndex;
                    sIndex.Format ("%d", index);
                    if ( !sOrder.IsEmpty() )
                         sOrder += ' ';
                    sOrder += sIndex;
               }
     
               // The order of the columns...
               reg.Set (szOrder, sOrder);
               
               CXHeaderCtrl* pHeader = GetHeaderCtrl();
               reg.Set (szSortCol, pHeader->GetSortCol());
               reg.Set (szAsc, pHeader->GetSortAsc());
          }
     }     
	CXListCtrl::OnDestroy();
}



