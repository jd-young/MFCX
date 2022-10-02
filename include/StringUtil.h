// StringUtil.h : A string utilities class

#ifndef   STRINGUTIL_H
#define   STRINGUTIL_H

/**
 *   A utility class for strings.
 */
class CStringUtil
{
public:
     /// Returns \b true if the given character is printable.
     static bool IsPrintable (TCHAR ch);

     /// Split the given text into a list of strings by the given delimiters.
     static void Split (CStringArray& arr, 
                        const CString& sText, 
                        const TCHAR* pszDelimiters);

     /// Join the given string list into a single string.
     static CString Join (const CStringList& lstStrs, const TCHAR* pSep = ",");
     
     /// Copy a CStringList to another.
     static void CopyFrom (const CStringList& lstFrom, CStringList& lstTo);
};

#endif   STRINGUTIL_H
