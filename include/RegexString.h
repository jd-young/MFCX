//
//   RegexString.h
//

#ifndef	__MFCX_REGEXSTRING_H
#define	__MFCX_REGEXSTRING_H

#include "XString.h"

/// A string that can be interpreted as a regex string (bounded by '/' 
/// characters).
class CRegexString : public CXString
{
public:
	// Constructors
	CRegexString();
	CRegexString (const TCHAR* str);
     CRegexString (const CString& s);
     CRegexString (const CRegexString& rs);

     void Copy(const CRegexString& from);

	// Assignment
//    const CRegexString& operator= (const char* str)
//         { CXString::operator=(str);   return *this; }
//    const CRegexString& operator= (CString s)
//         { CXString::operator=(s);     return *this; }

     friend CRegexString operator+ (TCHAR ch, const CRegexString& str);

     // Equivalence
     friend bool operator==(const CRegexString& s1, LPCTSTR psz2) throw()
     {
          return s1._string == psz2;
     }

     friend bool operator==(const CRegexString& s1, const CString& s2) throw()
     {
          return s1._string == s2;
     }

     bool IsRegex() const { return m_bIsRegex; }
     int  GetOptions() const;

     void Set (const char* psz) { Initialise(psz); }
     void SetOptions (int options);

     bool IsIgnoreCase() const { return m_bIgnoreCase; }
     bool IsMultiline() const { return m_bMultiline; }
     bool IsExtended() const { return m_bExtended; }

// Implementation
protected:
     bool m_bIsRegex;

     // The X-modifiers
     bool m_bIgnoreCase;      // The 'i' modifier 
     bool m_bMultiline;       // The 'm' or opposite of the 's' modifier
     bool m_bExtended;        // The 'x' modifier

     void Initialise(const TCHAR* s = NULL);
};

#endif	// __MFCX_REGEXSTRING_H
