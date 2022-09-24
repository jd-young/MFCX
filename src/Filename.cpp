/*!
     \file     Filename.cpp	General purpose file name manipulation routines.

     \author   John Young 
     \date     19-Mar-97
     
     Copyright (c) 1997 Thistle Software.
*/

#include "stdafx.h"
#include "../include/Filename.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CFilename::CFilename (const TCHAR* pszPath)
{
     m_sPath = pszPath;
}

CFilename::~CFilename()
{
}




/*!	Extracts the filename (plus extension if present) from the given path.
     
     Thus this method, when given \c c:\\Apps\\Utilities\\flib.exe will return
     \c flib.exe.

\return   The filename.  
*/
CString CFilename::GetFileName() const
{
     return GetFileName (m_sPath);
}

/*!	Extracts the filename (plus extension if present) from the given path.
     
     Thus this method, when given \c c:\\Apps\\Utilities\\flib.exe will return
     \c flib.exe.

\param	pszPathName	The full pathname of the file.
\return   The filename.  
*/
/*static*/ CString CFilename::GetFileName (const TCHAR* pszPathName)
{
	const TCHAR* pszStart = pszPathName;
	for (LPCTSTR psz = pszPathName; *psz != '\0'; psz = _tcsinc(psz))
	{
		// remember last directory/drive separator
		if (*psz == '\\' || *psz == '/' || *psz == ':')
			pszStart = _tcsinc(psz);
	}

     return pszStart;
}


CString CFilename::GetFolderName() const
{
     return GetFolderName (m_sPath);
}

/*static*/ CString CFilename::GetFolderName (const TCHAR* pszPathname)
{
     TCHAR szDrive [_MAX_DRIVE];
     TCHAR szDir [_MAX_DIR];
     _splitpath (pszPathname, szDrive, szDir, NULL, NULL);
     return CString (szDrive) + szDir;
}




CString CFilename::GetFileExt() const
{
     return GetFileExt (m_sPath);
}


/**	Gets the file extension from the given file.  The extension is deemed to
	be that bit of the file after the last dot.  If there is no dot, then the 
	empty string is returned.

\param	pszPathname 		The path from which to extract the extension.
\return	The extension (plus the leading dot), or the empty string if there is 
		no extension.
*/
/*static*/ CString CFilename::GetFileExt (const TCHAR* pszPathname)
{
     CString sFilename = GetFileName (pszPathname);

     CString sExt;
     int nIndex = sFilename.ReverseFind ('.');
     if ( nIndex != -1 )
	     sExt = sFilename.Right (sFilename.GetLength() - nIndex);
	return sExt;
}


/// Gets the basename for the given path (strips the directory and the 
/// extension).
CString CFilename::GetBaseName()
{
	return GetBaseName (m_sPath);
}

/// Gets the basename for the given path (strips the directory and the 
/// extension).
/*static*/ CString CFilename::GetBaseName (const TCHAR* pszPathname)
{
	CString sFile = GetFileName (pszPathname);
	
     int nIndex = sFile.ReverseFind ('.');
     if ( nIndex != -1 )
	     sFile = sFile.Left (nIndex);
	return sFile;
}





///	Takes the given path and returns its path relative to the current cwd.
///  This only works if the given path is a sub-directory of the current cwd.
/*static*/ bool CFilename::GetRelativePath (TCHAR* pszPath)
{
	TCHAR szCurrentDir [_MAX_PATH];
	::GetCurrentDirectory (_MAX_PATH, szCurrentDir);
     return GetRelativePath (pszPath, szCurrentDir);
}


/*!	Determines whether the given path is a relative path or an absolute path.

\param	pszPath		The path to examine.
\return	\b true if the path is relative, \b false if not (or if pszPath is 
		NULL).
*/
/*static*/ bool CFilename::IsRelativePath (const TCHAR* pszPath)
{
	if ( pszPath )
		return *pszPath != '/' && *(pszPath + 1) != ':';
	return false;
}

/*static*/ bool CFilename::GetRelativePath (CString& sPath, const TCHAR* pszFolder)
{
	bool bRet = GetRelativePath (sPath.GetBuffer (_MAX_PATH), pszFolder);
	sPath.ReleaseBuffer();
	return bRet;
}


/*static*/ bool CFilename::GetRelativePath (TCHAR* pszPath, const TCHAR* pszFolder)
{
	const TCHAR* p = pszPath;

	for ( ; *pszFolder && *p; pszFolder++, p++)		// p points at the first character
		if ( toupper (*pszFolder) != toupper (*p) )	// that is different from the cwd
			return false;

	if ( *p == '\\' )
		p++;

	if ( p != pszPath )
	{
     	strcpy (pszPath, p);
          return true;
     }
     return false;
}


