/*!
     \file     RegexString.cpp     Regular Expression string
     \author   John Young
     \date     10 February 2001

     Copyright (c) 2001 Young Associates
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

CRegexString::CRegexString (const char* str) : 
     CXString (str)
{
     Initialise();
}

CRegexString::CRegexString (CString s) :
     CXString (s)
{
     Initialise();
}


void CRegexString::Copy (const CRegexString& from)
{
     *this = (const char*) from;
     m_bIsRegex = from.m_bIsRegex;
     m_bIgnoreCase = from.m_bIgnoreCase;
     m_bMultiline = from.m_bMultiline;
     m_bExtended = from.m_bExtended;
}


/*
const CRegexString& CRegexString::operator= (const char* str)  
{
     CXString::operator=(str); 
     Initialise();
     return *this;
}

const CRegexString& CRegexString::operator= (CString s) 
{
     CXString::operator=(s); 
     Initialise();
     return *this; 
}
*/

/*static*/ void CRegexString::Initialise()
{
     m_bIsRegex = false;
     m_bIgnoreCase = false;
     m_bMultiline = false;
     m_bExtended = false;

     int nLen = GetLength();
     const char* s = (const char*) *this;
     if ( nLen && *s == '/' )
     {
          m_bIsRegex = true;
          int iSlash = ReverseFind ('/');
          if ( iSlash > 0 )
          {
               for (s = s + iSlash + 1; *s; s++)       // Deal with any modifiers
                    switch ( tolower (*s) )
                    {
                         case 'm': m_bMultiline = true;     break;
                         case 's': m_bMultiline = false;    break;
                         case 'i': m_bIgnoreCase = true;    break;
                         case 'x': m_bExtended = true;      break;
                    }
          
               char* p = GetBuffer (nLen);
               strncpy (p, p+1, iSlash-1);
               *(p + iSlash - 1) = '\0';
               ReleaseBuffer (iSlash);
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

