/*!
     \file     XTime.cpp			Extended CTime and CXTimeSpan classes
	\author	John Young
     \date     19 May 2001

     Copyright (c) 2003 Thistle Software
*/

#include "StdAfx.h"
#include "../include/XTime.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*!  This returns a string of the current time span in the following format:

\verbatim
     dd days, hh:mm:ss hours       if there are days
     hh:mm:ss hours                if there are hours
     mm:ss mins                    if there are mins
     ss secs
\endverbatim

\return   A string in one of the above formats.
*/
CString CXTimeSpan::GetStr()
{
     CString str;
     if ( GetDays() )
          str.Format ("%d days, %2d:%02d:%02d hours", 
               GetDays(), GetHours(), GetMinutes(), GetSeconds());
     else if ( GetTotalHours() )
          str.Format ("%2d:%02d:%02d hours", GetHours(), GetMinutes(), GetSeconds());
     else if ( GetTotalMinutes() )
          str.Format ("%2d:%02d mins", GetMinutes(), GetSeconds());
     else str.Format ("%2d secs", GetSeconds());
     return str;
}




/*static*/ SYSTEMTIME CXTime::CvtToSystime (const struct tm* pTime)
{
     SYSTEMTIME sysTime;
     sysTime.wYear = pTime->tm_year + 1900;
     sysTime.wMonth = pTime->tm_mon + 1;
     sysTime.wDayOfWeek = pTime->tm_wday;
     sysTime.wDay = pTime->tm_mday;
     sysTime.wHour = pTime->tm_hour;
     sysTime.wMinute = pTime->tm_min;
     sysTime.wSecond = pTime->tm_sec;
     sysTime.wMilliseconds = 0;
     return sysTime;
}



/*static*/ CString CXTime::FormatLongDate (const struct tm* pTime)
{
     TCHAR szDate [100];                // Should be big enough for any date.
     SYSTEMTIME sysTime = CvtToSystime (pTime);
     LCID lcidSystem = ::GetSystemDefaultLCID();
     //LCID lcidUser = ::GetUserDefaultLCID();
     ::GetDateFormat (lcidSystem, DATE_LONGDATE, &sysTime, NULL, szDate, sizeof (szDate));
     return CString (szDate);
}


/*static*/ CString CXTime::FormatShortDate (const struct tm* pTime)
{
     // Converts the given tm structure into a string given by the format in the
     // control panel.  Mimics the CRT asctime() but for Windows.  If pTime is 
     // NULL then it returns an empty string.

     if ( pTime == NULL || mktime((struct tm*) pTime) <= 0 )     // Invalid time?  Return the empty string.
          return _T("");

#if	0
     CString sDate;
     sDate.Format (_T("%d/%d/%d"), pTime->tm_mday, pTime->tm_mon+1, pTime->tm_year+1900);
     return sDate;

#else
     TCHAR szDate [100];                // Should be big enough for any date.
     SYSTEMTIME sysTime = CvtToSystime (pTime);
     LCID lcidSystem = ::GetSystemDefaultLCID();
     //LCID lcidUser = ::GetUserDefaultLCID();
     ::GetDateFormat (lcidSystem, DATE_SHORTDATE, &sysTime, NULL, szDate, sizeof (szDate));
     return CString (szDate);
#endif
}


/*!	Converts the given tm structure into a string given by the format in the
	control panel.  Mimics the CRT asctime() but for Windows.  If pTime is NULL 
	then it returns an empty string.
*/
/*static*/ CString CXTime::FormatTime (const struct tm* pTime, DWORD flags /*= includeSecs*/)
{

     if ( pTime == NULL || mktime((struct tm*) pTime) <= 0 )     // Invalid time?  Return the empty string.
          return _T("");

#if	0
     CString sTime;
     if ( flags & FMTTIME_INCLUDESECS )
          sTime.Format (_T("%d:%02d:%02d"), pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
     else if ( flags & FMTTIME_EXCLUDECOLONS )	// BJ - To exclude colons in time returned 
		  sTime.Format (_T("%02d%02d"), pTime->tm_hour, pTime->tm_min);
	 else 
		  sTime.Format (_T("%d:%02d"), pTime->tm_hour, pTime->tm_min);
     return sTime;
#else
     TCHAR szTime [100];                // Should be big enough for any time.
     SYSTEMTIME sysTime = CvtToSystime (pTime);
     LCID lcidSystem = ::GetSystemDefaultLCID();
     //LCID lcidUser = ::GetUserDefaultLCID();
     DWORD dwFlags = 0;
     if ( (flags & includeSecs) == 0)
          dwFlags |= TIME_NOSECONDS;
     ::GetTimeFormat (lcidSystem, dwFlags, &sysTime, NULL, szTime, sizeof (szTime));
     return CString (szTime);
#endif
}



/*static*/ bool CXTime::IsToday (const struct tm* pTime)
{
     if ( pTime )
     {
          time_t t = time (NULL);
          struct tm* now = localtime (&t);
          return pTime->tm_year == now->tm_year &&
                 pTime->tm_mon == now->tm_mon &&
                 pTime->tm_mday == now->tm_mday;
     }
     return false;
}


/*!	Returns a date time string in the form determined by the control panel.  
	The time comes first in the return string.  The date is returned as the 
	control panel 'short' form.

\param	pTime	The time to format as a tm structure.
\param	flags	These can be one or more of the following...

				- FMTIME_INCLUDEDATE
*/
/*static*/ CString CXTime::FormatDateTime (const struct tm* pTime, 
								   DWORD flags /*= FMTTIME_INCLUDEDATE | FMTTIME_INCLUDESECS*/)
{
     if ( pTime == NULL || mktime((struct tm*) pTime) <= 0 )     // Invalid time?  Return the empty string.
          return _T("");

     CString sTime = FormatTime (pTime, flags);

     if ( (flags & includeDate) ||
          ((flags & includeDiffDate) && !IsToday (pTime)) )
     {
          CString sDate = FormatShortDate (pTime);
          if ( !sDate.IsEmpty() )
               sTime += ' ' + sDate;
     }
     return sTime;
}

bool CXTime::GetLocalTm(struct tm& ttm) const
{
	struct tm * ptm = CTime::GetLocalTm(&ttm);
	return ptm != NULL;
}