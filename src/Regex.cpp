/*!
     \file     Regex.cpp      A regular expression class.

     \author   John Young
     \date     8 April 2001

     Copyright (c) 2001 Young Associates
*/


#include "stdafx.h"
#include "../include/Regex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* copied from regex/internal.h */
#define PUBLIC_OPTIONS \
  (PCRE_CASELESS | PCRE_EXTENDED | PCRE_ANCHORED | PCRE_MULTILINE | \
   PCRE_DOTALL | PCRE_DOLLAR_ENDONLY | PCRE_EXTRA | PCRE_UNGREEDY)


CRegex::CRegex (int options /*= -1*/)
{
     if ( options == -1 )
          m_nOptions = 0;
     else m_nOptions = options;
}

CRegex::~CRegex() 
{
}


/*!  Attempts to find a regex match with the given pattern in the given text.

\param    pszPattern     The regex pattern.
\param    pszText        The text in which to find a match.
\return   \b true if a match is found, \b false otherwise.
*/
bool CRegex::Match (const TCHAR* pszPattern, const TCHAR* pszText)
{
     m_nStart = 
     m_nEnd = 0;

	bool bMatched = FALSE;

	const char *error;
	int erroroffset;

     int compileOptions = m_nOptions & PUBLIC_OPTIONS;

	pcre* re = pcre_compile (pszPattern, compileOptions, &error, &erroroffset);
	if ( re )
	{
		int nLen = strlen (pszText);
          //TRACE ("Regex Searching %d chars\n", nLen);

		int offsets [30];        // 15 substrings allowed
		int size_offsets = sizeof (offsets) / sizeof (int);

		int count = pcre_exec (re, NULL, pszText, nLen, 
                                 m_nOptions, offsets, size_offsets);

		
          if (count == 0)
		{
			//TRACE ("Matched, but too many substrings\n");
			count = size_offsets / 2;
		}

		if (count >= 0)
		{
		     m_arrMatches.SetSize (count);
			count *= 2;
			for (int i = 0; i < count; i += 2)
			{
				if (offsets[i] < 0)
				{
					//TRACE ("%2d: <unset>\n", i/2);
					/* do nothing*/;
				}
				else
				{
					CString sMatch (pszText + offsets[i], 
					                offsets[i+1] - offsets[i]);
					m_arrMatches.SetAt (i/2, sMatch);
					//TRACE ("%2d: %s\n", i/2, sMatch);
				}
			}

               /*int options;
               int firstchar;
               int retval = pcre_info (re, &options, &firstchar);*/

			// Scan to position
               m_nStart = offsets [0];
               m_nEnd = offsets [1];

			bMatched = TRUE;
		}
		else
		{
			/*if ( count == -1 ) 
				TRACE ("No match\n");
			else TRACE ("Error %d\n", count);*/
			bMatched = FALSE;
		}

		free (re);
	}
     else
     {
          TRACE ("Regex search error: '%s' at offset %d\n", error, erroroffset);
     }

	return bMatched;
}



/**  Extracts the tagged expressions in the sReplaceWith text, using the matches 
     passed in arrMatches, and returns the result.  Tag expressions are:

          - $1, $2, etc.   First, second, etc bracket matches.
          - $&             The entire matched string.
          - $+             Last bracket match matched. (not yet implemented)

     Also translates escaped characters (such as '\n') to their equivalent 
     character.

\param  sReplaceExp  	The replace expression.
\return	The string with all the tagged expressions substituted.
*/
CString CRegex::Substitute (const CString& sReplaceExp) const
{
     CString sReplaceStr;

	// First compute the length of the string
	TCHAR c;
	for (const TCHAR* s = sReplaceExp; c = *s++; ) 
	{
	     int nSub = -1;      // Ordinary character.

	     if ( c == '$' )
	     {
	          // Expect a tagged expression
	          if ( *s == '&' )                   // The whole found string 
	          {
	               nSub = 0;
	               s++;
	          }
	          else if ( isdigit (*s) )           // a sub-string
	          {
                    nSub = atoi (s);
                    ASSERT ( nSub );
                    while ( isdigit (*s) ) s++;   // scan past all digits.
	          }
	     }
	     
		if ( nSub >= 0 ) 
		{
		     if ( nSub < m_arrMatches.GetSize() )
		          sReplaceStr += m_arrMatches [nSub];
		     //else ignore
		}
		else
		{	
			// Ordinary or escaped character (c is the current character, 
			// and s points at the next character).
			if ( c == '\\' )
			{
			     // The literals and their translation must be in the same
			     // order.
			     static char stringLiterals[] = "abefnrtv";
			     static char translation[] = "\a\b\x1f\f\n\r\t\v";
			     ASSERT ( sizeof stringLiterals == sizeof translation );
			     
			     char* p = NULL;
                    if ( *s == '0' || *s == 'x' )      // Octal / Hex number
                    {
                         int base = *s == '0' ? 8 : 16;
                         unsigned long l = strtoul (s+1, &p, base);
                         if ( l < 256 )
                         {
                              c = (char) l;
                              s = p;
                         }
                         else c = *s++;           // Invalid octal / hex number
                    }
                    else if ( (p = strchr (stringLiterals, *s)) != NULL )
                    {
                         c = translation [p - stringLiterals];
                         s++;
                    }
     		     else c = *s++;      // Mistakenly escaped?  Map to self.
               }
               sReplaceStr += c;
		} 
	}

	return sReplaceStr;
}


