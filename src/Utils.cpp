/*!
     \file     Utils.cpp           General purpose utilities
     
     \author   John Young
     \date     circa 1999
     
     Copyright (c) 1999 Young Associates

TODO: Split this into getargs() and CFontUtils.
*/

#include "stdafx.h"
#include "../include/Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*!  Parses the given command line into the standard C argc and argv arguments
     to the main() routine.

\param    command_line   The command line to parse.
\param    argc           The number of arguments
\param    argv           The arguments
\param    max_args       The maximum number of arguments that getargs is allowed
                         to create.  If this is exceeded, then the rest are 
                         ignored.
*/
// TODO: This doesn't appear to be used anywhere and causes an SEH exception
//       when called from the UtilsTest unit test.  Delete?
/*static*/ 
void CUtils::getargs (char *command_line, int *argc, char *argv[], int max_args)
{
	char *cptr;

	*argc = 0;
	argv[0] = NULL;

	cptr = command_line;
	for (;;)
     {
		while ( isspace (*cptr) )
		{
			*cptr = '\0';
			cptr++;
			if ( !*cptr )
				break;
		}

		if ( *cptr )
		{
			argv[*argc] = cptr;
			(*argc)++;
			argv[*argc] = NULL;

			if ( *argc > max_args )
				return;

			if (*cptr == '"')
			{
				argv[(*argc) - 1] = ++cptr;
	      		do
				{
					cptr++;
					if ( !*cptr )
						break;
				} while ( *cptr != '"' );
				*cptr++ = '\0';
			}
			else while ( !isspace (*cptr) )
			{
				cptr++;
				if ( !*cptr )
					break;
			}
		}
		else break;
	}
}


/*!  Calculates the point size of the given font for the given DC.

\param    lf        The font's LOGFONT structure.
\param    pDC       The device context.  If this is NULL, or is omitted, then 
                    the current active window's DC is used.

\return   The point size.
*/
/*static*/ 
 int CUtils::GetPointSize (const LOGFONT& lf, CDC* pDC /*= NULL*/)
{
     return GetPointSize (&lf, pDC);
}



/*!  Calculates the point size of the given font for the given DC.

\param    lf        The font's LOGFONT structure.
\param    pDC       The device context.  If this is NULL, or is omitted, then 
                    the current active window's DC is used.

\return   The point size.
*/
/*static*/ 
int CUtils::GetPointSize (const LOGFONT* lf, CDC* pDC /*= NULL*/)
{
     if ( lf == NULL )
          return -1;

     if ( lf->lfHeight < 0 )
     {
     	// Calculate given font height in logical units to point size based on 
     	// the given DC
     	if ( pDC == NULL )
     	{
     	     CWnd* pWnd = CWnd::GetActiveWindow();
               pDC = pWnd ? pWnd->GetDC() : NULL;
          }

          int  nPixelsPerLogInch = pDC ? pDC->GetDeviceCaps (LOGPIXELSY) : 0;
          if ( nPixelsPerLogInch )
               return -MulDiv (lf->lfHeight, 72, nPixelsPerLogInch);
     }
     else return lf->lfHeight;
     return -1;
}


