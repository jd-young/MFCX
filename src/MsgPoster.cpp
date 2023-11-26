/*!
	\file	MsgPoster.cpp

	\author	John Young
	\date	23 September 2021
	
	Copyright (c) 2021 Thistle Software.
*/


#include "stdafx.h"
#include "../include/MsgPoster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace MFCX {

bool CMsgPoster::PostMessage (HWND hWnd, UINT nMsg, 
                              WPARAM wParam /*= 0*/, 
                              LPARAM lParam /*= 0*/) const
{
     return ::PostMessage (hWnd, nMsg, wParam, lParam);
}

}    // namespace MFCX