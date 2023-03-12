// StringUtil.h : A string utilities class

#ifndef   STRINGUTIL_H
#define   STRINGUTIL_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace MFCX
{

/**
 *   A utility class for strings.
 */
class CStringUtil
{
public:
     /// Used by Split() to remove empty tokens.
     static const int SPLIT_REMOVE_EMPTY = 1;

     /// Returns \b true if the given character is printable.
     static bool IsPrintable (TCHAR ch);

     /// Split the given text into a list of strings by the given delimiters.
     [[deprecated("Use CStringUtil::StrTok() instead")]]
     static void Split (CStringArray& arr, 
                        const CString& sText, 
                        const TCHAR* pszDelims);

     /// Split the given text into a list of string by the given delimiters.
     static vector<string> StrTok (TCHAR* str, const TCHAR* pszDelims);
     
     /// Split the given text into a list of strings by the given delimiter.
     static vector<string> Split (const string& sText, string delimiter, 
                                  DWORD dwFlags = 0);

     /// Join the given string list into a single string.
     static CString Join (const CStringList& lstStrs, const TCHAR* pSep = ",");
     
     /// Copy a CStringList to another.
     static void CopyFrom (const CStringList& lstFrom, CStringList& lstTo);
};

}    // namespace MFCX

#endif   STRINGUTIL_H
