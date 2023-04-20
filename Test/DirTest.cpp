/*!
     \file     DirTest.cpp
     \author   John Young
     \date     17 April 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../include/Directory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void TestSetCurrDir (const TCHAR* pszTest, const TCHAR* pszExpDir, bool bExpResult)
{
     SCOPED_TRACE("TestSetCurrDir");

     TCHAR szOrig [_MAX_PATH];
     ::GetCurrentDirectory (_MAX_PATH - 1, szOrig);

     if ( bExpResult )
          EXPECT_TRUE (CDirectory::SetCurrentDir (pszTest));
     else EXPECT_FALSE (CDirectory::SetCurrentDir (pszTest));
     
     if ( bExpResult )
     {
          TCHAR szDir [_MAX_PATH];
          ::GetCurrentDirectory (_MAX_PATH - 1, szDir);
     
          EXPECT_STREQ (pszExpDir, szDir);
     }
     ::SetCurrentDirectory (szOrig);
}

void TestSetCurrDir (const TCHAR* pszTest, bool bExpResult)
{
     TestSetCurrDir (pszTest, pszTest, bExpResult);
}

void TestSetCurrDir (const TCHAR* pszTest, const TCHAR* pszExpDir)
{
     TestSetCurrDir (pszTest, pszExpDir, true);
}


TEST(DirTest, TestSetCurrDir)
{
     // Absolute paths
     TestSetCurrDir ("C:\\Program Files", true);
     TestSetCurrDir ("C:\\Blah", false);
     TestSetCurrDir ("C:\\Program Files\\Internet Explorer\\iexplore.exe", 
                     "C:\\Program Files\\Internet Explorer");
     TestSetCurrDir ("C:\\Program Files\\Internet Explorer\\blah", false); 

     // Relative paths (we are in the 'src' directory)
//     TestSetCurrDir ("../include", true);
}
