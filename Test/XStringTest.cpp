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
     EXPECT_EQ (14, sTest.Chop());       
     EXPECT_STREQ ("This is a test", sTest);

     CXString sNoChange = "This is a \ntest";
     EXPECT_EQ (15, sNoChange.Chop());
     EXPECT_STREQ ("This is a \ntest", sNoChange);
}

TEST(CXStringTest, TestChomp)
{
     CXString sTest = "This is a test\n\r\n\n\n";
     EXPECT_EQ (14, sTest.Chomp());       
     EXPECT_STREQ ("This is a test", sTest);

     CXString sNoChange = "This is a \ntest";
     EXPECT_EQ (15, sNoChange.Chomp());
     EXPECT_STREQ ("This is a \ntest", sNoChange);
}

TEST(CXStringTest, TestChopToLength)
{
     CXString sElipsis = "This is a test string";
     EXPECT_EQ (17, sElipsis.Chop (17, true));
     EXPECT_STREQ ("This is a test...", sElipsis);

     CXString sTest = "This is a test string";
     EXPECT_EQ (17, sTest.Chop (17, false));
     EXPECT_STREQ ("This is a test st", sTest);
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
     EXPECT_EQ (17, sTest.Chop (17, true));
     EXPECT_STREQ ("This is a test...", sTest);

     CXString sTest = "This a test string";
     EXPECT_EQ (17, sTest.Chop (17, false));
     EXPECT_STREQ ("This is a test st", sTest);
}
#endif


TEST(CXStringTest, TestIsString)
{
     EXPECT_TRUE (CXString::IsSpace (" \t   \r\n    "));
     EXPECT_FALSE (CXString::IsSpace ("    .    "));
}


TEST(CXStringTest, TestIsDigit)
{
     EXPECT_FALSE (CXString::IsDigit (nullptr));
     EXPECT_FALSE (CXString::IsDigit (""));
     EXPECT_TRUE (CXString::IsDigit ("1234567890"));
     EXPECT_FALSE (CXString::IsDigit ("0.1234567890"));
}

TEST(CXStringTest, TestIsNumber)
{
     EXPECT_FALSE (CXString::IsNumber (nullptr));
     EXPECT_FALSE (CXString::IsNumber (""));
     
     EXPECT_TRUE (CXString::IsNumber ("1234567890"));
     EXPECT_TRUE (CXString::IsNumber ("1,234,567,890"));
     EXPECT_TRUE (CXString::IsNumber ("0.1234567890"));
     EXPECT_FALSE (CXString::IsNumber ("1,23,45"));
     
     // European
     EXPECT_TRUE (CXString::IsNumber ("1.234.567", '.', ','));
     EXPECT_TRUE (CXString::IsNumber ("1,234567", '.', ','));
}

TEST(CXStringTest, TestIsUpper)
{
     // TODO: I18N?
     EXPECT_TRUE (CXString::IsUpper ("ABCDEFGHIJKLMOPQRSTUVXYZ"));
     EXPECT_TRUE (CXString::IsUpper ("ABCD EFGH IJKLM OP Q RSTUV XYZ"));
     EXPECT_FALSE (CXString::IsUpper ("ABCDEFGHIJKLMOPQRSTUVXYZz"));
     EXPECT_FALSE (CXString::IsUpper ("aABCDEFGHIJKLMOPQRSTUVXYZ"));
}

TEST(CXStringTest, TestIsLower)
{
     // TODO: I18N?
     EXPECT_TRUE (CXString::IsLower ("abcdefghijklmopqrstuvxyz"));
     EXPECT_TRUE (CXString::IsLower ("abcd efgh ijklm op q rstuv xyz"));
     EXPECT_FALSE (CXString::IsLower ("abcdefghijklmopqrstuvxyzZ"));
     EXPECT_FALSE (CXString::IsLower ("Aabcdefghijklmopqrstuvxyz"));
}

TEST(CXStringTest, TestIsCapitalised)
{
     EXPECT_FALSE (CXString::IsCapitalised (nullptr));
     EXPECT_FALSE (CXString::IsCapitalised (""));
     EXPECT_TRUE (CXString::IsCapitalised ("Capital"));
     EXPECT_FALSE (CXString::IsCapitalised ("capital"));
     EXPECT_TRUE (CXString::IsCapitalised ("Capital City In Japan"));
     EXPECT_FALSE (CXString::IsCapitalised ("Capital City in Japan"));
     EXPECT_FALSE (CXString::IsCapitalised ("Capital City IN Japan"));
     EXPECT_TRUE (CXString::IsCapitalised ("    Capital City In Japan"));
}

TEST(CXStringTest, TestGetTime)
{
     GTEST_SKIP() << "GetTime() is buggy - fix me!";
     CXString sTime = "2023-02-27 17:51:45";
     EXPECT_TIME_EQ (2023, 2, 27, 17, 51, 45, sTime.GetTime());
}

void TestCommatise (const TCHAR* pszExp, const TCHAR* pszSource)
{
     CXString sTest = pszSource;
     sTest.Commatise();
     EXPECT_STREQ (pszExp, sTest);
}

