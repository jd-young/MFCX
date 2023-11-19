//
//   DataQueue.h      Thread safe data queue
//

#ifndef   __MFCX_DATAQUEUE_H
#define   __MFCX_DATAQUEUE_H

namespace MFCX {

class IMsgPoster;

/*!  A thread-safe data queue.
     
     This class allows thread-safe queuing, in other words, one thread can be 
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
     CDataQueue (const CDataQueue&) = delete;

public:
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

     /// Tells whether the queue is empty or not.
     bool IsEmpty();

     /// Empties the queue.
     void MakeEmpty();
     
     /// Adds new data to the back of the queue.
     bool Add (const TCHAR* psz, LPARAM lParam = 0);

     /// Removes data from the front of the queue.
     CString Remove();

#ifndef   GTEST    // To allow unit testing.
private:
#endif

     /// Constructor that allows us to supply a poster for testing.
     CDataQueue (HWND hTo, UINT wmMsg, IMsgPoster* pPoster);

     bool _Add (const TCHAR* psz, LPARAM lParam = 0);
     CString _Remove();

     /// A node in the queue.
     class CNode
     {
     public:
          CNode (const TCHAR* psz);
          CString sData;                     // <- The data
          CNode* pNext;
     };
     CNode* m_pHead;               ///< The queue's head pointer.
     CNode* m_pTail;               ///< The queue's tail pointer.
     
     HWND m_hTarget;               ///< Where to send messages when data is added.
     UINT m_wmMsg;                 ///< What message to send when data is added.

     IMsgPoster* _pPoster;         ///< The Windows message poster.

     CCriticalSection m_critSection;    ///< This ensures thread-safety.
};

} // namespace MFCX

#endif    // __MFCX_DATAQUEUE_H
