/*!
     \file     DataQueueTest.cpp
     \author   John Young
     \date     14 November 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <random>
#include <string>
#include <vector>
#include "MockMsgPoster.h"
#include "TestUtils.h"
#include "../include/DataQueue.h"
#include "../include/Regex.h"
#include "../include/StringUtil.h"
#include "../include/XTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using std::string;
using std::vector;

using namespace ::testing;

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

#define   TEST_VOLUME_MSG_COUNT    40000
#define   TEST_VOLUME_MAX_MSECS    400

/*!  This test sends 40,000 messages in under 200 mS, however, when coverage is
 *   run, then it is considerably slower and so this test fails.
 */
TEST(DataQueueTest, TestVolume)
{
     // Needs to be on the heap
     CMockMsgPoster* pMsgPoster = new CMockMsgPoster();
     CDataQueue queue (HWND_TEST, MSG_ID, pMsgPoster);

     EXPECT_TRUE (queue.IsEmpty());
     
     CString sMsg;
     CStopWatch sw;
     for (int i = 0; i < TEST_VOLUME_MSG_COUNT; i++)
     {
          sMsg.Format ("This is message: %d", i);
          queue.Add (sMsg);
     }
     sw.Stop();
     
     int nMaxMillis = TEST_VOLUME_MAX_MSECS;
     EXPECT_GT (nMaxMillis, sw.MilliSeconds()) << "Should be less than "
                                               << nMaxMillis << "ms but was " 
                                               << sw.HumanReadable();
     EXPECT_FALSE (queue.IsEmpty());
     queue.MakeEmpty();
          
     EXPECT_TRUE (queue.IsEmpty());
}

void Writer (CDataQueue& queue, int id)
{
     std::random_device rd;   // a seed source for the random number engine
     std::mt19937 gen (rd()); // mersenne_twister_engine seeded with rd()
     std::uniform_int_distribution<> distrib (1, 10);  // 1 ~ 10 ms

     int nMsgs = id;
     for (int i = 0; i < nMsgs; i++)
     {
          // Sleep for a random number of 
          int rand = distrib (gen);     // 1 ~ 10
          std::this_thread::sleep_for (std::chrono::milliseconds (rand));

          string s = CStringUtil::Format ("Message %d from thread %d", i, id);
          queue.Add (s.c_str(), id);
     }
}

/// Returns the count of messages from the given thread ID.
int CountMsgFromID (const vector<string>& msgs, int nID)
{
     // Messages are in the form: 'Message N from thread M'
     int nCount = 0;
     vector<string>::const_iterator it = msgs.cbegin();
     vector<string>::const_iterator end = msgs.cend();
     for ( ; it != end; ++it)
     {
          CRegex re;
          if ( re.Match ("(\\d+)$", (*it).c_str()) )
          {
               int nFromID = std::atoi (re [0]);
               if ( nFromID == nID )
                    nCount++;
          }
     }
     return nCount;
}

#define   NR_WRITERS     10

/*!  Creates several threads to write to and one thread to read from the 
 *   CDataQueue object.
 *
 */
TEST(DataQueueTest, TestMultipleThreads)
{
     // Needs to be on the heap
     CMockMsgPoster* pMsgPoster = new CMockMsgPoster();
     CDataQueue queue (HWND_TEST, MSG_ID, pMsgPoster);

     // Create writer threads
     int nWriters = NR_WRITERS;
     std::vector<std::thread> writerThreads;
     for (int i = 0; i < nWriters; i++)
          writerThreads.emplace_back (Writer, std::ref (queue), i + 1);

     // Wait for the writers to finish
     for (auto& thread : writerThreads)
          thread.join();
     
     // Check that Windows messages were sent;
     const vector<string> msgs = pMsgPoster->Messages();
     ASSERT_EQ (55, msgs.size());
     
     for (int i = 0; i < 55; i++)
     {
          const string& msg = msgs [i];
          EXPECT_THAT (msg, 
                       MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x\\w+\\)"))
               << "Index " << i << " expected: 'PostMessage (0xffffffff, 32768, 0xXXXXXXX, 0xXX)'\n"
               << "         but got: '" << msg << "'";
     }
     
     // Check that the correct number of posted messages come from the expected
     // thread. 
     int nMsgFromThread [NR_WRITERS];   // [0] stores count for ID 1 (ie offset by 1).
     for (int i = 0; i < nWriters; i++)
          nMsgFromThread [i] = 0;       // Initialise as zero
          
     for (int i = 0; i < 55; i++)
     {
          const string& msg = msgs [i];

          // Get the lParam number (the last arg in the PostMessage string).
          CRegex re;
          ASSERT_TRUE (re.Match ("(0x[0-9a-z]+)\\)", msg.c_str()));
          CString sParam = re [0];
          long nID = std::strtol (sParam, nullptr, 16);
          ASSERT_THAT (nID, AllOf (Ge (1), Le (10)));
          nMsgFromThread [nID - 1]++;
     }
     
     for (int id = 1; id <= nWriters; id++)
          EXPECT_EQ (id, nMsgFromThread [id - 1]);

     // Check that the number of message from the queue are correct.
     vector<string> sOutputs;
     while ( !queue.IsEmpty() )
           sOutputs.push_back ((const TCHAR*) queue.Remove());
     EXPECT_EQ (55, sOutputs.size());

     for (int id = 0; id < nWriters; id++)
     {
          int nMsgs = CountMsgFromID (sOutputs, id + 1);
          EXPECT_EQ (id + 1, nMsgs) << "Thread ID sent " << nMsgs 
                                    << " but should have sent " << id;
     }
}