
// XTime.h


#ifndef   __MFXC_XTIME_H
#define   __MFXC_XTIME_H

#include <atltime.h>


/// An extended CTimeSpan class
class CXTimeSpan : public CTimeSpan
{
public:
	CXTimeSpan() : CTimeSpan() {};
	CXTimeSpan (time_t time) : CTimeSpan (time) {};
	CXTimeSpan (LONG lDays, int nHours, int nMins, int nSecs) : 
	     CTimeSpan (lDays, nHours, nMins, nSecs) {};
	CXTimeSpan (const CTimeSpan& timeSpanSrc) : CTimeSpan (timeSpanSrc) {};

     CString GetStr();   ///< Returns the time span as text.
};



class CXTime : public CTime
{
public:
	enum { 
			includeDate = 1, 
			includeSecs = 2, 
			includeDiffDate = 4 
		};

	CXTime() : CTime () {};
	CXTime (const CTime& timeSrc) : CTime (timeSrc) {}
	CXTime (time_t time) : CTime (time) {}
	CXTime (int nYear, int nMonth, int nDay, 
		  int nHour, int nMin, int nSec, int nDST = -1) : 
		CTime (nYear, nMonth, nDay, nHour, nMin, nSec, nDST) {}
	CXTime (WORD wDosDate, WORD wDosTime, int nDST = -1) : 
		CTime (wDosDate, wDosTime, nDST) {}
	CXTime (const SYSTEMTIME& sysTime, int nDST = -1) : 
		CTime (sysTime, nDST) {}
	CXTime (const FILETIME& fileTime, int nDST = -1) : 
		CTime (fileTime, nDST ) {}
     bool GetLocalTm(struct tm&) const;
	static CString FormatShortDate (const struct tm* pTime);
	static CString FormatLongDate (const struct tm* pTime);
	static CString FormatTime (const struct tm* pTime, DWORD flags = includeSecs);
	static CString FormatDateTime (const struct tm* pTime, 
							 DWORD flags = includeDate | includeSecs);

	static bool IsToday (const struct tm* pTime);

	static SYSTEMTIME CvtToSystime (const struct tm* pTime);
};



#endif    // __MFXC_XTIME_H
