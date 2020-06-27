/*!
	\file	XString.cpp		Extended CString class.
	\author	John Young
	Copyright (c) 1996 Thistle Softare
*/	

#include "StdAfx.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include  "../include/XString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**	The CDateParser class parses a string for a valid date.

	To use it...
\verbatim
	CDateParser dp ("26 Feb 1961");
	CTime time = dp.Parse();
\endverbatim
*/
class CDateParser
{
public:
	enum { TM_NONE, TM_HOUR, TM_MIN, TM_SEC, TM_AMPM, 
		  TM_DAY, TM_MONTHNR, TM_MONTHNAME, TM_YEAR };

	CDateParser (const TCHAR* psz);
	//~CDateParser();

	CTime Parse();

private:
	int	m_nHour;
	int	m_nMins;
	int  m_nSecs;
	int	m_nDay;
	int	m_nMonth;
	int	m_nYear;

	const TCHAR* m_psz;

	int	m_nExpecting;
	int	m_bError;

	int  GetNext (TCHAR* pszItem);
	bool IsValid();

	void SetDay (int day);
	void SetMonth (int month);
	void SetYear (int year);

	void SetHour (int hour);
	void SetMin (int min);
	void SetSec (int sec);

	int GetMonth (const TCHAR* s);
	int GetWeekDay (const TCHAR *s);
	void ProcessAmPm (const TCHAR* s);

	const TCHAR* ExpectingStr();		// Debug only
};


const TCHAR* CDateParser::ExpectingStr()
{
	switch (m_nExpecting)
	{
		case TM_NONE:		return "TM_NONE";
		case TM_HOUR:		return "TM_HOUR";
		case TM_MIN:		return "TM_MIN";
		case TM_SEC:		return "TM_SEC";
		case TM_AMPM: 		return "TM_AMPM";
		case TM_DAY:		return "TM_DAY";
		case TM_MONTHNR:	return "TM_MONTHNR";
		case TM_MONTHNAME:	return "TM_MONTHNAME";
		case TM_YEAR:		return "TM_YEAR";
		default:			return "Unknown m_nExpecting";
	}
}
	
CDateParser::CDateParser (const TCHAR* psz)
{
	m_nHour = -1;
	m_nMins = -1;
	m_nSecs = -1;
	m_nDay = -1;
	m_nMonth = -1;
	m_nYear = -1;
	m_psz = psz;
	

	m_nExpecting = TM_NONE;
	m_bError = !m_psz;
}

#if	0
static const TCHAR* Stringise (TCHAR c)
{
	static TCHAR buf [10];
	
	if ( c )
	{
		buf [0] = c;
		buf [1] = '\0';
		return buf;
	}
	else	return "\\0";
}
#endif

