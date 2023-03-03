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

TEST(CXTimeSpanTest, TestGetStr)
{
     // TODO: Shouldn't have a leading space
     CXTimeSpan tsSecs (0, 0, 0, 1);
     EXPECT_STREQ (" 1 secs", tsSecs.GetStr());

     CXTimeSpan tsMins (0, 0, 2, 1);
     EXPECT_STREQ (" 2:01 mins", tsMins.GetStr());

     CXTimeSpan tsHours (0, 3, 2, 1);
     EXPECT_STREQ (" 3:02:01 hours", tsHours.GetStr());

     // TODO: This is wrong - fix me!
     CXTimeSpan tsDays (4, 3, 2, 1);
     EXPECT_STREQ ("4 days,  0:03:02 hours", tsDays.GetStr());
}

#if 0
// TODO: Figure out how to unit test CXTime.
//       All functions call ::GetSystmeDefaultLCID() which means that the 
//       test will need to expect different results depending on different
//       locales.  Maybe separate out each function into separate functions that
//       take a locale as a parameter?
TEST(CXTimeTest, TestFormatTime)
{
     // TODO: Shouldn't have a leading space
     CXTime tm (0, 0, 0, 1);
     EXPECT_STREQ (" 1 secs", tsSecs.GetStr());
}
#endif
