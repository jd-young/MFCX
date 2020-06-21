/*!
     \file GKeyboard.cpp      The global keyboard object.

     \author   John Young
     \date     13 Nov 95

     Modified from ideas by Paul DiLascia's 1993 MFX
*/

#include "StdAfx.h"
#include "../include/GKeyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Instantiate global objects
GKeyboard   theKeyboard;


BOOL GKeyboard::IsPressed (int vkey) const
{
	int nKey = State (vkey);
	return (nKey & 0x8000) ? TRUE : FALSE;
}
