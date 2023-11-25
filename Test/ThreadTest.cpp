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
#include "../include/Directory.h"
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

     /// Override to replace the message poster to our one.
     void SetHandle (HWND hwnd, UINT wmMsg);
//
     void OnStart();
     void OnFinished();

// mock stuff

     bool StartCooperatingThread() { return CThread::Start (CooperatingFunction); }

// Not private so that we can check stuff.

     CMockMsgPoster* _pMsgPoster;   // CDataQueue owns this.
     bool _bStarted;
     bool _bFinished;

     UINT CooperatingFunction();
     static UINT CooperatingFunction (void* lParam);
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
/*static*/ UINT CTestThread::CooperatingFunction (VOID* pParam)
{
     CTestThread* pThread = static_cast<CTestThread*>(pParam);
     ASSERT ( pThread );       // Pass me something I can work with!
	
     if ( pThread == NULL )
          return 1;
	return pThread->CooperatingFunction();
}

UINT CTestThread::CooperatingFunction()
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

     thrd.Start (CTestThread::CooperatingFunction);
 
     // Wait for the thread to end.     
     thrd.Join();
    
     const vector<string>& msgs = thrd._pMsgPoster->Messages();
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
     
     thrd.Start (CTestThread::CooperatingFunction);
     
     // Wait for the thread to end.     
     thrd.Join();

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
TEST(ThreadTest, TestStopCooperatingThread)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CMockMsgPoster* pPoster = new CMockMsgPoster();
     CDataQueue* pQueue = new CDataQueue (HWND_TEST, MSG_ID, pPoster); 
     CTestThread thrd (HWND_TEST, pPoster, pQueue);
     
     thrd.Start (CTestThread::CooperatingFunction);

      // Wait for the thread function to post a message.  Reading a vector 
      // doesn't need to be synchronised.
      while ( pPoster->Messages().size() == 0 )
           std::this_thread::sleep_for (std::chrono::milliseconds (10));

     // We now have at least one posted message from the thread function.
     thrd.Stop();
     
     // Wait for the thread to end.     
     thrd.Join();

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

/// Tests that CThread starts CLI and can capture its output.
TEST(ThreadTest, TestStartCli)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CMockMsgPoster* pPoster = new CMockMsgPoster();
     CDataQueue* pQueue = new CDataQueue (HWND_TEST, MSG_ID, pPoster); 
     CTestThread thrd (HWND_TEST, pPoster, pQueue);
     thrd.SetUserData ((LPARAM) 0x55);

     map<string, string> mapEnvs;
     mapEnvs ["CLI_PROCESS"] = "my-process";
     thrd.StartCliProcess ("spawned-process.bat",
                           "100",
                           "..\\test\\resources",
                           &mapEnvs);
     EXPECT_STREQ ("cmd /c spawned-process.bat 100", thrd._sCmd);
     EXPECT_STREQ ("my-process", thrd._mapEnvs ["CLI_PROCESS"].c_str());

     // Wait for the thread to end.     
     thrd.Join();

     const vector<string>& msgs = pPoster->Messages();
     int nMsgs = msgs.size();
     EXPECT_EQ (8, nMsgs);

     for (int i = 0; i < nMsgs; i++)
     {
          const string& msg = msgs [i];
          EXPECT_THAT (msg, 
                       MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x55\\)"))
               << "Index " << i << " expected: 'PostMessage (0xffffffff, 32768, 0xXXXXXXX, 0x55)'\n"
               << "         but got: '" << msg << "'";
     }

     EXPECT_STREQ ("Printing 2 messages every 50 msecs for 100 msecs.\r\n",
                   pQueue->Remove());
     EXPECT_THAT (pQueue->Remove(), MatchesRegex ("^.*\\\\MFCX\\\\test\\\\resources\r\n$"));
     
     for (int i = 1; i < nMsgs - 5; i++)
     {
          string exp = CStringUtil::Format ("Message %d\r\n", i);
          EXPECT_STREQ (exp.c_str(), pQueue->Remove());
     }

     EXPECT_STREQ ("Script complete\r\n", pQueue->Remove());
     EXPECT_THAT ((const TCHAR*) pQueue->Remove(), StartsWith ("Start time: "));
     EXPECT_THAT ((const TCHAR*) pQueue->Remove(), StartsWith ("End time: "));

     EXPECT_STREQ ("Finished\r\n", pQueue->Remove());
     EXPECT_STREQ ("", pQueue->Remove());
}



