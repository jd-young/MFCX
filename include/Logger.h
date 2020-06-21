// Logger.h

#ifndef   __MFCX_LOGGER_H
#define   __MFCX_LOGGER_H

#ifdef    MFCX_LOG_USELOG
#    define   LOG             logger.LogEvent
#    define   LOG_SHOWTIME    logger.ShowTime
#    define   LOG_SHOWTHREAD  logger.ShowThreadID
#	define   LOG_SHOWSTACK   logger.LogStack
#else
#    define   LOG             (void)
#    define   LOG_SHOWTIME()
#    define   LOG_SHOWTHREAD()
#	define   LOG_SHOWSTACK()
#endif

#ifdef    MFCX_LOG_USELOG
#include <afxmt.h>       // To get multi-thread stuff


/**	A thread-safe logger class.  This is used for general purpose application 
	logging.  There is generally only one logger object, visible to all the 
	application - \b logger.  To use the logger, simply define the 
	MFCX_LOG_USELOG directive and do something like the following...
	
\verbatim

	#define	MFCX_LOG_USELOG
	
	void MyApp::InitInstance()
	{
		// Override the default log location.
		logger.OpenLogFile ("c:\\tmp\\Error.log");
	
		logger.LovEvent ("MyApp started (version %d.%d)\n", 
							MajorVer(), MinorVer());
	
		:
		LOG ("Something bad happened - %s\n", GetError());
		:
	}
\endverbatim

	You can use the handy define \b LOG, which is just an alias for \b 
	logger.LogEvent.
	
	The default pathname for the log file is in the application's directory, 
	and the filename is the application's name, with a '.log' extension.  Use 
	OpenLogFile to change this.
	
	By default the logger prints the date and time, but not the process and 
	thread IDs.  If you are working with multithreaded applications it may be 
	useful to turn this on using ShowThreadID.
	
	The output of the logger looks something like...
	
\verbatim
	

\endverbatim

*/
class CLogger
{
public:
     CLogger();
     ~CLogger();
     
     /// Opens the give file as the log file.
     FILE* OpenLogfile (const TCHAR* pszLogfile);
     int  LogEvent (const TCHAR* format, ...);
     int  LogStack();
     void Enable (bool f = true) { m_bEnabled = f; }
     void ShowTime (bool f = true) { m_bShowTime = f; }
     void ShowThreadID (bool f = true) { m_bShowThreadID = f; }

private:
     // Implementation     
     void  CloseLogFile();
     void  PrintTime();
     void  PrintThreadID();

     FILE* m_logfl;
     bool  m_bEnabled;
     bool  m_bOpenFailed;
     bool  m_bShowTime;
     bool  m_bShowThreadID;
     CCriticalSection m_crit;
};


extern CLogger logger;

#endif    // MFCX_LOG_USELOG
#endif    // __MFCX_LOGGER_H
