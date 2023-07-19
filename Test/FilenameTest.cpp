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


TEST(FilenameTest, TestGetFileName)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     EXPECT_STREQ ("third.ext", sWindows.GetFileName());

     CFilename sRelative ("second\\third.ext");
     EXPECT_STREQ ("third.ext", sRelative.GetFileName());

     CFilename sLinux ("C:/first/second/third.ext");
     EXPECT_STREQ ("third.ext", sLinux.GetFileName());
}

TEST(FilenameTest, TestGetFolderName)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     EXPECT_STREQ ("C:\\first\\second", sWindows.GetFolderName());

     CFilename sRelative ("second\\third.ext");
     EXPECT_STREQ ("second", sRelative.GetFolderName());

     CFilename sLinux ("C:/first/second/third.ext");
     EXPECT_STREQ ("C:/first/second", sLinux.GetFolderName());

     CFilename sEmpty;
     EXPECT_STREQ ("", sEmpty.GetFolderName()); 

     CFilename sUsers ("C:\\Users");
     EXPECT_STREQ ("C:", sUsers.GetFolderName());

     CFilename sC ("C:");
     EXPECT_STREQ ("", sC.GetFolderName());
}

TEST(FilenameTest, TestGetFileExt)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     EXPECT_STREQ (".ext", sWindows.GetFileExt());

     CFilename sRelative ("second\\third.ext");
     EXPECT_STREQ (".ext", sRelative.GetFileExt());

     CFilename sLinux ("C:/first/second/third.ext");
     EXPECT_STREQ (".ext", sLinux.GetFileExt());
}

TEST(FilenameTest, TestGetBaseName)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     EXPECT_STREQ ("third", sWindows.GetBaseName());

     CFilename sRelative ("second\\third.ext");
     EXPECT_STREQ ("third", sRelative.GetBaseName());

     CFilename sLinux ("C:/first/second/third.ext");
     EXPECT_STREQ ("third", sLinux.GetBaseName());
}

TEST(FilenameTest, TestIsRelativePath)
{
     CFilename sWindows ("C:\\first\\second\\third.ext");
     EXPECT_FALSE (sWindows.IsRelativePath());

     CFilename sRelative ("second\\third.ext");
     EXPECT_TRUE (sRelative.IsRelativePath());

     CFilename sLinux ("C:/first/second/third.ext");
     EXPECT_FALSE (sLinux.IsRelativePath());
     
     EXPECT_FALSE (CFilename::IsRelativePath (nullptr));
}

TEST(FilenameTest, TestGetRelativePath)
{
     CString sCWD ("C:\\first\\second");
     CString sWindows ("C:\\first\\second\\third.ext");
     EXPECT_TRUE (CFilename::GetRelativePath (sWindows, sCWD));
     EXPECT_STREQ ("third.ext", sWindows);

     CString sRelative ("second\\third.ext");
     EXPECT_FALSE (CFilename::GetRelativePath (sRelative, sCWD));

     CString sLinux ("C:/first/second/third.ext");
     EXPECT_FALSE (CFilename::GetRelativePath (sLinux, sCWD));
}


TEST(FilenameTest, TestGetFullPath)
{
     EXPECT_STREQ ("C:\\first\\second\\file.ext", 
                   CFilename::GetFullPath ("c:/first//second", "file.ext"));

     EXPECT_STREQ ("C:\\first\\second",
                   CFilename::GetFullPath ("c:/first//second", ""));

     EXPECT_STREQ ("C:\\file.ext", 
                   CFilename::GetFullPath ("c:", "file.ext"));


     // nullptr should not cause problems
     EXPECT_STREQ ("", CFilename::GetFullPath (nullptr, nullptr));
     EXPECT_STREQ ("C:", CFilename::GetFullPath ("c:", nullptr));
     EXPECT_STREQ ("file.ext", CFilename::GetFullPath (nullptr, "file.ext"));
}


