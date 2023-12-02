// SysError.h


#ifndef	__MFCX_SYSERROR_H
#define	__MFCX_SYSERROR_H

namespace MFCX {

class CSysError
{
public:
	CSysError();
	
	static void GetLastError (DWORD& dwError, CString& sError);
	static CString GetErrorStr (DWORD dwError);
};

}    // namespace MFCX

#endif	// __MFCX_SYSERROR_H
