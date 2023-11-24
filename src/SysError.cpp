/*!
	\file	SysError.cpp
	\author	John Young
	\date	29 April 2002
	
	Copyright (c) 2002 Thistle Software.
*/


#include "stdafx.h"
#include "../include/SysError.h"
#include "../include/XString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace MFCX {

/*static*/ CString CSysError::GetErrorStr (DWORD dwError)
{
	TCHAR* pMsgBuf;
	::FormatMessage( 
	    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	    FORMAT_MESSAGE_FROM_SYSTEM | 
	    FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,                                         // source
	    dwError,                                      // The ID of the error
	    MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),   // Default language
	    (LPTSTR) &pMsgBuf,                            // Receives the message
	    0,                                            // Size of the buffer
	    NULL                                          // 
	);
	CXString sSysErr = pMsgBuf;
	sSysErr.Chomp();
	LocalFree (pMsgBuf);		// Free the buffer.

	return sSysErr;
}

} // namespace MFCX
