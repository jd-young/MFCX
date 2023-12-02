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


using namespace MFCX;

CFilename::CFilename (const TCHAR* pszPath)
 :   m_sPath (pszPath)
{
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
     _splitpath (pszPathname, szDrive, szDir, nullptr, nullptr);
     if ( strlen (szDir) == 0 )
          return "";  // We were passed 'C:' for the pathname. 
          
     CString sFolder = CString (szDrive) + szDir;
     int nLen = sFolder.GetLength();
     if ( nLen > 0 )
     {
          // Remove trailing slash if necessary.
          TCHAR cLast = sFolder [nLen - 1];
          if ( cLast == '\\' || cLast == '/' )
               return sFolder.Left (nLen - 1);
     }
     return sFolder;
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
		nullptr).
*/
/*static*/ bool CFilename::IsRelativePath (const TCHAR* pszPath)
{
	if ( pszPath )
		return *pszPath != '/' && *(pszPath + 1) != ':';
	return false;
}


/*!  Get the relative path of the given path from the given folder.
 *
 * \param sPath          The path to put the 
 */
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
 *   any  problems with the slashes, etc.
 *
 * \param pszDir         The directory where the file is.
 * \param pszFilename    The name of the file.
 *
 * \return   The full path of the file.
 */
/*static*/ CString CFilename::GetFullPath (const TCHAR* pszDir, 
                                           const TCHAR* pszFilename)
{
     CString sDir (pszDir ? pszDir : "");
     CString sFilename (pszFilename ? pszFilename : "");
	if ( sDir.GetLength() && sFilename.GetLength() &&
		sDir [sDir.GetLength() - 1] != '\\' )
		sDir += '\\';
	CString sFullPath = sDir + sFilename;
	return CanonPath (sFullPath);
}


void CFilename::ReplaceAll (CString& str, const TCHAR* pszOld, const TCHAR* pszNew)
{
     while (str.Replace (pszOld, pszNew))
          /*do nothing*/;
}

/*!	Canonicalises the given path.
 *
 *   That means:
 *   - Make the drive letter (if given) uppercase.
 *   - Convert '/' to '\'.
 *   - Remove repeated slashes.
 *   - Remove ./.
 *   - Remove xxx/../
 *
 * \param pszFilename         The file name to canonicalise. 
 */
