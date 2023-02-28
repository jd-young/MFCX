//
//   Regex.h
//

#ifndef   __MFCX_REGEX_H
#define   __MFCX_REGEX_H

#include "pcre.h"

/**  A regular expression class.  

     This class encapsulates Perl regular expressions.  To use the class, do 
     something like the following...
     
\verbatim
     CRegex regex;
     CString sText ("1234567890");
     if ( regex.Match ("^(\d)\d{4}(.*)", sText) )
     {
          CString sMatch1 = regex [1];
          CString sMatch2 = regex [2];
          printf ("Match 1 is '%s', Match 2 is '%s'\n", sMatch1, sMatch2);
     }
\endverbatim
     produces the following output...

\verbatim
     Match 1 is '1', Match 2 is '67890'
\endverbatim

TODO:
     * Separate out compile and search so that we can get warning of bad 
       patterns (for example, missing closing brace).  Provide some way to query
       what was wrong with the pattern.
     * Fix the operator[] to either return 0 as the whole string, or the docco
       to reflect what it actually does.
     * Store the start / end position of each match, not just the last one.
*/
class CRegex
{
public:
     CRegex (int options = -1);    ///< Constructs a regex object
     ~CRegex();

     /// Attempts to find a match in the given text with the given pattern.
     bool Match (const TCHAR* pszPattern, const TCHAR* pszText);
     
     /// Returns the matches after calling Match.
     const CStringArray& GetMatches() const { return m_arrMatches; }

	/// Returns the match.  0 - means the whole string, 1, 2, ... means the 
	/// sub match.
     CString operator[] (int i);

     /// Uses the Perl replacement characters ($1, $2, etc) to replace text in
     /// the given expression, plus all the escaped characters (\\r, \\n, etc.)
     CString Substitute (const CString& sReplaceExp) const;
     
     /// Uses the Perl replacement characters ($1, $2, etc) to replace text in
     /// the given expression.
     CString SubstituteDollar (const CString& sReplaceExp) const;

	static CString GetReplaceText (const CString& sReplaceExp, 
							 const CStringArray& arrMatches);

private:
     int m_nOptions;
     CStringArray m_arrMatches;
     int m_nStart, m_nEnd;
};


#endif    // __MFCX_REGEX_H