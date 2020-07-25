//
//   CRegKey .h          John Young's Registry class
//

#ifndef   __MFCX_REGKEY_H
#define   __MFCX_REGKEY_H

#include <winreg.h>

#include "../include/XString.h"


namespace MFCX
{

/**	The CRegKey class encapsulates the Registry API and simplies the 
	programming task of reading from and writing to the Registry.

	To write to the registry...

\verbatim
	CRegKey rk (HKEY_CURRENT_USER, "Software\\Thistle Software\\MegaApp", KEY_ALL_ACCESS);
	if ( rk.Open() )
	{
		rk.Set ("Value1", 123);
		rk.Set ("Value2", "The second value");
		rk.Set ("Value3", 987.6543);
	}
\endverbatim

	To read back from the registry...
	
\verbatim
	CRegKey rk (HKEY_CURRENT_USER, "Software\\Thistle Software\\MegaApp");
	
	int	nVal;
	CString sVal2;
	double dVal3;
	
	if ( rk.Open() )
	{
		DWORD dwTmp;
		if ( rk.Query ("Value1", dwTmp) )
			dVal = dwTmp;
		
		CString sTmp;
		if ( rk.Set ("Value2", sTmp) )
			sVal2 = sTmp;
		
		double dTmp;
		if ( rk.Set ("Value3", dTmp) )
			dVal = dTmp;
	}
\endverbatim
	
*/
class CRegKey
{
	DECLARE_DYNAMIC (CRegKey)

// Construction
public:
	CRegKey (REGSAM sam = KEY_READ);
	CRegKey (const HKEY hKey, const TCHAR* pszSubKey, REGSAM sam = KEY_READ);
	CRegKey (const CRegKey& rkKey, const TCHAR* pszSubKey, REGSAM sam = KEY_READ);
	virtual ~CRegKey();

// Attributes
protected:
	HKEY m_hKey;
	LONG	m_lError;
	BOOL m_bOpen;
	REGSAM m_sam;

	CString m_strKey;				///< Stores the name of this key

	CString m_sClass;				///< class name string 
	DWORD  m_dwClassSize;			///< size of class name string
	DWORD  m_dwSubKeys;				///< number of subkeys 
	DWORD  m_dwMaxSubkey;			///< longest subkey name length  
	DWORD  m_dwMaxClass;			///< longest class string length 
	DWORD  m_dwValues;				///< number of value entries 
	DWORD  m_dwMaxValueName;			///< longest value name length 
	DWORD  m_dwMaxValueData;			///< longest value data length 
	DWORD  m_dwSecurityDescriptor;	///< security descriptor length 
	FILETIME  m_ftLastWriteTime;		///< last write time 

// Operations
public:
	BOOL IsOpen() const { return m_bOpen; }
	int	NrSubKeys() { return m_dwSubKeys; }
     int  CountValues() { return m_dwValues; }
	CString GetKeyPath() { return m_strKey; }

     BOOL Open (const CRegKey& regKey, const TCHAR* pszSubKey, REGSAM sam = KEY_READ)
          { return Open (regKey.m_hKey, pszSubKey, sam); }
	BOOL Open (const HKEY hKey, const TCHAR* pszSubKey, REGSAM sam = KEY_READ);
	BOOL Close();

	BOOL Set (const TCHAR* pszValueName, DWORD dwValue);
	BOOL Set (const TCHAR* pszName, const TCHAR* szValue);
	BOOL Set (const TCHAR* pszName, const void* pValue, int nSize);
	BOOL Query (const TCHAR* pszValueName, CString& sValue);
	BOOL Query (const TCHAR* pszValueName, DWORD& dwValue);
	BOOL Query (const TCHAR* pszValueName, void* pData, int nSize);
	BOOL Query (const TCHAR* pszValueName, CString& sData, int& nDataSize);
	int	QueryDataSize (const TCHAR* pszValueName);

	BOOL SetDouble (const TCHAR* pszValueName, double dValue);
	BOOL QueryDouble (const TCHAR* pszValueName, double& dValue);

     BOOL DeleteValue (const TCHAR* pszValue);
     BOOL DeleteAllValues();
     BOOL DeleteAll();
     BOOL DeleteAllKeys();                                  ///< Recursive delete
	BOOL DeleteSubKey (const TCHAR* pszSubKey);
     BOOL DeleteSubBranch (const TCHAR* pszSubKey);         ///< Recursive delete

     BOOL EnumerateKeys (const DWORD dwSubkeyIndex,
                         CString& strSubkeyName,
                         CString& strClassName);
     BOOL EnumerateValues (const DWORD    dwValueIndex,
                           CString&       sValueName,
                           DWORD*         pdwValueType = NULL,
                           BYTE*          pbDataBuffer = NULL,
                           DWORD*         pdwSizeDataBuffer = NULL);

     // Useful helper functions.
     BOOL ReadFont (const TCHAR* szFont, LOGFONT& pLogFont);
     BOOL WriteFont (const TCHAR* szFont, const LOGFONT& pLogFont);

     BOOL CopyTo (CRegKey& rkDest, BOOL bDeleteFirst = FALSE);
     BOOL MoveTo (CRegKey& rkDest);

// Implementation
protected:
	void Initialise();
	BOOL QueryInfo();
	BOOL Set (const TCHAR* pszValueName, 
	          DWORD nValueType, 
	          BYTE* pData, 
	          DWORD nDataSize);
	BOOL Query (const TCHAR* szValueName, 
	            DWORD& nValueType, 
	            BYTE* pData, 
	            DWORD& nDataSize);
};

}    // namespace MFCX
#endif    //   __MFCX_REGKEY_H
