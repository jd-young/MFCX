/*!
     \file     Thread.cpp          Thread stuff
     \author   John Young
     \date     16-Dec-99

     Copyright (c) 1999, Thistle Software
*/

#include "stdafx.h"
#include "../include/EnvVars.h"
#include "../include/Filename.h"
#include "../include/StringUtil.h"
#include "../include/SysError.h"
#include "../include/Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using MFCX::CEnvVars;
using MFCX::CStringUtil;
using MFCX::CSysError;


CThread::CThread()
 :   m_hTarget (NULL), m_pDataQueue (nullptr),
     m_pThread (nullptr), m_bStopThread (false), _nRetCode (-1), 
     _fnThread (nullptr), _nUserData (NULL)
{
}

CThread::CThread (HWND hwnd, UINT wmMsg)
 :   m_hTarget (hwnd), m_pDataQueue (new CDataQueue (hwnd, wmMsg)),
     m_pThread (nullptr), m_bStopThread (false), _nRetCode (-1), 
     _fnThread (nullptr), _nUserData (NULL)
{
}


CThread::~CThread()
{
     delete m_pDataQueue;
}


void CThread::SetHandle (HWND hwnd, UINT wmMsg)
{
     m_hTarget = hwnd;
     if ( m_pDataQueue )
          delete m_pDataQueue;
     
     m_pDataQueue = new CDataQueue (m_hTarget, wmMsg);
}

/*!  Starts a worker thread and calls the given function.
 *
 * \param pFunc          The function to call on the newly started thread.
 * \return true if the thread was started.
 */
bool CThread::Start (AFX_THREADPROC pFunc)
{
     if ( pFunc )
     {
          _fnThread = pFunc;
          m_pThread = AfxBeginThread (WrapperThread, static_cast<void*>(this)); 
     }
     return m_pThread != NULL;
}

/*!  A thread function that calls the non-static version.
 *
 *   This is to allow us to have OnStart() and OnFinished() called automatically
 *   without derived classes having to call them explicitly.
 *
 * \param lParam         A void point to this object.
 */
/*static*/ UINT CThread::WrapperThread (VOID* lParam)
{
     CThread* pThread = static_cast<CThread*>(lParam);
     ASSERT (pThread);
     return pThread->WrapperThread();    
}

/*!  Calls the user supplied thread function wrapped between OnStart() and 
 *   OnFinished().
 *
 *   This also catches the return code and stores it for later retrieval if 
 *   required (see GetExitCode()).
 */
UINT CThread::WrapperThread()
{
     OnStart();
     _nRetCode = _fnThread (static_cast<void*>(this));
     OnFinished();
     return _nRetCode;
}

/*!  Signals the thread to stop.
 *
 *   Note that this doesn't actually stop the thread, but sets a flag that is 
 *   should be inspected in the thread function, giving it the opportunity to 
 *   stop gracefully.
 */
void CThread::Stop()
{
     m_bStopThread = true;
}

/*!  Called when the thread is started.
 *
 *   Override this if you need any additional processing.
 */
void CThread::OnStart()
{
     m_bStopThread = false;
}

/*!  Called when the thread finishes.
 *   
 *   Override this to do any additional processing when the thread finishes.
 */
void CThread::OnFinished()
{
     m_pThread = NULL;
}

DWORD CThread::GetExitCode() const
{
     return _nRetCode;
}

/*!  Waits for a maximum of 5 seconds for the thread to terminate.
 *
 * \return a WAIT_XXX value:
 *             - WAIT_OBJECT_0 - The thread exited.
 *             - WAIT_TIMEOUT  - Thread didn't terminate after 5 seconds.
 *             - WAIT_FAILED   - The function failed.  Use GetLastError() to get
 *                               extended error information.
 */
DWORD CThread::Join()
{
     // TODO: Figure out what the timeout should be.
     return m_pThread
               ? ::WaitForSingleObject (m_pThread->m_hThread, INFINITE)
               : WAIT_OBJECT_0;
}

