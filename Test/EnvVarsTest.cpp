/*!
     \file     EnvVarsTest.cpp
     \author   John Young
     \date     20 March 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include "gtest/gtest.h"
#include "../include/EnvVars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using std::map;
using std::string;

using MFCX::CEnvVars;

TEST(EnvVarsTest, GetEnvBlockTest)
{
     map<string, string> init;
     init ["ONE"] = "ONEONE";
     init ["TWO"] = "TWOTWO";
     init ["THREE"] = "THREETHREE";
     init ["FOUR"] = "FOURFOUR";
     init ["FIVE"] = "FIVEFIVE";
     init ["SIX"] = "SIXSIX";
     init ["SEVEN"] = "SEVENSEVEN";

     CEnvVars env (init);
//     env.SetEnvs (init);

     TCHAR* pEnv = env.GetEnvBlock();
     
     TCHAR* p = pEnv;
     EXPECT_STREQ ("FIVE=FIVEFIVE", p);
     
     p += 14;
     EXPECT_STREQ ("FOUR=FOURFOUR", p);
     
     p += 14;
     EXPECT_STREQ ("ONE=ONEONE", p);
     
     p += 11;
     EXPECT_STREQ ("SEVEN=SEVENSEVEN", p);
     
     p += 17;
     EXPECT_STREQ ("SIX=SIXSIX", p);
     
     p += 11;
     EXPECT_STREQ ("THREE=THREETHREE", p);
     
     p += 17;
     EXPECT_STREQ ("TWO=TWOTWO", p);
     
     p += 11;
     EXPECT_EQ ('\0', *p);
     
     delete pEnv;
}

/// A class to access protected methods of CEnvVars for testing.
class CTestEnvVars : public CEnvVars
{
public: 
	CTestEnvVars (const map<string, string>& envs) : CEnvVars (envs) {};

     TCHAR* TestMergeEnvBlock (TCHAR* pOther) const
     { return MergeEnvBlock (pOther); }
     
     TCHAR* TestGetEnvBlock (const map<string, string>& other) const
     { return GetEnvBlock (other); }
};

TEST(EnvVarsTest, GetEnvBlockWithOtherTest)
{
     map<string, string> init;
     init ["ONE"] = "ONEONE";           // 3 + 1 +  6 + 1 (\0) = 11
     init ["TWO"] = "TWOTWO";           // 3 + 1 +  6 + 1 (\0) = 11
     init ["THREE"] = "THREETHREE";     // 5 + 1 + 10 + 1 (\0) = 17
     init ["FOUR"] = "FOURFOUR";        // 4 + 1 +  8 + 1 (\0) = 14
     init ["FIVE"] = "FIVEFIVE";        // 4 + 1 +  8 + 1 (\0) = 14
     init ["SIX"] = "SIXSIX";           // 3 + 1 +  6 + 1 (\0) = 11
     init ["SEVEN"] = "SEVENSEVEN";     // 5 + 1 + 10 + 1 (\0) = 17
     init ["PATH"] = "C:\\MyPath;%PATH%";//4 + 1 + 16 + 1 (\0) = 22
                                        //                Total 106
     CTestEnvVars env (init);

     map<string, string> other;
     other ["ALPHA"] = "ALPHAALPHA";    // 5 + 1 + 10 + 1 (\0) = 17
     other ["BETA"] = "BETABETA";       // 4 + 1 +  8 + 1 (\0) = 14
     other ["GAMMA"] = "GAMMAGAMMA";    // 5 + 1 + 10 + 1 (\0) = 17
     other ["Two"] = "OVERWRITTEN";     // 3 + 1 + 11 + 1 (\0) = 16 (shouldn't be included)
     other ["Path"] = "C:\\Apps";       // 4 + 1 +  7 + 1 (\0) = 13 (should be added to 'PATH')
                                        //                Total  64
     // Total should be 84 + 64 = 148

     TCHAR* pEnv = env.TestGetEnvBlock (other);
     
     TCHAR* p = pEnv;
     EXPECT_STREQ ("ALPHA=ALPHAALPHA", p);
     
     p += 17;
     EXPECT_STREQ ("BETA=BETABETA", p);
     
     p += 14;
     EXPECT_STREQ ("FIVE=FIVEFIVE", p);
     
     p += 14;
     EXPECT_STREQ ("FOUR=FOURFOUR", p);
     
     p += 14;
     EXPECT_STREQ ("GAMMA=GAMMAGAMMA", p);
     
     p += 17;
     EXPECT_STREQ ("ONE=ONEONE", p);
     
     p += 11;
     EXPECT_STREQ ("Path=C:\\MyPath;C:\\Apps", p);
     
     p += 23;
     EXPECT_STREQ ("SEVEN=SEVENSEVEN", p);
     
     p += 17;
     EXPECT_STREQ ("SIX=SIXSIX", p);
     
     p += 11;
     EXPECT_STREQ ("THREE=THREETHREE", p);
     
     p += 17;
     EXPECT_STREQ ("Two=TWOTWO", p);
     
     p += 11;
     EXPECT_EQ ('\0', *p);
     
     delete pEnv;
}

TEST(EnvVarsTest, MergeEnvBlockTest)
{
     // The tools' environment variables.
     map<string, string> init;
     init ["ONE"] = "ONEONE";
     init ["TWO"] = "TWOTWO";
     init ["THREE"] = "THREETHREE";
     init ["FOUR"] = "FOURFOUR";
     init ["FIVE"] = "FIVEFIVE";
     init ["SIX"] = "SIXSIX";
     init ["SEVEN"] = "SEVENSEVEN";
     init ["PATH"] = "c:\\Program Files\\MS\\Tools;%PATH%";

     CTestEnvVars env (init);

     // The process's environment variables.
     TCHAR envBlock[] = 
          "ALPHA=ALPHAALPHA\0"
          "BETA=BETABETA\0"
          "GAMMA=GAMMAGAMMA\0"
          "ONE=OVERWRITE\0"
          "Path=C:\\Path\\to\\utils\0\0";

     TCHAR* pEnvBlock = env.TestMergeEnvBlock (envBlock);

     TCHAR* p = pEnvBlock;
     EXPECT_STREQ ("ALPHA=ALPHAALPHA", p);
     
     p += 17;
     EXPECT_STREQ ("BETA=BETABETA", p);
     
     p += 14;
     EXPECT_STREQ ("FIVE=FIVEFIVE", p);
     
     p += 14;
     EXPECT_STREQ ("FOUR=FOURFOUR", p);
     
     p += 14;
     EXPECT_STREQ ("GAMMA=GAMMAGAMMA", p);
     
     p += 17;
     EXPECT_STREQ ("ONE=ONEONE", p);
     
     p += 11;
     EXPECT_STREQ ("Path=c:\\Program Files\\MS\\Tools;C:\\Path\\to\\utils", p);
     
     p += 48;
     EXPECT_STREQ ("SEVEN=SEVENSEVEN", p);
     
     p += 17;
     EXPECT_STREQ ("SIX=SIXSIX", p);
     
     p += 11;
     EXPECT_STREQ ("THREE=THREETHREE", p);
     
     p += 17;
     EXPECT_STREQ ("TWO=TWOTWO", p);
     
     p += 11;
     EXPECT_EQ ('\0', *p);
     
     delete pEnvBlock;
}