/**  Returns the pathname for the given directory and filename.  It sorts out 
     ny  problems with the slashes, etc.

@param    pszDir         The directory where the file is.
@param    pszFilename    The name of the file.

@return   The full path of the file.

*/
/*static*/ CString CFilename::GetFullPath (const TCHAR* pszDir, 
                                           const TCHAR* pszFilename)
{
     CString sDir (pszDir);
	if ( sDir.GetLength() && 
		sDir [sDir.GetLength() - 1] != '\\' )
		sDir += '\\';
	CString sFilename = sDir + pszFilename;
	return CanonPath (sFilename);
}


/*!	

*/
/*static*/ CString CFilename::CanonPath (const TCHAR* pszFilename)
{
	CString sFilename = pszFilename;
	int nLen = sFilename.GetLength();

	if ( nLen )
	{
		// Drive letter -> uppercase
		TCHAR drive = sFilename [0];
		if ( islower (drive) )
			sFilename.SetAt (0, toupper (drive));
		
		// '/' -> '\'
		sFilename.Replace ('/', '\\');	

		// xx\\\\xx -> xx\xx
		if ( nLen > 2 && strncmp (sFilename, "\\\\", 2) == 0 )
		{
			sFilename = sFilename.Mid (2);
			sFilename.Replace ("\\\\", "\\");
			sFilename = "\\\\" + sFilename;
		}
		else	sFilename.Replace ("\\\\", "\\");
		
		
		// xx/././xx -> xx/xx
		sFilename.Replace ("\\.\\", "\\");

		// ./xx -> xx (unless path is ".\"
		if ( nLen > 2 && strncmp (sFilename, ".\\", 2) == 0 )
			sFilename = sFilename.Mid (2);
			
		// Get rid of 'dir/../'  xx/yy/../zz -> xx/zz
		int i; 
		while ( (i = sFilename.Find ("\\..\\")) != -1 )
		{
			int end = i + 4;	// '\..\'
			const TCHAR* s = ((const TCHAR*) sFilename) + i;
			while ( i > 0 )
				if ( *--s != '\\' )
					i--;
				else break;
			sFilename = sFilename.Left (i) + sFilename.Mid (end);
		}
	}
	return sFilename;
}








/*!	Extracts the filename (plus extension if present) from the 
	given path.
     
     Thus this method, when given \c c:\\Apps\\Utilities\\flib.exe will return
     \c flib.exe.

\param    pszPathName    The path name.
\param    pszFilename    The file name part.
\param    nMax           The maximum length of pszFilename.
\return   The length of the filename.  If the pszFilename paramater is NULL then 
          no copying is done.
*/
/*static*/ UINT CFilename::ExtractFileName (const TCHAR* pszPathName, 
                                            TCHAR* pszFilename, 
                                            UINT nMax)
{
	if ( pszPathName == NULL )
	{
		*pszFilename = '\0';
		return 0;
	}

	const TCHAR* pszStart = pszPathName;
	for (LPCTSTR psz = pszPathName; *psz != '\0'; psz = _tcsinc(psz))
	{
		// remember last directory/drive separator
		if (*psz == '\\' || *psz == '/' || *psz == ':')
			pszStart = _tcsinc(psz);
	}

	// otherwise copy it into the buffer provided
	if ( pszFilename != NULL )
		strncpy (pszFilename, pszStart, nMax);
	return lstrlen(pszStart);
}



