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

using namespace MFCX;


CThread::CThread()
 :   m_hTarget (NULL), m_pDataQueue (nullptr),
     m_pThread (nullptr), m_bStopThread (false), _fnThread (nullptr), 
     _nUserData (NULL)
{
}

CThread::CThread (HWND hwnd, UINT wmMsg)
 :   m_hTarget (hwnd), m_pDataQueue (new CDataQueue (hwnd, wmMsg)),
     m_pThread (nullptr), m_bStopThread (false), _fnThread (nullptr), 
     _nUserData (NULL)
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
     UINT nRetCode = _fnThread (static_cast<void*>(this));
     OnFinished();
     return nRetCode;
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

bool CThread::CreatePipe (HANDLE* pReadPipe, HANDLE* pWritePipe, const TCHAR* pszDesc)
{
     SECURITY_ATTRIBUTES sa = { 0 };

     sa.nLength = sizeof sa;
     sa.bInheritHandle = TRUE;
     sa.lpSecurityDescriptor = NULL;

     if ( !::CreatePipe (pReadPipe, pWritePipe, &sa, 0) )
     {
          CString sMsg;
          sMsg.Format ("Couldn't create the %s pipe", pszDesc);
          GetQueue()->Add (sMsg);
          Stop();
          return false;
     }
     return true;
}

/*!  Creates a process and attaches the given pipes to it to capture its output.
 *
 *   If this function fails to spawn a new process, it reports the error in an 
 *   error message to the data-queue.
 *
 * \param hPipeInputRead      The input read pipe.
 * \param hPipeOutputWrite    The output write pipe.
 * \return the handle of the new process or NULL if unsuccessful.
 */
HANDLE CThread::CreateProcess (HANDLE hPipeInputRead, HANDLE hPipeOutputWrite)
{
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

     // Make child process use hPipeOutputWrite as standard out,
     // and make sure it does not show on screen.
     STARTUPINFO si = { 0 };
     si.cb = sizeof(si);
     si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
     si.wShowWindow = SW_HIDE;
     si.hStdInput   = hPipeInputRead;
     si.hStdOutput  = hPipeOutputWrite;
     si.hStdError   = hPipeOutputWrite;

     PROCESS_INFORMATION pi = { 0 };

     BOOL bCreated = ::CreateProcess (nullptr,              // Application name
                                      sSpawn.GetBuffer (0), // Command line
                                      nullptr, nullptr,     // Process / thread attributes
                                      TRUE,                 // Inherit handle
                                      IDLE_PRIORITY_CLASS,  // Creation flags
                                      envBlock.get(),       // Environment
                                      pDir,                 // Current dir 
                                      &si, &pi);            // Startup info / process info
     if ( !bCreated )
     {
          
     	DWORD dwError = ::GetLastError();
     	CString sError = CSysError::GetErrorStr (dwError);
          string sEvent = CStringUtil::Format ("Create process failed: '%s' (errno=%u)\r\n", 
                                               sError, dwError);
          GetQueue()->Add (sEvent.c_str());
     }

     // Now that handles have been inherited, close it to be safe.
     // You don't want to read or write to them accidentally.
     CloseHandle (hPipeOutputWrite);
     CloseHandle (hPipeInputRead);
     return bCreated ? pi.hProcess : NULL;
}

// The size of the buffer used to take from the tools' output and sent to the 
// output window.  This should be bigger than an output line from the tool for
// efficiency.
#define   BUFFER_SIZE    512

/*!  Reads a string from the given pipe into the given buffer.
 *
 *   This blocks until a string is available, or the attached process 
 *   terminates.
 *
 * \param hPipe          The handle of the pipe to read from.
 * \param sBuf           The buffer to receive the string.
 * \return TRUE if successful in which case sBuf contains the string read, false
 *         if the pipe was closed in which case sBuf contains 'Finished' if the
 *         process terminated, or an error message.
 */
BOOL CThread::ReadPipe (HANDLE hPipe, CString& sBuf)
{
     DWORD nBytesRead = 0;

     BOOL bGood = ::ReadFile (hPipe,                   // handle of the read end of our pipe
                              sBuf.GetBuffer (BUFFER_SIZE - 1),  // buffer to receive data
                              BUFFER_SIZE - 1,         // max bytes to read
                              &nBytesRead,             // number of bytes read
                              nullptr);                // non-overlapped.
     sBuf.ReleaseBuffer (nBytesRead);
     
     if ( !bGood )
     {
          int nError = ::GetLastError();
          if ( nError == ERROR_BROKEN_PIPE )
               sBuf = "Finished\r\n";
          else sBuf.Format ("Error %s (#%d) reading pipe.\r\n", 
                            CSysError::GetErrorStr (nError), nError);
     }
     return bGood;
}


UINT CThread::CliProcess()
{
     HANDLE hPipeOutputRead  = NULL;
     HANDLE hPipeOutputWrite = NULL;
     HANDLE hPipeInputRead   = NULL;
     HANDLE hPipeInputWrite  = NULL;

     // Create pipe for standard output redirection.
     if ( !CreatePipe (&hPipeOutputRead, &hPipeOutputWrite, "output") ) return 1;
     if ( !CreatePipe (&hPipeInputRead, &hPipeInputWrite, "input") ) return 1;

     HANDLE hProcess = CreateProcess (hPipeInputRead, hPipeOutputWrite);
     if ( hProcess == NULL ) return 1;

     // Now test to capture DOS application output by reading
     // hPipeOutputRead.  Could also write to DOS application
     // standard input by writing to hPipeInputWrite.

     BOOL bFinished = FALSE;
     while ( !bFinished )
     {
          CString sBuf;
          bFinished = !ReadPipe (hPipeOutputRead, sBuf);
          GetQueue()->Add (sBuf, _nUserData);
          
          if ( !bFinished && IsStopSignalled() )
          {
               ::TerminateProcess (hProcess, 1);
               GetQueue()->Add ("Stopped\r\n", _nUserData);
               bFinished = TRUE;
          }
     }
                         
     // Wait for the process to finish.
     ::WaitForSingleObject (hProcess, INFINITE);

     // Close all remaining handles
     ::CloseHandle (hProcess);
     ::CloseHandle (hPipeOutputRead);
     ::CloseHandle (hPipeInputWrite);

     return 0;
}
