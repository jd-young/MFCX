//
//   CRegKey .h          John Young's Registry class
//

#ifndef   __MFCX_REGKEY_H
#define   __MFCX_REGKEY_H

#include <winreg.h>

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
	bool m_bOpen;
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
	bool IsOpen() const { return m_bOpen; }
	int	NrSubKeys() { return m_dwSubKeys; }
     int  CountValues() { return m_dwValues; }
	CString GetKeyPath() { return m_strKey; }

     bool Open (const CRegKey& regKey, const TCHAR* pszSubKey, REGSAM sam = KEY_READ)
          { return Open (regKey.m_hKey, pszSubKey, sam); }
	bool Open (const HKEY hKey, const TCHAR* pszSubKey, REGSAM sam = KEY_READ);
	bool Close();

	bool Set (const TCHAR* pszValueName, DWORD dwValue);
	bool Set (const TCHAR* pszName, const TCHAR* szValue);
	bool Set (const TCHAR* pszName, const void* pValue, int nSize);
	bool Query (const TCHAR* pszValueName, CString& sValue);
	bool Query (const TCHAR* pszValueName, DWORD& dwValue);
	bool Query (const TCHAR* pszValueName, void* pData, int nSize);
	bool Query (const TCHAR* pszValueName, CString& sData, int& nDataSize);
	int	QueryDataSize (const TCHAR* pszValueName);

	bool SetDouble (const TCHAR* pszValueName, double dValue);
	bool QueryDouble (const TCHAR* pszValueName, double& dValue);

     bool DeleteValue (const TCHAR* pszValue);
     bool DeleteAllValues();
     bool DeleteAll();
     bool DeleteAllKeys();                                  ///< Recursive delete
	bool DeleteSubKey (const TCHAR* pszSubKey);
     bool DeleteSubBranch (const TCHAR* pszSubKey);         ///< Recursive delete

     bool EnumerateKeys (const DWORD dwSubkeyIndex,
                         CString& strSubkeyName,
                         CString& strClassName);
     bool EnumerateValues (const DWORD    dwValueIndex,
                           CString&       sValueName,
                           DWORD*         pdwValueType = NULL,
                           BYTE*          pbDataBuffer = NULL,
                           DWORD*         pdwSizeDataBuffer = NULL);

     // Useful helper functions.
     bool ReadFont (const TCHAR* szFont, LOGFONT& pLogFont);
     bool WriteFont (const TCHAR* szFont, const LOGFONT& pLogFont);

     bool CopyTo (CRegKey& rkDest, bool bDeleteFirst = false);
     bool MoveTo (CRegKey& rkDest, bool bDeleteFirst = false);

     bool RenameKey (const TCHAR* pszFrom, const TCHAR* pszTo);

     CString GetLastError() const;

// Implementation
protected:
	void Initialise();
	bool QueryInfo();
	bool Set (const TCHAR* pszValueName, 
	          DWORD nValueType, 
	          BYTE* pData, 
	          DWORD nDataSize);
	bool Query (const TCHAR* szValueName, 
	            DWORD& nValueType, 
	            BYTE* pData, 
	            DWORD& nDataSize);
     bool DoRenameKey (const TCHAR* pszFrom, const TCHAR* pszTo);
};

}    // namespace MFCX
#endif    //   __MFCX_REGKEY_H
