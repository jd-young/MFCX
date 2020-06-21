/*!
     \file     Directory.cpp
     \author   John Young
     \date     16 May 2001
     
     Copyright (c) 2001 Young Associates
*/


#include "stdafx.h"
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../include/Directory.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/*!  Called to set the working directory to that of the given file name.

\param    pszPath   The path of a file, or the sub-directory to change to.
*/
void CDirectory::SetCurrentDir (const TCHAR* pszPath)
{
	char szDrive [_MAX_DRIVE];
	char szDir [_MAX_DIR];

	_splitpath (pszPath, szDrive, szDir, NULL, NULL);
	if ( szDrive [0] == '\0' && szDir [0] == '\0' )
	{
          // relative path?
          ::SetCurrentDirectory (pszPath);
	}
	else
	{
		CString sCwd (szDrive);
		sCwd += szDir;
		chdir (sCwd);
	}
}



/*static*/ bool CDirectory::CreateDir (const TCHAR* pszPath)
{
	struct _stat stat;
	if ( _stat (pszPath, &stat) != 0 )
	{
		// Doesn't exist... create it.
		return mkdir (pszPath) == 0;
	}
	return true;	// Already exists
}


/*static*/ CString CDirectory::GetCurrentDir()
{
	char szDir [_MAX_PATH];
     ::GetCurrentDirectory (sizeof szDir - 1, szDir);
     return szDir;
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
