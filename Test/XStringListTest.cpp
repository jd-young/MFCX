/*!
     \file     XStringListTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../include/XStringList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString FindIndex (const CXStringList& list, int idx)
{
     POSITION pos = list.FindIndex (idx);
     return list.GetAt (pos);
}

TEST(CXStringListTest, TestInsertSort)
{
     CXStringList list;
     list.InsertSort ("First");
     ASSERT_EQ (1, list.GetSize());
     EXPECT_STREQ ("First", FindIndex (list, 0));
     
     list.InsertSort ("AAAA");
     ASSERT_EQ (2, list.GetSize());
     EXPECT_STREQ ("AAAA", FindIndex (list, 0));
     EXPECT_STREQ ("First", FindIndex (list, 1));
     
     list.InsertSort ("aaaaa");
     ASSERT_EQ (3, list.GetSize());
     EXPECT_STREQ ("AAAA", FindIndex (list, 0));
     EXPECT_STREQ ("First", FindIndex (list, 1));
     EXPECT_STREQ ("aaaaa", FindIndex (list, 2));
     
     list.InsertSort ("b");
     ASSERT_EQ (4, list.GetSize());
     EXPECT_STREQ ("AAAA", FindIndex (list, 0));
     EXPECT_STREQ ("First", FindIndex (list, 1));
     EXPECT_STREQ ("aaaaa", FindIndex (list, 2));
     EXPECT_STREQ ("b", FindIndex (list, 3));

     list.InsertSort ("zzz");
     ASSERT_EQ (5, list.GetSize());
     EXPECT_STREQ ("AAAA", FindIndex (list, 0));
     EXPECT_STREQ ("First", FindIndex (list, 1));
     EXPECT_STREQ ("aaaaa", FindIndex (list, 2));
     EXPECT_STREQ ("b", FindIndex (list, 3));
     EXPECT_STREQ ("zzz", FindIndex (list, 4)); // This is what we should see.

     list.InsertSort ("bb");
     ASSERT_EQ (6, list.GetSize());
     EXPECT_STREQ ("AAAA", FindIndex (list, 0));
     EXPECT_STREQ ("First", FindIndex (list, 1));
     EXPECT_STREQ ("aaaaa", FindIndex (list, 2));
     EXPECT_STREQ ("b", FindIndex (list, 3));
     EXPECT_STREQ ("bb", FindIndex (list, 4));
     EXPECT_STREQ ("zzz", FindIndex (list, 5));
}
