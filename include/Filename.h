/*!
     \file	Filename.h
     
     JY's filename manipulation routines
 */

#ifndef	__MFCX__FILENAME_H
#define	__MFCX__FILENAME_H

/*!  A filename utility

 */
class CFilename
{
public:
     CFilename (const TCHAR* pszPath = NULL);
     ~CFilename();

     /// Gets the filename from the file.
     CString GetFileName() const;
     
     /// Gets the filename from the given file.
     static CString GetFileName (const TCHAR* pszPathName);

     /// Gets the folder name of the file.
     CString GetFolderName() const;
     
     /// Gets the folder name of the given file.
     static CString GetFolderName (const TCHAR* pszPathname);

     /// Gets the file extension.
     CString GetFileExt() const;

     /// Gets the file extension for the given path.
     static CString GetFileExt (const TCHAR* pszPathname);

	/// Gets the basename for the given path (strips the directory and the 
	/// extension).
	CString GetBaseName();

	/// Gets the basename for the given path (strips the directory and the 
	/// extension).
	static CString GetBaseName (const TCHAR* pszPathname);


     CString GetPath() const { return m_sPath; }

	bool GetFileTime (CTime& time)
		{	return GetFileTime (m_sPath, time);  }

	static bool GetFileTime (LPCTSTR pszPathName, CTime& time);

	/// Returns true if the given path is relative.
	static bool IsRelativePath (const TCHAR* pszPath);

	/// Gets the path relative to the current directory.
     static bool GetRelativePath (TCHAR* sPath);
     
     /// Gets the path relative to the given directory.
     static bool GetRelativePath (TCHAR* pszPath, const TCHAR* pszFolder);

     /// Gets the path relative to the given directory.
     static bool GetRelativePath (CString& sPath, const TCHAR* pszFolder);

	/// Abreviates the given path to the given number of characters by 
	/// inserting an ellipsis.
     static void AbbreviatePath (TCHAR* pszCanon, int nChars, bool bAtLeastName);
     static bool GetLongFileName (const TCHAR* pszShort, TCHAR* pszLong, int nChars);
     static bool IsInPath (const TCHAR* pszExe);
     static int  ParseFileName (const TCHAR* psz, CStringArray& arrFilename);
     static bool GetIncludeName (const TCHAR* pszName, 
                                 CString& sPath, 
                                 const CStringArray& sIncludePaths);
     static int GetFilterIndex (const TCHAR* pszFilters, const TCHAR* pExt);
     static CString GetCmdPathName (const TCHAR* pszExe);
     static bool IsConsoleCmd (const TCHAR* pszCmd);

     static UINT ExtractFileName (const TCHAR* pszPathName, 
						    TCHAR* pszFilename, 
                                  UINT nMax);

     /// 
	static CString GetFullPath (const TCHAR* pszDir, const TCHAR* pszFilename);
	static CString CanonPath (const TCHAR* pszFilename);
	void CanonPath()
		{	m_sPath = CanonPath (m_sPath); }


protected:
     CString m_sPath;
};

#endif    // __MFCX_FILENAME_H
