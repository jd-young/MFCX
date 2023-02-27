/*!
     \file     XStringTest.cpp
     \author   John Young
     \date     22 August 2021
     
     Copyright (c) 2019 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include "win32/gtest-windows.h"
#include "../include/XString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TEST(CXStringTest, TestChop)
{
     CXString sTest = "This is a test\n\r\n\n\n";
     ASSERT_EQ (14, sTest.Chop());       
     ASSERT_STREQ ("This is a test", sTest);

     CXString sNoChange = "This is a \ntest";
     ASSERT_EQ (15, sNoChange.Chop());
     ASSERT_STREQ ("This is a \ntest", sNoChange);
}

TEST(CXStringTest, TestChomp)
{
     CXString sTest = "This is a test\n\r\n\n\n";
     ASSERT_EQ (14, sTest.Chomp());       
     ASSERT_STREQ ("This is a test", sTest);

     CXString sNoChange = "This is a \ntest";
     ASSERT_EQ (15, sNoChange.Chomp());
     ASSERT_STREQ ("This is a \ntest", sNoChange);
}

TEST(CXStringTest, TestChopToLength)
{
     CXString sElipsis = "This is a test string";
     ASSERT_EQ (17, sElipsis.Chop (17, true));
     ASSERT_STREQ ("This is a test...", sElipsis);

     CXString sTest = "This is a test string";
     ASSERT_EQ (17, sTest.Chop (17, false));
     ASSERT_STREQ ("This is a test st", sTest);
}

#if 0
class CMockDC : public CDC
{
public:
     CMockDC() : CDC();
};

TEST(CXStringTest, TestChopToWidth)
{
     MockDB mockDC;
     CXString sTest = "This a test string";
     ASSERT_EQ (17, sTest.Chop (17, true));
     ASSERT_STREQ ("This is a test...", sTest);

     CXString sTest = "This a test string";
     ASSERT_EQ (17, sTest.Chop (17, false));
     ASSERT_STREQ ("This is a test st", sTest);
}
#endif


TEST(CXStringTest, TestIsString)
{
     ASSERT_TRUE (CXString::IsSpace (" \t   \r\n    "));
     ASSERT_FALSE (CXString::IsSpace ("    .    "));
}


TEST(CXStringTest, TestIsDigit)
{
     ASSERT_TRUE (CXString::IsDigit ("1234567890"));
     ASSERT_FALSE (CXString::IsDigit ("0.1234567890"));
}

TEST(CXStringTest, TestIsNumber)
{
     ASSERT_TRUE (CXString::IsNumber ("1234567890"));
     ASSERT_TRUE (CXString::IsNumber ("1,234,567,890"));
     ASSERT_TRUE (CXString::IsNumber ("0.1234567890"));
     ASSERT_FALSE (CXString::IsNumber ("1,23,45"));
     
     // European
     ASSERT_TRUE (CXString::IsNumber ("1.234.567", '.', ','));
     ASSERT_TRUE (CXString::IsNumber ("1,234567", '.', ','));
}

TEST(CXStringTest, TestIsUpper)
{
     // TODO: I18N?
     ASSERT_TRUE (CXString::IsUpper ("ABCDEFGHIJKLMOPQRSTUVXYZ"));
     ASSERT_TRUE (CXString::IsUpper ("ABCD EFGH IJKLM OP Q RSTUV XYZ"));
     ASSERT_FALSE (CXString::IsUpper ("ABCDEFGHIJKLMOPQRSTUVXYZz"));
     ASSERT_FALSE (CXString::IsUpper ("aABCDEFGHIJKLMOPQRSTUVXYZ"));
}

TEST(CXStringTest, TestIsLower)
{
     // TODO: I18N?
     ASSERT_TRUE (CXString::IsLower ("abcdefghijklmopqrstuvxyz"));
     ASSERT_TRUE (CXString::IsLower ("abcd efgh ijklm op q rstuv xyz"));
     ASSERT_FALSE (CXString::IsLower ("abcdefghijklmopqrstuvxyzZ"));
     ASSERT_FALSE (CXString::IsLower ("Aabcdefghijklmopqrstuvxyz"));
}

TEST(CXStringTest, TestGetTime)
{
     GTEST_SKIP() << "GetTime() is buggy - fix me!";
     CXString sTime = "2023-02-27 17:51:45";
     ASSERT_TIME_EQ (2023, 2, 27, 17, 51, 45, sTime.GetTime());
}
