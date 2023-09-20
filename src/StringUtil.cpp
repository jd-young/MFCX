/**
 *   \file     StringUtil.cpp      A string utilities class
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
 *
 *   Use this function rather than the standard isxxxx() routines if you want to
 *   check characters that are not in the range 0..127.
 *
 * \param ch        The character to check.
 * \return \b true if the character is printable, \b false otherwise.
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

/*!  Split a string  into a list of strings with the given delimiters.

\param arr          The resultant string array.
\param sText        The text to split.
\param pszDelims    The list of delimiter characters used to split the list.  
                    This is the same as what is used by strtok(), which is used
                    internally.
*/
/*static*/ void CStringUtil::Split (CStringArray& arr, 
                                    const CString& sText, 
                                    const TCHAR* pszDelims)
{
     arr.RemoveAll();

     CString sBuf = sText;
     vector<string> tmp = StrTok (sBuf.GetBuffer (0), pszDelims);
     sBuf.ReleaseBuffer();

     vector<string>::iterator it = tmp.begin();
     vector<string>::iterator end = tmp.end();
     for ( ; it != end; ++it)
          arr.Add ((*it).c_str());
}


/*static*/
vector<string> CStringUtil::Split (const string &s, TCHAR delim /*= ','*/)
{
     string sDelim (1, delim);     // Initialise with exactly one character - 'delim'.
     return Split (s, sDelim);
}

/*! Splits the given string into a list of tokens with the given delimiter.
 *
 * \param sText     The text to split.
 * \param sDelim    The delimiter to split the text.
 * \param dwFlags   If set to SPLIT_REMOVE_EMPTY, then all empty string tokens
 *                  are ignored and not returned in the token string.
 * \return a list of tokens.
 */
/*static*/
vector<string> CStringUtil::Split (const string& sText, string sDelim, 
                                   DWORD dwFlags /*= 0 */)
{
     vector<string> arr;
     int end = -1 * sDelim.size();
     do
     {
          int start = end + sDelim.size();
          end = sText.find (sDelim, start);
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
                           const TCHAR* pszSep /*= ","*/)
{
     CString sJoined;
     for (POSITION pos = lstStrs.GetHeadPosition(); pos; )
     {
          CString sStr = lstStrs.GetNext (pos);
          if ( !sJoined.IsEmpty() ) sJoined += pszSep;
          sJoined += sStr;
     }
     return sJoined;
}


/*!  Join the given string list into a single string.
 *
 */
/*static*/
CString CStringUtil::Join (const vector<string>& lstStrs, const TCHAR* pszSep)
{
     CString sJoined;
     vector<string>::const_iterator it = lstStrs.cbegin();
     vector<string>::const_iterator end = lstStrs.cend();
     for ( ; it != end; ++it)
     {
          if ( !sJoined.IsEmpty() ) sJoined += pszSep;
          sJoined += (*it).c_str();
     }
     return sJoined;
}



/*!  Copies from one list to another.
 *
 * Note that the destination list is not cleared, so the entries in the from 
 * list are actually appended to the destination list.
 * 
 * \param from      The source list.
 * \param to        The destination list.
 */
void CStringUtil::CopyFrom (const CStringList& from, CStringList& to)
{
     for (POSITION pos = from.GetHeadPosition(); pos; )
     {
          CString sPath = from.GetNext (pos);
          to.AddTail (sPath);
     }
}

/**
 *   Format a string from the given format string.
 *
 * @param fmt       The format string as per printf.
 * @return a formatted string.
 */
/*static*/
std::string __stdcall CStringUtil::Format (const TCHAR* fmt, ...)
{
     int nSize = strlen (fmt) * 2 + 50;
     std::string str;
     va_list ap;
     while (1)  // Maximum two passes on a POSIX system...
     {
          str.resize (nSize);
          va_start (ap, fmt);
          int n = vsnprintf (const_cast<TCHAR*>(str.data()), nSize, fmt, ap);
          va_end(ap);
          if ( n > -1 && n < nSize )     // Everything worked
          {
               str.resize (n);
               return str;
          }
          
          if ( n > -1 )  // Needed size returned
               nSize = n + 1; // For null char
          else
               nSize *= 2;    // Guess at a larger size (OS specific)
     }
     return str;
}

} // namespace MFCX
