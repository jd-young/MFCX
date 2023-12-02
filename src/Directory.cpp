/*!
     \file     Directory.cpp
     \author   John Young
     \date     16 May 2001
     
     Copyright (c) 2001 Thistle Software
*/


#include "stdafx.h"
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../include/Filename.h"
#include "../include/Directory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*static*/ CString CDirectory::GetCurrentDir()
{
	char szDir [_MAX_PATH];
     ::GetCurrentDirectory (sizeof szDir - 1, szDir);
     return szDir;
}


/*!  Called to set the working directory to that of the given file name.

\param    pszPath   The path of a file, or the sub-directory to change to.
*/
bool CDirectory::SetCurrentDir (const TCHAR* pszPath)
{
 	struct _stat stat;
	if ( _stat (pszPath, &stat) != 0 )
	     return false;  // Doesn't exist.
     
     if ( (stat.st_mode & S_IFDIR) == 0 )
     {
          // It's a file - extract the directory name.
     	char szDrive [_MAX_DRIVE];
     	char szDir [_MAX_DIR];
     
     	_tsplitpath (pszPath, szDrive, szDir, nullptr, nullptr);

		CString sCwd (szDrive);
		sCwd += szDir;
		return _tchdir (sCwd) == 0;
     }
     return _tchdir (pszPath) == 0;
}

/*static*/ bool CDirectory::CreateDir (const TCHAR* pszPath)
{
	struct _stat stat;
	if ( _stat (pszPath, &stat) != 0 )
	{
		// Doesn't exist... create it.
		return _mkdir (pszPath) == 0;
	}
	return true;	// Already exists
}

/*!  Deletes the given directory recursively.
 *
 * \param pszDir         The path to the directory to delete.
 * \param bRecycleBin    true if the directory (and any contained files) are 
 *                       sent to the recycle bin.
 * \return true if successful, false, otherwise.
 */
bool DeleteDir (const TCHAR* pszDir, bool bRecycleBin = false)
{
     CString sDir = CFilename::IsRelativePath (pszDir)
                         ? CFilename::GetFullPath (CDirectory::GetCurrentDir(), pszDir)
                         : pszDir;

     int len = sDir.GetLength();
     TCHAR* pszzDir = sDir.GetBuffer (len + 1);
     pszzDir [len + 1] = '\0';    // must be double null-terminated.
     
     SHFILEOPSTRUCT fileop;
     fileop.hwnd   = NULL;         // no status display
     fileop.wFunc  = FO_DELETE;    // delete operation
     fileop.pFrom  = pszzDir;      // source file name as double null terminated string
     fileop.pTo    = NULL;         // no destination needed
     fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;  // do not prompt the user
     if ( bRecycleBin )
          fileop.fFlags |= FOF_ALLOWUNDO;
     
     fileop.fAnyOperationsAborted = FALSE;
     fileop.hNameMappings         = NULL;
     fileop.lpszProgressTitle     = NULL;
     
     int ret = SHFileOperation (&fileop);
     
     return ret == 0;
}



/*! 	Adds a trailing slash if there is none, except if the sDir parameter is 
	empty.

\param	sDir		The directory string to add a tailing space to.
*/
/*static*/ void CDirectory::AddTrailingSlash (CString& sDir)
{
	if ( sDir.GetLength() && 
		sDir [sDir.GetLength() - 1] != '\\' )
		sDir += '\\';
}
