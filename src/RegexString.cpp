/*!
     \file     RegexString.cpp     Regular Expression string
     \author   John Young
     \date     10 February 2001

     Copyright (c) 2001 Thistle Software
*/

#include "stdafx.h"
#include <ctype.h>
#include "../include/RegexString.h"
#include "../include/pcre.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRegexString::CRegexString()
{
     Initialise();
}

CRegexString::CRegexString (const TCHAR* str)
{
     Initialise(str);
}

CRegexString::CRegexString (const CString& s)
{
     Initialise((const TCHAR*) s);
}

CRegexString::CRegexString (const CRegexString& rs)
{
     _string = rs._string;
     m_bIsRegex = rs.m_bIsRegex;

     m_bIgnoreCase = rs.m_bIgnoreCase;
     m_bMultiline = rs.m_bMultiline;
     m_bExtended = rs.m_bExtended;

}



void CRegexString::Copy(const CRegexString& from)
{
     _string = from._string;
     m_bIsRegex = from.m_bIsRegex;
     m_bIgnoreCase = from.m_bIgnoreCase;
     m_bMultiline = from.m_bMultiline;
     m_bExtended = from.m_bExtended;
}

/*static*/ void CRegexString::Initialise(const TCHAR* psz /*= NULL*/)
{
     _string = psz ? psz : "";
     m_bIsRegex = false;
     m_bIgnoreCase = false;
     m_bMultiline = false;
     m_bExtended = false;

     int nLen = _string.GetLength();
     const TCHAR* s = (const TCHAR*) _string;
     if ( nLen && *s == '/' )
     {
          m_bIsRegex = true;
          int iSlash = _string.ReverseFind ('/');
          if ( iSlash > 0 )
          {
               for (s = s + iSlash + 1; *s; s++)       // Deal with any modifiers
                    switch ( tolower (*s) )
                    {
                         // TODO: Multi and single are not mutually exclusive.
                         case 'm': m_bMultiline = true;     break;
                         case 's': m_bMultiline = false;    break;
                         case 'i': m_bIgnoreCase = true;    break;
                         case 'x': m_bExtended = true;      break;
                    }
          
               char* p = _string.GetBuffer (nLen);
               strncpy (p, p+1, iSlash-1);
               *(p + iSlash - 1) = '\0';
               _string.ReleaseBuffer (iSlash);
          }
     }
}

int  CRegexString::GetOptions() const
{
     int nOptions = 0;
	if ( m_bIgnoreCase ) nOptions |= PCRE_CASELESS;
	if ( m_bExtended )   nOptions |= PCRE_EXTENDED;
	if ( m_bMultiline )  nOptions |= PCRE_MULTILINE;

     return nOptions;
}


void CRegexString::SetOptions (int options)
{
     m_bIgnoreCase = (options & PCRE_CASELESS) == PCRE_CASELESS;
     m_bExtended   = (options & PCRE_EXTENDED) == PCRE_EXTENDED;
     m_bMultiline  = (options & PCRE_MULTILINE) == PCRE_MULTILINE;
}

CRegexString operator+ (TCHAR ch, const CRegexString& str)
{
     CRegexString strResult (str);
     strResult._string = ch + strResult._string;
     return strResult;
}

