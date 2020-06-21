/**
 *	@file VerInfo.cpp	CVersionInfo class
 *
 *	@author   John Young
 *   @date     4 September 1996
 *
 *	Extracts information from the the VERSIONINFO structure for the app as
 *	follows...
 *
 *   \verbatim
  		VS_VERSION_INFO VERSIONINFO
       	:
       	PRODUCTVERSION 1,2,3,1441
       	:
       	BEGIN
       		BLOCK "StringFileInfo"
       		BEGIN
       			BLOCK "080904b0"
       			BEGIN
  					:
       				VALUE "Author", "Written by John Young\0"
       				VALUE "FileDescription", "Some cool utility\0"
       				VALUE "InternalName", "CoolUtility\0"
       				VALUE "LegalCopyright", "Copyright © 1996 John Young\0"
       				VALUE "ProductName", "Cool utility\0"
       				VALUE "ProductVersion", "1,2,3,1441\0"
       				VALUE "BuildDate", "26-Feb-1996\0"
       				VALUE "BetaBuild", "ß1\0"
       			END
       		END
       		BLOCK "VarFileInfo"
       		BEGIN
       			VALUE "Translation", 0x809, 1200
       		END
       	END
     \endverbatim
 
     To use this class, create a \b VERSIONINFO resource in your .rc2 file and
     fill it in similar to the above.  Then use the CVersionInfo as follows...
     
     \code
     CVersionInfo verApp;

	CString sTitle;
	if ( verApp.GetRevision() )
          sTitle.Format ("%s %d.%d.%d", verApp.GetName(), 
                              verApp.GetMajorVer(), 
                              verApp.GetMinorVer(),
     				     verApp.GetRevision());
     else sTitle.Format ("%s %d.%d", verApp.GetName(), 
                              verApp.GetMajorVer(), 
                              verApp.GetMinorVer());

     CString sVersion;
    	sVersion.Format ("Build %d, %s", verApp.GetBuildNr(), verApp.GetBuildDate());
#ifdef    BETA
     sVersion += CString (" ") + verApp.GetBetaBuild();
#endif
     \endcode
 */
 
/*	class CVersionInfo
 *		GetMajorVer()		returns first digit in PRODUCTVERSION
 *		GetMinorVer()		returns second digit in PRODUCTVERSION
 *	     GetRevision()		returns third first digit in PRODUCTVERSION
 *	     GetBuildNr()		returns fourth digit in PRODUCTVERSION
 *		GetName()			returns "InternalName"
 *	     GetDescription()	returns "ProductName"
 *	     GetAuthor()		returns "Author"     
 *	     GetVersion()		returns "ProductVersion"
 *	     GetCopyright()		returns "LegalCopyright"
 *	     GetBetaBuild()		returns "BetaBuild"
 *	     GetBuildDate()		returns "BuildDate"
 */

#include	"stdafx.h"
#include  "../include/VerInfo.h"
#include  <winver.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**  Gets the VERSIONINFO resource from the current module and extracts 
 *   various fields from it.
 */
CVersionInfo::CVersionInfo()
{
     DWORD dwHandle;                          // version info handle
     DWORD dwSize;                            // size of version buffer
     TCHAR szAppName [_MAX_PATH+1];            // application filename

     m_nMajorVer =
     m_nMinorVer =
     m_nRevision =
     m_nBuildNr = 0;

     // Retrieve a handle to the version information.
     ::GetModuleFileName (AfxGetInstanceHandle(), szAppName, sizeof szAppName);
     dwHandle = 0;
     if ( dwSize = ::GetFileVersionInfoSize (szAppName, &dwHandle) )
     {
          UINT  nVerInfoSize;
          LPSTR pszTemp;

          // Allocate an array for the version info and read it.
          LPSTR pVerInfo = new TCHAR [(int) dwSize];
          ::GetFileVersionInfo (szAppName, dwHandle, dwSize, pVerInfo);

          // Get the program version string.
          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\ProductVersion",
                                (void **)(&pszTemp),  &nVerInfoSize ) )
          {
               m_sVersion = pszTemp;
               ExtractVersion();
          }
          else m_sVersion = "No version info";
 
          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\InternalName",
                                (void **)(&pszTemp),  &nVerInfoSize) )
               m_sName = pszTemp;

          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\ProductName",
                                (void **)(&pszTemp),  &nVerInfoSize) )
               m_sDescription = pszTemp;

          // Get the copyright information.
          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\LegalCopyright",
                                (void **)(&pszTemp),  &nVerInfoSize) )
               m_sCopyright = pszTemp;
          else m_sCopyright = "No copyright info";

          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\BetaBuild",
                                (void **)(&pszTemp),  &nVerInfoSize) )
                m_sBetaBuild = pszTemp;


          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\Author",
                                (void **)(&pszTemp),  &nVerInfoSize) )
               m_sAuthor = pszTemp;

          if ( ::VerQueryValue (pVerInfo,
                                "\\StringFileInfo\\080904b0\\BuildDate",
                                (void **)(&pszTemp),  &nVerInfoSize) )
               m_sBuildDate = pszTemp;

          // Free the version info buffer.
          delete pVerInfo;
     }
}


BOOL CVersionInfo::ExtractVersion()
{
     return ExtractVersion (m_sVersion,
                           &m_nMajorVer, &m_nMinorVer, &m_nRevision, &m_nBuildNr);
}

BOOL CVersionInfo::ExtractVersion (const TCHAR* pszVer,
                                   int* nMajor, int* nMinor, int* nRev, int* nBuild)
{
	// Version string is always in the form MM,mm,rr,bb where
     // MM - major version, mm = minor version, rr - revision, bb - build
     // Now extract them...

     *nMajor = atoi (pszVer);
     if ( pszVer = strchr (pszVer, ',') )
     {
          *nMinor = atoi (++pszVer);
          if ( pszVer = strchr (pszVer, ',') )
          {
               *nRev = atoi (++pszVer);
               if ( pszVer = strchr (pszVer, ',') )
                    *nBuild = atoi (++pszVer);
          }
     }
     return pszVer ? TRUE : FALSE;
}
