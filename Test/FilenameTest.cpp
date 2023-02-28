/*!
     \file     FilenameTest.cpp
     \author   John Young
     \date     28 February 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include "../include/Filename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TEST(CFilenameTest, TestGetFileName)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     ASSERT_STREQ ("third.ext", sWindows.GetFileName());

     CFilename sRelative ("second\\third.ext");
     ASSERT_STREQ ("third.ext", sRelative.GetFileName());

     CFilename sLinux ("C:/first/second/third.ext");
     ASSERT_STREQ ("third.ext", sLinux.GetFileName());
}

TEST(CFilenameTest, TestGetFolderName)
{
     // TODO: Update GetFolderName() to remove the trailing slash.
     CFilename sWindows ("C:\\first\\second\\third.ext");
     ASSERT_STREQ ("C:\\first\\second\\", sWindows.GetFolderName());

     CFilename sRelative ("second\\third.ext");
     ASSERT_STREQ ("second\\", sRelative.GetFolderName());

     CFilename sLinux ("C:/first/second/third.ext");
     ASSERT_STREQ ("C:/first/second/", sLinux.GetFolderName());
}

TEST(CFilenameTest, TestGetFileExt)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     ASSERT_STREQ (".ext", sWindows.GetFileExt());

     CFilename sRelative ("second\\third.ext");
     ASSERT_STREQ (".ext", sRelative.GetFileExt());

     CFilename sLinux ("C:/first/second/third.ext");
     ASSERT_STREQ (".ext", sLinux.GetFileExt());
}

TEST(CFilenameTest, TestGetBaseName)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     ASSERT_STREQ ("third", sWindows.GetBaseName());

     CFilename sRelative ("second\\third.ext");
     ASSERT_STREQ ("third", sRelative.GetBaseName());

     CFilename sLinux ("C:/first/second/third.ext");
     ASSERT_STREQ ("third", sLinux.GetBaseName());
}

TEST(CFilenameTest, TestIsRelativePath)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     ASSERT_FALSE (sWindows.IsRelativePath());

     CFilename sRelative ("second\\third.ext");
     ASSERT_TRUE (sRelative.IsRelativePath());

     CFilename sLinux ("C:/first/second/third.ext");
     ASSERT_FALSE (sLinux.IsRelativePath());
}

TEST(CFilenameTest, TestGetRelativePath)
{
     CString sCWD ("C:\\first\\second");
     CString sWindows ("C:\\first\\second\\third.ext");
     ASSERT_TRUE (CFilename::GetRelativePath (sWindows, sCWD));
     ASSERT_STREQ ("third.ext", sWindows);

     CString sRelative ("second\\third.ext");
     ASSERT_FALSE (CFilename::GetRelativePath (sRelative, sCWD));

     CString sLinux ("C:/first/second/third.ext");
     ASSERT_FALSE (CFilename::GetRelativePath (sLinux, sCWD));
}
