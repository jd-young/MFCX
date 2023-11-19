//
//   MockMsgPoster.h    A Windows message poster.
//

#ifndef   MOCKMSGPOSTER_H
#define   MOCKMSGPOSTER_H

#include <string>
#include <vector>
#include "../include/MsgPoster.h"

using std::string;
using std::vector;
using MFCX::IMsgPoster;

/*!
     An interface that posts Windows messages to a specific target window.
 */
class CMockMsgPoster : public IMsgPoster
{
public:
     CMockMsgPoster();
     virtual ~CMockMsgPoster() {};

     bool PostMessage (HWND hWnd, UINT nMsg, WPARAM wParam = 0, 
                       LPARAM lParam = 0) const;

// Mock stuff
     const vector<string>& Messages() const;
};

#endif    //   MOCKMSGPOSTER_H
