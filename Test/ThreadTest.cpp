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
#include "../include/SysError.h"
#include "../include/Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ::testing;
using MFCX::CStringUtil;
using MFCX::CSysError;

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
     bool StartCliProcess (const TCHAR* pszCmd, const TCHAR* pszDir = nullptr);

// Not private so that we can check stuff.

     CMockMsgPoster* _pMsgPoster;   // CDataQueue owns this.
     bool _bStarted;
     bool _bFinished;

     UINT CooperatingFunction();
     static UINT CooperatingFunction (void* lParam);

     UINT CliProcess();
     static UINT CliProcess (void* lParam);

     CString _sCmd;
     CString _sDir;

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


bool CTestThread::StartCliProcess (const TCHAR* pszCmd, 
                                   const TCHAR* pszDir /*= nullptr*/)
{
     _sCmd = pszCmd;
     if ( pszDir )
     {
          _tfullpath (_sDir.GetBuffer (MAX_PATH + 1), pszDir, MAX_PATH);
          _sDir.ReleaseBuffer();
     }
     else _sDir = "";
     return Start (CliProcess);
}

UINT CTestThread::CliProcess (void* pParam)
{
     CTestThread* pThread = static_cast<CTestThread*>(pParam);
     ASSERT ( pThread );       // Pass me something I can work with!
	
     if ( pThread == NULL )
          return 1;
	return pThread->CliProcess();
}


// The size of the buffer used to take from the tools' output and sent to the 
// output window.  This should be bigger than an output line from the tool for
// efficiency.
#define   BUFFER_SIZE    512

UINT CTestThread::CliProcess()
{
     SECURITY_ATTRIBUTES sa = { 0 };
     STARTUPINFO         si = { 0 };
     PROCESS_INFORMATION pi = { 0 };
     HANDLE hPipeOutputRead  = NULL;
     HANDLE hPipeOutputWrite = NULL;
     HANDLE hPipeInputRead   = NULL;
     HANDLE hPipeInputWrite  = NULL;

     sa.nLength = sizeof sa;
     sa.bInheritHandle = TRUE;
     sa.lpSecurityDescriptor = NULL;

     CTime tmStart = CTime::GetCurrentTime();

     // Create pipe for standard output redirection.
     if ( !CreatePipe (&hPipeOutputRead, &hPipeOutputWrite, &sa, 0) )
     {
          GetQueue()->Add ("Couldn't create the output pipe");
          Stop();
          return 1;
     }

     // Create pipe for standard input redirection.
     if ( !CreatePipe (&hPipeInputRead, &hPipeInputWrite, &sa, 0) )
     {
          GetQueue()->Add ("Couldn't create the input pipe");
          Stop();
          return 1;
     }

     // Make child process use hPipeOutputWrite as standard out,
     // and make sure it does not show on screen.
     si.cb = sizeof(si);
     si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
     si.wShowWindow = SW_HIDE;
     si.hStdInput   = hPipeInputRead;
     si.hStdOutput  = hPipeOutputWrite;
     si.hStdError   = hPipeOutputWrite;

     // Need to escape all the string delimitters before passing them to the 
     // spawned process.
     CString sCmd = _sCmd;
     for (int indx = 0; 
          indx < sCmd.GetLength() && (indx=sCmd.Find ('"', indx)) != -1;
          indx += 2 )
          sCmd.Insert (indx, '\\');

//     std::unique_ptr<TCHAR> envBlock (m_mapEnv.GetProcEnvBlock());
	
     CString sSpawn = _sCmd;
     const TCHAR* pDir = _sDir.IsEmpty() 
                         ? nullptr 
                         : static_cast<const TCHAR*>(_sDir);

     BOOL bRet = ::CreateProcess (nullptr,			// Application name
                                  sSpawn.GetBuffer (0),  // Command line
                                  nullptr, nullptr,      // Process / thread attributes
                                  TRUE,                  // Inherit handle
                                  IDLE_PRIORITY_CLASS,   // Creation flags
                                  NULL,   // envBlock.get(),   // Environment
                                  pDir,                 // Current dir 
                                  &si, &pi);		     // Startup info / process info

     if ( !bRet )
     {
     	DWORD dwError = ::GetLastError();
     	CString sError = CSysError::GetErrorStr (dwError);
          string sEvent = CStringUtil::Format ("Create process failed: ret=%d, %s (errno=%u)", 
                                               bRet, sError, dwError);
          GetQueue()->Add (sEvent.c_str());
              
          CloseHandle (hPipeOutputWrite);
          CloseHandle (hPipeInputRead);
          return 1;
     }

     // Now that handles have been inherited, close it to be safe.
     // You don't want to read or write to them accidentally.
     CloseHandle (hPipeOutputWrite);
     CloseHandle (hPipeInputRead);

     // Now test to capture DOS application output by reading
     // hPipeOutputRead.  Could also write to DOS application
     // standard input by writing to hPipeInputWrite.

     BOOL bFinished = FALSE;
     while ( !bFinished )
     {
          TCHAR   szBuffer [BUFFER_SIZE];
          DWORD  dwNumberOfBytesRead = 0;

          BOOL bTest = ::ReadFile (hPipeOutputRead,      // handle of the read end of our pipe
                                   &szBuffer,            // address of buffer that receives data
                                   BUFFER_SIZE - 1,      // number of bytes to read
                                   &dwNumberOfBytesRead, // address of number of bytes read
                                   nullptr);             // non-overlapped.
          
          if ( IsStopSignalled() )
          {
               // TODO: Figure out what the exit code should be.
               ::TerminateProcess (pi.hProcess, 1);
               wsprintf (szBuffer, "Stopped\r\n");
               bFinished = TRUE;
          }
          else if ( !bTest )
          {
               int nError = ::GetLastError();
               if ( nError == ERROR_BROKEN_PIPE )
                    wsprintf (szBuffer, "Finished\r\n");
               else wsprintf (szBuffer, "Error #%d reading pipe.\r\n", nError);
               bFinished = TRUE;
          }
          else szBuffer [dwNumberOfBytesRead] = 0;  // null terminate

          // Send the data to the CDataQueue that was passed in...
          GetQueue()->Add (szBuffer, GetToolIndex());
     }

     // Wait for the process to finish.
     ::WaitForSingleObject (pi.hProcess, 5000);

     // Close all remaining handles
     ::CloseHandle (pi.hProcess);
     ::CloseHandle (hPipeOutputRead);
     ::CloseHandle (hPipeInputWrite);

     return 0;
}

TEST(ThreadTest, TestThread_UsingDeprecated)
{
     HWND HWND_TEST = (HWND) -1;
     UINT MSG_ID = WM_APP;

     CTestThread thrd;
     thrd.SetHandle (HWND_TEST, MSG_ID);
     
     CDataQueue* pQueue = thrd.GetQueue(); 
//     CMockMsgPoster* pPoster = thrd.GetMsgPoster(); 

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

     // Wait for the thread function to post 2 messages.  Reading a vector 
     // doesn't need to be synchronised.
     while ( pPoster->Messages().size() < 2 )
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
     
     
     EXPECT_STREQ ("Printing 100 messages every 50 msecs for 5000 msecs.\r\n",
                   pQueue->Remove());
     
     for (int i = 1; i < nMsgs - 1; i++)
     {
          string exp = CStringUtil::Format ("Message %d\r\n", i);
          EXPECT_STREQ (exp.c_str(), pQueue->Remove());
     }
     
     EXPECT_STREQ ("Stopped\r\n", pQueue->Remove());

     EXPECT_EQ (0, thrd.GetExitCode());
}
 