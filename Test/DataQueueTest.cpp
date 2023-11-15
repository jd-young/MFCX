/*!
     \file     DataQueueTest.cpp
     \author   John Young
     \date     14 November 2023
     
     Copyright (c) 2023 Thistle Software.
*/

#include "stdafx.h"
#include <gtest/gtest.h>
#include "../include/DataQueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static HWND HWND_TEST = (HWND) -1;
static UINT MSG_ID = WM_APP;

TEST(DataQueueTest, TestDataQueue)
{
     //     CMockMsgPoster poster;

//     CDataQueue queue (poster);
     CDataQueue queue;
     queue.SetTargetWnd (HWND_TEST, MSG_ID);

     EXPECT_TRUE (queue.IsEmpty());
     
     queue.Add ("This is the first message");
     queue.Add ("This is the second message");
     EXPECT_FALSE (queue.IsEmpty());

     EXPECT_STREQ ("This is the first message", queue.Remove());
     EXPECT_STREQ ("This is the second message", queue.Remove());
     
     EXPECT_TRUE (queue.IsEmpty());
}

TEST(DataQueueTest, TestEmpty)
{
     //     CMockMsgPoster poster;

//     CDataQueue queue (poster);
     CDataQueue queue;
     queue.SetTargetWnd (HWND_TEST, MSG_ID);

     EXPECT_TRUE (queue.IsEmpty());
     
     queue.Add ("This is the first message");
     queue.Add ("This is the second message");
     EXPECT_FALSE (queue.IsEmpty());

     queue.MakeEmpty();
          
     EXPECT_TRUE (queue.IsEmpty());
}
