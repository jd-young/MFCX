/*!
     \file     RegKeyTest.cpp
     \author   John Young
     \date     1 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "TestUtils.h"
#include "../include/RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MFCX::CRegKey;


static char TEST_BASE[] = "Software\\Thistle Software\\MFCX-UnitTests";

const TCHAR* GetErrMsg (int nr)
{
     switch (nr)
     {
          case E2BIG:    return "E2BIG";
          case ENOENT:   return "ENOENT";
          case ENOEXEC:  return "ENOEXEC";
          case ENOMEM:   return "ENOMEM";
          default:
               return "Unknown";
     }
}


class CRegKeyTest : public ::testing::Test
{
protected:
     void SetUp() override
     {
          std::cout << "Initialising the registry for the CRegKey test\n";
          int ret_code = system ("..\\test\\resources\\seed-reg.bat");
          if ( ret_code == 0 )
          {
               // Wait a maximum of 2 secs until the reg key is open.
               MFCX::CRegKey rkRegex (HKEY_CURRENT_USER, TEST_BASE);
               bool bOk = CTestUtils::Await (2000, [&]() {
                                   return rkRegex.IsOpen();
                              });
               if ( bOk )
                    std::cout << "Registry initialised successfully\n";
               else std::cout << "Registry not initialised in 2 seconds\n";
          }
          else
          {
               std::cout << "Couldn't seed the registry with test values: error code"
                      << GetErrMsg (errno);
          }
     }
};

TEST_F(CRegKeyTest, TestRead)
{
     MFCX::CRegKey rkRegex (HKEY_CURRENT_USER, TEST_BASE);
     ASSERT_TRUE (rkRegex.IsOpen());
}
