/*!
     \file     XTime.cpp			Extended CTime and CXTimeSpan classes
	\author	John Young
     \date     19 May 2001

     Copyright (c) 2003 Thistle Software
*/

#include "StdAfx.h"
#include "../include/XTime.h"
#include "../include/StringUtil.h"

namespace MFCX {


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
          str.Format ("%d days, %d:%02d:%02d hours", 
               GetDays(), GetHours(), GetMinutes(), GetSeconds());
     else if ( GetTotalHours() )
          str.Format ("%d:%02d:%02d hours", GetHours(), GetMinutes(), GetSeconds());
     else if ( GetTotalMinutes() )
          str.Format ("%d:%02d mins", GetMinutes(), GetSeconds());
     else str.Format ("%d secs", GetSeconds());
     return str;
}



// TODO: This is not required as CTime provides GetAsSystemTime() for this.
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


/*!  Formats the given date according to the long format user locale.
 *
 * \param pTime          The time to format into a string.
 * \return The date formatted as a string.
 */
/*static*/ CString CXTime::FormatLongDate (const struct tm* pTime)
{
     TCHAR szDate [100];                // Should be big enough for any date.
     SYSTEMTIME sysTime = CvtToSystime (pTime);
     //LCID lcidSystem = ::GetSystemDefaultLCID();
     //::GetDateFormat (lcidSystem, DATE_LONGDATE, &sysTime, NULL, szDate, sizeof (szDate));
     LCID lcid = ::GetUserDefaultLCID();
     ::GetDateFormat (lcid, DATE_LONGDATE, &sysTime, NULL, szDate, sizeof (szDate));
     return CString (szDate);
}


/*!  Formats the given date according to the short format user locale.
 *
 * \param pTime          The time to format into a string.
 * \return The date formatted as a string.
 */
/*static*/ CString CXTime::FormatShortDate (const struct tm* pTime)
{
     // Converts the given tm structure into a string given by the format in the
     // control panel.  Mimics the CRT asctime() but for Windows.  If pTime is 
     // NULL then it returns an empty string.

     if ( pTime == NULL || mktime((struct tm*) pTime) <= 0 )     // Invalid time?  Return the empty string.
          return _T("");

     TCHAR szDate [100];                // Should be big enough for any date.
     SYSTEMTIME sysTime = CvtToSystime (pTime);
     //LCID lcidSystem = ::GetSystemDefaultLCID();
     //LCID lcidUser = ::GetUserDefaultLCID();
     LCID lcid = ::GetUserDefaultLCID();
     ::GetDateFormat (lcid, DATE_SHORTDATE, &sysTime, NULL, szDate, sizeof (szDate));
     return CString (szDate);
}


/*!  Converts the given tm structure into a string given by the format in the
 *   control panel.  
 *
 *   Mimics the CRT asctime() but for Windows.
 *
 * \param pTime     The time to convert to a string.
 * \param flags     If includeSecs is set, then the seconds are included.
 * \return A date time string representing the given time.  If pTime is NULL 
 *         then it returns an empty string.
 */
/*static*/
CString CXTime::FormatTime (const struct tm* pTime, 
                            DWORD flags /*= includeSecs*/)
{
     if ( pTime == NULL || mktime((struct tm*) pTime) <= 0 )     // Invalid time?  Return the empty string.
          return _T("");

     TCHAR szTime [100];                // Should be big enough for any time.
     SYSTEMTIME sysTime = CvtToSystime (pTime);
     //LCID lcidSystem = ::GetSystemDefaultLCID();
     LCID lcid = ::GetUserDefaultLCID();
     DWORD dwFlags = 0;
     if ( (flags & includeSecs) == 0)
          dwFlags |= TIME_NOSECONDS;
     ::GetTimeFormat (lcid, dwFlags, &sysTime, NULL, szTime, sizeof (szTime));
     return CString (szTime);
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
								   DWORD flags /*= includeDate | includeSecs*/)
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

bool CXTime::GetLocalTm (struct tm& ttm) const
{
	struct tm * ptm = CTime::GetLocalTm(&ttm);
	return ptm != NULL;
}


CStopWatch::CStopWatch()
 :   _start (std::chrono::steady_clock::now()),
     _end (_start)
{
}

void CStopWatch::Start()
{
    _start = std::chrono::steady_clock::now();
}

void CStopWatch::Stop()
{
    _end = std::chrono::steady_clock::now();
    _duration = _end - _start;
}

LONGLONG CStopWatch::Days() const
{
     return _duration / 24h;
}

LONGLONG CStopWatch::Hours() const
{
     return _duration / 1h;
}

LONGLONG CStopWatch::Minutes() const
{
     return _duration / 1min;
}

LONGLONG CStopWatch::Seconds() const
{
     return _duration / 1s;
}

LONGLONG CStopWatch::MilliSeconds() const
{
     return _duration / 1ms;
}

LONGLONG CStopWatch::MicroSeconds() const
{
     return _duration / 1us;
}

LONGLONG CStopWatch::NanoSeconds() const
{
     return _duration / 1ns;
}

#define MILLIS_PER_SEC     (1000)
#define MICROS_PER_SEC     (1000 * MILLIS_PER_SEC)
#define NANOS_PER_SEC      (1000 * MICROS_PER_SEC)

string CStopWatch::HumanReadable() const
{
     LONGLONG dur = NanoSeconds();
     
     int nanos = dur % NANOS_PER_SEC;  dur /= NANOS_PER_SEC;
     int secs  = dur % 60;             dur /= 60;
     int mins  = dur % 60;             dur /= 60;
     int hours = dur % 24;
     int days  = dur / 24;

     // TODO: This is English only - locale?     
     string sTime;
     const TCHAR* pszSuffix = nullptr;
     if ( days )
     {
          sTime = CStringUtil::Format ("%d days, %d:%02d:%02d", days, hours, mins, secs);
          pszSuffix = "hours";
     }
     else if ( hours )
     {
          sTime = CStringUtil::Format ("%d:%02d:%02d", hours, mins, secs);
          pszSuffix = "hours";
     }
     else if ( mins )
     {
          sTime = CStringUtil::Format ("%d:%02d", mins, secs);
          pszSuffix = "mins";
     }
     else if ( secs )
     {
          sTime = CStringUtil::Format ("%d", secs);
          pszSuffix = "secs";
     }

     if ( !sTime.empty() )
          sTime += CStringUtil::Format (".%d %s", nanos, pszSuffix);
     else
     {
          int ms = nanos / MICROS_PER_SEC;
          if ( ms )
          {
               int us = nanos % (1000 * 1000);
               sTime = CStringUtil::Format ("%d.%06d msecs", ms, us);
          }
          else
          {
               int us = nanos / 1000;
               int ns = nanos % 1000;
               if ( us )
                    sTime = CStringUtil::Format ("%d.%03d usecs", us, ns);
               else sTime = CStringUtil::Format ("%d nsecs", ns);
          }
     }
     return sTime;
}



}    // namespace MFCX