/*!	Returns a CTime from the string

	Times are always HH:MM:SS (seconds optional) with or without am/pm suffix
	Dates are 
		- dd/mm/yy		eg	26/2/96	26.2.61
		- dd-mmm-yy		eg	26-Feb-96
		- mmm dd yy		eg	February 26 1961

	yy can be 1970-2038, or 70..38
		
	mmm can be the full month name or the abreviated month name
	
	Weekday names (abbreviated or not) are ignored

	Allowed omissions:
		- Time -> current time
		- Date -> today's date
		- Secs -> 0
		- Year -> current year
*/
CTime CDateParser::Parse()
{
	int	nr;			// work variable
	TCHAR szBuf [80];	// buffer for date/time items

	//TRACE ("Parse() \"%s\"\n", m_psz);
	while ( *m_psz && !m_bError )
	{		
		if ( GetNext (szBuf) )
		{	
			if ( CXString::IsDigit (szBuf) )
			{				
				nr = atoi (szBuf);

				//TRACE ("nr = %d *m_psz = \'%s\' m_nExpecting = %s\n",
				//			nr, Stringise (*m_psz), ExpectingStr());

				if ( isspace (*m_psz) || *m_psz == '\0' )
				{
					if ( m_nExpecting == TM_NONE ||
						m_nExpecting == TM_DAY ||
						m_nExpecting == TM_AMPM )
					{
						SetDay (nr);
						if ( m_nMonth == -1 )
							m_nExpecting = TM_MONTHNAME;
						else m_nExpecting = TM_YEAR;
					}
					else if ( m_nExpecting == TM_YEAR )
					{
						SetYear (nr);
						m_nExpecting = TM_NONE;
					}
					else if ( m_nExpecting == TM_MIN )
					{
						SetMin (nr);
						m_nExpecting = TM_AMPM;
					}
					else if ( m_nExpecting == TM_SEC )
					{
						SetSec (nr);
						m_nExpecting = TM_AMPM;
					}
					else m_bError = true;
				}
				switch ( *m_psz )
				{
					case '.':
					case ':':		// Must be a hour or min field
						if ( m_nExpecting == TM_NONE )
						{
							SetHour (nr);
							m_nExpecting = TM_MIN;
						}
						else if ( m_nExpecting == TM_MIN )
						{
							SetMin (nr);
							m_nExpecting = TM_SEC;
						}
						break;
			
					case '/':		// Can only be day or month
					case '-':
						if ( m_nExpecting == TM_NONE ||
							m_nExpecting == TM_AMPM )
						{
							SetDay (nr);
							m_nExpecting = TM_MONTHNR;
						}
						else if ( m_nExpecting == TM_MONTHNR )
						{
							SetMonth (nr);
							m_nExpecting = TM_YEAR;
						}
						else m_bError = true;
						break;
							
					default:
						break;
				}
			}
			else
			{
				//TRACE ("Not digit string\n");
				if ( !GetWeekDay (szBuf) )			// Ignore weekdays
					if ( nr = GetMonth (szBuf) )
					{
						SetMonth (nr);
						if ( m_nDay == -1 )
							m_nExpecting = TM_DAY;
						else m_nExpecting = TM_YEAR;
					}
					else if ( m_nExpecting == TM_MIN ||
							m_nExpecting == TM_SEC )
					{
						// Looks like a "99pm" sequence
						int len = strlen (szBuf);
						if ( len > 2 )
						{
							nr = atoi (szBuf);
							if ( m_nExpecting == TM_MIN )
								SetMin (nr);
							else SetSec (nr);
							
							ProcessAmPm (szBuf+len-2);
							m_nExpecting = TM_NONE;
						}
						else m_bError = true;
					}
					else if ( m_nExpecting == TM_AMPM )
					{
						ProcessAmPm (szBuf);
						m_nExpecting = TM_NONE;
					}
					else m_bError = true;
				
			}
			if ( *m_psz )		// Skip past any separator
				m_psz++;
		}
	}
     
	//TRACE ("%d:%d:%d %d/%d/%d\n", m_nHour, m_nMins, m_nSecs, m_nDay, m_nMonth, m_nYear);

	if ( m_bError ||
		(m_nYear == -1 && m_nMonth == -1 && m_nDay == -1 &&
		 m_nHour == -1 && m_nMins == -1  && m_nSecs == -1) )
	{
		CTime time ((time_t) 0);		// Return an invalid time
		return time;
	}

	CTime now = CTime::GetCurrentTime();
	if ( m_nYear == -1 && m_nMonth == -1 && m_nDay == -1 )
	{
		// Default to today's date
		m_nDay = now.GetDay();
		m_nMonth = now.GetMonth();
		m_nYear = now.GetYear();
	}
	else	if ( m_nYear == -1 )		// Year may also be omitted
	{
		// Default to this year
		m_nYear = now.GetYear();
	}

	if ( m_nHour == -1 && m_nMins == -1 && m_nSecs == -1 )
	{
		// Default to current time
		m_nHour = now.GetHour();
		m_nMins = now.GetMinute();
		m_nSecs = now.GetSecond();
	}
	else if ( m_nSecs == -1 )		// Seconds can be omitted
		m_nSecs = 0;

	
	if ( !IsValid() )
	{
		return CTime ((time_t) 0);
		/*CTime time ((time_t) 0);		// Return an invalid time
		return time;*/
	}
	else
	{
		CTime time (m_nYear, m_nMonth, m_nDay, m_nHour, m_nMins, m_nSecs);
		return time;
	}
}


int CDateParser::GetNext (TCHAR* pszItem)
{
	// Gets the next item in the string and returns it's length
	TCHAR* d = pszItem;

	//TRACE ("GetNext (\"%s\") -> ", m_psz);

	while ( isspace (*m_psz) ) m_psz++;		// Skip leading spaces
	
	while ( *m_psz )                  
		if ( isspace (*m_psz) || *m_psz == ':' ||
			*m_psz == '.' ||
			*m_psz == '/' || *m_psz == '-' )
			break;
		else *d++ = *m_psz++;
	*d = '\0';
	
	//TRACE ("\"%s\"\n", pszItem);
	return strlen (pszItem);
}


void CDateParser::SetHour (int hour)
{
	if ( m_nHour == -1 )
		m_nHour = hour;
	else m_bError = true;
	//TRACE ("SetHour (%d) -> %d\n", hour, m_nHour);
}


void CDateParser::SetMin (int min)
{
	if ( m_nMins == -1 )
		m_nMins = min;
	else m_bError = true;
	//TRACE ("SetMin (%d) -> %d\n", min, m_nMins);
}


void CDateParser::SetSec (int sec)
{
	if ( m_nSecs == -1 )
		m_nSecs = sec;
	else m_bError = true; 
	//TRACE ("SetSec (%d) -> %d\n", sec, m_nSecs);
}


