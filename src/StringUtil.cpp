/**
 *   StringUtil.cpp           A string utilities class
 *
 *   \author   John Young
 *   \date     20 October 2020
 *     
 *   Copyright (c) 2020 Thistle Software
 */

#include "StdAfx.h"
#include "../include/StringUtil.h"

namespace MFCX {

/*!  Checks whether the given character is printable or not.

     I use this routine rather than the standard isxxxx() routines because I 
     sometimes want to print characters that are not in the range 0..127.

\param    c         The character to check.
\return   \b true if the character is printable, \b false otherwise.
*/
/*static*/ bool CStringUtil::IsPrintable (TCHAR ch)
{
	return !iscntrl (ch < 0 ? ch + 256 : ch);
}

/*!  Splits the given string into an array of tokens separated by any of the 
 *   characters in the pszDelims string.
 *
 * \param str            The string to tokenise.
 * \param pszDelims      A list of characters that can be delimiters.
 * \return a vector of tokens.
 */
vector<string> CStringUtil::StrTok (TCHAR* str, const TCHAR* pszDelims)
{
     vector<string> arr;
     
     TCHAR* token;
     TCHAR* rest = str;
     while (token = _tcstok_s (rest, pszDelims, &rest))
          arr.push_back (token);
     return arr;
}

/*!  Split a string  into a list of strings with the given delimiter.

\param arr          The resultant string array.
\param sText        The text to split.
\param pszDelimiter The list of delimiter characters used to split the list.  
                    This is the same as what is used by strtok(), which is used
                    internally.
*/
/*static*/ void CStringUtil::Split (CStringArray& arr, 
                                    const CString& sText, 
                                    const TCHAR* pszDelims)
{
     arr.RemoveAll();

     CString sBuf = sText;
     TCHAR* s = sBuf.GetBuffer (0);
     vector<string> tmp = StrTok (sBuf.GetBuffer (0), pszDelims);
     sBuf.ReleaseBuffer();

     vector<string>::iterator it = tmp.begin();
     vector<string>::iterator end = tmp.end();
     for ( ; it != end; ++it)
          arr.Add ((*it).c_str());
}

/*static*/
vector<string> CStringUtil::Split (const string& sText, string delims, 
                                   DWORD dwFlags /*= DELIM_MULTI | DELIM_COLLAPSE*/)
{
     vector<string> arr;
     int start, end = -1 * delims.size();
     do
     {
          start = end + delims.size();
          end = sText.find (delims, start);
          string token = sText.substr (start, end - start);
          if ( (dwFlags & SPLIT_REMOVE_EMPTY) && token.size() == 0 )
               continue;
          arr.push_back (token);
     }
     while (end != string::npos );
     return arr;
}
 


/*!  Joins the strings in the given list 

\param lstStrs      The string list to join.
\param pszSep       The separator to join the list with.  If omitted, then a 
                    comma is used.
\return the string list as a single string, each element separated by the given 
        separator.
*/
/*static*/
CString CStringUtil::Join (const CStringList& lstStrs, 
                           const TCHAR* pSep /*= ","*/)
{
     CString sJoined;
     for (POSITION pos = lstStrs.GetHeadPosition(); pos; )
     {
          CString sStr = lstStrs.GetNext (pos);
          if ( !sJoined.IsEmpty() ) sJoined += pSep;
          sJoined += sStr;
     }
     return sJoined;
}


void CStringUtil::CopyFrom (const CStringList& from, CStringList& to)
{
     for (POSITION pos = from.GetHeadPosition(); pos; )
     {
          CString sPath = from.GetNext (pos);
          to.AddTail (sPath);
     }
}

} // namespace MFCX
