//
//   Directory.h
//

#ifndef   __MFCX_DIRECTORY_H
#define   __MFCX_DIRECTORY_H


class CDirectory
{
public:
     /// Called to set the working directory to that of the given file name.
     static void SetCurrentDir (const TCHAR* pszPath);

	/// Creates the given directory (if not already there).
	static bool CreateDir (const TCHAR* pszPath);

     /// Gets the current working directory.
     static CString GetCurrentDir();
     
     static void AddTrailingSlash (CString& sDir);
};

#endif    // __MFCX_DIRECTORY_H