/*!  Abbreviates the given file name to the given number of characters.

     pszCanon = C:\\MYAPP\\DEBUGS\\C\\TESWIN.C

\verbatim
     nChars   b   Result
     ------   -   ---------
      1- 7    F   \<empty\>
      1- 7    T   TESWIN.C
      8-14    x   TESWIN.C
     15-16    x   C:\\...\\TESWIN.C
     17-23    x   C:\\...\\C\\TESWIN.C
     24-25    x   C:\\...\\DEBUGS\\C\\TESWIN.C
     26+      x   C:\\MYAPP\\DEBUGS\\C\\TESWIN.C
\endverbatim

\param    pszCanon       The path name to abbreviate.
\param    nChars         The number of characters to abbreviate to.
\param    bAtLeastName   If \b true, then all the filename will ber returned,
                         even if it is longer than nChars.
*/
/*static*/ void CFilename::AbbreviatePath (TCHAR* pszCanon, 
               	                       int nChars, 
			                            bool bAtLeastName /*=TRUE*/)
{
	int cchFullPath, cchFileName, cchVolName;
	const TCHAR* pszCur;
	const TCHAR* pszBase;
	const TCHAR* pszFileName;

	pszBase = pszCanon;
	cchFullPath = strlen (pszCanon);

	cchFileName = ExtractFileName (pszCanon, NULL, 0);
	pszFileName = pszBase + (cchFullPath-cchFileName);

	// If nChars is more than enough to hold the full path name, we're done.
	// This is probably a pretty common case, so we'll put it first.
	if ( nChars >= cchFullPath )
		return;

	// If nChars isn't enough to hold at least the basename, we're done
	if ( nChars < cchFileName )
	{
		strcpy (pszCanon, (bAtLeastName) ? pszFileName : "");
		return;
	}

	// Calculate the length of the volume name.  Normally, this is two characters
	// (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
	// "\\server\share").
	//
	// If nChars isn't enough to hold at least <volume_name>\...\<base_name>, the
	// result is the base filename.

	pszCur = pszBase + 2;                 // Skip "C:" or leading "\\"

	if (pszBase[0] == '\\' && pszBase[1] == '\\') // UNC pathname
	{
		// First skip to the '\' between the server name and the share name,
		while (*pszCur != '\\')
		{
			pszCur = _tcsinc(pszCur);
			ASSERT(*pszCur != '\0');
		}
	}
	// if a UNC get the share name, if a drive get at least one directory
	ASSERT(*pszCur == '\\');
	// make sure there is another directory, not just c:\filename.ext
	if ( cchFullPath - cchFileName > 3 )
	{
		pszCur = _tcsinc(pszCur);
		while (*pszCur != '\\')
		{
			pszCur = _tcsinc(pszCur);
			ASSERT(*pszCur != '\0');
		}
	}
	ASSERT(*pszCur == '\\');

	cchVolName = pszCur - pszBase;
	if ( nChars < cchVolName + 5 + cchFileName )
	{
		lstrcpy(pszCanon, pszFileName);
		return;
	}

	// Now loop through the remaining directory components until something
	// of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
	//
	// Assert that the whole filename doesn't fit -- this should have been
	// handled earlier.

	ASSERT (cchVolName + (int)lstrlen(pszCur) > nChars);
	while ( cchVolName + 4 + (int)lstrlen(pszCur) > nChars )
	{
		do
		{
			pszCur = _tcsinc(pszCur);
			ASSERT(*pszCur != '\0');
		}
		while (*pszCur != '\\');
	}

	// Form the resultant string and we're done.
	pszCanon[cchVolName] = '\0';
	lstrcat(pszCanon, _T("\\..."));
	lstrcat(pszCanon, pszCur);
}


/*!  Converts the given short filename to the associated long name (if any).

     Returns FALSE if the long name couldn't be formed from the short.
     Apparently Win NT 5.0 will have an API of the same name that will
     make this routine obsolete.  I'm no' hauding ma brieth!
     
     It recurses through all the sub-directories and gets each long name
     and stores it in 'pszLong'

\param    pszShort       The short file name.
\param    pszLong        The returned long file name.
\param    nChars         The maximum number of characters that are placed in the
                         pszLong buffer.
\return   /b true if successful.
*/
/*static*/ bool CFilename::GetLongFileName (const TCHAR* pszShort, 
                                            TCHAR* pszLong, int nChars)
{
	if ( nChars <= 0 )
		return FALSE;

	TCHAR szWork [_MAX_PATH];
	TCHAR* p = szWork;

	strncpy (szWork, pszShort, _MAX_PATH);

	// Skip past the drive letter if it is there...
	if ( *(p+1) == ':' )
	{
		strncpy (pszLong, szWork, 2);
		pszLong [2] = '\0';
		p += 2;
	}
	else 
	{
		pszLong [0] = '\0';
		// Skip past one '.\' or many '..\'s
		if ( strncmp (p, ".\\", 2) == 0 )
		{
			strcat (pszLong, ".\\");
			p += 2;
		}
		else
			while ( strncmp (p, "..\\", 3) == 0 )
			{
				strcat (pszLong, "..\\");
				p += 3;
			}
	}

	bool bPrependBackslash = FALSE;
	if ( *p == '\\' )
	{
		p++;
		bPrependBackslash = TRUE;
	}


	for (bool bFinished = (*p == '\0'); !bFinished; p++)
	{
		bFinished = (*p == '\0');
		if ( *p == '\\' || *p == '/' || *p == '\0' )
		{
			*p = '\0';

			WIN32_FIND_DATA findData;
			HANDLE handle = FindFirstFile (szWork, &findData);
			if ( handle == INVALID_HANDLE_VALUE )
				return FALSE;

			FindClose (handle);
			if ( bPrependBackslash )
				strcat (pszLong, "\\");

			strcat (pszLong, findData.cFileName);
			bPrependBackslash = TRUE;
			*p = '\\';		// Replace the sub-directory divider
							// (this is only relevant if !bFinished)
		}
	}
	return TRUE;
}

