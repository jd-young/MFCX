/*!
     \file     RegexKeyTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../include/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TEST(CStringUtilTest, TestIsPrintable)
{
     // TODO: I18n?
     for (int i = 0; i < 32; i++)
     {
          EXPECT_FALSE (CStringUtil::IsPrintable (i)) << "Character " << i;
     }
     
     for (int i = 32; i < 127; i++)
     {
          EXPECT_TRUE (CStringUtil::IsPrintable (i)) << "Character " << i;
     }
     
     EXPECT_FALSE (CStringUtil::IsPrintable (127)) << "Character " << 127;
     
     for (int i = 128; i < 256; i++)
     {
          EXPECT_TRUE (CStringUtil::IsPrintable (i)) << "Character " << i;
     }
}

TEST(CStringUtilTest, TestSplit)
{
     CStringArray result;

     // Single delimiter
     CString sText = "A,comma,separated,text,example";
     CStringUtil::Split (result, sText, ",");
     ASSERT_EQ (5, result.GetSize());
     EXPECT_STREQ ("A", result.GetAt (0));
     EXPECT_STREQ ("comma", result.GetAt (1));
     EXPECT_STREQ ("separated", result.GetAt (2));
     EXPECT_STREQ ("text", result.GetAt (3));
     EXPECT_STREQ ("example", result.GetAt (4));

     // Two delimiters, and a trailing delimiter (which should be ignored).
     sText = "A;comma,separated;text;example;";
     CStringUtil::Split (result, sText, ",;");
     ASSERT_EQ (5, result.GetSize());
     EXPECT_STREQ ("A", result.GetAt (0));
     EXPECT_STREQ ("comma", result.GetAt (1));
     EXPECT_STREQ ("separated", result.GetAt (2));
     EXPECT_STREQ ("text", result.GetAt (3));
     EXPECT_STREQ ("example", result.GetAt (4));
}

TEST(CStringUtilTest, TestJoin)
{
     CStringList lstStrings;
     lstStrings.AddTail ("A");
     lstStrings.AddTail ("comma");
     lstStrings.AddTail ("separated");
     lstStrings.AddTail ("text");
     lstStrings.AddTail ("example");

     // Single delimiter
     EXPECT_STREQ ("A,comma,separated,text,example", 
                   CStringUtil::Join (lstStrings, ","));
}

CString GetListStr (const CStringList& lst, int idx)
{
     POSITION pos = lst.FindIndex (idx);
     return lst.GetAt (pos);
}

TEST(CStringUtilTest, TestCopyFrom)
{
     CStringList lstOrig;
     lstOrig.AddTail ("A");
     lstOrig.AddTail ("comma");
     lstOrig.AddTail ("separated");
     lstOrig.AddTail ("text");
     lstOrig.AddTail ("example");

     CStringList lstTarget;
     CStringUtil::CopyFrom (lstOrig, lstTarget);
     ASSERT_EQ (5, lstTarget.GetSize());
     
     EXPECT_STREQ ("A",         (const TCHAR*) GetListStr (lstTarget, 0));
     EXPECT_STREQ ("comma",     (const TCHAR*) GetListStr (lstTarget, 1));
     EXPECT_STREQ ("separated", (const TCHAR*) GetListStr (lstTarget, 2));
     EXPECT_STREQ ("text",      (const TCHAR*) GetListStr (lstTarget, 3));
     EXPECT_STREQ ("example",   (const TCHAR*) GetListStr (lstTarget, 4));
}