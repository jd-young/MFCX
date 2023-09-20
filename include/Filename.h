/*!
     \file     Filename.h
     
     MFCX's filename manipulation routines
 */

#ifndef   __MFCX__FILENAME_H
#define   __MFCX__FILENAME_H

/*!  A filename utility

 */
class CFilename
{
public:
     /// Construct a CFilename object
     explicit CFilename (const TCHAR* pszPath = nullptr);
     ~CFilename();

     /// Gets the filename from the file.
     CString GetFileName() const;
     
     /// Gets the filename (include extension) from the given file path.
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

     /// Returns the full path of this object.
     CString GetPath() const { return m_sPath; }

     /// Gets the latest modified time of the file. 
     bool GetFileTime (CTime& time) { return GetFileTime (m_sPath, time); }

     /// Gets the latest modified time of the given filenamd.
     static bool GetFileTime (const TCHAR* pszPathName, CTime& time);

     /// Returns true or false if this object is a relative filename.
     bool IsRelativePath() const { return IsRelativePath (m_sPath); }

     /// Returns true if the given path is relative.
     static bool IsRelativePath (const TCHAR* pszPath);

     /// Gets the path relative to the current directory.
     static bool GetRelativePath (TCHAR* pszPath);
     
     /// Gets the path relative to the given directory.
     static bool GetRelativePath (TCHAR* pszPath, const TCHAR* pszFolder);

     /// Gets the path relative to the given directory.
     static bool GetRelativePath (CString& sPath, const TCHAR* pszFolder);

     /// Abreviates the given path to the given number of characters by
     /// inserting an ellipsis.
     static void AbbreviatePath (TCHAR* pszCanon, 
                                 int nChars, 
                                 bool bAtLeastName = true);
     
     /// Returns true if the given executable is in the PATH.
     static bool IsInPath (const TCHAR* pszExe);
     
     /// Parses a line for an include (TODO: This is too C/C++ centric - shouldn't be here).
     static int  ParseFileName (const TCHAR* psz, CStringArray& arrFilenames);

     /// Searches the include paths for the given filename.
     static bool GetIncludeName (const TCHAR* pszName, 
                                 CString& sPath, 
                                 const CStringArray& arrIncludes);
     static int GetFilterIndex (const TCHAR* pszFilters, const TCHAR* pszExt);
     
     /// Gets the full path of the give command in the current directory, or in
     /// the path.
     static CString GetCmdPathName (const TCHAR* pszExe);

     /// Checks if the given file is a console command.
     static bool IsConsoleCmd (const TCHAR* pszCmd);

     /// Extracts the filename (and extension) - deprecated - use GetFileName() instead. 
     static UINT ExtractFileName (const TCHAR* pszPathName, 
                                  TCHAR* pszFilename,
                                  UINT nMax);

     /// Joins the given directory and filename and returns a canonicalised path.
     static CString GetFullPath (const TCHAR* pszDir, const TCHAR* pszFilename);

     /// Canonicalises the given path.
     static CString CanonPath (const TCHAR* pszFilename);

     /// Canonicalises this pathname.
     void CanonPath() { m_sPath = CanonPath (m_sPath); }

#ifndef   GTEST     // To let unit tests access protected members.
protected:
#endif
     CString m_sPath;    ///< The file name.
     
     static void ReplaceAll (CString& str, const TCHAR* pszOld, const TCHAR* pszNew);
     
     /// Returns true if the given path exists.
     static bool FileExists (const TCHAR* pszPath);
     
     /// Returns the value of the given environment variable.
     static CString GetEnvVar (const TCHAR* pszEnv);

     /// Returns the path of the given command.
     static CString GetCmdPathName (const TCHAR* pszExe,
                                    bool (*FileExists)(const TCHAR*),
                                    CString (*GetEnvVar)(const TCHAR*));

     /// Gets the path of the given include file.
     static bool GetIncludeName (const TCHAR* pszInc,
                                 CString& sPath,
                                 const CStringArray& arrIncludes,
                                 bool (*FileExists)(const TCHAR*),
                                 CString (*GetEnvVar)(const TCHAR*),
                                 CString (*FullPath)(const TCHAR*));
};

#endif    // __MFCX_FILENAME_H
