//
//   DataQueue.h      Thread safe data queue
//

#ifndef   __MFCX_DATAQUEUE_H
#define   __MFCX_DATAQUEUE_H

#include	<afxmt.h>


/// A thread-safe data queue.
/*!  This class allows thread-safe queuing, in other words, one thread can be 
     adding stuff to the tail and another thread can be removing stuff from the 
     front of the queue at the "same" time.

     You use it by constructing an object of type CDataQueue and calling the 
     Create method.  To the create method, you pass either a CWnd pointer or a
     window handle, and a message ID.  Every time something is added to the 
     queue, the window is sent a message to tell it there is something in the
     queue.
*/
class CDataQueue
{
public:
	/// Default queue constructor.
     CDataQueue();
     
     // TODO: Remove this constructor, since it has MFC CWnd.
     /// Constructor.  Will notify the given window with the given message
     /// when something is added.
     [[deprecated]]  // Use CDataQueue(HWND, UINT) instead.
     CDataQueue (CWnd* pTo, UINT wmAddMsg);

     /// Constructor.  Will notify the given window with the given message
     /// when something is added.
     CDataQueue (HWND hTo, UINT wmAddMsg);

	/// Cleans up the queue.
     virtual ~CDataQueue();

	/// Sets the target window for update messages.
	void SetTargetWnd (HWND hLogWnd, UINT wmMsg);

	/// Returns the target window handle.
	HWND GetTarget() const { return m_hTarget; }

/*
     /// Create the queue.
     bool Create (CWnd* pTo, UINT wmAddMsg);

     /// Create the queue.
     bool Create (HWND hTo, UINT wmAddMsg);
*/
     /// Tells wither the queue is empty or not.
     bool IsEmpty() { return m_pHead == NULL; }

     /// Empties the queue.
     void MakeEmpty();
     
     /// Adds new data to the back of the queue.
     bool Add (const TCHAR* psz, LPARAM lParam = 0);

     /// Removes data from the front of the queue.
     CString Remove();

private:
	void CommonConstruct (HWND hTarget, UINT wmMsg);

     /// A node in the queue.
     class CNode
     {
     public:
          CNode();
          CNode (const TCHAR* psz);
          CString sData;                     // <- The data
          CNode* pNext;
     };
     CNode* m_pHead;               ///< The queue's head pointer.
     CNode* m_pTail;               ///< The queue's tail pointer.
     
     HWND m_hTarget;               ///< Where to send messages when data is added.
     UINT m_wmMsg;                 ///< What message to send when data is added.

     CCriticalSection m_critSection;    ///< This ensures thread-safety.
};


#endif    // __MFCX_DATAQUEUE_H
