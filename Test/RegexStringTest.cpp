/*!
     \file     RegexStringTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include "../include/RegexString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TEST(CRegexStringTest, TestDefConstructor)
{
     CRegexString sRegex;
     
     ASSERT_FALSE (sRegex.IsRegex());
     ASSERT_FALSE (sRegex.IsIgnoreCase());
     ASSERT_FALSE (sRegex.IsMultiline());
     ASSERT_FALSE (sRegex.IsExtended());
     ASSERT_STREQ ("", (const TCHAR*) sRegex);

     ASSERT_EQ (0, sRegex.GetOptions());
}

TEST(CRegexStringTest, TestTCHARConstructor)
{
     CRegexString sRegex ("/this is a string/imxs");
     
     ASSERT_TRUE (sRegex.IsRegex());
     ASSERT_TRUE (sRegex.IsIgnoreCase());
     ASSERT_FALSE (sRegex.IsMultiline());
     ASSERT_TRUE (sRegex.IsExtended());
     ASSERT_STREQ ("this is a string", (const TCHAR*) sRegex);

     ASSERT_EQ (0x3, sRegex.GetOptions());
}
