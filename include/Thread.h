//
//   Thread.h
//

#ifndef   __MFCX_THREAD_H
#define   __MFCX_THREAD_H

#include <map>
#include <string>
#include "DataQueue.h"

using std::map;
using std::string;
using MFCX::CDataQueue;

/*!  A class that represents a thread.
 *
 *   This class can start up thread in three different ways:
 *     - Start up a thread that runs a user supplied function.
 *     - Start up a separate command-line process and send its output to a 
 *       specified window.
 *     - Start up a separate command-line or GUI process as a standalone 
 *       process.
 *
 *   Derive a class from this, provide a static function that takes a void* 
 *   pointer and returns a UINT (AFX_THREADPROC) to Start().  This will spawn
 *   a new thread and call the function.  THe void* will be a pointer to the 
 *   derived class object.
 *
 *   To send a data to a particular window (specified with the HWND in 
 *   SetHandle()), call the 
 */
// TODO: Make this templated to be able to put data other than CString objects
//       on the data queue.
// TODO: Split this into separate classes; user-supplied function, create 
//       process, and stand-alone process.
class CThread
{
     CThread (const CThread&) = delete; // No copy construction. 
     CThread& operator= (const CThread&) = delete;

public:
     CThread();

     /// Constructs a thread object that sets the Window handle and the message
     /// number to send in a PostMessage() call.
     CThread (HWND hwnd, UINT wmMsg);
     virtual ~CThread();

     /// Returns true if the thread has been started.
     virtual bool IsActive() const { return m_pThread != NULL; }
     
     /// The handle of the target window.
     HWND GetTarget() const { return m_hTarget; }

     /// The thread-safe data queue to send a message to.
     CDataQueue* GetQueue() const { return m_pDataQueue; }
     
     /// Returns true if the Stop() method was called. 
     bool IsStopSignalled() const { return m_bStopThread; }

     /// Starts the thread and passes control to the given function.
     virtual bool Start (AFX_THREADPROC pFunc);
     
     /// Starts the given command.
     virtual bool StartCliProcess (const TCHAR* pszCmd, 
                                   const TCHAR* pszArgs = nullptr,
                                   const TCHAR* pszDir = nullptr,
                                   const map<string, string>* pEnvVars = nullptr);

     /// Sets a flag to tell the thread function to stop.
     virtual void Stop();

     /// Sets the Window handle and the message number to send in a 
     /// PostMessage() call.
     [[deprecated("If we provide this method, then it should really be "
                  "synchronised.  Use the constructor instead.")]]
     void SetHandle (HWND hwnd, UINT wmMsg);

     LPARAM GetUserData() const { return _nUserData; }

     /// Set user data to be sent to a window in the thread-safe queue.
     void SetUserData (LPARAM nUserData) { _nUserData = nUserData; }
     
     virtual void OnStart();       ///< Called when the thread starts
     virtual void OnFinished();    ///< Called when the thread finishes.

     ///< Wait for the thread to terminate.
     DWORD Join();
     
protected:
     // TODO: If we don't need SetHandle(), then we can remove this.
     HWND m_hTarget;               ///< The window to send messages to.
     CWinThread* m_pThread;        ///< So that I can switch myself off.
     CDataQueue* m_pDataQueue;     ///< The queue to send back the tool output.

     CString _sCmd;                ///< The command line to run as a separate process.
     CString _sDir;                ///< The directory to start the thread in.
     map<string, string> _mapEnvs; ///< The environment variables for the process.

     LPARAM _nUserData;            ///< User supplied data.  This is sent with 
                                   ///< messages to windows by the data-queue.

private:
     bool m_bStopThread;           ///< So that others can switch me off.
     AFX_THREADPROC _fnThread;     ///< A user supplied function to run on a separate thread.

     static UINT WrapperThread (void* lParam);
     UINT WrapperThread();
     
     static UINT CliProcess (void* lParam);
     UINT CliProcess();

     bool CreatePipe (HANDLE* pReadPipe, HANDLE* pWritePipe, const TCHAR* pszDesc);
     HANDLE CreateProcess (HANDLE hPipeInputRead, HANDLE hPipeOutputWrite);
     BOOL ReadPipe (HANDLE hPipeOutputRead, CString& sBuf);
};


#endif    // __MFCX_THREAD_H
