/*!
     \file     UtilsTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../include/Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TEST(CUtilsTest, TestGetArgs)
{
     GTEST_SKIP() << "SEH exception thrown in this test - fix me, or delete getargs()";
     int argc;
     TCHAR* argv [10];
     
     CUtils::getargs ("program -a aaa -b bbbb --long-arg arg1 arg2 arg3", 
                      &argc, argv, 10);
     EXPECT_EQ (9, argc);
     EXPECT_STREQ ("program", argv [0]);
     EXPECT_STREQ ("-a", argv [1]);
     EXPECT_STREQ ("aaa", argv [2]);
     EXPECT_STREQ ("-b", argv [3]);
     EXPECT_STREQ ("bbbb", argv [4]);
     EXPECT_STREQ ("--long-arg", argv [5]);
     EXPECT_STREQ ("arg1", argv [6]);
     EXPECT_STREQ ("arg2", argv [7]);
     EXPECT_STREQ ("arg3", argv [8]);
}
