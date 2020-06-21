/*!
     \file     Thread.cpp          Thread stuff
     \author   John Young
     \date     16-Dec-99

     Copyright (c) 1999, Young Associates
*/

#include "stdafx.h"
#include "../include/Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CThread::CThread()
{
     m_hTarget = NULL;
     m_pDataQueue = NULL;
     m_idxTool = -1;
     m_pThread = NULL;
     m_bStopThread = false;
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


bool CThread::Start (AFX_THREADPROC fnThread)
{
     if ( fnThread )
     {
          m_pThread = AfxBeginThread (fnThread, (LPVOID) this); 
          OnStart();
     }
     return m_pThread != NULL;
}

void CThread::Stop()
{
     m_bStopThread = true;
}



void CThread::OnStart()
{
     m_bStopThread = false;
}


void CThread::OnFinished()
{
     m_pThread = NULL;
}
