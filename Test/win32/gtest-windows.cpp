/*!
     \file     WorkspaceTest.cpp
     \author   John Young
     \date     22 August 2021
     
     Copyright (c) 2019 Thistle Software.
*/

#include "stdafx.h"
#include "gtest/gtest.h"
#include "gtest-windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void ASSERT_POINT_EQ (int x, int y, const POINT& pt)
{
     ASSERT_EQ (x, pt.x);
     ASSERT_EQ (y, pt.y);
}

void ASSERT_RECT_EQ (int left, int top, int right, int bottom, const RECT& rect)
{
     ASSERT_EQ (left, rect.left);
     ASSERT_EQ (top, rect.top);
     ASSERT_EQ (right, rect.right);
     ASSERT_EQ (bottom, rect.bottom);
}

void ASSERT_TIME_EQ (int year, int month, int day, int hour, int minute, int second,
                     const CTime& act)
{
     ASSERT_EQ (year,   act.GetYear());
     ASSERT_EQ (month,  act.GetMonth());
     ASSERT_EQ (day,    act.GetDay());
     ASSERT_EQ (hour,   act.GetHour());
     ASSERT_EQ (minute, act.GetMinute());
     ASSERT_EQ (second, act.GetSecond());
}