/*static*/ bool CFilename::IsInPath (const TCHAR* pszExe)
{
     return !GetCmdPathName (pszExe).IsEmpty();
}

/*!  Searches for the given exe in all the usual places, the current working 
     directory, and then the search path.  If the given file name doesn't have 
     one of the default filename extensions (.EXE, .COM, .BAT, .CMD) then the 
     are searched for (in that order).

\param    pszExe    The name of the executable file.
\return   The full path of the executible that will run.
*/
/*static*/ CString CFilename::GetCmdPathName (const TCHAR* pszExe)
{
     static TCHAR *szDefExt[] = { ".exe", ".com", ".bat", ".cmd" };
     
     // Check if the given command has one of the default extensions...
     CString sCmd = pszExe;
     sCmd.TrimLeft();
     sCmd.TrimRight();

     int nExt = sCmd.ReverseFind ('.');

     bool bHasExt = false;
     if ( nExt == -1 )
          bHasExt = false;
     else
          for (int i = 0; i < SIZEOF_ARRAY (szDefExt); i++)
          {
               CString sExt = sCmd.Right (sCmd.GetLength() - nExt);
               sExt.MakeLower();
               if ( sExt == szDefExt [i] )
               {
                    bHasExt = true;
                    break;
               }
          }
     
	struct stat statBuf;
	
	// Check the local directory...
	if ( bHasExt )
	{
     	if ( stat (sCmd, &statBuf) != -1 )
     	     return sCmd;
     }
     else
          for (int i = 0; i < SIZEOF_ARRAY (szDefExt); i++)
          {
               CString sPath = sCmd + szDefExt [i];
          	if ( stat (sPath, &statBuf) != -1 )
          	     return sPath;
          }
     
     CString sSearchPath = getenv ("path");
     TCHAR* s = sSearchPath.GetBuffer (0);
     if ( (s = strtok (s, ";")) )
     {
          do
          {
               CString sPath = s;
               if ( sPath [sPath.GetLength() - 1] != '\\' )
                    sPath += '\\';

               sPath += sCmd;
          	if ( bHasExt )
          	{
               	if ( stat (sPath, &statBuf) != -1 )
               	     return sPath;
               }
               else
                    for (int i = 0; i < SIZEOF_ARRAY (szDefExt); i++)
                    {
                         CString sFullPath = sPath + szDefExt [i];
                    	if ( stat (sFullPath, &statBuf) != -1 )
                    	     return sFullPath;
                    }
          }
          while ( (s = strtok (NULL, ";")) && *s );
     }
     sSearchPath.ReleaseBuffer();
     return "";
}



/*!  Attempts to get a filename from the given string.  It looks at all the
     quoted strings in the string and puts them into the array of filenames.

\param    psz            The string to parse.
\param    arrFilenames   Array of potential filenames.
\return   Number of files found.
*/
/*static*/ int CFilename::ParseFileName (const TCHAR* psz, CStringArray& arrFilenames)
{
     CString sCopy = psz;
     TCHAR* s = sCopy.GetBuffer(0);
     
     arrFilenames.RemoveAll();
     s = strtok (s, "\"<");
     while ( s )
     {
          arrFilenames.Add (s);
          s = strtok (NULL, "\">");
     }
     sCopy.ReleaseBuffer();
     return arrFilenames.GetSize();
}