/*static*/ CString CFilename::CanonPath (const TCHAR* pszFilename)
{
	CString sFilename = pszFilename;
	int nLen = sFilename.GetLength();

	if ( nLen )
	{
		// Drive letter -> uppercase
		if ( nLen >= 2 && sFilename [1] == ':' )
		{
		     TCHAR drive = sFilename [0];
		     if ( islower (drive) )
			     sFilename.SetAt (0, toupper (drive));
		}

		// '/' -> '\'
		sFilename.Replace ('/', '\\');	

		// xx\\\\xx -> xx\xx - but watch out for UNC filenames.
		if ( nLen > 2 && strncmp (sFilename, "\\\\", 2) == 0 )
		{
			sFilename = sFilename.Mid (2);
			ReplaceAll (sFilename, "\\\\", "\\");
			sFilename = "\\\\" + sFilename;
		}
		else ReplaceAll (sFilename, "\\\\", "\\");
		
		
		// xx/././xx -> xx/xx
		ReplaceAll (sFilename, "\\.\\", "\\");

		// ./xx -> xx (unless path is ".\"
		if ( nLen > 2 && strncmp (sFilename, ".\\", 2) == 0 )
			sFilename = sFilename.Mid (2);
			
		// Get rid of 'dir/../'  xx/yy/../zz -> xx/zz
		int i; 
		while ( (i = sFilename.Find ("\\..\\")) != -1 )
		{
			int end = i + 4;	// '\..\'
			const TCHAR* s = (static_cast<const TCHAR*>(sFilename)) + i;
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
 *	given path.
 *   
 *   Thus this method, when given \c c:\\Apps\\Utilities\\flib.exe will return
 *   \c flib.exe.
 *
 * \param    pszPathName    The path name.
 * \param    pszFilename    The file name part.
 * \param    nMax           The maximum length of pszFilename.
 * \return   The length of the filename.  If the pszFilename paramater is nullptr then 
 *           no copying is done.
 */
[[deprecated( "This function can easily cause NPE's if improperly used - Use GetFileName() instead" )]]	
/*static*/ UINT CFilename::ExtractFileName (const TCHAR* pszPathName, 
                                            TCHAR* pszFilename, 
                                            UINT nMax)
{
	if ( pszPathName == nullptr )
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
	if ( pszFilename != nullptr )
		strncpy (pszFilename, pszStart, nMax);
	return _tcslen (pszStart);
}

/*!  Skips to the given character in the given string. 
 *
 * \param psz       The string in which to skip to.
 * \param ch        The character to skip to.
 * \return A pointer to the given character in the string.
 */
const TCHAR* SkipTo (const TCHAR* psz, TCHAR ch)
{
	do
	{
		psz = _tcsinc (psz);
		ASSERT (*psz != '\0');
	}
	while (*psz != '\\');

	return psz;
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
\param    bAtLeastName   If \b true, then all the filename will be returned,
                         even if it is longer than nChars.
*/
/*static*/ void CFilename::AbbreviatePath (TCHAR* pszCanon, 
               	                       int nChars, 
			                            bool bAtLeastName /*=true*/)
{
	const TCHAR* pszBase = pszCanon;
	int cchFullPath = strlen (pszCanon);

	int cchFileName = ExtractFileName (pszCanon, nullptr, 0);
	const TCHAR* pszFileName = pszBase + (cchFullPath-cchFileName);

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

	const TCHAR* pszCur = pszBase + 2;                 // Skip "C:" or leading "\\" (UNC)

	if (pszBase[0] == '\\' && pszBase[1] == '\\') // UNC pathname
	{
		// UNC - skip the UNC and share name.
		pszCur = SkipTo (pszCur, '\\');
		pszCur = SkipTo (pszCur, '\\');
	}
	
	// if a UNC get the share name, if a drive get at least one directory
	ASSERT (*pszCur == '\\');
	
	int cchVolName = pszCur - pszBase;
	// '\...\' between the starting 'c:\' and the rest is 5 characters.
	if ( nChars < cchVolName + 5 + cchFileName )
	{
		lstrcpy (pszCanon, pszFileName);
		return;
	}

	// Now loop through the remaining directory components until something
	// of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
	//
	// Assert that the whole filename doesn't fit -- this should have been
	// handled earlier.

	ASSERT (cchVolName + (int) _tcslen (pszCur) > nChars);
	while ( cchVolName + 4 + (int) _tcslen (pszCur) > nChars )
	{
	     pszCur = SkipTo (pszCur, '\\');
	}

	// Form the resultant string and we're done.
	pszCanon [cchVolName] = '\0';
	_tcscat (pszCanon, _T("\\..."));
	_tcscat (pszCanon, pszCur);
}

/*static*/ bool CFilename::IsInPath (const TCHAR* pszExe)
{
     return !GetCmdPathName (pszExe).IsEmpty();
}

/*static*/ bool CFilename::FileExists (const TCHAR* pszPath)
{
	struct stat statBuf;
     return stat (pszPath, &statBuf) != -1;
}

/*!  Gets the environment variable.
 *
 * \param pszEnv    The name of the environment variable to get.
 * \return The value of the environment variable.
 */
/*static*/ CString CFilename::GetEnvVar (const TCHAR* pszEnv)
{
     return ::getenv (pszEnv);
}


static TCHAR *szDefExt[] = { ".exe", ".com", ".bat", ".cmd" };

/*!  Checks that the given path has a valid extension.
 *
 * \param pszPath        The path of the file to check.
 * \return true if the file has a command extension.
 */
bool HasValidCmdExt (const TCHAR* pszPath)
{
     CString sExt = CFilename::GetFileExt (pszPath);
     sExt.MakeLower();
     for (int i = 0; i < SIZEOF_ARRAY (szDefExt); i++)
     {
          if ( sExt == szDefExt [i] )
               return true;
     }
     return false;     // The extension is not one of the required four.
}

/*!  Checks that the given file exists in the given directory.
 *
 * \param pszDir         The directory in which the file should be.
 * \param sFile          The name of the file.
 * \return The full path name of the file if it exists, or the empty string if 
 *         not.
 */
CString CheckFileExists (const TCHAR* pszDir, const CString& sFile, bool bHasExt,
                         bool (*FileExists)(const TCHAR*))
{
     CString sPath = pszDir;
     if ( !sPath.IsEmpty() && sPath [sPath.GetLength() - 1] != '\\' )
          sPath += '\\';

     sPath += sFile;
     if ( bHasExt )
     {
     	if ( FileExists (sPath) )
     	     return sPath;
     }
     else
          for (int i = 0; i < SIZEOF_ARRAY (szDefExt); i++)
          {
               CString sFullPath = sPath + szDefExt [i];
               if ( FileExists (sFullPath) )
                    return sFullPath;
          }
     return "";
}

/*!  Searches for the given exe in all the usual places, the current working 
 *   directory, and then the search path.
 *     
 *   If the given file name doesn't have one of the default filename extensions
 *   (.EXE, .COM, .BAT, .CMD) then the are searched for (in that order).
 *
 * \param pszExe    The name of the executable file.
 * \return The full path of the executible that will run.
*/
/*static*/ CString CFilename::GetCmdPathName (const TCHAR* pszExe)
{
     return GetCmdPathName (pszExe, FileExists, GetEnvVar);
}


/*static*/ 
CString CFilename::GetCmdPathName (const TCHAR* pszExe,
                                   bool (*FileExists)(const TCHAR*),
                                   CString (*GetEnvVar)(const TCHAR*))
{     
     // Check if the given command has one of the default extensions...
     CString sCmd = pszExe;
     sCmd.TrimLeft();
     sCmd.TrimRight();

     bool bHasExt = sCmd.ReverseFind ('.') != -1;
     if ( bHasExt && !HasValidCmdExt (pszExe) )
          return "";

	// Check the local directory...
	CString sPath = CheckFileExists ("", sCmd, bHasExt, FileExists);
	if ( !sPath.IsEmpty() )
          return sPath;
	
     CString sSearchPath = GetEnvVar ("path");
     TCHAR* s = sSearchPath.GetBuffer (0);
     if ( (s = strtok (s, ";")) )
     {
          do
          {
          	sPath = CheckFileExists (s, sCmd, bHasExt, FileExists);
          	if ( !sPath.IsEmpty() )
                    return sPath;
          }
          while ( (s = strtok (nullptr, ";")) && *s );
     }
     sSearchPath.ReleaseBuffer();
     return "";
}



/*!  Attempts to get a filename from the given string.  
 *   
 *   It looks at all the  quoted strings in the string and puts them into the 
 *   array of filenames.
 *
 * \param psz            The string to parse.
 * \param arrFilenames   Array of potential filenames.
 * \return Number of files found.
 */
[[ deprecated("Too C/C++ specific - this will be removed in a future release")]]
/*static*/ 
int CFilename::ParseFileName (const TCHAR* psz, CStringArray& arrFilenames)
{
     arrFilenames.RemoveAll();

     if ( _tcschr (psz, '<') == nullptr && 
          _tcschr (psz, '"') == nullptr )
          return 0;      // No quoted string.
     
     CString sCopy = psz;
     TCHAR* s = sCopy.GetBuffer(0);
     
     s = strtok (s, "\"<");
     while ( s )
     {
          s = strtok (nullptr, "\">");
          if ( s )
               arrFilenames.Add (s);
     }
     sCopy.ReleaseBuffer();
     return arrFilenames.GetSize();
}

/*!  Attempts to file an include filename from the given name.

     First looks in the given local directory (if given), then looks in the 
     INCLUDE environment directory.

\param    pszName        The filename to search for.
\param    sPath          The returned full path name.
\param    arrIncludes  The additional paths to search.
\return   \b true if the path is found.
*/
/*static*/ bool CFilename::GetIncludeName (const TCHAR* pszName, 
                           			   CString& sPath, 
								   const CStringArray& arrIncludes)
{
     TCHAR szCurrDir [_MAX_PATH];
     ::GetCurrentDirectory (_MAX_PATH, szCurrDir);
     bool bRet = GetIncludeName (pszName, sPath, arrIncludes, FileExists, GetEnvVar, 
                                 [](const TCHAR* pszRel) -> CString
                                 {
                                   CString sFullPath;
                                   TCHAR* psz = _tfullpath (sFullPath.GetBuffer (_MAX_PATH),
                                                            pszRel, _MAX_PATH);
                                   sFullPath.ReleaseBuffer();
                                   return psz == nullptr ? "" : sFullPath;
                                 });
     ::SetCurrentDirectory (szCurrDir);
     return bRet;
}


/*!  Gets the include path name.
 *
 * \param pszInc         A filename to get a path on the INCLUDE environment 
 *                       variable paths.
 * \param sPath          The string to receive the path.
 * \param arrIncludes    An array of paths to search.
 * \param FileExists     A function that returns true if the file exists.
 * \param GetEnvVar      A function the gets an environment variable.
 * \param FullPath       A function that returns the full path of a file.
 * \return true if the include file was found.
 */
/*static*/
bool CFilename::GetIncludeName (const TCHAR* pszInc,
                                CString& sPath,
                                const CStringArray& arrIncludes,
                                bool (*FileExists)(const TCHAR*),
                                CString (*GetEnvVar)(const TCHAR*),
                                CString (*FullPath)(const TCHAR*))
{
     for (int i = 0; i < arrIncludes.GetSize(); i++)
     {
          CString sIncludePath = arrIncludes [i];
          CString sFullPath = FullPath (pszInc);
          if ( FileExists (sFullPath) )
          {
               sPath = sFullPath;
               return true;
          }
     }
     
     CString sSearchPath = GetEnvVar ("include");
     TCHAR* s = sSearchPath.GetBuffer (0);
     for (s = strtok (s, ";"); s; )
     {
          CString sPotential = s;
          if ( sPotential [sPotential.GetLength() - 1] != '\\' )
               sPotential += '\\';
          sPotential += pszInc;
          if ( FileExists (sPotential) )
          {
               sPath = sPotential;
               return true;
          }
     	s = strtok (nullptr, ";");
     }
     sSearchPath.ReleaseBuffer();
     return false;
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
          match then -1 is returned.

\attention
          This routine looks decidedly dodgy, however, it seems to work, so
          don't twiddle with it - JY.
*/
/*static*/ int CFilename::GetFilterIndex (const TCHAR* pszFilters, const TCHAR* pszExt)
{
     ASSERT ( pszFilters );
     ASSERT ( pszExt );

     if ( pszFilters == nullptr || pszExt == nullptr )
          return -1;

     if ( pszExt [0] == '.' )      // Skip any leading '.'
          pszExt++;
     
     int lenExt = strlen (pszExt);
     const TCHAR* s = pszFilters;
     for (int  nIndex = 1; s && *s; nIndex++)
          if ( s = _tcschr (s, '|') )
          {
               s++;
               for (const TCHAR* p = nullptr; s && *s; s++)
                    if ( *s == '.' )
                         p = s+1;
                    else if ( *s == ';' || *s == '|' )
                    {
                         int lenTest = s - p;
                         if ( p && lenExt == lenTest && 
                              strncmp (pszExt, p, lenExt) == 0 )
                              return nIndex;

                         p = nullptr;
                         if ( *s == '|' )
                         {
                              s++;
                              break;
                         }
                    }
          }
     return -1;
}


/*! Checks if the command is a console command.
 *
 * \param pszCmd         The command to check.
 * \return true if the command is a console command, false otherwise.
 */
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

/*!  Gets the timestamp for the given file.
 *
 * \param pszPathname    The path to the file.
 * \param time           The CTime to receive the timestamp of the file.
 * \return true if the time was successfully received, false otherwise.
 */
/*static*/ bool CFilename::GetFileTime (const TCHAR* pszPathName, CTime& time)
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