TEST(FilenameTest, TestCanonPath)
{
     EXPECT_STREQ ("C:\\first\\second\\file.ext", 
                   CFilename::CanonPath ("c:/first//second///file.ext"));

     EXPECT_STREQ ("C:\\dir1\\dir2\\file.ext", 
                   CFilename::CanonPath ("c:\\dir1\\\\dir2\\file.ext"));
     EXPECT_STREQ ("\\\\server\\share\\dir2\\file.ext", 
                   CFilename::CanonPath ("//server/share/dir1/../dir2/file.ext"));
     EXPECT_STREQ ("dir2\\file.ext", 
                   CFilename::CanonPath ("dir1/../dir2/file.ext"));
     EXPECT_STREQ ("reldir\\dir1\\file", 
                   CFilename::CanonPath ("./reldir/dir1/file"));
     EXPECT_STREQ (".\\", 
                   CFilename::CanonPath ("./"));

}



// These must match the TestWithParam tuples.
#define IDX_ORIG_PATH    0
#define IDX_NR_CHARS     1
#define IDX_EXP_PATH     2

class CFilenameTester : public ::testing::TestWithParam<std::tuple<
          /*0*/     const TCHAR*,  // Original path
          /*1*/     int,           // Number of characters.
          /*2*/     const TCHAR*   // Expected abbreviated path.
          >>      
{
public:
     CFilenameTester() {}; 
     virtual ~CFilenameTester() {};
};


#if 0
// TODO: AbbreviatePath() doesn't appear to be used in PrEditor.
static const TCHAR* pszLongPath = "c:/first/second/third/fourth/fifth/file.ext";

TEST_P(CFilenameTester, TestAbbreviatePath)
{
     CString sPath (std::get<IDX_ORIG_PATH>(GetParam()));
     int nChars = std::get<IDX_NR_CHARS>(GetParam());
     const TCHAR* pszExp = std::get<IDX_EXP_PATH>(GetParam());

     CFilename::AbbreviatePath (sPath.GetBuffer(), nChars);
     sPath.ReleaseBuffer();
     EXPECT_STREQ (pszExp, sPath);
}

INSTANTIATE_TEST_CASE_P(FilenameTest, CFilenameTester, 
                        ::testing::Values(
/*0*/     std::make_tuple(pszLongPath, 4, "file.ext"),
/*1*/     std::make_tuple(pszLongPath, 5, "file.ext"),
/*2*/     std::make_tuple(pszLongPath, 6, "file.ext"),
/*3*/     std::make_tuple(pszLongPath, 7, "file.ext"),
/*4*/     std::make_tuple(pszLongPath, 8, "file.ext"),
/*5*/     std::make_tuple(pszLongPath, 9, "file.ext"),
/*6*/     std::make_tuple(pszLongPath, 10, "file.ext"),
/*7*/     std::make_tuple(pszLongPath, 11, "file.ext"),
/*8*/     std::make_tuple(pszLongPath, 12, "file.ext"),
/*9*/     std::make_tuple(pszLongPath, 13, "file.ext"),
/*10*/    std::make_tuple(pszLongPath, 14, "file.ext"),
/*11*/    std::make_tuple(pszLongPath, 15, "file.ext"),
/*12*/    std::make_tuple(pszLongPath, 16, "file.ext"),
/*13*/    std::make_tuple(pszLongPath, 17, "file.ext"),
/*14*/    std::make_tuple(pszLongPath, 18, "file.ext"),
/*15*/    std::make_tuple(pszLongPath, 19, "file.ext"),
/*16*/    std::make_tuple(pszLongPath, 20, "file.ext"),
/*17*/    std::make_tuple(pszLongPath, 21, "file.ext"),
/*18*/    std::make_tuple(pszLongPath, 22, "file.ext"),
/*19*/    std::make_tuple(pszLongPath, 20, "file.ext")
));

#endif