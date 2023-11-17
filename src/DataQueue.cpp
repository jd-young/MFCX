/*!
     \file     DataQueue.cpp         Thread-safe data queue

     \author   John Young
     \date     12-Nov-99
     
     Copyright (c) 1999 Thistle Software
*/

#include "stdafx.h"
#include "../include/DataQueue.h"


CDataQueue::CDataQueue()
{
	CommonConstruct (NULL, 0);
}

CDataQueue::CDataQueue (CWnd* pTo, UINT wmMsg)
{
     CommonConstruct (pTo->m_hWnd, wmMsg);
}


CDataQueue::CDataQueue (HWND hTo, UINT wmMsg)
{
     CommonConstruct (hTo, wmMsg);
}

void CDataQueue::CommonConstruct (HWND hTarget, UINT wmMsg)
{
     m_pHead = NULL;
     m_pTail = NULL;
     SetTargetWnd (hTarget, wmMsg);
}



/*virtual*/ CDataQueue::~CDataQueue()
{
     MakeEmpty();
}

bool CDataQueue::IsEmpty()
{
     return m_pHead == NULL;
}


void CDataQueue::MakeEmpty()
{
     while ( !IsEmpty() )
          Remove();
}

void CDataQueue::SetTargetWnd (HWND hTarget, UINT wmMsg)
{
     m_hTarget = hTarget;
     m_wmMsg = wmMsg;
}


/*!  Adds an item to the back of the queue in a thread-safe way.

\param    psz       The data to add.
\param    lParam    A user-defined data item that the caller wants to be passed
                    to the observer window.
\return   \b true if the data is added, \b false otherwise.
*/
bool CDataQueue::Add (const TCHAR* psz, LPARAM lParam /*= 0*/)
{
     CSingleLock singleLock (&m_critSection, TRUE);
     ASSERT ( singleLock.IsLocked() );
     
     CNode* pNew = new CNode (psz);
     if ( !pNew )
     	return false;

     if ( m_pHead == NULL )
     {
          m_pHead =
          m_pTail = pNew;
     }
     else
     {
          m_pTail->pNext = pNew;
          m_pTail = pNew;
     }
     
     // TODO: Replace with IMessagePoster
     ::PostMessage (m_hTarget, m_wmMsg, (UINT) this, lParam);
     return true;
}

/*!  Removes data from the front of the queue, in a thread-safe way.

\return   The data.
*/
CString CDataQueue::Remove()
{
     CSingleLock singleLock (&m_critSection, TRUE);
     ASSERT ( singleLock.IsLocked() );

     CString sData;
     if ( !IsEmpty() )
     {
          CNode* pFront = m_pHead;
          m_pHead = m_pHead->pNext;
          
          sData = pFront->sData;
          delete pFront;
     }
     return sData;
}

CDataQueue::CNode::CNode (const TCHAR* psz)
{
     sData = psz;
     pNext = NULL;
}