/*!  Starts the given CLI command on a separate thread.
 *
 *   Its output is captured in the m_pDataQueue thread-safe data queue which 
 *   sends a message to the target window which can then read the message from 
 *   the queue.
 *
 * \param pszCmd         The command line of the process to run.  The .exe 
 *                       extension can be omitted (it is added automatically),
 *                       however if you are running a .com, .cmd or .bat file, 
 *                       then the extension must be supplied.  Also, note that
 *                       .bat files are automatically prepended with 'cmd /c' to
 *                       prevent some start-up problems (CreateProcess() doesn't
 *                       seem to be able to start up .bat files if the directory
 *                       is supplied).
 * \param pszArgs        The process arguments, or nullptr if there are none.
 * \param pszDir         The startup directory for the command.  If this is 
 *                       omitted, then the current drive & directory of this 
 *                       process is used.
 * \param pEnvVars       A map of additional environment variables for the new 
 *                       process.  These are added to this process's existing
 *                       environment variables.
 * \return true on success, false on failure.
 */
bool CThread::StartCliProcess (const TCHAR* pszCmd,
                               const TCHAR* pszArgs /*= nullptr*/,
                               const TCHAR* pszDir /*= nullptr*/,
                               const map<string, string>* pEnvVars /*= nullptr*/)
{
     _sCmd = pszCmd;
     if ( _sCmd.Find (' ') != -1 )
     {
          // Enclose in double-quotes
          _sCmd.Insert (0, '"');
          _sCmd += '"';
     }
     
     CString sExt = CFilename::GetFileExt (pszCmd);
     if ( sExt == ".bat" )
          _sCmd = "cmd /c " + _sCmd;
     
     if ( pszArgs )
     {
          _sCmd += ' ';
          _sCmd += pszArgs;
     }
     
     if ( pszDir )
     {
          _tfullpath (_sDir.GetBuffer (MAX_PATH + 1), pszDir, MAX_PATH);
          _sDir.ReleaseBuffer();
     }
     else _sDir = "";
     
     if ( pEnvVars )
          _mapEnvs = *pEnvVars;
     
     return Start (CliProcess);
}

UINT CThread::CliProcess (void* lParam)
{
     CThread* pThread = static_cast<CThread*>(lParam);
     ASSERT ( pThread );       // Pass me something I can work with!
	
     if ( pThread == NULL )
          return 1;
	return pThread->CliProcess();
}


// The size of the buffer used to take from the tools' output and sent to the 
// output window.  This should be bigger than an output line from the tool for
// efficiency.
#define   BUFFER_SIZE    512

UINT CThread::CliProcess()
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

     CEnvVars envVars (_mapEnvs);
     std::unique_ptr<TCHAR> envBlock (envVars.GetProcEnvBlock());
	
     CString sSpawn = _sCmd;
     const TCHAR* pDir = _sDir.IsEmpty() 
                         ? nullptr 
                         : static_cast<const TCHAR*>(_sDir);

     BOOL bRet = ::CreateProcess (nullptr,			// Application name
                                  sSpawn.GetBuffer (0),// Command line
                                  nullptr, nullptr,    // Process / thread attributes
                                  TRUE,                // Inherit handle
                                  IDLE_PRIORITY_CLASS, // Creation flags
                                  envBlock.get(),      // Environment
                                  pDir,                // Current dir 
                                  &si, &pi);		     // Startup info / process info

     if ( !bRet )
     {
     	DWORD dwError = ::GetLastError();
     	CString sError = CSysError::GetErrorStr (dwError);
          string sEvent = CStringUtil::Format ("Create process failed: returned %d: '%s' (errno=%u)\r\n", 
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
          GetQueue()->Add (szBuffer, _nUserData);
     }

     // Wait for the process to finish.
     ::WaitForSingleObject (pi.hProcess, INFINITE);

     // Close all remaining handles
     ::CloseHandle (pi.hProcess);
     ::CloseHandle (hPipeOutputRead);
     ::CloseHandle (hPipeInputWrite);

     return 0;
}
