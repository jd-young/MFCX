// gtest-windows.h       ASSERTS for common Windows structures


#if ! defined   _GTEST_WINDOWS_H 
#define   _GTEST_WINDOWS_H 

#include "gtest/gtest.h"

void ASSERT_POINT_EQ (int x, int y, const POINT& pt);
void ASSERT_RECT_EQ (int left, int top, int right, int bottom, const RECT& rect);
void ASSERT_TIME_EQ (int year, int mon, int day, int hour, int min, int sec,
                     const CTime& act);

#endif    //   _GTEST_WINDOWS_H 