/*!  Attempts to file an include filename from the given name.

     First looks in the given local directory (if given), then looks in the 
     INCLUDE environment directory.

\param    pszName        The filename to search for.
\param    sPath          The returned full path name.
\param    sIncludePaths  The additional paths to search.
\return   \b true if the path is found.
*/
/*static*/ bool CFilename::GetIncludeName (const TCHAR* pszName, 
                           			   CString& sPath, 
								   const CStringArray& sIncludePaths)
{
	TCHAR szCurrentDir [_MAX_PATH];
	::GetCurrentDirectory (_MAX_PATH, szCurrentDir);
     bool bFound = false;

     struct stat statBuf;
     for (int i = 0; i < sIncludePaths.GetSize(); i++)
     {
          CString sIncludePath = sIncludePaths [i];
          ::SetCurrentDirectory (sIncludePath); 
          
          TCHAR szFullPath [_MAX_PATH];
          _tfullpath (szFullPath, pszName, _MAX_PATH);
     	if ( stat (szFullPath, &statBuf) != -1 )
     	{
     	     sPath = szFullPath;
     	     bFound = true;
     	     break;
          }
     }
     
     ::SetCurrentDirectory (szCurrentDir);        // Leave things as they were.
     if ( bFound )
          return true;

     CString sSearchPath = getenv ("include");
     TCHAR* s = sSearchPath.GetBuffer (0);
     for (s = strtok (s, ";"); s; )
     {
          CString sPotential = s;
          if ( sPotential [sPotential.GetLength() - 1] != '\\' )
               sPotential += '\\';

          sPotential += pszName;
     	if ( stat (sPotential, &statBuf) != -1 )
     	{
     	     sPath = sPotential;
     	     bFound = true;
     	     break;
     	}
     	else s = strtok (NULL, ";");
     }
     sSearchPath.ReleaseBuffer();
     return bFound;
}

/*!  Finds a match with the given MFC style open file filter.

     The file filter string looks like...

\verbatim
          "Text files (*.txt)|*.txt|"
     	"HTML files (*.html;*.htm)|*.html;*.htm|"
     	"All Files (*.*)|*.*||";
\endverbatim

\param    pszFilters     The filter string to search.
\param    pszExt         The extension to search for.  You can supply both 
                         ".txt" (leading dot) and "txt" (no dot), and the 
                         function will do the right thing.
\return   Returns the 1 based index of the given file filter. If there is no 
          match then 0 is returned.

\attention
          This routine looks decidedly dodgy, however, it seems to work, so
          don't twiddle with it - JY.
*/
/*static*/ int CFilename::GetFilterIndex (const TCHAR* pszFilters, const TCHAR* pszExt)
{
     ASSERT ( pszFilters );
     ASSERT ( pszExt );

	if ( pszFilters == NULL || pszExt == NULL )
		return -1;

     if ( pszExt [0] == '.' )      // Skip any leading '.'
          pszExt++;
     
     int lenExt = strlen (pszExt);
	const TCHAR* s = pszFilters;
	for (int	nIndex = 1; s && *s; nIndex++)
		if ( s = strchr (s, '|') )
		{
			s++;
			for (const TCHAR* p = NULL; s && *s; s++)
				if ( *s == '.' )
					p = s+1;
				else if ( *s == ';' || *s == '|' )
				{
                         int lenTest = s - p;
					if ( p && lenExt == lenTest && 
                              strncmp (pszExt, p, lenExt) == 0 )
						return nIndex;

					p = NULL;
					if ( *s == '|' )
					{
						s++;
						break;
					}
				}
		}
	return -1;
}



/*static*/ bool CFilename::IsConsoleCmd (const TCHAR* pszCmd)
{
     CString sPath = GetCmdPathName (pszCmd);
     if ( sPath.IsEmpty() )
          return false;
     
     SHFILEINFO fileInfo;
	fileInfo.hIcon = 0;
	fileInfo.iIcon = 0;
     fileInfo.dwAttributes = 0; 
	DWORD dwType = ::SHGetFileInfo (sPath, 0, &fileInfo, 
	                                sizeof fileInfo, SHGFI_EXETYPE);

     // The Win32 docco has a really funny way of specifying the return value of
     // SHGetFileInfo:
     //
     //   0                             Non-executable file
     //   LOWORD = NE or PE and         Windows application
     //   HIWORD = 3.0, 3.5, or 4.0
     //   LOWORD = MZ, HIWORD = 0       MS-DOS file
     //   LOWORD = PE and HIWORD = 0    Win32 Console app
     //
     const DWORD NE = 0x454e;
     const DWORD PE = 0x4550;
     const DWORD MZ = 0x5a4d;

     if ( dwType == 0 )
          return false;
     
     DWORD hw = HIWORD (dwType);
     DWORD lw = LOWORD (dwType);
     
     if ( lw == MZ && hw == 0 )         // DOS exe, bat, or com.
          return true;
          
     if ( lw == PE && hw == 0 )         // Win32 console
          return true;
          
     if ( lw == PE || lw == NE )
          return false;

     return false; 
}

/*static*/ bool CFilename::GetFileTime (LPCTSTR pszPathName, CTime& time)
{
	// Store the modification date/time for the file, so that
	// we can check for other applications modifying the file.

	struct stat statBuf;
	if ( stat (pszPathName, &statBuf) == -1 )
	{
		time = (time_t) 0;
	     return false;
	}
	time = statBuf.st_mtime;
	return true;
}




