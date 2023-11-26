/*!
     \file     Sound.cpp     Sound methods

     \author   John Young
     \date     26-Mar-1998

     Copyright (c) 1998 Thistle Software
     All rights reserved

     PlayWaveSound (UINT nID);
     PlayWaveSound (LPTCSTR pszRes);
          Plays a wave file from the resources.
*/

#include "stdafx.h"
#include <MMSystem.h>
#include "../include/Sound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool PlayWaveSound (UINT nID)
{
     return PlayWaveSound (MAKEINTRESOURCE (nID));
}


/*!  Plays a WAV sound.  

     The WAV is either stored as a resource in the EXE or is the given file 
     name.  It looks for a resource first.

\param    pszWave   The name of the stored resource, or a filename.  The stored 
                    resources are given precedence.
\return   \b true if successful.
*/
bool PlayWaveSound (const TCHAR* pszWave)
{
     bool bRet = false; 
	HRSRC   hRes;       // resource handle to wave file
	HGLOBAL hData;
     HINSTANCE hInst = AfxGetResourceHandle();

	if ( (hRes = ::FindResource (hInst, pszWave, _T("sound"))) &&
	     (hData = ::LoadResource (hInst, hRes)) )
	{
		// found the resource, play it
		bRet = ::sndPlaySound ((LPCTSTR)::LockResource (hData), 
                                 SND_MEMORY | SND_ASYNC | SND_NOSTOP) == TRUE;
		FreeResource (hData);
	}
     else
     {
          bRet = ::sndPlaySound (pszWave, SND_ASYNC | SND_NOSTOP) == TRUE;
     }

     return bRet;
}
