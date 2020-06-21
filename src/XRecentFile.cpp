/*!
     \file     XRecentFile.cpp     A better CRecentFile class.

     Ideas based on...

          - CXRecentFileList.cpp
          - Visual Interface
          - by Todd C. Gleason
          - Begun:  02-20-1998

     This is much the same as CRecentFileList::UpdateMenu(), only that this 
     function takes into account that the MRU might be in a popup menu.  If it 
     is, the MRU items should still appear in the correct place.
     Note that this problem would only happen when the MRU ID is the only thing 
     in the submenu.
     
     (Actually, I only tested it with just the MRU ID vs. having something 
     before it.  It might be that it fails if there is simply nothing before it 
     in the submenu.)

     USAGE: Include this in your project and override your app's 
     LoadStdProfileSettings() with the example shown at the bottom of the file.
*/   

#include "stdafx.h"
#include "../include/XRecentFile.h"

/*!	This updates the recently used file list menu.

\param	pCmdUI		The CCmdUI object for the menu entry.
*/
void CXRecentFileList::UpdateMenu (CCmdUI* pCmdUI)
{
     int iMRU;
     ASSERT(m_arrNames != NULL);

     CMenu* pMenu = pCmdUI->m_pMenu;

     if ( pMenu == NULL )
          return;

     if ( m_strOriginal.IsEmpty() && pMenu != NULL)
          pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

     if (m_arrNames[0].IsEmpty())
     {
          // no MRU files
          if (!m_strOriginal.IsEmpty())
               pCmdUI->SetText(m_strOriginal);
          pCmdUI->Enable(FALSE);
          return;
     }

     ASSERT(pMenu == pCmdUI->m_pMenu);  // make sure preceding code didn't mess with it
     ASSERT(pMenu->m_hMenu);

     // look   for a submenu to use instead
     CMenu *pSubMenu;
     if (pMenu)
          pSubMenu = pMenu->GetSubMenu(pCmdUI->m_nIndex);
     if (pSubMenu) {
          ASSERT(pSubMenu->m_hMenu);
          pMenu =   pSubMenu;
     }

     ASSERT(pMenu->m_hMenu);

     for (iMRU = 0; iMRU < m_nSize; iMRU++) 
     {
          //pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);
          // This   hopefully will not remove the popup
          //pCmdUI->m_pMenu->RemoveMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);
          pMenu->RemoveMenu(pCmdUI->m_nID    + iMRU,   MF_BYCOMMAND);
     }


#ifndef   _MAC
     TCHAR szCurDir[_MAX_PATH];
     GetCurrentDirectory(_MAX_PATH, szCurDir);
     int nCurDir = lstrlen(szCurDir);
     ASSERT(nCurDir >= 0);
     szCurDir[nCurDir] = '\\';
     szCurDir[++nCurDir] = '\0';
#endif

     CString   strName;
     CString   strTemp;
     for (iMRU = 0; iMRU < m_nSize; iMRU++)
     {
#ifndef   _MAC
          if (!GetDisplayName(strName, iMRU, szCurDir, nCurDir))
               break;
#else
          if (!GetDisplayName(strName, iMRU, NULL, 0))
               break;
#endif
          // double up any '&' characters    so they   are not   underlined
          LPCTSTR   lpszSrc   = strName;
          LPTSTR lpszDest     = strTemp.GetBuffer(strName.GetLength()*2);
          while (*lpszSrc     != 0)
          {
               if (*lpszSrc == '&')
                    *lpszDest++ = '&';

               if (_istlead(*lpszSrc))
                    *lpszDest++ = *lpszSrc++;

               *lpszDest++ = *lpszSrc++;

          }
          *lpszDest = 0;

          strTemp.ReleaseBuffer();

          // insert mnemonic + the file name
          TCHAR buf[10];
          wsprintf(buf, _T("&%d "), (iMRU+1+m_nStart) % 10);
          /*pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
          MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
          CString(buf) + strTemp);*/

            // Note we use our pMenu    which may not be pCmdUI->m_pMenu
          pMenu->InsertMenu(pCmdUI->m_nIndex++,
               MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
               CString(buf) + strTemp);
     }

     // update end menu count
     pCmdUI->m_nIndex--; // point to    last menu added
     pCmdUI->m_nIndexMax = pMenu->GetMenuItemCount();

     pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}



#if  0
#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*  if you want to use this code instead of the CRecentFileList entirely, you'll 
need to make sure the app creates a CXRecentFileList instead of a 
RecentFileList. Do this by overriding the CWinApp::LoadStdProfileSettings(). An 
example: */

void CVIStudioApp::LoadStdProfileSettings (UINT nMaxMRU)
{
     // We override the CWinApp version to make it use CXRecentFileList
     // instead of CRecentFileList.
     
     ASSERT_VALID ( this );
     ASSERT ( m_pRecentFileList == NULL );
     
     // Copied from APPCORE.cpp
     const TCHAR szFileSection[] = _T("Recent File List");
     const TCHAR szFileEntry[] = _T("File%d");
     
     // Calling the original with zero means that the recent file list is not
     // created.
     CWinApp::LoadStdProfileSettings (0);

     // Now we create the recent file list (with our version)
     if ( nMaxMRU != 0 )
     {
          // Here's the important part--overriding CRecentFileList
          m_pRecentFileList = new CXRecentFileList (0, szFileSection,  
                                                    szFileEntry, nMaxMRU);
          m_pRecentFileList->ReadList();
     }
}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
#endif
