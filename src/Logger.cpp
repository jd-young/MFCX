/**
     \file     Logger.cpp     Logging support.

     CLogger::LogEvent  - printf look-alike but prints to the log file.  Use the
     \b LOG macro in your program to simplify switching on and off logging for
     differnent builds.

     \author   John Young
     \date     9 October 2000

     Copyright (c) 2000 Thistle Software
*/

#include "stdafx.h"
#include  <string.h>
#include  <time.h>
#include  <stdio.h>
#include  <process.h>
#include  "../include/Logger.h"

#pragma comment (exestr, "Logger.cpp v1.0")

#ifdef	MFCX_LOG_USELOG


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CLogger logger;

/*! Constructs a logger.

 */
CLogger::CLogger()
{
     m_logfl = NULL;
     m_bEnabled = true;
     m_bOpenFailed = false;
     m_bShowTime = true;
     m_bShowThreadID = false;
}


CLogger::~CLogger()
{
     CloseLogFile();
}


/**	Opens the given file as the log file.  This will close the existing file 
	(if opened).

\param	pszFilename		The path of the file to open as the log file.
\return	A pointer to the newly opened FILE, or NULL if there was a problem.
*/
FILE* CLogger::OpenLogfile (const TCHAR* pszFilename)
{
     if ( m_logfl != NULL )
          CloseLogFile();

     TCHAR szFilename [_MAX_PATH];
     if ( pszFilename == NULL )
     {
          pszFilename = szFilename;
          ::GetModuleFileName (NULL, szFilename, sizeof szFilename);
          TCHAR* ext = _tcsrchr (szFilename, '.');
          _tcscpy (ext, ".log");
     }

     CSingleLock lock (&m_crit);
     m_logfl = fopen (pszFilename, "a+");
     return m_logfl;
}



/**	Closes the log file.
*/
void CLogger::CloseLogFile()
{
     if ( m_logfl )
     {
          fclose (m_logfl);
          m_logfl = NULL;
     }
}


/**  Logs the given text to a log file, with the current time.

\param	format       Format to print (same as printf)
\return   The number of characters written.
*/
int CLogger::LogEvent (const TCHAR* format, ...)
{
     if ( m_bOpenFailed || !m_bEnabled )
          return 0;

     if ( m_logfl == NULL )
          if ( OpenLogfile (NULL) == NULL )
          {
               m_bOpenFailed = true;
               return 0;
          }

     CSingleLock lock (&m_crit);
     if ( m_bShowTime )
          PrintTime();
     if ( m_bShowThreadID )
          PrintThreadID();

     va_list arglist;
     va_start (arglist, format);
     int count = vfprintf (m_logfl, format, arglist);

#ifdef    _DEBUG
	int nBuf;
	TCHAR szBuffer[512];
	nBuf = _vsntprintf (szBuffer, (sizeof szBuffer) / (sizeof TCHAR), format, arglist);
     TRACE0 (szBuffer);
#endif

     va_end (arglist);

     fflush (m_logfl);

     return count;
}

int  CLogger::LogStack()
{
     return 0;
}



/**  Print the current time (without new line).*/
void CLogger::PrintTime()
{
     if ( !m_logfl )
          return;

#if	0
     char timebuf [128];
     time_t now = time (NULL);
     struct tm* pTime = localtime (&now);

     _tcsftime (timebuf, sizeof timebuf, "%H:%M:%S %d-%b-%y", pTime);

     fprintf (m_logfl, "%s: ", timebuf);
     TRACE ("%s: ", timebuf);
#endif

/*	static TCHAR arrMonths[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
						    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
*/

	SYSTEMTIME time;
	::GetLocalTime (&time);

	CString sTime;
	sTime.Format ("%04d-%02d-%02d %02d:%02d:%02d.%03d", 
				time.wYear, time.wMonth, time.wDay, 
				time.wHour, time.wMinute, time.wSecond, 
				time.wMilliseconds);

     fprintf (m_logfl, "%s: ", (LPCTSTR) sTime);
     TRACE ("%s: ", sTime);
}


void  CLogger::PrintThreadID()
{
     if ( !m_logfl )
          return;

     CWinThread* pThread = AfxGetThread();
     fprintf (m_logfl, "%d-%d: ", _getpid(), pThread->m_nThreadID);
     TRACE ("%d-%d: ", _getpid(), pThread->m_nThreadID);
}

#endif	// MFCX_LOG_USELOG