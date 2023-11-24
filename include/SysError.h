// SysError.h


#ifndef	_SYSERROR_H
#define	_SYSERROR_H

namespace MFCX {

class CSysError
{
public:
	CSysError();
	
	static CString GetErrorStr (DWORD dwError);
};

}    // namespace MFCX

#endif	// _SYSERROR_H
