// StringUtil.h : A string utilities class

#ifndef   __MFCX_STRINGUTIL_H
#define   __MFCX_STRINGUTIL_H

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
     
     /// Split the given text into a list of strings by the given delimiter 
     /// character.
     static vector<string> Split (const string &s, TCHAR delim = ',');
     
     /// Split the given text into a list of strings by the given delimiter 
     /// string.
     static vector<string> Split (const string& sText, string sDelim, 
                                  DWORD dwFlags = 0);

     /// Join the given string list into a single string with the given 
     /// character delimiter.
     static CString Join (const CStringList& lstStrs, TCHAR cSep = ',');

     /// Join the given string list into a single string.
     static CString Join (const CStringList& lstStrs, const TCHAR* pszSep);

     /// Join the given string list into a single string.
     static CString Join (const vector<string>& lstStrs, const TCHAR* pszSep);

     /// Copy a CStringList to another.
     static void CopyFrom (const CStringList& from, CStringList& to);
     
     /// Return a string with the given format (see printf for details).
     static std::string __stdcall Format (const TCHAR* fmt, ...);
};

}    // namespace MFCX

#endif   __MFCX_STRINGUTIL_H
