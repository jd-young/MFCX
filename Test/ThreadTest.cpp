/*!
     \file     ThreadTest.cpp
     \author   John Young
     \date     14 November 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockMsgPoster.h"
#include "../include/StringUtil.h"
#include "../include/Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ::testing;
using MFCX::CStringUtil;

/*!  A class derived from CThread to override the SetHandle() method for 
 *   testing.
 *
 *   The data queue instantiation is changed to that we can supply our own 
 *   message poster.
 */
class CTestThread : public CThread
{
public:
     CTestThread();
     
     // An override to provide a mock message poster for testing.
     CTestThread (HWND hwnd, CMockMsgPoster* pPoster, CDataQueue* pQueue);

     bool Start() { return CThread::Start (TestFunction); }

     /// Override to replace the message poster to our one.
     void SetHandle (HWND hwnd, UINT wmMsg);
//
     void OnStart();
     void OnFinished();

     CMockMsgPoster* GetMsgPoster() { return _pMsgPoster; }

private:
     CMockMsgPoster* _pMsgPoster;   // CDataQueue owns this.
     bool _bStarted;
     bool _bFinished;

     UINT TestFunction();
     static UINT TestFunction (void* lParam);
};

CTestThread::CTestThread()
 :   _bStarted (false), _bFinished (false)
{
}

CTestThread::CTestThread (HWND hwnd, CMockMsgPoster* pPoster, CDataQueue* pQueue)
 :   CThread (hwnd, 0), _bStarted (false), _bFinished (false), 
     _pMsgPoster (pPoster)
{
     // Replace with our own
     delete m_pDataQueue;
     m_pDataQueue = pQueue;
}


void CTestThread::SetHandle (HWND hwnd, UINT wmMsg)
{
     m_hTarget = hwnd;
     if ( m_pDataQueue )
          delete m_pDataQueue;
     
//     m_pDataQueue = new CDataQueue (m_hTarget, wmMsg);    <-- original
     _pMsgPoster = new CMockMsgPoster();
     m_pDataQueue = new CDataQueue (m_hTarget, wmMsg, _pMsgPoster);
}

void CTestThread::OnStart()
{ 
     _bStarted = true; 
     CThread::OnStart();
}

void CTestThread::OnFinished()
{
     _bFinished = true;
     CThread::OnFinished();
}



/*!  This function is run on a separate thread.
 *
 *   It calls the CTestThread's internal TestFunction() which cooperates (ie it
 *   respects the IsStopSignalled() call).
 */
/*static*/ UINT CTestThread::TestFunction (VOID* pParam)
{
     CTestThread* pThread = static_cast<CTestThread*>(pParam);
     ASSERT ( pThread );       // Pass me something I can work with!
	
     if ( pThread == NULL )
          return 1;
	return pThread->TestFunction();
}

UINT CTestThread::TestFunction()
{
     int nNextID = 1;
     m_pDataQueue->Add ("TestFunction started", nNextID++);
     
     for (int i = 0; i < 20; i++)
     {
          if ( IsStopSignalled() )
          {
               m_pDataQueue->Add ("TestFunction stopped", nNextID++);
               return 1;
          }
          std::this_thread::sleep_for (std::chrono::milliseconds (20));
          m_pDataQueue->Add (CStringUtil::Format ("Message number: %d", i + 1).c_str());
     }
     
     m_pDataQueue->Add ("TestFunction finished", nNextID++);
     return 0;
}


TEST(ThreadTest, TestThread_UsingDeprecated)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CTestThread thrd;
     thrd.SetHandle (HWND_TEST, MSG_ID);
     
     CDataQueue* pQueue = thrd.GetQueue(); 
     CMockMsgPoster* pPoster = thrd.GetMsgPoster(); 

     thrd.Start();
     
     // CWinThread doesn't seem to have a join() function to wait until it 
     // finishes, so we use this crude measure to wait for it to finish.
     while (thrd.IsActive())
          std::this_thread::sleep_for (std::chrono::milliseconds (50));

     const vector<string>& msgs = pPoster->Messages();
     ASSERT_EQ (22, msgs.size());
     
     for (int i = 0; i < 22; i++)
     {
          const string& msg = msgs [i];
          EXPECT_THAT (msg, 
                       MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x\\w+\\)"))
               << "Index " << i << " expected: 'PostMessage (0xffffffff, 32768, 0xXXXXXXX, 0xXX)'\n"
               << "         but got: '" << msg << "'";
     }
     
     EXPECT_STREQ ("TestFunction started", pQueue->Remove());
     EXPECT_STREQ ("Message number: 1", pQueue->Remove());
     EXPECT_STREQ ("Message number: 2", pQueue->Remove());
     EXPECT_STREQ ("Message number: 3", pQueue->Remove());
     EXPECT_STREQ ("Message number: 4", pQueue->Remove());
     EXPECT_STREQ ("Message number: 5", pQueue->Remove());
     EXPECT_STREQ ("Message number: 6", pQueue->Remove());
     EXPECT_STREQ ("Message number: 7", pQueue->Remove());
     EXPECT_STREQ ("Message number: 8", pQueue->Remove());
     EXPECT_STREQ ("Message number: 9", pQueue->Remove());
     EXPECT_STREQ ("Message number: 10", pQueue->Remove());
     EXPECT_STREQ ("Message number: 11", pQueue->Remove());
     EXPECT_STREQ ("Message number: 12", pQueue->Remove());
     EXPECT_STREQ ("Message number: 13", pQueue->Remove());
     EXPECT_STREQ ("Message number: 14", pQueue->Remove());
     EXPECT_STREQ ("Message number: 15", pQueue->Remove());
     EXPECT_STREQ ("Message number: 16", pQueue->Remove());
     EXPECT_STREQ ("Message number: 17", pQueue->Remove());
     EXPECT_STREQ ("Message number: 18", pQueue->Remove());
     EXPECT_STREQ ("Message number: 19", pQueue->Remove());
     EXPECT_STREQ ("Message number: 20", pQueue->Remove());
     EXPECT_STREQ ("TestFunction finished", pQueue->Remove());

     EXPECT_EQ (0, thrd.GetExitCode());
}

