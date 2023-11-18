/*!
     \file     MockMsgPoster.cpp
     \author   John Young
     \date     17 November 2023
     
     Copyright (c) 2023 Thistle Software
*/


#include "stdafx.h"
#include "MockMsgPoster.h"
#include "../include/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MFCX::CStringUtil;

static vector<string> msgs;

bool CMockMsgPoster::PostMessage (HWND hWnd, UINT nMsg, WPARAM wParam /*= 0*/, 
                                  LPARAM lParam /*= 0*/) const
{
     string sMsg = CStringUtil::Format ("PostMessage (0x%x, %d, 0x%x, 0x%x)", 
                                        hWnd, nMsg, wParam, lParam);
     msgs.push_back (sMsg);
     return true;
}

const vector<string>& CMockMsgPoster::Messages() const
{
     return msgs;
}
