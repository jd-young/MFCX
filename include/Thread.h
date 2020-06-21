//
//   Thread.h
//

#ifndef   __MFCX_THREAD_H
#define   __MFCX_THREAD_H

#include "DataQueue.h"

class CThread
{
public:
     CThread();
     virtual ~CThread();

     bool IsActive() const { return m_pThread != NULL; }
     HWND GetTarget() const { return m_hTarget; }
     int GetToolIndex() const { return m_idxTool; }
     CDataQueue* GetQueue() const { return m_pDataQueue; }
     bool IsStopSignalled() const { return m_bStopThread; }

     virtual bool Start (AFX_THREADPROC);
     virtual void Stop();

     void SetHandle (HWND hwnd, UINT wmMsg);
     void SetToolIndex (int idx) { m_idxTool = idx; }

     virtual void OnStart();
     virtual void OnFinished();
     
private:
     CWinThread* m_pThread;        ///< So that I can switch myself off.
     CDataQueue* m_pDataQueue;     ///< The queue to send back the tool output.
     HWND m_hTarget;               ///< The window to send messages to.
     int m_idxTool;                ///< The index of the tool.
     bool m_bStopThread;           ///< So that others can switch me off.
};


#endif    // __MFCX_THREAD_H
