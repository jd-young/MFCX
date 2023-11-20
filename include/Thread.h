//
//   Thread.h
//

#ifndef   __MFCX_THREAD_H
#define   __MFCX_THREAD_H

#include "DataQueue.h"

using MFCX::CDataQueue;

// TODO: This is not really an MFCX class because of m_idxTool.  That's app
//       related.  Let's remove it and implement it in derived classes if 
//       needed.
/*!  A class that represents a thread.
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
class CThread
{
public:
     [[deprecated("Use the other constructor instead")]] 
     CThread();

     /// Constructs a thread object that sets the Window handle and the message
     /// number to send in a PostMessage() call.
     CThread (HWND hwnd, UINT wmMsg);
     virtual ~CThread();

     /// Returns true if the thread has been started.
     bool IsActive() const { return m_pThread != NULL; }
     
     /// The handle of the target window.
     HWND GetTarget() const { return m_hTarget; }

     // TODO: remove this.
     int GetToolIndex() const { return m_idxTool; }

     /// The thread-safe data queue to send a message to.
     CDataQueue* GetQueue() const { return m_pDataQueue; }
     
     /// Returns true if the Stop() method was called. 
     bool IsStopSignalled() const { return m_bStopThread; }

     /// Starts the thread and passes control to the given function.
     virtual bool Start (AFX_THREADPROC pfn);
     
     /// Sets a flag to tell the thread function to stop.
     virtual void Stop();

     /// Sets the Window handle and the message number to send in a 
     /// PostMessage() call.
     [[deprecated("If we provide this method, then it should really be "
                  "synchronised.  Use the constructor instead.")]]
     void SetHandle (HWND hwnd, UINT wmMsg);

     // TODO: Remove.
     void SetToolIndex (int idx) { m_idxTool = idx; }

     virtual void OnStart();       ///< Called when the thread starts
     virtual void OnFinished();    ///< Called when the thread finishes.

     ///< Gets the return code of the user supplied thread function.
     DWORD GetExitCode() const;

protected:
     // TODO: If we don't need SetHandle(), then we can remove this.
     HWND m_hTarget;               ///< The window to send messages to.
     CDataQueue* m_pDataQueue;     ///< The queue to send back the tool output.

private:
     CWinThread* m_pThread;        ///< So that I can switch myself off.
     bool m_bStopThread;           ///< So that others can switch me off.
     UINT _nRetCode;
     AFX_THREADPROC _fnThread;
     
     // TODO: Remove.
     int m_idxTool;                ///< The index of the tool.

//     static UINT _fnThread (VOID* pParam);
     static UINT WrapperThread (VOID* pParam);
     UINT WrapperThread();
};


#endif    // __MFCX_THREAD_H
