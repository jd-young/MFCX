/*!
     \file     EnvVars.cpp
     \author   John Young
     \date     25-Apr-2021
     
     Copyright (c) 2021 Thistle Software
*/

#include "stdafx.h"
#include <algorithm>
#include <set>
#include <string.h>
#include <vector>
#include "../include/EnvVars.h"
#include "../include/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using std::set;
using std::vector;

namespace MFCX {

CEnvVars::CEnvVars()
{
}

CEnvVars::CEnvVars (const map<string, string>& from)
 :   _mapEnvStrs (from)
{
}

CEnvVars::CEnvVars (const CEnvVars& from)
 :  _mapEnvStrs (from._mapEnvStrs)
{
}

CEnvVars& CEnvVars::operator= (const CEnvVars& from)
{
     _mapEnvStrs = from._mapEnvStrs;
     return *this;
}

/*!  Creates an environment variable block.
 *
 * This is the same format as produced by the call to ::GetEnvironmentStrings(),
 * that is:
 *
 *   var1=value1\0
 *   var2=value2\0
 *    :
 *   varN=valueN\0
 *
 * \return an environment block suitable for passing to CreateProcess().  Note
 *         that this is generated on the heap and it is the caller's 
 *         responsibility to delete it when no longer needed. 
 */
TCHAR* CEnvVars::GetProcEnvBlock() const
{
     TCHAR* pEnvs = ::GetEnvironmentStrings();
     TCHAR* pRet = MergeEnvBlock (pEnvs);
     ::FreeEnvironmentStrings (pEnvs);
     return pRet;
}

/*!  Creates an environment variable block.
 *
 * \see GetProcEnvBlock.
 */
TCHAR* CEnvVars::GetEnvBlock() const
{
     map<string, string> empty;
     return GetEnvBlock (empty);
}

/*!  Split apart null-terminated strings.
 *
 * \param pEnvBlock      A block of null-terminated strings, with the last
 *                       string being empty.
 * \return A vector of all the strings in the given block.
 */
vector<string> SplitStrings (TCHAR* pEnvBlock)
{
     vector<string> strs;
     for (;;)
     {
          string str (pEnvBlock);
          if ( str.empty() )
               break;
          strs.push_back (str);
          pEnvBlock += str.length() + 1;
     }
     return strs;
}

/*! Merges the tools' environment variables with the other.
 *
 * In windows, the environment keys are case-insensitive, but the values are 
 * case-sensitive.  These other keys are assumed to be the process' environment
 * variables, and so take precedence over the tools' keys.
 *
 * 'Path' appears to be a special case, where if you can do this in cmd.exe:
 *
 *   SET PATH=%PATH%;C:\SomePath
 * 
 * which will append 'C:\SomePath' to the 'Path' environment variable.  We take
 * care here to avoid adding both 'Path' and 'PATH' to the resultant environment
 * variable block.
 *
 * \param pOther         The environment variable block to merge. See 
 *                       GetProcEnvBlock() for its format.
 * \return the merged environment variable block.
 */
TCHAR* CEnvVars::MergeEnvBlock (TCHAR* pOther) const
{
     map<string, string> mapOther;

     vector<string> arrOther = SplitStrings (pOther);
     vector<string>::iterator it = arrOther.begin();
     vector<string>::iterator end = arrOther.end();
     for ( ; it != end; ++it)
     {
          vector<string> parts = CStringUtil::Split (*it, "=");
          string sEnv = parts [0];
          string sValue = parts [1];

          if ( !sEnv.empty() )
               mapOther [sEnv] = sValue;
     }

     return GetEnvBlock (mapOther);
}


string ToLower (const string& s)
{
     string l = s;
     std::transform (l.begin(), l.end(), l.begin(), ::tolower);
     return l;
}


map<string, string> LowerCaseKeys (const map<string, string>& m)
{
     map<string, string> keys;
     map<string, string>::const_iterator it = m.begin();
     map<string, string>::const_iterator end = m.end();
     for ( ; it != end; ++it)
     {
          keys [ToLower (it->first)] = (it->first);
     }
     return keys;
}


/*! Get matching (case-insensitive) keys from the given maps.
 *
 * \param lcPriorityKeys A map of lower-case keys to actual keys in the priority
 *                       map.
 * \param lcSecondKeys   A map of lower-case keys to the actual keys in the \
 *                       second map.
 * \return a map of all common keys (in lower case) to the key to be used.
 */
map<string, string> MatchingKeys (const map<string, string>& lcPriorityKeys,
                                  const map<string, string>& lcSecondKeys)
{
     map<string, string> matching;
     map<string, string>::const_iterator it = lcPriorityKeys.begin();
     map<string, string>::const_iterator end = lcPriorityKeys.end();
     for ( ; it != end; ++it)
     {
          string lc = it->first;
          string sKey = it->second;
          if ( lcSecondKeys.find (lc) != lcSecondKeys.end() )
               matching [lc] = sKey;
     }
     return matching;
}

/*!  Gets the environment block merged with the 'other' variables.
 *
 * Other:
 *   Path -> 'C:\somepath;c:\otherpath'
 * 
 * \param other     The environment variables to merge with.  Our variables take 
 *                  precedence, however, %ENV% macros in our variables are 
 *                  expanded in the values.
 * \return an environment variable block.
 */
TCHAR* CEnvVars::GetEnvBlock (const map<string, string>& other) const
{
     map<string, string> lcKeys = LowerCaseKeys (_mapEnvStrs);
     map<string, string> lcOtherKeys = LowerCaseKeys (other);

     map<string, string> matching_keys = MatchingKeys (lcOtherKeys, lcKeys);
     
     // Merge our env vars (where ours take precedence)
     map<string, string> merged (other);
     map<string, string>::const_iterator it = _mapEnvStrs.cbegin();
     map<string, string>::const_iterator end = _mapEnvStrs.cend();
	for ( ; it != end; ++it)
	{
		string sVar = it->first;
		string sValue = it->second;

          string sLCKey = ToLower (sVar);
          if ( matching_keys.find (sLCKey) != matching_keys.end() )
               sVar = matching_keys [sLCKey];

		// Expand %ENV% macros with the existing value(s)...
		string sExpanded = ExpandMacros (sValue, other, lcOtherKeys, "%");
          merged [sVar] = sExpanded;
	}

     return CreateEnvBlock (merged);
}

/*static*/ TCHAR* CEnvVars::CreateEnvBlock (map<string, string>& vars)
{
     // Now calculate the size of the env block.
     int nSize = 1; // This is for the terminating '\0'
     map<string, string>::const_iterator it = vars.cbegin();
     map<string, string>::const_iterator end = vars.cend();
	for ( ; it != end; ++it)
	{
		string sVar = it->first;
		string sValue = it->second;
          nSize += sVar.size() + sValue.size() + 2;     // +2 for '=' and '\0'
     }

     TCHAR* pEnvBlock = new TCHAR [nSize];
     TCHAR* p = pEnvBlock;
     
     it = vars.cbegin();
     end = vars.cend();
	for ( ; it != end; ++it)
	{
		string sVar = it->first;
		string sValue = it->second;
          #pragma warning(suppress : 4996)
          _tcsncpy (p, sVar.c_str(), sVar.length()); p += sVar.length();
          *p++ = '=';
          #pragma warning(suppress : 4996)
          _tcsncpy (p, sValue.c_str(), sValue.length());  p += sValue.length();
          *p++ = '\0';
     }
     *p++ = '\0';   // Terminating NULL.
     return pEnvBlock;
}


/*!  Expand the given text containing macros delimited by the given delimiter.
 *
 * For example, if macros contained 'Path' -> "c:\\MyPath", and the text was
 * "C:\\ThePath;%PATH%", then the result would expand to:
 *
 *   "C:\\ThePath;c:\\MyPath"
 *
 * Note that the macros are matched case-insensitive.
 *
 * \param sText          The text to expand.
 * \param macros         A map of macros to values expand in the given text.
 * \param lcKeys         A map of lower-case macro keys to actual keys in the 
 *                       macros map.  This is to facilitate case-insensitive 
 *                       matching.
 * \param sDelim         The start and end delimiter.
 * \return the expanded text. 
 * 
 */
/*static*/
string CEnvVars::ExpandMacros (const string& sText, 
                               const map<string, string>& macros, 
                               const map<string, string>& lcKeys,
                               const TCHAR* pszDelim)
{
     return ExpandMacros (sText, macros, lcKeys, pszDelim, pszDelim);
}

/*static*/
string CEnvVars::ExpandMacros (const string& sText, 
                               const map<string, string>& macros, 
                               const map<string, string>& lcKeys,
                               const TCHAR* pszStartDelim, 
                               const TCHAR* pszEndDelim)
{
	if ( pszStartDelim == nullptr || *pszStartDelim == '\0' )
		return sText;

	string sConverted;
	
	int nStartDelim = strlen (pszStartDelim);
	int nEndDelim = strlen (pszEndDelim);
	const TCHAR* pCurr = sText.c_str();
	const TCHAR* pMacro = NULL;
	map<string, string>::const_iterator end = macros.cend();
	for ( ; *pCurr; pCurr++)
	{
		if ( pMacro == NULL )
		{
			// Look for a start delimiter...
			if ( strncmp (pCurr, pszStartDelim, nStartDelim) == 0 )
				pMacro = pCurr + nStartDelim;
			else sConverted += *pCurr;
		}
		else
			// Look for an end delimiter...
			if ( (nEndDelim == 0 && (isalnum (*pCurr) || *pCurr == '_')) ||
				(nEndDelim != 0 && 
				 strncmp (pCurr, pszEndDelim, nEndDelim) == 0) )
			{
				// End of a macro, look it up and expand it...
				string sMacro (pMacro, pCurr - pMacro);
				string sLCMacro = ToLower (sMacro);
				map<string, string>::const_iterator it = lcKeys.find (sLCMacro);
				string sKey = it == lcKeys.end()
				               ? sMacro
				               : it->second;
				it = macros.find (sKey);
				string sValue = it == end
				     ? pszStartDelim + sMacro + pszEndDelim
				     : it->second;
				sConverted += sValue;
				pMacro = NULL;
			}
	}

	if ( pMacro )
		sConverted += pMacro;

	return sConverted;
}


} // namespace MFCX