/*!
     \file     DataQueueTest.cpp
     \author   John Young
     \date     14 November 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "MockMsgPoster.h"
#include "../include/DataQueue.h"
#include "../include/StringUtil.h"
#include "../include/XTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using std::string;
using std::vector;

using ::testing::MatchesRegex;

using MFCX::CDataQueue;
using MFCX::CStopWatch;
using MFCX::CStringUtil;

static HWND HWND_TEST = (HWND) -1;
static UINT MSG_ID = WM_APP;

TEST(DataQueueTest, TestDataQueue)
{
     CDataQueue queue;
     queue.SetTargetWnd (HWND_TEST, MSG_ID);

     EXPECT_TRUE (queue.IsEmpty());
     
     queue.Add ("This is the first message");
     queue.Add ("This is the second message");
     EXPECT_FALSE (queue.IsEmpty());

     EXPECT_STREQ ("This is the first message", queue.Remove());
     EXPECT_STREQ ("This is the second message", queue.Remove());
     
     EXPECT_TRUE (queue.IsEmpty());
}

TEST(DataQueueTest, TestEmpty)
{
     // Needs to be on the heap
     CMockMsgPoster* pMsgPoster = new CMockMsgPoster();
     CDataQueue queue (HWND_TEST, MSG_ID, pMsgPoster);

     EXPECT_TRUE (queue.IsEmpty());
     
     queue.Add ("This is the first message");
     queue.Add ("This is the second message");
     EXPECT_FALSE (queue.IsEmpty());

     queue.MakeEmpty();
          
     EXPECT_TRUE (queue.IsEmpty());

     const vector<string>& msgs = pMsgPoster->Messages();
     ASSERT_EQ (2, msgs.size());
     
     EXPECT_THAT (msgs [0].c_str(), MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x0\\)"));
     EXPECT_THAT (msgs [1].c_str(), MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x0\\)"));
}


TEST(DataQueueTest, TestVolume)
{
     // Needs to be on the heap
     CMockMsgPoster* pMsgPoster = new CMockMsgPoster();
     CDataQueue queue (HWND_TEST, MSG_ID, pMsgPoster);

     EXPECT_TRUE (queue.IsEmpty());
     
     CString sMsg;
     CStopWatch sw;
     for (int i = 0; i < 40000; i++)
     {
          sMsg.Format ("This is message: %d", i);
          queue.Add (sMsg);
     }
     sw.Stop();
     
     int nMaxMillis = 200;
     EXPECT_GT (nMaxMillis, sw.MilliSeconds()) << "Should be less than "
                                               << nMaxMillis << "ms but was " 
                                               << sw.HumanReadable();
     EXPECT_FALSE (queue.IsEmpty());
     EXPECT_EQ (queue.
     queue.MakeEmpty();
          
     EXPECT_TRUE (queue.IsEmpty());
}