TEST(CXStringTest, TestCommatise)
{
     TestCommatise ("1", "1");
     TestCommatise ("12", "12");
     TestCommatise ("123", "123");
     TestCommatise ("1,234", "1234");
     TestCommatise ("12,345", "12345");
     TestCommatise ("123,456", "123456");
     TestCommatise ("1,234,567", "1234567");
     TestCommatise ("12,345,678", "12345678");
     TestCommatise ("123,456,789", "123456789");
}

void TestDecommatise (const TCHAR* pszExp, const TCHAR* pszSource)
{
     CXString sTest = pszSource;
     sTest.Decommatise();
     EXPECT_STREQ (pszExp, sTest);
}

TEST(CXStringTest, TestDecommatise)
{
     TestDecommatise ("1", "1");
     TestDecommatise ("12", "12");
     TestDecommatise ("123", "123");
     TestDecommatise ("1234", "1,234");
     TestDecommatise ("12345", "12,345");
     TestDecommatise ("123456", "123,456");
     TestDecommatise ("1234567", "1,234,567");
     TestDecommatise ("12345678", "12,345,678");
     TestDecommatise ("123456789", "123,456,789");
}

void TestCapitalise (const TCHAR* pszExp, const TCHAR* pszSource)
{
     CXString sTest = pszSource;
     sTest.Capitalise();
     EXPECT_STREQ (pszExp, sTest);
}

TEST(CXStringTest, TestCapitalise)
{
     TestCapitalise ("Capital", "capital");
     TestCapitalise ("Capital City", "capital city");
}

void TestToggleCase (const TCHAR* pszExp, const TCHAR* pszSource)
{
     CXString sTest = pszSource;
     sTest.ToggleCase();
     EXPECT_STREQ (pszExp, sTest);
}

TEST(CXStringTest, TestToggleCase)
{
     TestToggleCase ("CAPITAL", "capital");
     TestToggleCase ("cAPITAL cITY", "Capital City");
}


TEST(CXStringTest, TestLimitPath)
{
     GTEST_SKIP() << "LimitPath() is buggy - fix me!";
     CXString sTest;
     sTest = "C:\\first\\second\\third\\fourth\\fifth\\filename.ext";

     EXPECT_EQ (19, sTest.LimitPath(25));
     EXPECT_STREQ ("C:\\...\\filename.ext", sTest);

     EXPECT_EQ (19, sTest.LimitPath(55));
     EXPECT_STREQ ("C:\\...\\fifth\\filename.ext", sTest);
}

TEST(CXStringTest, TestFindReplace)
{
     CXString sTest;
     sTest = "This is a test";

     EXPECT_TRUE (sTest.FindReplace ("test", "thing"));
     EXPECT_STREQ ("This is a thing", sTest);

     sTest = "This is a test";
     EXPECT_FALSE (sTest.FindReplace ("tttt", "thing"));
     EXPECT_STREQ ("This is a test", sTest);
}

TEST(CXStringTest, TestGetMaxLineLength)
{
     CXString sTest;
     sTest = "This is a test\n"
             "This is another longer line\n"
             "And this the longest line of them all (and without an EOL marker)";

     EXPECT_EQ (65, sTest.GetMaxLineLength());
}

TEST(CXStringTest, TestExpandMacroStr)
{
     GTEST_SKIP() << "ExpandMacroStr() is buggy - fix me!";

     CXString sTest;
     sTest = "A string containing a %MACRO% and another %NON-EXISTANT-ONE%";

     CMapStringToString macros;
     macros.SetAt ("MACRO", "replaced macro");
     
     sTest.ExpandMacroStr (macros, "%");
     EXPECT_STREQ ("A string containing a replaced macro and another %NON-EXISTANT-ONE%",
                   sTest);
}

TEST(CXStringTest, TestHexStr)
{
     CXString sTest = "A string";
     EXPECT_STREQ ("4120737472696e67", sTest.HexStr());
}

TEST(CXStringTest, TestFromHex)
{
     CXString sTest = "4120737472696e67";
     EXPECT_STREQ ("A string", sTest.FromHex());
}

TEST(CXStringTest, TestReverseFindString)
{
     CXString sTest = "Find the last the the in this string";
     EXPECT_EQ (18, sTest.ReverseFind ("the"));
     EXPECT_EQ (33, sTest.ReverseFind ("ing"));
     EXPECT_EQ (0, sTest.ReverseFind ("Find"));
     EXPECT_EQ (-1, sTest.ReverseFind ("x"));
     EXPECT_EQ (0, sTest.ReverseFind ("Find the last the the in this string"));
}

TEST(CXStringTest, TestReverseFindChar)
{
     CXString sTest = "Find the last the the in this string";
     EXPECT_EQ (35, sTest.ReverseFind ('g'));
     EXPECT_EQ (30, sTest.ReverseFind ('s'));
     EXPECT_EQ (20, sTest.ReverseFind ('e'));
     EXPECT_EQ (0, sTest.ReverseFind ('F'));
     EXPECT_EQ (-1, sTest.ReverseFind ('x'));
}
