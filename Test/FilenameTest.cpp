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


CString TestAbbreviatePath (const TCHAR* pszPath, int nChars, bool bAtLeastName)
{
     CString sTest = pszPath;
     CFilename::AbbreviatePath (sTest.GetBuffer(MAX_PATH), nChars, bAtLeastName);
     sTest.ReleaseBuffer();
     return sTest;
}

TEST(FilenameTest, TestAbbreviatePath)
{
// C:\MYAPP\DEBUGS\C\TESWIN.C
//
//     nChars   b   Result
//     ------   -   ---------
//      1- 7    F   \<empty\>
//      1- 7    T   TESWIN.C
//      8-14    x   TESWIN.C
//     15-16    x   C:\...\TESWIN.C
//     17-23    x   C:\...\C\TESWIN.C
//     24-25    x   C:\...\DEBUGS\C\TESWIN.C
//     26+      x   C:\MYAPP\DEBUGS\C\TESWIN.C

     const TCHAR* pszLocalPath = "C:\\MYAPP\\DEBUGS\\C\\TESWIN.C";
     EXPECT_STREQ ("", TestAbbreviatePath (pszLocalPath, 1, false));
     EXPECT_STREQ ("", TestAbbreviatePath (pszLocalPath, 6, false));
     EXPECT_STREQ ("", TestAbbreviatePath (pszLocalPath, 7, false));

     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszLocalPath, 1, true));
     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszLocalPath, 7, true));
     
     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszLocalPath, 8, false));
     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszLocalPath, 14, false));
     
     EXPECT_STREQ ("C:\\...\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 15, false));
     EXPECT_STREQ ("C:\\...\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 16, false));
     
     EXPECT_STREQ ("C:\\...\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 17, false));
     EXPECT_STREQ ("C:\\...\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 23, false));
     EXPECT_STREQ ("C:\\...\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 24, false));
     EXPECT_STREQ ("C:\\...\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 25, false));
     EXPECT_STREQ ("C:\\MYAPP\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 26, false));
     EXPECT_STREQ ("C:\\MYAPP\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 27, false));
     EXPECT_STREQ ("C:\\MYAPP\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszLocalPath, 100, false));

// \\UNC\SHARE\MYAPP\DEBUGS\C\TESWIN.C
//
//     nChars   b   Result
//     ------   -   ---------
//      1 - 7   F   \<empty\>
//      1 - 7   T   TESWIN.C
//      8 - 23  x   TESWIN.C
//     24 - 25  x   \\UNC\SHARE\...\TESWIN.C      
//     26 - 32  x   \\UNC\SHARE\...\C\TESWIN.C
//     33 - 34  x   \\UNC\SHARE\...\DEBUGS\C\TESWIN.C
//     35+      x   \\UNC\SHARE\MYAPP\DEBUGS\C\TESWIN.C

     const TCHAR* pszUNCPath = "\\\\UNC\\SHARE\\MYAPP\\DEBUGS\\C\\TESWIN.C";
     EXPECT_STREQ ("", TestAbbreviatePath (pszUNCPath, 1, false));
     EXPECT_STREQ ("", TestAbbreviatePath (pszUNCPath, 7, false));

     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszUNCPath, 1, true));
     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszUNCPath, 7, true));
     
     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszUNCPath, 8, false));
     EXPECT_STREQ ("TESWIN.C", TestAbbreviatePath (pszUNCPath, 23, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\...\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 24, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\...\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 25, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\...\\C\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 26, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\...\\C\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 32, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\...\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 33, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\...\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 34, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\MYAPP\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 35, false));
     EXPECT_STREQ ("\\\\UNC\\SHARE\\MYAPP\\DEBUGS\\C\\TESWIN.C", TestAbbreviatePath (pszUNCPath, 100, false));
}


bool FileExists (const TCHAR* pszPath)
{
     static const TCHAR* pPath1 = "C:\\Path1\\OnPath1.exe";
     static const TCHAR* pPath2 = "C:\\Path2\\OnPath2.cmd";
     
     // Checks that if a file with an invalid extension exists, it is not chosen.
     static const TCHAR* pPath1xxx = "C:\\Path1\\OnPath1.xxx";
     static const TCHAR* pPath2xxx = "C:\\Path2\\OnPath2.xxx";
     
     static const TCHAR* pCurrentDir = "CurrentDir.bat";
     static const TCHAR* pCurrentDirxxx = "CurrentDir.xxx";

     return strcmp (pszPath, pPath1) == 0 ||
            strcmp (pszPath, pPath1xxx) == 0 ||
            strcmp (pszPath, pPath2) == 0 ||
            strcmp (pszPath, pPath2xxx) == 0 ||
            strcmp (pszPath, pCurrentDir) == 0 ||
            strcmp (pszPath, pCurrentDirxxx) == 0;
}

CString GetEnvVar (const TCHAR* pszEnv)
{
     return "C:\\Path1;C:\\Path2";
}

TEST(FilenameTest, TestGetCmdPathName)
{
     // No extension
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("NotOnPath", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("C:\\Path1\\OnPath1.exe", CFilename::GetCmdPathName ("OnPath1", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("C:\\Path2\\OnPath2.cmd", CFilename::GetCmdPathName ("OnPath2", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("CurrentDir.bat", CFilename::GetCmdPathName ("CurrentDir", &FileExists, &GetEnvVar));

     // .exe
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("NotOnPath.exe", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("C:\\Path1\\OnPath1.exe", CFilename::GetCmdPathName ("OnPath1.exe", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath2.exe", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("CurrentDir.exe", &FileExists, &GetEnvVar));

     // .com
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("NotOnPath.com", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath1.com", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath2.com", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("CurrentDir.com", &FileExists, &GetEnvVar));

     // .cmd
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("NotOnPath.cmd", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath1.cmd", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("C:\\Path2\\OnPath2.cmd", CFilename::GetCmdPathName ("OnPath2.cmd", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("CurrentDir.cmd", &FileExists, &GetEnvVar));

     // .bat
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("NotOnPath.bat", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath1.bat", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath2.bat", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("CurrentDir.bat", CFilename::GetCmdPathName ("CurrentDir.bat", &FileExists, &GetEnvVar));

     // .xxx
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("NotOnPath.xxx", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath1.xxx", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("OnPath2.xxx", &FileExists, &GetEnvVar));
     EXPECT_STREQ ("", CFilename::GetCmdPathName ("CurrentDir.xxx", &FileExists, &GetEnvVar));
}
