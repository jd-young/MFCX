/*!
     \file     Thread.cpp          Thread stuff
     \author   John Young
     \date     16-Dec-99

     Copyright (c) 1999, Thistle Software
*/

#include "stdafx.h"
#include "../include/Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CThread::CThread()
 :   m_hTarget (NULL), m_pDataQueue (nullptr),
     m_pThread (nullptr), m_bStopThread (false), _nRetCode (-1), 
     _fnThread (nullptr), m_idxTool (-1)
{
}

CThread::CThread (HWND hwnd, UINT wmMsg)
 :   m_hTarget (hwnd), m_pDataQueue (new CDataQueue (hwnd, wmMsg)),
     m_pThread (nullptr), m_bStopThread (false), _nRetCode (-1), 
     _fnThread (nullptr), m_idxTool (-1)
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
 * \param fnThread       The function to call on the newly started thread.
 * \return true if the thread was started.
 */
bool CThread::Start (AFX_THREADPROC fnThread)
{
     if ( fnThread )
     {
          _fnThread = fnThread;
          m_pThread = AfxBeginThread (WrapperThread, (LPVOID) this); 
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
     _nRetCode = _fnThread ((void*) this);
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
