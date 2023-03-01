/*!
     \file     RegexKeyTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../include/RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MFCX::CRegKey;


static char TEST_BASE[] = "Software\\Thistle Software\\MFCX-UnitTests";

class CRegKeyTest : public ::testing::Test
{
protected:
     void SetUp() override
     {
          std::cout << "Initialising the registry for the CRegKey test\n";
          system ("..\\test\\resources\\seed-reg.bat");
     }
};

TEST_F(CRegKeyTest, TestRead)
{
     MFCX::CRegKey rkRegex (HKEY_CURRENT_USER, TEST_BASE);
     
     ASSERT_TRUE (rkRegex.IsOpen());
}