TEST(ThreadTest, TestThread)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CMockMsgPoster* pPoster = new CMockMsgPoster();
     CDataQueue* pQueue = new CDataQueue (HWND_TEST, MSG_ID, pPoster); 
     CTestThread thrd (HWND_TEST, pPoster, pQueue);
     
     thrd.Start();
     
     // CWinThread doesn't seem to have a join() function to wait until it 
     // finishes, so we use this crude measure to wait for it to finish.
     while (thrd.IsActive())
          std::this_thread::sleep_for (std::chrono::milliseconds (50));

     const vector<string>& msgs = pPoster->Messages();
     ASSERT_EQ (22, msgs.size());
     
     for (int i = 0; i < 22; i++)
     {
          const string& msg = msgs [i];
          EXPECT_THAT (msg, 
                       MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x\\w+\\)"))
               << "Index " << i << " expected: 'PostMessage (0xffffffff, 32768, 0xXXXXXXX, 0xXX)'\n"
               << "         but got: '" << msg << "'";
     }
     
     EXPECT_STREQ ("TestFunction started", pQueue->Remove());
     EXPECT_STREQ ("Message number: 1", pQueue->Remove());
     EXPECT_STREQ ("Message number: 2", pQueue->Remove());
     EXPECT_STREQ ("Message number: 3", pQueue->Remove());
     EXPECT_STREQ ("Message number: 4", pQueue->Remove());
     EXPECT_STREQ ("Message number: 5", pQueue->Remove());
     EXPECT_STREQ ("Message number: 6", pQueue->Remove());
     EXPECT_STREQ ("Message number: 7", pQueue->Remove());
     EXPECT_STREQ ("Message number: 8", pQueue->Remove());
     EXPECT_STREQ ("Message number: 9", pQueue->Remove());
     EXPECT_STREQ ("Message number: 10", pQueue->Remove());
     EXPECT_STREQ ("Message number: 11", pQueue->Remove());
     EXPECT_STREQ ("Message number: 12", pQueue->Remove());
     EXPECT_STREQ ("Message number: 13", pQueue->Remove());
     EXPECT_STREQ ("Message number: 14", pQueue->Remove());
     EXPECT_STREQ ("Message number: 15", pQueue->Remove());
     EXPECT_STREQ ("Message number: 16", pQueue->Remove());
     EXPECT_STREQ ("Message number: 17", pQueue->Remove());
     EXPECT_STREQ ("Message number: 18", pQueue->Remove());
     EXPECT_STREQ ("Message number: 19", pQueue->Remove());
     EXPECT_STREQ ("Message number: 20", pQueue->Remove());
     EXPECT_STREQ ("TestFunction finished", pQueue->Remove());

     EXPECT_EQ (0, thrd.GetExitCode());
}

/// Tests that when Stop() is called, that the thread function (which cooperates
/// with the request) stops early.
TEST(ThreadTest, TestStopThread)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CMockMsgPoster* pPoster = new CMockMsgPoster();
     CDataQueue* pQueue = new CDataQueue (HWND_TEST, MSG_ID, pPoster); 
     CTestThread thrd (HWND_TEST, pPoster, pQueue);
     
     thrd.Start();

     // Wait for the thread function to post a message.  Reading a vector 
     // doesn't need to be synchronised.
     while ( pPoster->Messages().size() == 0 )
          std::this_thread::sleep_for (std::chrono::milliseconds (10));

     // We now have at least one posted message from the thread function.
     thrd.Stop();
     
     // CWinThread doesn't seem to have a join() function to wait until it 
     // finishes, so we use this crude measure to wait for it to finish.
     while (thrd.IsActive())
          std::this_thread::sleep_for (std::chrono::milliseconds (50));

     const vector<string>& msgs = pPoster->Messages();
     int nMsgs = msgs.size();
     ASSERT_GT (22, nMsgs);        // We stopped early so less than 22 messages.
     
     for (int i = 0; i < nMsgs; i++)
     {
          const string& msg = msgs [i];
          EXPECT_THAT (msg, 
                       MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x\\w+\\)"))
               << "Index " << i << " expected: 'PostMessage (0xffffffff, 32768, 0xXXXXXXX, 0xXX)'\n"
               << "         but got: '" << msg << "'";
     }
     
     EXPECT_STREQ ("TestFunction started", pQueue->Remove());
     for (int i = 1; i < nMsgs - 1; i++)
          EXPECT_THAT (pQueue->Remove(), 
                       MatchesRegex ("Message number: \\d+")) 
                       << "Mismatch message " << i;
     EXPECT_STREQ ("TestFunction stopped", pQueue->Remove());

     EXPECT_EQ (1, thrd.GetExitCode());
}