void CDateParser::SetDay (int day)
{
	if ( m_nDay == -1 )
		m_nDay = day;
	else m_bError = true;
	//TRACE ("SetDay (%d) -> %d\n", day, m_nDay);
}

void CDateParser::SetMonth (int month)
{
	if ( m_nMonth == -1 )
		m_nMonth = month;
	else m_bError = true;
	//TRACE ("SetMonth (%d) -> %d\n", month, m_nMonth);
}

void CDateParser::SetYear (int year)
{
	// Year can only legally be in the 1970..2038 CTime / time_t range
	// Take an educated guess as to the correct year
	
	if ( m_nYear == -1 )
	{
		if ( year < 100 )
			if ( year >= 70 )
				year += 1900;
			else year += 2000;
		m_nYear = year;
	}
	else m_bError = true;
	//TRACE ("SetYear (%d) -> %d\n", year, m_nYear);
}


void CDateParser::ProcessAmPm (const TCHAR* s)
{
	if ( _stricmp (s, "am") == 0 )
		;
	else if ( _stricmp (s, "pm") == 0 )
	{
		if ( m_nHour < 12 )
			m_nHour += 12;
	}
	else m_bError = true;
}



bool CDateParser::IsValid()
{
	if ( m_nYear < 1970 || m_nYear > 2038 ||
		m_nMonth < 1 || m_nMonth > 12 ||
		m_nDay < 1 || m_nDay > 31 ||
		m_nHour < 0 || m_nHour > 23 ||
		m_nMins < 0 || m_nMins > 59 ||
		m_nSecs < 0 || m_nSecs > 59 )
		return false;
	else return true;
}


int CDateParser::GetMonth (const TCHAR* s)
{
	static TCHAR *szMonths[] = { "January",
						   "February",
						   "March",
						   "April",
						   "May",
						   "June",
						   "July",
						   "August",
						   "September",
						   "October",
						   "November",
						   "December" };

	if ( s == NULL )
		return 0;

	int	i;
	size_t len = strlen (s);
	TCHAR** pszMon = szMonths;
	
	for (i = 1; i <= 12; i++, pszMon++)
		if ( len <= strlen (*pszMon) &&
			!_strnicmp (s, *pszMon, len) )
			return i;
	
	return 0;
}



