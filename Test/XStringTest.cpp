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
     ASSERT_FALSE (CXString::IsDigit (nullptr));
     ASSERT_FALSE (CXString::IsDigit (""));
     ASSERT_TRUE (CXString::IsDigit ("1234567890"));
     ASSERT_FALSE (CXString::IsDigit ("0.1234567890"));
}

TEST(CXStringTest, TestIsNumber)
{
     ASSERT_FALSE (CXString::IsNumber (nullptr));
     ASSERT_FALSE (CXString::IsNumber (""));
     
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

TEST(CXStringTest, TestIsCapitalised)
{
     GTEST_SKIP() << "nullptr and separated words don't work - fix me!";
     ASSERT_FALSE (CXString::IsCapitalised (nullptr));
     ASSERT_FALSE (CXString::IsCapitalised (""));
     ASSERT_TRUE (CXString::IsCapitalised ("Capital"));
     ASSERT_FALSE (CXString::IsCapitalised ("capital"));
     ASSERT_TRUE (CXString::IsCapitalised ("Capital City In Japan"));
     ASSERT_FALSE (CXString::IsCapitalised ("Capital City in Japan"));
}

TEST(CXStringTest, TestGetTime)
{
     GTEST_SKIP() << "GetTime() is buggy - fix me!";
     CXString sTime = "2023-02-27 17:51:45";
     ASSERT_TIME_EQ (2023, 2, 27, 17, 51, 45, sTime.GetTime());
}

void TestCommatise (const TCHAR* pszExp, const TCHAR* pszSource)
{
     CXString sTest = pszSource;
     sTest.Commatise();
     ASSERT_STREQ (pszExp, sTest);
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
     ASSERT_STREQ (pszExp, sTest);
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
     ASSERT_STREQ (pszExp, sTest);
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
     ASSERT_STREQ (pszExp, sTest);
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

     ASSERT_EQ (19, sTest.LimitPath(25));
     ASSERT_STREQ ("C:\\...\\filename.ext", sTest);

     ASSERT_EQ (19, sTest.LimitPath(55));
     ASSERT_STREQ ("C:\\...\\fifth\\filename.ext", sTest);
}

TEST(CXStringTest, TestFindReplace)
{
     CXString sTest;
     sTest = "This is a test";

     ASSERT_TRUE (sTest.FindReplace ("test", "thing"));
     ASSERT_STREQ ("This is a thing", sTest);

     sTest = "This is a test";
     ASSERT_FALSE (sTest.FindReplace ("tttt", "thing"));
     ASSERT_STREQ ("This is a test", sTest);
}

TEST(CXStringTest, TestGetMaxLineLength)
{
//     GTEST_SKIP() << "GetMaxLineLength() is buggy - fix me!";
     CXString sTest;
     sTest = "This is a test\n"
             "This is another longer line\n"
             "And this the longest line of them all (and without an EOL marker)";

     ASSERT_EQ (65, sTest.GetMaxLineLength());
}

TEST(CXStringTest, TestExpandMacroStr)
{
     GTEST_SKIP() << "ExpandMacroStr() is buggy - fix me!";

     CXString sTest;
     sTest = "A string containing a %MACRO% and another %NON-EXISTANT-ONE%";

     CMapStringToString macros;
     macros.SetAt ("MACRO", "replaced macro");
     
     sTest.ExpandMacroStr (macros, "%");
     ASSERT_STREQ ("A string containing a replaced macro and another %NON-EXISTANT-ONE%",
                   sTest);
}

TEST(CXStringTest, TestHexStr)
{
     CXString sTest = "A string";
     ASSERT_STREQ ("4120737472696e67", sTest.HexStr());
}

TEST(CXStringTest, TestFromHex)
{
     CXString sTest = "4120737472696e67";
     ASSERT_STREQ ("A string", sTest.FromHex());
}

TEST(CXStringTest, TestReverseFind)
{
     GTEST_SKIP() << "ReverseFind() is buggy - fix me!";

     CXString sTest = "Find the last the the in this string";
     ASSERT_EQ (18, sTest.ReverseFind ("the"));
}
