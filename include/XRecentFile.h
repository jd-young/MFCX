/*!
     \file     XRecentFile.h

     Based on ideas from 
          - CXRecentFileList.h
          - Visual Interface
          - by Todd C. Gleason
          - Begun:  02-19-1998

     This class is a subclass of CRecentFileList which overrides the 
     UpdateMenu() member function. 

     This class may be freely used, distributed, and modified.  The four-line 
     header and this comment must be redistributed unmodified with any modified 
     version of this class.  In addition, whenever a newly modified form of this 
     class is redistributed, the author must email tcg@visint.com with the 
     changes.

     This file, and the implementation file, are 
     Copyright (c) 1998 Visual Interface.
*/

#ifndef   __MFXCX_RECENTFILE_H
#define   __MFXCX_RECENTFILE_H

#include < afxadv.h>  // for CRecentFileList

/*!  This is an enhanced CRecentFileList which allows you to have the recent 
     files in a separate popup menu.
     
     It overcomes a 'shortcoming' (bug) in the MFC version. It simply replaces
     the UpdateMenu method to do this. To use this override your app's
     LoadStdProfileSettings() with the example shown below...

\verbatim
     /*! We override the CWinApp version to make it use CXRecentFileList instead 
          of CRecentFileList.
     *\/
     void CMyWinApp::LoadStdProfileSettings (UINT nMaxMRU)
     {
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
\endverbatim
*/
class CXRecentFileList : public CRecentFileList 
{
public:
     CXRecentFileList (UINT nStart, LPCTSTR lpszSection,
                     LPCTSTR lpszEntryFormat, int nSize,
                     int nMaxDispLen = AFX_ABBREV_FILENAME_LEN)
           : CRecentFileList (nStart, lpszSection,lpszEntryFormat, 
                              nSize, nMaxDispLen) {}

     /// Override to pro
     virtual void UpdateMenu(CCmdUI* pCmdUI);

};


#endif    // __MFXC_XRECENTFILE_H
