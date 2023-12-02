/*!
     \file Directory.h
 */

#ifndef   __MFCX_DIRECTORY_H
#define   __MFCX_DIRECTORY_H

namespace MFCX {

/*!  A directory class helper.
 */
class CDirectory
{
public:
     /// Gets the current working directory.
     static CString GetCurrentDir();

     /// Called to set the working directory to that of the given file name.
     static bool SetCurrentDir (const TCHAR* pszPath);

	/// Creates the given directory (if not already there).
	static bool CreateDir (const TCHAR* pszPath);

     /// Deletes the given directory (if present) recursively.
     static bool DeleteDir (const TCHAR* pszPath, bool bRecycleBin = false);
     
     /// Add a trailing slash if necessary.
     static void AddTrailingSlash (CString& sDir);
};

}    // namespace MFCX

#endif    // __MFCX_DIRECTORY_H
