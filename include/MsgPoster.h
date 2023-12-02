//
//   MsgPoster.h    A Windows message poster.
//

#ifndef   __MFCX_MSGPOSTER_H
#define   __MFCX_MSGPOSTER_H

namespace MFCX {
/*!
     An interface that posts Windows messages to a specific target window.
 */
class /*interface*/ IMsgPoster
{
public:
     virtual ~IMsgPoster() {};

     virtual bool PostMessage (HWND hWnd, UINT nMsg, WPARAM wParam = 0, 
                               LPARAM lParam = 0) const = 0;
};


/*!  A message poster.
 *
 *   This uses the Windows ::PostMessage() API to post messages.
 */
class CMsgPoster : public IMsgPoster
{
public:
     CMsgPoster() {};
     ~CMsgPoster() {};
     
     bool PostMessage (HWND hWnd, UINT nMsg, WPARAM wParam = 0, 
                       LPARAM lParam = 0) const;
};


} // namespace MFCX
#endif    //   __MFCX_MSGPOSTER_H
