/*!
     \file     RegexTest.cpp
     \author   John Young
     \date     28 February 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include "../include/Regex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static char TEXT[] = "This is some text to find\n"
                     "over several lines\n"
                     "   Indented text\n";

TEST(CRegexTest, TestMatch)
{
     // TODO: The index of 0 should return the original text?
     CRegex sRegex;
     ASSERT_FALSE (sRegex.Match ("tO fIND", TEXT));
     ASSERT_TRUE (sRegex.Match ("to find", TEXT));
     ASSERT_STREQ ("to find", sRegex [0]);
     
     ASSERT_TRUE (sRegex.Match ("t*", "  TttTTTT "));
     ASSERT_STREQ ("", sRegex [0]);

     ASSERT_TRUE (sRegex.Match ("t+", "  TttTTTT "));
     ASSERT_STREQ ("tt", sRegex [0]);

     ASSERT_TRUE (sRegex.Match ("T*", "  TttTTTT "));
     ASSERT_STREQ ("", sRegex [0]);
     ASSERT_STREQ ("", sRegex [1]);
}

TEST(CRegexTest, TestMatchIgnoreCase)
{
     // TODO: The index of 0 should return the original text?
     CRegex sIgnoreCase (PCRE_CASELESS);
     ASSERT_TRUE (sIgnoreCase.Match ("tO fIND", TEXT));
     ASSERT_STREQ ("to find", sIgnoreCase [0]);

     ASSERT_TRUE (sIgnoreCase.Match ("t*", "  TttTTTT "));
     ASSERT_STREQ ("", sIgnoreCase [0]);

     ASSERT_TRUE (sIgnoreCase.Match ("t+", "  TttTTTT "));
     ASSERT_STREQ ("TttTTTT", sIgnoreCase [0]);
}