/// Tests that when Stop() is called, that the thread function (a separate 
/// process) stops early.
TEST(ThreadTest, TestStopSpawnedProcess)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CMockMsgPoster* pPoster = new CMockMsgPoster();
     CDataQueue* pQueue = new CDataQueue (HWND_TEST, MSG_ID, pPoster); 
     CTestThread thrd (HWND_TEST, pPoster, pQueue);

     CString sCWD = CDirectory::GetCurrentDir();
     thrd.StartCliProcess ("..\\test\\resources\\spawned-process.bat");
     EXPECT_STREQ ("cmd /c ..\\test\\resources\\spawned-process.bat", thrd._sCmd);

     // Wait for the thread function to post 2 messages.  Reading a vector 
     // doesn't need to be synchronised.
     while ( pPoster->Messages().size() < 3 )
          std::this_thread::sleep_for (std::chrono::milliseconds (10));

     // We now have at least one posted message from the thread function.
     thrd.Stop();

     // Wait for the thread to end.     
     thrd.Join();

     const vector<string>& msgs = pPoster->Messages();
     int nMsgs = msgs.size();
     ASSERT_GT (22, nMsgs);        // We stopped early so less than 22 messages.

     for (int i = 0; i < nMsgs; i++)
     {
          const string& msg = msgs [i];
          EXPECT_THAT (msg, 
                       MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x0\\)"))
               << "Index " << i << " expected: 'PostMessage (0xffffffff, 32768, 0xXXXXXXX, 0x0)'\n"
               << "         but got: '" << msg << "'";
     }
     
     EXPECT_STREQ ("Printing 100 messages every 50 msecs for 5000 msecs.\r\n",
                   pQueue->Remove());
     EXPECT_THAT (pQueue->Remove(), MatchesRegex ("^.*\\\\MFCX\\\\src\r\n$"));
     
     for (int i = 1; i < nMsgs - 2 ; i++)
     {
          string exp = CStringUtil::Format ("Message %d\r\n", i);
          EXPECT_STREQ (exp.c_str(), pQueue->Remove());
     }
     EXPECT_STREQ ("Stopped\r\n", pQueue->Remove());

     EXPECT_EQ (0, thrd.GetExitCode());
}

/// Tests that CThread copes with attempting to start a non-existent program.
TEST(ThreadTest, TestStartNonExistent)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CMockMsgPoster* pPoster = new CMockMsgPoster();
     CDataQueue* pQueue = new CDataQueue (HWND_TEST, MSG_ID, pPoster); 
     CTestThread thrd (HWND_TEST, pPoster, pQueue);

     CString sCWD = CDirectory::GetCurrentDir();
     thrd.StartCliProcess ("non existent.exe");
     EXPECT_STREQ ("\"non existent.exe\"", thrd._sCmd);

     // Wait for the thread to end.     
     thrd.Join();

     const vector<string>& msgs = pPoster->Messages();
     int nMsgs = msgs.size();
     ASSERT_EQ (1, nMsgs);

     EXPECT_THAT (msgs [0], 
                  MatchesRegex ("PostMessage \\(0xffffffff, 32768, 0x\\w+, 0x0\\)"));

     EXPECT_STREQ ("Create process failed: returned 0: "
                   "'The system cannot find the file specified.' (errno=2)\r\n",
                   pQueue->Remove());
     EXPECT_STREQ ("", pQueue->Remove());
}
 