/**  Extracts the tagged expressions in the sReplaceWith text, using the matches 
     passed in arrMatches, and returns the result.  Tag expressions are:

          - $1, $2, etc.   First, second, etc bracket matches.
          - $&             The entire matched string.
          - $+             Last bracket match matched. (not yet implemented)

\param  	sReplaceExp  	The replace expression.
\return	The string with all the $ parameters substituted.
*/
CString CRegex::SubstituteDollar (const CString& sReplaceExp) const
{
     CString sReplaceStr;

	TCHAR c;
	for (const TCHAR* s = sReplaceExp; c = *s++; ) 
	{
	     int nSub = -1;      // Ordinary character.

	     if ( c == '$' )
	     {
	          // Expect a tagged expression
	          if ( *s == '&' )                   // The whole found string 
	          {
	               nSub = 0;
	               s++;
	          }
	          else if ( isdigit (*s) )           // a sub-string
	          {
                    nSub = atoi (s);
                    ASSERT ( nSub );
                    while ( isdigit (*s) ) s++;   // scan past all digits.
	          }
	     }
	     
		if ( nSub >= 0 ) 
		{
		     if ( nSub < m_arrMatches.GetSize() )
		          sReplaceStr += m_arrMatches [nSub];
		     //else ignore
		}
		else
		{	
			// Ordinary character 
               sReplaceStr += c;
		} 
	}

	return sReplaceStr;
}



CString CRegex::operator[] (int i)
{
	if ( i >= 0 && i < m_arrMatches.GetSize() )
		return m_arrMatches [i];
	else return "";
}



/**  Extracts the tagged expressions in the sReplaceWith text, using the matches 
     passed in arrMatches, and returns the result.  Tag expressions are:

          - $1, $2, etc.   First, second, etc bracket matches.
          - $&             The entire matched string.
          - $+             Last bracket match matched. (not yet implemented)

     Also translates escaped characters (such as '\n') to their equivalent 
     character.

     \param  sReplaceExp  
               The replace expression.
     \param  arrMatches
               The array of tagged expressions.  Index 0 means the entire string 
               ($&), 
*/
/*static*/ CString CRegex::GetReplaceText (const CString& sReplaceExp, 
								   const CStringArray& arrMatches)
{
     CString sReplaceStr;

	// First compute the length of the string
	TCHAR c;
	for (const TCHAR* s = sReplaceExp; c = *s++; ) 
	{
	     int nSub = -1;      // Ordinary character.

	     if ( c == '$' )
	     {
	          // Expect a tagged expression
	          if ( *s == '&' )                   // The whole found string 
	          {
	               nSub = 0;
	               s++;
	          }
	          else if ( isdigit (*s) )           // a sub-string
	          {
                    nSub = atoi (s);
                    ASSERT ( nSub );
                    while ( isdigit (*s) ) s++;   // scan past all digits.
	          }
	     }
	     
		if ( nSub >= 0 ) 
		{
		     if ( nSub < arrMatches.GetSize() )
		          sReplaceStr += arrMatches [nSub];
		     //else ignore
		}
		else
		{	
			// Ordinary or escaped character (c is the current character, 
			// and s points at the next character).
			if ( c == '\\' )
			{
			     // The literals and their translation must be in the same
			     // order.
			     static char stringLiterals[] = "abefnrtv";
			     static char translation[] = "\a\b\x1f\f\n\r\t\v";
			     ASSERT ( sizeof stringLiterals == sizeof translation );
			     
			     char* p = NULL;
                    if ( *s == '0' || *s == 'x' )      // Octal / Hex number
                    {
                         int base = *s == '0' ? 8 : 16;
                         unsigned long l = strtoul (s+1, &p, base);
                         if ( l < 256 )
                         {
                              c = (char) l;
                              s = p;
                         }
                         else c = *s++;           // Invalid octal / hex number
                    }
                    else if ( (p = strchr (stringLiterals, *s)) != NULL )
                    {
                         c = translation [p - stringLiterals];
                         s++;
                    }
     		     else c = *s++;      // Mistakenly escaped?  Map to self.
               }
               sReplaceStr += c;
		} 
	}

	return sReplaceStr;
}
