// SysError.h


#ifndef	_SYSERROR_H
#define	_SYSERROR_H

namespace MFCX {

class CSysError
{
public:
	CSysError();
	
	static void GetLastError (DWORD& dwError, CString& sError);
	static CString GetErrorStr (DWORD dwError);
};

}    // namespace MFCX

#endif	// _SYSERROR_H