int CDateParser::GetWeekDay (const TCHAR *s)
{
	static TCHAR *szWeekDays[] = { "Sunday",
							"Monday",
							"Tuesday",
							"Wednesday",
							"Thursday",
							"Friday",
							"Saturday" };

	if ( s == NULL )
		return 0;

	int	i;
	size_t len = strlen (s);
	TCHAR** pszDay = szWeekDays;
	
	for (i = 1; i <= 7; i++, pszDay++)
		if ( len <= strlen (*pszDay) &&
			!_strnicmp (s, *pszDay, len) )
			return i;
	
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// CXString class


/** Chops any cr/lf pairs off the end of the string */
int CXString::Chop()
{
	int	  nLen = GetLength();
	LPTSTR s = GetBuffer (nLen) + nLen - 1;
	
	while ( nLen && (*s == '\n' || *s == '\r') )
	{
		--s;
		--nLen;
	}
	ReleaseBuffer (nLen);
	return GetLength();
}


/**	Chops the string to the given length.  If the bEllipsis flag is set, then
	an ellipsis is added to the end of the string

	Example:

\verbatim
	"This is a test string" -> "This is a test..."	: Chop (17, true);
	"This is a test string" -> "This is a test st"	: Chop (17, false);
\endverbatim
	
	Special case when nChars < 4 and bEllipsis is true
	"This is a test string" -> "T"	nChars = 1
					    -> "T."	nChars = 2
					    -> "T.."	nChars = 3

\param	nChars		The number of characters to limit the string to.
\param	bEllipsis		Flag to add an ellipsis string ("...") or not.

\return The number of characters in the resultant string.
*/
int CXString::Chop (int nChars, bool bEllipsis)
{
	int	nLen = GetLength();

	if ( nLen == 0 || nChars >= nLen )
		return nLen;

	LPTSTR s = GetBuffer (nLen);
	if ( bEllipsis )
	{
		int	nDots = 3;			// Normal case
		
		if ( nChars > 0 )
		{
			if ( nChars < 4 )	// Adjust for diminutive string case 
				nDots = nChars - 1;
		}
		else nDots = 0;
	
		s += nChars - nDots;
		while ( nDots-- )
			*s++ = '.';
	}

	ReleaseBuffer (nChars);
	return GetLength();
}

/**	Clips the text to the given rectangle and adds an ellipsis if asked to.  
	Note that it always returns at least one character of a non NULL string.

\param	pDC			The device context to write to.
\param	nWidth		The width in device coordinates to limit the text to.
\param	bEllipsis		A flag to include an ellipsis or not.
\return	The number of characters in the chopped string.
*/
int	CXString::Chop (CDC* pDC, int nWidth, bool bEllipsis /*= true*/)
{
     UNREFERENCED_PARAMETER (bEllipsis);

	int nLen = GetLength();
	
	if ( nLen < 2 )
		return nLen;
	 
	int	nMaxBuf = nLen + 5;			// Since CalcStringEllipsis can return
								// a string that's longer than the orig!
	LPTSTR psz = GetBuffer (nMaxBuf);		// 

	CalcStringEllipsis (pDC->m_hDC, psz, nLen, nMaxBuf, nWidth);
	ReleaseBuffer();
	return GetLength();
}



/*!	This determines whether the specified string is too wide to fit in an 
	allotted space, and if not truncates the string and adds some points of 
	ellipsis to the end of the string.

\param	hdc			Handle of the DC the string will be drawn on.
\param	pszString 	Pointer to the string to verify
\param	nLen	     	The length of the passed in string.
\param	nMaxBuf	  	Maximum size of the pszString buffer.
\param	nMaxWidth  	Width of the space in pixels to fit the string into.

\return	Returns true if the string needed to be truncated, or false if it fit
		into nMaxWidth.
*/
bool CXString::CalcStringEllipsis (HDC hdc, LPTSTR pszString, int nLen, 
							int nMaxBuf, UINT nMaxWidth)
{

	static bool fOnce = true;
	static BOOL (__stdcall *pGetTextExtentPoint) (HDC, LPCTSTR, int, LPSIZE);	

	// We make heavy use of the GetTextExtentPoint32() API in this function,
	// but GetTextExtentPoint32() isn't implemented in Win32s.  Here we check
	// our OS type and if we're on Win32s we degrade and use
	// GetTextExtentPoint().
	if ( fOnce )
	{
		fOnce = false;
		OSVERSIONINFO osvi;

		ZeroMemory (&osvi, sizeof OSVERSIONINFO);
		osvi.dwOSVersionInfoSize = sizeof OSVERSIONINFO;

		if ( !GetVersionEx (&osvi) || osvi.dwPlatformId == VER_PLATFORM_WIN32s )
			pGetTextExtentPoint = &GetTextExtentPoint;
		else	pGetTextExtentPoint = &GetTextExtentPoint32;
	}


	SIZE   sizeString;
	// Get the width of the string in pixels
	if ( !(pGetTextExtentPoint)(hdc, pszString, nLen, &sizeString) )
	{
		AfxMessageBox ("Error occured");
		return false;
	}

	// If the width of the string is greater than the column width shave
	// the string and add the ellipsis
	if ( (ULONG) sizeString.cx <= nMaxWidth )
		return true;		// Don't need to do anything 

	const TCHAR szEllipsis3[] = TEXT("...");
	const TCHAR szEllipsis2[] = TEXT ("..");
	const TCHAR szEllipsis1[] = TEXT (".");
	const TCHAR *szEllipsis[] = { NULL, szEllipsis1, szEllipsis2, szEllipsis3 };
	SIZE   sizeEllipsis;
	int	nDots = 3;		// Number of ellipsis dots

	if ( !(pGetTextExtentPoint) (hdc,
						    szEllipsis [nDots],
						    nDots,
						    &sizeEllipsis) )
	{
		AfxMessageBox ("Error occured");
		return false;
	}

	for (;;)
	{
		if ( nLen > 1 )
			pszString [--nLen] = '\0';
		else
		{
			if ( --nDots <= 0 )
				return true;

			if ( !(pGetTextExtentPoint) (hdc,
								    szEllipsis [nDots],
								    nDots,
								    &sizeEllipsis) )
			{
				AfxMessageBox ("Error occured");
				return false;
			}
		}				


		if ( !(pGetTextExtentPoint)(hdc, pszString, nLen, &sizeString) )
		{
			AfxMessageBox ("Error occured");
			return false;
		}

		if ( (ULONG) (sizeString.cx + sizeEllipsis.cx) <= nMaxWidth )
		{
			// The string with the ellipsis finally fits, now make sure
			// there is enough room in the string for the ellipsis
			if ( nDots && nMaxBuf >= nLen + nDots )
			{
				// Concatenate the two strings and break out of the loop
				lstrcat (pszString, szEllipsis [nDots]);
				return true;;
			}
		}
	}
	return true;
}



		 		

#if	0
int CXString::Limit (int nChars)
{
	//	Limit a long pathname to the given number of characters.
	//	e.g. c:\first\second\third\fourth\filename.ext ->
	//		c:\...\fourth\filename.ext
	//
	//	Returns the number of characters in the string

	int	nLen = GetLength();
	LPTSTR s = GetBuffer (nLen);

	if ( nLen > nChars-1 )
	{
		TCHAR* p;
		TCHAR* pszTemp = new TCHAR [nChars];
		if ( !pszTemp )
			Chop (nChars, false);

		if ( (p = strchr (pszTemp, '\\')) == 0 )
			Chop (nChars, false);
		*(++p) = '\0';
		
		p = s + nLen - nChars+6;
		if ( p = strchr (p, '\\') )
			sprintf (s, "%s...%s", pszTemp, p);
		else sprintf (s, "%s...%s", pszTemp, pszTemp + nLen - nChars+6);
	}

	ReleaseBuffer();
	return GetLength();
}
#endif



/**	Returns \b true if the entire string is made of whitespace, that is, space 
	characters, tab characters, carriage return, or linefeed characters.
\param	s		The string to check.
\return	\b true if the string is whitespace, \b false otherwise.
*/
/*static*/ bool PASCAL CXString::IsSpace (const TCHAR* s)
{
	if ( s )
		while ( *s )
			if ( !isspace (*s++) )
				return false;
	
	return true;
}


/**	Returns \b if the string is composed entirely of digits.

\param	s		The string to check.
\return	\b true if the string is digits, \b false otherwise.
*/
/*static*/ bool PASCAL CXString::IsDigit (const TCHAR *s)
{ 
	if ( s == NULL || *s == '\0' )
		return false;
			
	while ( *s )
		if ( !isdigit (*s++) )
			return false;
	
	return true;
}


/**	Returns true if all the digits in the given string are digits or valid 
	separators, with an optional preceding '+' or '-'.

	Example
\verbatim
     9,999,999,999.9999999    is valid

     9,99,99                  is NOT valid
     9.99,999                 is NOT valid
\endverbatim

\param	s			The string to check.
\param	cSeparator	The thousands separator (defaults to a comma).
\param	cDecimalPoint	The decimal point character (defaults to a full stop).
\return	\b true if the string is a number, \b false if otherwise.
*/
/*static*/ bool PASCAL CXString::IsNumber (const TCHAR *s /* = NULL */, 
								   int cSeparator /* = ',' */,
								   int cDecimalPoint /* = '.' */)
{
	int nSep = -1;
	int bDec = 0;

	if ( s == NULL || *s == '\0' )
		return false;

     if ( *s == '+' || *s == '-' )
          s++;

	//TRACE ("IsNumber (\"%s\", \'%c\', \'%c\')\n", s, cSeparator, cDecimalPoint);
	for ( ; *s; ++s)
	{
		//TRACE ("*s=\'%c\' nSep=%d, bDec=%d\n", *s, nSep, bDec);
		if ( isdigit (*s) )
		{
			if ( !bDec && nSep != -1 && --nSep == -1 )
				return false;
		}
		else
		{
			if ( *s == cSeparator )
			{
				//TRACE ("Separator found\n");
				if ( bDec )
					return false;
					
				if ( nSep == -1 || nSep == 0 )
					nSep = 3;
				else if ( nSep )
					return false;
			}
			else if ( *s == cDecimalPoint )
			{
				//TRACE ("Decimal point found\n");
				if ( nSep != -1 && nSep )
					return false;
				bDec = true;
			}
		}
	}
	return nSep == -1 || nSep == 0;
}


bool CXString::IsUpper() const
{
     int len = GetLength();
     if ( len == 0 )
          return false;

     for (const TCHAR* s = (PCXSTR) this; *s; s++)
          if ( isalpha (*s) && !isupper (*s) )
               return false;
     
     return true;
}

bool CXString::IsLower() const
{
     int len = GetLength();
     if ( len == 0 )
          return false;

     for (const TCHAR* s = (PCXSTR) this; *s; s++)
          if ( isalpha (*s) && !islower (*s) )
               return false;
     
     return true;
}

/**  Determines whether this string is capitalised or not.  Capitalised means 
     that the first character of each word is an upper case letter, whereas the 
     rest of the word is lower case.

\return   \b true if this string is capitalised, \b false otherwise.
*/
bool CXString::IsCapitalised() const
{
     int len = GetLength();
     if ( len == 0 )
          return false;

     const TCHAR* s = (PCXSTR) this;
     const TCHAR* p = NULL;
     for (; *s; s++)
     {
          if ( p == NULL || isspace (*p) )
          {
               if ( !isupper (*s) )
                    return false;
          }
          else if ( !islower (*s) )
                    return false;
          p = s;
     }
     
     return true;
}





/**	Returns a CTime from the string.  It uses the CDateParser::Parse() to do 
	this.

\return	A CTime object representation of the CXString objects date / time.
*/
CTime CXString::GetTime()
{
	CDateParser dp ((PCXSTR) this);
	CTime time = dp.Parse();
	return time;
}




void CXString::Commatise()
{
	// Insert commas in a numeric CXString 
	
	if ( IsEmpty() )
		return;

     // TODO: Big enough for any number string (?!) - Need to to better
     const int MAX_BUF = 40;

     if (GetLength() >= MAX_BUF)
          return;

     TCHAR str[MAX_BUF];
	int	len = 0;
	int	start = 0;

	// Put number portion into the buffer
	const TCHAR* orig = (PCXSTR) this;
	TCHAR* d = str;
	
     // Skip leading space
	while ( isspace (*orig) )
	{
		orig++;
		start++;
	}

	int	end = start;
	while ( isdigit (*orig) )			// Get the number portion
	{
		*d++ = *orig++;
		end++;
		if ( ++len > sizeof str )
			return;					// don't do anything. The number is too big
	}
	
	TCHAR* s = str+len-1;				/* s points at the lsd */
	d = s+((len-1)/3)+1;	*d-- = '\0';	/* d is shifted by nr of commas */
	for (int digits = 0; s != d; *d-- = *s--)	/* shift the string right and */
		if ( digits++ >= 3 )				/* insert commas			*/
		{
			*d-- = ',';
			digits = 1;
		}
	
	CString strTrail = Mid (end);
	*this = Left (start);
	*this += str;
	*this += strTrail;
}


void CXString::Decommatise()
{
     Decommatise (GetBuffer (0));
     ReleaseBuffer();
}

/*static*/ void CXString::Decommatise (TCHAR* psz)
{
     // Extracts the commas from a numberic string and returns the length of the 
     // new string.  Note that this version does no format checking, it simple 
     // removes all the commas from the given string.

     ASSERT ( IsNumber (psz) );
     
     for ( ; *psz; psz++)
          if ( *psz == ',' )
               for (TCHAR* s = psz; *s; s++)
                    *s = *(s+1);
}

void CXString::Capitalise()
{
     Capitalise (*this);
}


void CXString::Capitalise (CString& s)
{
	int nLen = s.GetLength();
	Capitalise (s.GetBuffer (0));
	s.ReleaseBuffer (nLen);
}

// @mfunc Makes the first letter of each word upper case, the rest lower.
//
// @parm  The string to capitalise.
//
/*static*/ void CXString::Capitalise (TCHAR* psz)
{
     const TCHAR* prev = NULL;
     for ( ; *psz; psz++)
     {
          if ( prev == NULL || isspace (*prev) )
               *psz = (TCHAR) toupper (*psz);
          else *psz = (TCHAR) tolower (*psz);
          prev = psz;
     }
}


void CXString::ToggleCase()
{
     ToggleCase (*this);
}


void CXString::ToggleCase (CString& s)
{
	int nLen = s.GetLength();
     ToggleCase (s.GetBuffer (0));
     s.ReleaseBuffer();
}

// @mfunc Toggles the case of every character in the given string.
//
//        Non alphabetic characters are left unchanged.
//
// @parm  The string to toggle.
//
/*static*/ void CXString::ToggleCase (TCHAR* psz)
{
     for ( ; *psz; psz++)
          if ( isalpha (*psz) )
          {
               if ( isupper (*psz) )
                    *psz = (TCHAR) tolower (*psz);
               else if ( islower (*psz) )
                    *psz = (TCHAR) toupper (*psz);
          }
}


int CXString::Limit (int nChars)
{
	//	Limit a long pathname to the given number of characters.
	//	e.g. c:\first\second\third\fourth\filename.ext ->
	//		c:\first\...\fourth\filename.ext
	//	or \\server_one\sys\archives\project5001\software\version1.00.4\config.zip ->
	//	   \\server_one\...\version1.00.4\config.zip
	//
	//	Returns the number of characters in the string

	int	nLen = GetLength();
	LPTSTR pszData = GetBuffer (nLen);

	if ( nLen > nChars-1 )
	{
		TCHAR* psz1 = strchr (pszData, '\\');		// End of first string

		if ( psz1 )
		{
			psz1++;
			*psz1++ = '.';
			*psz1++ = '.';
			*psz1++ = '.';

			TCHAR* psz2 = pszData + nLen;
			TCHAR* pszBackSlash = psz2;
			int nNewLen = psz1 - pszData;
			while ( psz2 > psz1 && nNewLen < nChars )
			{
				if ( *psz2 == '\\' )
					pszBackSlash = psz2;

				--psz2;
				++nNewLen;
			}

			psz2 = pszBackSlash;
			while ( *psz2 )
				*psz1++ = *psz2++;
			*psz1 = '\0';
		}
		else Chop (nChars);
	}

	ReleaseBuffer();
	return GetLength();
}

/// Replaces the pszFind text with the pszReplace text
bool CXString::FindReplace (const TCHAR* pszFind, const TCHAR* pszReplace)
{
	int nIndex = Find (pszFind);
	if ( nIndex != -1 )
		return FindReplace (nIndex, _tcslen (pszFind), pszReplace);
	return false;
}

///	Replaces the nLen chars at nIndex with the pszReplace text.
bool CXString::FindReplace (int nIndex, int nLen, const TCHAR* pszReplace)
{
	int nOldLen = GetLength();
	int nReplaceLen = _tcslen (pszReplace);
	int nNewLen = nOldLen - nLen + nReplaceLen;
	TCHAR* pThis = GetBuffer (nNewLen > nOldLen ? nNewLen : nOldLen);

	// Move last portion (if find and replace lengths are different)
	// The +1 is to get the terminating '\0'.
	int nLastLen = nOldLen - nIndex - nLen + 1;
	if ( nLen > nReplaceLen )
	{
		// start at the left
		const TCHAR* s = pThis + nIndex + nLen;
		TCHAR* d = pThis + nIndex + nReplaceLen;
		for (int i = 0; i < nLastLen; i++)
			*d++ = *s++;
	}
	else
	{
		// start at the right (end)
		const TCHAR* s = pThis + nOldLen;
		TCHAR* d = pThis + nNewLen;
		for (int i = 0; i < nLastLen; i++)
			*d-- = *s--;
	}

	// Copy the replace text into the hole
	const TCHAR* s = pszReplace;
	TCHAR* d = pThis + nIndex;
	for (int i = 0; i < nReplaceLen; i++)
		*d++ = *s++;
	ReleaseBuffer (nNewLen);
	return true;
}

/*!  Gets the maximum line length of the string passed to it.

     \param    psz  The string to get the maximum line length from.
     \return   The length of the longest line.
*/
/*static*/ int CXString::GetMaxLineLength (const TCHAR* psz)
{
     int max = 0;
     int len = 0;
     for (; *psz; psz++)
     {
          if ( *psz == '\n' || *psz == '\r' )
          {
               if ( len > max )
                    max = len;
               len = 0;
          }
          else len++;
     }
     if ( len > max )
          max = len;
     
     return max;
}


bool CXString::LoadDescription (UINT nIDS)
{
     if ( LoadString (nIDS) )
     {
          int index = Find ('\n');
          if ( index != -1 )
               *this = Left (index);
          return true;
     }
     return false;
}

bool CXString::LoadToolTip (UINT nIDS)
{
     if ( LoadString (nIDS) )
     {
          int index = Find ('\n');
          if ( index != -1 )
          {
               *this = Mid (index + 1);
               return true;
          }
     }
     return false;
}



void CXString::MakeSameCase (const CXString& sSource)
{
	MakeSameCase (*this, sSource);
}


/*!	Makes a string the same case as the given source template.

	Rules for keep case is if source text is:
		- UPPER, then make UPPER CASE
		- lower, then lower case
		- Capitalised, then make Capitalised
		- MiXeD, then just use the text as is

\param	sTarget		The string to change.
\param	sSource		The string to make the same as.
*/
/*static*/ void CXString::MakeSameCase (CXString& sTarget, const CXString& sSource)
{
     if ( sSource.IsUpper() )
          sTarget.MakeUpper();
     else if ( sSource.IsLower() )
          sTarget.MakeLower();
     else if ( sSource.IsCapitalised() )
          sTarget.Capitalise();
     //else leave as is...
}

/**	Expands the given environment variable string from the current processes 
	environment variable strings.

	Any environment variables - which are delimitted by % characters - are 
	expanded to the environment variable value.  If a % delimitted string is 
	found which is not environment variable, then it is not expanded, but left 
	as is.

	For example, the string..
	
\verbatim
	%PATH%;c:\\Program Files\\MegaWhipple
\endverbatim

	would be expanded to...

\verbatim
	c:\\WinNT;c:\\WinNT\\System32;c:\\Program Files\\MegaWhipple" 
\endverbatim
	
	(assuming that the PATH variable is set to "c:\\WinNT;c:\\WinNT\\System32").

\param	sEnv			The string to expand.  It is returned as expanded.
*/
/*static*/ void CXString::ExpandEnvStr (CString& sEnv)
{
	sEnv = ExpandMacroStrImp (sEnv, NULL, "%", "%");
}


/**	Expands the given string value with the given macros.  Macros are delimited 
	by the given start and end delimiters.  If the end delimiter is omitted, 
	then a non-word character terminates the word (a word character is a 
	letter, digit, or underscore).

	For example, the following code...

\verbatim
	CString sText = "This is the ${LANGUAGE} language version of ${APP}, v${VER}";
	CMapStringToString mapMacros;
	mapMacros.SetAt ("LANGUAGE, "English");
	mapMacros.SetAt ("APP", "Wagamama");
	CXString::ExpandMacroStr (sText, mapMacros, "${", "}");
	printf ("Output is '%s'\n", sText);
\endverbatim

	would produce the following output...
	
\verbatim
	This is the English language version of Wagamama, v${VER}"
\endverbatim

\param	sText		The text to expand.
\param	mapMacros		The map of macros against values to expand the sText 
					with.
\param	pszStartDelim	The macro start delimiter.
\param	pszEndDelim 	The macro stop delimiter.  This can be omitted or set 
					to \b NULL, in which case the macro is terminated by a 
					non-word character (a word character is an alphnumeric, 
					or underscore).
*/
/*static*/ void CXString::ExpandMacroStr (CString sText, 
								  const CMapStringToString& mapMacros,
								  const TCHAR* pszStartDelim,
								  const TCHAR* pszEndDelim /*= NULL*/)
{
	sText = ExpandMacroStrImp (sText, &mapMacros, pszStartDelim, 
						     pszEndDelim ? pszEndDelim : "");
}

/*static*/ CString CXString::ExpandMacroStrImp (CString sText, 
									   const CMapStringToString* pmapMacros,
									   const CString& sStartDelim, 
									   const CString& sEndDelim)
{
	if ( sStartDelim.IsEmpty() )
		return sText;

	CString sConverted;
	
	const TCHAR* pCurr = sText;
	const TCHAR* pMacro = NULL;
	for ( ; *pCurr; pCurr++)
	{
		if ( pMacro == NULL )
		{
			// Look for a start delimiter...
			if ( strncmp (pCurr, sStartDelim, sStartDelim.GetLength()) == 0 )
				pMacro = pCurr + sStartDelim.GetLength();
			else sConverted += *pCurr;
		}
		else
			// Look for an end delimiter...
			if ( (sEndDelim.IsEmpty() && (isalnum (*pCurr) || *pCurr == '_')) ||
				(!sEndDelim.IsEmpty() && 
				 strncmp (pCurr, sEndDelim, sEndDelim.GetLength()) == 0) )
			{
				// End of a macro, look it up and expand it...
				CString sMacro = CString (pMacro, pCurr - pMacro);
				CString sValue;
				if ( pmapMacros )
				{
					if ( !pmapMacros->Lookup (sMacro, sValue) )
						sValue = sStartDelim + sMacro + sEndDelim;
				}
				else
				{
					// Get the environment variable.
					DWORD nLen = ::GetEnvironmentVariable (sMacro, NULL, 0) - 1;
					if ( nLen )
					{
						TCHAR* t = sValue.GetBuffer (nLen + 1);
						nLen = ::GetEnvironmentVariable (sMacro, t, nLen + 1);
						sValue.ReleaseBuffer (nLen);
					}
					if ( nLen == 0 )
						sValue = sStartDelim + sMacro + sEndDelim;
				}
				sConverted += sValue;
				pMacro = NULL;
			}
	}

	if ( pMacro )
		sConverted += pMacro;

	return sConverted;
}


/**	Returns a hex string representing the given string.  This is useful when 
	you need to print or display a string that contains unprintable characters.

\param	psz		The string to convert.  Note that this may contain NULL 
				characters.  The length of the string is determined only by
				the nLen parameter and convertion is not stopped by a NULL 
				character.
\param	nLen		The length of the string to convert.
\return	The converted string.
*/
/*static*/ CString CXString::HexStr (const TCHAR* psz, int nLen)
{
	CString sHex;
	while ( nLen-- )
	{
		CString s;
		s.Format ("%02x", (int) (unsigned char) (*psz++));
		sHex += s;
	}
	return sHex;
}


CString CXString::FromHex (const TCHAR* pszHex)
{
	CString sText;
	while ( *pszHex )
	{
		CString s (pszHex, 2);
		long l = strtol (s, NULL, 16);
		sText += (TCHAR) l;

		if ( *++pszHex == '\0' )
			break;
		pszHex++;
	}
	return sText;
}



/** Searches for the last occurance of \e sTarget in this CXString object.

\param    sTarget        The string to find.

\return   The index of the last occurance of the given string, or -1 if not 
          found.
*/
int CXString::ReverseFind (const CString& sTarget) const
{
     int nLen = sTarget.GetLength();
     const TCHAR* s = (PCXSTR) this;
     for (const TCHAR* p = s + GetLength() - nLen; p >= s; p--)
     {
          if ( strncmp (p, sTarget, nLen) == 0 )
               return p - s;
     }
     
	return -1;
}
