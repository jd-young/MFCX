/*!
     \file     XTimeTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../include/XTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MFCX::CXTime;
using MFCX::CXTimeSpan;
using MFCX::CStopWatch;

TEST(XTimeSpanTest, TestGetStr)
{
     CXTimeSpan tsSecs (0, 0, 0, 1);
     EXPECT_STREQ ("1 secs", tsSecs.GetStr());

     CXTimeSpan tsMins (0, 0, 2, 1);
     EXPECT_STREQ ("2:01 mins", tsMins.GetStr());

     CXTimeSpan tsHours (0, 3, 2, 1);
     EXPECT_STREQ ("3:02:01 hours", tsHours.GetStr());

     // TODO: This is wrong - fix me!
     CXTimeSpan tsDays (4, 3, 2, 1);
     EXPECT_STREQ ("4 days, 0:03:02 hours", tsDays.GetStr());
}


// These tests are completely reliant on the fact that the writer is a Brit and
// has the dd/mm/yyyy short date format set in Windows.
TEST(XTimeTest, TestFormatShortDate)
{
     CXTime time (2023, 11, 16, 18, 4, 45);
     struct tm tm;
     EXPECT_TRUE (time.GetLocalTm(tm));
  	CString sDate = CXTime::FormatShortDate (&tm);
     EXPECT_STREQ ("16/11/2023", sDate);
}

TEST(XTimeTest, TestFormatLongDate)
{
     CXTime time (2023, 11, 16, 18, 4, 45);
     struct tm tm;
     EXPECT_TRUE (time.GetLocalTm(tm));
  	CString sDate = CXTime::FormatLongDate (&tm);
     EXPECT_STREQ ("16 November 2023", sDate);
}

TEST(XTimeTest, TestFormatTime)
{
     CXTime time (2023, 11, 16, 18, 4, 45);
     struct tm tm;
     EXPECT_TRUE (time.GetLocalTm(tm));

  	CString sDate = CXTime::FormatTime (&tm);
     EXPECT_STREQ ("18:04:45", sDate);

  	sDate = CXTime::FormatTime (&tm, 0);
     EXPECT_STREQ ("18:04", sDate);
}

TEST(XTimeTest, TestFormatDateTime)
{
     CXTime time (2023, 11, 16, 18, 4, 45);
     struct tm tm;
     EXPECT_TRUE (time.GetLocalTm(tm));
     
  	CString sDate = CXTime::FormatDateTime (&tm);
     EXPECT_STREQ ("18:04:45 16/11/2023", sDate);

  	sDate = CXTime::FormatDateTime (&tm, 0);
     EXPECT_STREQ ("18:04", sDate);

  	sDate = CXTime::FormatDateTime (&tm, CXTime::includeDate);
     EXPECT_STREQ ("18:04 16/11/2023", sDate);

  	sDate = CXTime::FormatDateTime (&tm, CXTime::includeSecs);
     EXPECT_STREQ ("18:04:45", sDate);
}

TEST(XTimeTest, TestIsToday)
{
     struct tm tm;

     CXTime past (2023, 11, 16, 18, 4, 45);  // Not today
     EXPECT_TRUE (past.GetLocalTm (tm));
     EXPECT_FALSE (CXTime::IsToday (&tm));

     CXTime today (time (0));
     EXPECT_TRUE (today.GetLocalTm (tm));
     EXPECT_TRUE (CXTime::IsToday (&tm));
}

/// Checks that stopwatch basic functionality works, but it's not very precise.
TEST(StopWatchTest, TestDurationAgainstSleep)
{
     CStopWatch sw;
     Sleep (50);     // 1 ms
     sw.Stop();
     EXPECT_LT (45, sw.MilliSeconds());
     EXPECT_GE (64, sw.MilliSeconds());

     EXPECT_LT (45000, sw.MicroSeconds());
     EXPECT_GE (64000, sw.MicroSeconds());

     EXPECT_LT (45000000, sw.NanoSeconds());
     EXPECT_GE (64000000, sw.NanoSeconds());
}

TEST(StopWatchTest, TestDurationPrecise)
{
     CStopWatch ns (123);
     EXPECT_EQ (0, ns.Days());
     EXPECT_EQ (0, ns.Hours());
     EXPECT_EQ (0, ns.Minutes());
     EXPECT_EQ (0, ns.Seconds());
     EXPECT_EQ (0, ns.MilliSeconds());
     EXPECT_EQ (0, ns.MicroSeconds());
     EXPECT_EQ (123, ns.NanoSeconds());
     EXPECT_STREQ ("123 nsecs", ns.HumanReadable().c_str());
     
     CStopWatch us (123456);
     EXPECT_EQ (0, us.Days());
     EXPECT_EQ (0, us.Hours());
     EXPECT_EQ (0, us.Minutes());
     EXPECT_EQ (0, us.Seconds());
     EXPECT_EQ (0, us.MilliSeconds());
     EXPECT_EQ (123, us.MicroSeconds());
     EXPECT_EQ (123456, us.NanoSeconds());
     EXPECT_STREQ ("123.456 usecs", us.HumanReadable().c_str());
     
     CStopWatch ms (123456789);
     EXPECT_EQ (0, ms.Days());
     EXPECT_EQ (0, ms.Hours());
     EXPECT_EQ (0, ms.Minutes());
     EXPECT_EQ (0, ms.Seconds());
     EXPECT_EQ (123, ms.MilliSeconds());
     EXPECT_EQ (123456, ms.MicroSeconds());
     EXPECT_EQ (123456789, ms.NanoSeconds());
     EXPECT_STREQ ("123.456789 msecs", ms.HumanReadable().c_str());
     
     CStopWatch secs (12345678901);
     EXPECT_EQ (0, secs.Days());
     EXPECT_EQ (0, secs.Hours());
     EXPECT_EQ (0, secs.Minutes());
     EXPECT_EQ (12, secs.Seconds());
     EXPECT_EQ (12345, secs.MilliSeconds());
     EXPECT_EQ (12345678, secs.MicroSeconds());
     EXPECT_EQ (12345678901, secs.NanoSeconds());
     EXPECT_STREQ ("12.345678901 secs", secs.HumanReadable().c_str());

     CStopWatch mins (123456789012);
     EXPECT_EQ (0, mins.Days());
     EXPECT_EQ (0, mins.Hours());
     EXPECT_EQ (2, mins.Minutes());
     EXPECT_EQ (123, mins.Seconds());
     EXPECT_EQ (123456, mins.MilliSeconds());
     EXPECT_EQ (123456789, mins.MicroSeconds());
     EXPECT_EQ (123456789012, mins.NanoSeconds());
     EXPECT_STREQ ("2:03.456789012 mins", mins.HumanReadable().c_str());

     CStopWatch mins20 (1234567890123);
     EXPECT_EQ (0, mins20.Days());
     EXPECT_EQ (0, mins20.Hours());
     EXPECT_EQ (20, mins20.Minutes());
     EXPECT_EQ (1234, mins20.Seconds());
     EXPECT_EQ (1234567, mins20.MilliSeconds());
     EXPECT_EQ (1234567890, mins20.MicroSeconds());
     EXPECT_EQ (1234567890123, mins20.NanoSeconds());
     EXPECT_STREQ ("20:34.567890123 mins", mins20.HumanReadable().c_str());

     CStopWatch hours (12345678901234);
     EXPECT_EQ (0, hours.Days());
     EXPECT_EQ (3, hours.Hours());
     EXPECT_EQ (205, hours.Minutes());
     EXPECT_EQ (12345, hours.Seconds());
     EXPECT_EQ (12345678, hours.MilliSeconds());
     EXPECT_EQ (12345678901, hours.MicroSeconds());
     EXPECT_EQ (12345678901234, hours.NanoSeconds());
     EXPECT_STREQ ("3:25:45.678901234 hours", hours.HumanReadable().c_str());

     CStopWatch days (123456789012345);
     EXPECT_EQ (1, days.Days());
     EXPECT_EQ (34, days.Hours());
     EXPECT_EQ (2057, days.Minutes());
     EXPECT_EQ (123456, days.Seconds());
     EXPECT_EQ (123456789, days.MilliSeconds());
     EXPECT_EQ (123456789012, days.MicroSeconds());
     EXPECT_EQ (123456789012345, days.NanoSeconds());
     EXPECT_STREQ ("1 days, 10:17:36.789012345 hours", days.HumanReadable().c_str());
}
