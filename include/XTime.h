
// XTime.h


#ifndef   __MFXC_XTIME_H
#define   __MFXC_XTIME_H

#include <chrono>
#include <string>
#include <atltime.h>

using std::string;
using namespace std::literals;

namespace MFCX {


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

	CXTime() : CTime() {};
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

/*! Helps with ascertaining and displaying durations from a start point.
 *
 */
class CStopWatch
{
public:
     CStopWatch();
     
     void Start();
     void Stop();

     LONGLONG Days() const;
     LONGLONG Hours() const;
     LONGLONG Minutes() const;
     LONGLONG Seconds() const;
     LONGLONG MilliSeconds() const;
     LONGLONG MicroSeconds() const;
     LONGLONG NanoSeconds() const;
     string HumanReadable() const;

#ifdef    GTEST
     CStopWatch (const CStopWatch&) = delete;
     CStopWatch (LONGLONG msecs)
       : _duration (msecs) {} 
#endif


private:
     std::chrono::time_point<std::chrono::steady_clock> _start;
     std::chrono::time_point<std::chrono::steady_clock> _end;
     std::chrono::duration<__int64, std::nano> _duration;
};

}    // namespace MFCX
#endif    // __MFXC_XTIME_H
