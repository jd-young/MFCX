/**
	\file	RegKey.cpp	Registry Keys class (CRegKey) implementation file

	Written by John Young, 2 April 1996

	The CRegKey class encapsulates the Registry API and simplies the 
	programming task of reading from and writing to the Registry.

	HISTORY
\verbatim
     Version   Date      Description
     ------------------------------------------------------------------------
     1.0      2-Apr-96   Created.  Inspired by "Take Advantage of Windows 95
                         with the Registry", by Jim Ross (Oct '95 Visual C++
                         Developer).
     1.1     10-Dec-97   Updated for Unicode support.
             21-Jan-98   Added ReadFont() and WriteFont() helpers
              3-Mar-98   Added DeleteAllValues()
              2-Jul-98   Added DeleteAllKeys() and DeleteAll()
             24-Feb-99   Added CopyTo() and MoveTo().
\endverbatim
*/

#include "stdafx.h"
#include <winreg.h>
#include "../include/RegKey.h"

#define	RK_NOTOPEN	-1

#define	RK_MAXNAMELENGTH	256l
#define	RK_MAXDATALENGTH	1024l



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegKey
IMPLEMENT_DYNAMIC (CRegKey, CObject)


/**	Creates an empty CRegKey object.  If you use this version, you need to call 
	Open to perform any operations on the registry key.
	
\param	sam			The Security Access Mask...

					- \b KEY_CREATE_LINK Permission to create a symbolic 
						link.
					- \b KEY_CREATE_SUB_KEY Permission to create subkeys. 
					- \b KEY_ENUMERATE_SUB_KEYS Permission to enumerate 
						subkeys.
					- \b KEY_EXECUTE Permission for read access.
					- \b KEY_NOTIFY Permission for change notification.
					- \b KEY_QUERY_VALUE Permission to query subkey data.
					- \b KEY_SET_VALUE Permission to set subkey data.
					- \b KEY_ALL_ACCESS Combines the KEY_QUERY_VALUE, 
						KEY_ENUMERATE_SUB_KEYS, KEY_NOTIFY, 
						KEY_CREATE_SUB_KEY, KEY_CREATE_LINK, and 
						KEY_SET_VALUE access rights, plus all the standard 
						access rights except SYNCHRONIZE.
					- \b KEY_READ Combines the STANDARD_RIGHTS_READ, 
						KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, and 
						KEY_NOTIFY access rights.
					- \b KEY_WRITE Combines the STANDARD_RIGHTS_WRITE, 
						KEY_SET_VALUE, and KEY_CREATE_SUB_KEY access 
						rights.
*/
CRegKey::CRegKey (REGSAM sam /* = KEY_READ */)
{
	Initialise();
	m_sam = sam;
}



/**	Creates a new key from the key hKey\\pszSubKey.  The hKey MUST be one of 
	the five pre-defined keys.

\param	hKey			This is one of...

					- HKEY_CLASSES_ROOT
					- HKEY_CURRENT_USER
					- HKEY_LOCAL_MACHINE
					- HKEY_USERS
					- HKEY_CURRENT_CONFIG

\param	pszSubKey 	The sub-key under the main hKey hive.  This can be 
					NULL, in which case the key is the specified hKey hive.
\param	sam			The Security Access Mask...

					- \b KEY_CREATE_LINK Permission to create a symbolic 
						link.
					- \b KEY_CREATE_SUB_KEY Permission to create subkeys. 
					- \b KEY_ENUMERATE_SUB_KEYS Permission to enumerate 
						subkeys.
					- \b KEY_EXECUTE Permission for read access.
					- \b KEY_NOTIFY Permission for change notification.
					- \b KEY_QUERY_VALUE Permission to query subkey data.
					- \b KEY_SET_VALUE Permission to set subkey data.
					- \b KEY_ALL_ACCESS Combines the KEY_QUERY_VALUE, 
						KEY_ENUMERATE_SUB_KEYS, KEY_NOTIFY, 
						KEY_CREATE_SUB_KEY, KEY_CREATE_LINK, and 
						KEY_SET_VALUE access rights, plus all the standard 
						access rights except SYNCHRONIZE.
					- \b KEY_READ Combines the STANDARD_RIGHTS_READ, 
						KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, and 
						KEY_NOTIFY access rights.
					- \b KEY_WRITE Combines the STANDARD_RIGHTS_WRITE, 
						KEY_SET_VALUE, and KEY_CREATE_SUB_KEY access 
						rights.
*/
CRegKey::CRegKey (const HKEY hKey, const TCHAR* pszSubKey, REGSAM sam /* = KEY_READ */)
{
	if ( hKey == HKEY_CLASSES_ROOT ||
		hKey == HKEY_CURRENT_USER ||
		hKey == HKEY_LOCAL_MACHINE ||
		hKey == HKEY_USERS ||
		hKey == HKEY_CURRENT_CONFIG ||
		hKey == HKEY_DYN_DATA )
	{
		m_bOpen = Open (hKey, pszSubKey, sam);
	     m_strKey = pszSubKey;
	}
	else ASSERT (FALSE);
}


/**	Creates / Opens a sub key under the given key.

\param	rkKey		The key under which to create a new key or open an 
					existing key.

\param	pszSubKey 	The sub-key under the main hKey hive.  This can be 
					NULL, in which case the key is the specified hKey hive.
\param	sam			The Security Access Mask...

					- \b KEY_CREATE_LINK Permission to create a symbolic 
						link.
					- \b KEY_CREATE_SUB_KEY Permission to create subkeys. 
					- \b KEY_ENUMERATE_SUB_KEYS Permission to enumerate 
						subkeys.
					- \b KEY_EXECUTE Permission for read access.
					- \b KEY_NOTIFY Permission for change notification.
					- \b KEY_QUERY_VALUE Permission to query subkey data.
					- \b KEY_SET_VALUE Permission to set subkey data.
					- \b KEY_ALL_ACCESS Combines the KEY_QUERY_VALUE, 
						KEY_ENUMERATE_SUB_KEYS, KEY_NOTIFY, 
						KEY_CREATE_SUB_KEY, KEY_CREATE_LINK, and 
						KEY_SET_VALUE access rights, plus all the standard 
						access rights except SYNCHRONIZE.
					- \b KEY_READ Combines the STANDARD_RIGHTS_READ, 
						KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, and 
						KEY_NOTIFY access rights.
					- \b KEY_WRITE Combines the STANDARD_RIGHTS_WRITE, 
						KEY_SET_VALUE, and KEY_CREATE_SUB_KEY access 
						rights.
*/
CRegKey::CRegKey (const CRegKey& rkKey, const TCHAR* pszSubKey, 
			   REGSAM sam /* = KEY_READ */)
{
	ASSERT ( pszSubKey ) ;
	ASSERT ( rkKey.m_bOpen );

	m_bOpen = Open (rkKey.m_hKey, pszSubKey, sam);
     m_strKey = rkKey.m_strKey;
     if ( *pszSubKey != '\\' )
     	m_strKey += '\\'; 
     m_strKey += pszSubKey;
}

CRegKey::~CRegKey ()
{
	if ( m_hKey )
		Close();
}

/**	Initialises the current key.
*/
void CRegKey::Initialise()
{
	m_hKey = NULL;
	m_lError = RK_NOTOPEN;

	m_sam = 0;

	m_bOpen = FALSE;
	m_dwClassSize = 
	m_dwSubKeys = 
	m_dwMaxSubkey = 
	m_dwMaxClass = 
	m_dwValues = 
	m_dwMaxValueName = 
	m_dwMaxValueData = 
	m_dwSecurityDescriptor = 0;
	m_ftLastWriteTime.dwLowDateTime = 0; 
	m_ftLastWriteTime.dwHighDateTime = 0; 

	m_strKey.Empty();
	m_sClass.Empty();
}



/**	Opens / Creates a key under the hKey root.

	Note that if pszSubKey doesn't have a leading backslash, then one is added.

\param	hKey			Must be one of the pre-defined keys (see CRegKey::CRegKey).
\param	pszSubKey		The sub-key to open.
\param	sam			The desired Security Access Mask (see CRegKey::CRegKey).

\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Open (const HKEY hKey, const TCHAR* pszSubKey, REGSAM sam /* = KEY_READ */)
{
	ASSERT ( hKey != NULL );	  // must have valid HKEY

	Initialise();
	m_lError = RK_NOTOPEN;	  // we're not open yet
	m_sam = sam;

	if ( pszSubKey )
	{
          ASSERT ( pszSubKey [0] != '\\' );       // Subkey cannot start with a backslash
		// try to open the key
          if ( m_sam & KEY_CREATE_SUB_KEY )
		{
			DWORD dwDisposition;
			m_lError = ::RegCreateKeyEx (hKey, pszSubKey, 0, NULL,
									REG_OPTION_NON_VOLATILE, m_sam,
									NULL, &m_hKey, &dwDisposition);
		}
		else	m_lError = ::RegOpenKeyEx (hKey, pszSubKey, 0, m_sam, &m_hKey);
	}
	else m_lError = ERROR_SUCCESS;

	// if successful, store the new hKey;
	if ( m_lError == ERROR_SUCCESS )
	{
		m_bOpen = TRUE;
		QueryInfo();
	}
	else	m_hKey = NULL;

	return m_lError == ERROR_SUCCESS;
}


/**	Deletes all the keys and values from the current key.

\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::DeleteAll()
{
     return DeleteAllKeys() && DeleteAllValues();
}


/**	Deletes all the keys from the current key.

\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::DeleteAllKeys()
{
     // Deletes all the keys in 'this'.
	ASSERT ( m_hKey != NULL && 
			m_bOpen && 
			(m_sam & KEY_WRITE) );	     // must have valid HKEY

     // First of all collect up all the sub-keys and 
	DWORD dwCount = 0;
     CString sKey;
	CString sClass;
     CStringList listKeys;
	while ( EnumerateKeys (dwCount++, sKey, sClass) )
		listKeys.AddTail (sKey);

     // key deletion pass...
     BOOL bRet = TRUE;
     for (POSITION pos = listKeys.GetHeadPosition(); pos; )
     {
          CString sKey = listKeys.GetNext (pos);
          if ( !DeleteSubBranch (sKey) )
               bRet = FALSE;
     }
     return bRet;
}



/**	Deletes the specified sub-branch.

     Deletes the entire branch regardless of whether we're running under WinNT 
     (with its whimpy "won't delete a key with sub-keys" policy) or Win95 (which 
     will delete everything under a deleted key).  This calls DeleteSubKey.

\param	pszSubKey		The name of the branch to delete.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::DeleteSubBranch (const TCHAR* pszSubKey)
{
     //TRACE (_T("\"%s\"->DeleteSubBranch (\"%s\")\n"), m_strKey, pszSubKey);

	ASSERT ( m_hKey != NULL && 
			m_bOpen && 
			(m_sam & KEY_WRITE) );	     // must have valid HKEY

     CRegKey rkSub (*this, pszSubKey, KEY_ALL_ACCESS);

     if ( !rkSub.IsOpen() )
          return ERROR_SUCCESS;	// No' open - so it disnae exist - therefore 
          					// we micht as weel 'a' deleted it!
     
	DWORD dwCount = 0;
     CString sKey;
	CString sClass;
     CStringList listSubKeys;
	while ( rkSub.EnumerateKeys (dwCount++, sKey, sClass) )
		listSubKeys.AddTail (sKey);

     // key deletion pass...
     for (POSITION pos = listSubKeys.GetHeadPosition(); pos; )
     {
          CString sSubKey = listSubKeys.GetNext (pos);
          rkSub.DeleteSubBranch (sSubKey);
     }
     
     LONG lRet = ::RegDeleteKey (m_hKey, pszSubKey);
     return lRet == ERROR_SUCCESS;
}

/**	Deletes the given sub-key (but only if it is empty).

	Okay, heads up!  This routine is fairly well dangerous and will royally 
	**** up THE REGISTRY if you don't use this correctly.

	If you are reading this with the intent to delete stuff from the registry, 
	how about backing it up now?  Once again, HOW ABOUT BACKING IT UP NOW!!!!!!  
	You will not regret it.  On the other hand you might not regret not backing 
	up the registry now, but you'll be taking a chance.

\param	pszSubKey		The name of the branch to delete.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::DeleteSubKey (const TCHAR* pszSubKey)
{
	//TRACE ("CRegKey::Delete (\"%s\")\n", pszSubKey);
	ASSERT ( m_hKey != NULL && 
			m_bOpen && 
			(m_sam & KEY_WRITE) );	     // must have valid HKEY

	if ( !m_bOpen || m_dwSubKeys > 0 )      // Cannot delete a key with sub-
									// keys (default NT behaviour)
		return FALSE;

     m_lError = ::RegDeleteKey (m_hKey, pszSubKey);

     QueryInfo();		// Update the info because we may have changed something

	return m_lError != ERROR_SUCCESS;
	//	RegDeleteValue (HKEY_CURRENT_USER, NULL); - just kidding!
}



/**	Gets information about contents of this key and stores it internally.

\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::QueryInfo()
{
	ASSERT (m_sam && KEY_QUERY_VALUE);

	m_lError = ::RegQueryInfoKey (m_hKey, m_sClass.GetBufferSetLength (RK_MAXNAMELENGTH),
							 &m_dwClassSize, NULL, &m_dwSubKeys, &m_dwMaxSubkey,
							 &m_dwMaxClass, &m_dwValues, &m_dwMaxValueName,
							 &m_dwMaxValueData, &m_dwSecurityDescriptor,
							 &m_ftLastWriteTime);
	return m_lError == ERROR_SUCCESS;
}




/**	Set a value under this subkey, generic type.  

	This routine is protected to prevent users calling this by mistake.

\param	pszValueName		The name of the value to store.
\param	nValueType		The type of value.  This is the same as used by 
						::RegSetValueEx...
						
						- \b REG_BINARY Binary data in any form. 
						- \b REG_DWORD A 32-bit number. 
						- \b REG_DWORD_LITTLE_ENDIAN A 32-bit number in 
							little-endian format. This is equivalent to 
							REG_DWORD. 
							
							In little-endian format, a multi-byte value 
							is stored in memory from the lowest byte (the 
							"little end") to the highest byte. For 
							example, the value 0x12345678 is stored as 
							(0x78 0x56 0x34 0x12) in little-endian format.
						
							Windows NT/Windows 2000, Windows 95, and 
							Windows 98 are designed to run on little-
							endian computer architectures. A user may 
							connect to computers that have big-endian 
							architectures, such as some UNIX systems. 
						 
							REG_DWORD_BIG_ENDIAN A 32-bit number in
							big-endian format. In big-endian format, a
							multi-byte value is stored in memory from the
							highest byte (the "big end") to the lowest
							byte. For example, the value 0x12345678 is
							stored as (0x12 0x34 0x56 0x78) in big-endian
							format.
						 
						- \b REG_EXPAND_SZ A null-terminated string
							that contains unexpanded references to
							environment variables (for example,
							"%PATH%"). It will be a Unicode or ANSI
							string depending on whether you use the
							Unicode or ANSI functions. To expand the
							environment variable references, use the
							ExpandEnvironmentStrings function. REG_LINK A
							Unicode symbolic link. Used internally;
							applications should not use this type.
							REG_MULTI_SZ An array of null-terminated
							strings, terminated by two null characters.
							REG_NONE No defined value type. REG_QWORD A
							64-bit number. REG_QWORD_LITTLE_ENDIAN A
							64-bit number in little-endian format. This
							is equivalent to REG_QWORD. REG_RESOURCE_LIST
							A device-driver resource list. REG_SZ A
							null-terminated string. It will be a Unicode
							or ANSI string, depending on whether you use
							the Unicode or ANSI functions.

\param	pData			The data to store.
\param	nDataSize			The size of the data to store.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Set (const TCHAR* pszValueName, DWORD nValueType,
			    BYTE* pData, DWORD nDataSize)
{
	ASSERT (pData);
	ASSERT (nDataSize >= 0);
	ASSERT (m_sam & KEY_SET_VALUE);

	m_lError = ::RegSetValueEx (m_hKey, pszValueName, NULL,
						    nValueType, pData, nDataSize);

	return m_lError == ERROR_SUCCESS;
}


/**	Sets the given registry value to the given string.

\param	pszValueName		The name of the value to set.
\param	szValue			The value to be set.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Set (const TCHAR* pszValueName, const TCHAR* szValue)
{
    ASSERT (szValue);

    return Set (pszValueName, REG_SZ, (BYTE*) szValue, (_tcsclen (szValue) + 1) * sizeof TCHAR);
}

/**	Sets the given registry value to the given DWORD.

\param	pszValueName		The name of the value to set.
\param	dwValue			The value to be set.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Set (const TCHAR* pszValueName, DWORD dwValue)
{
    return Set (pszValueName, REG_DWORD, (BYTE*) &dwValue, sizeof DWORD);
}

/**	Sets the given value to the given double value.

	This is done by first convert it to a string for storing, this is because 
	there is no natural REG_type for double, and it is easier for humans to 
	read in the registry than pure binary data.

\param	pszValueName		The name of the value to set.
\param	dValue			The value to be set.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::SetDouble (const TCHAR* pszValueName, double dValue)
{
	CString sValue;
	sValue.Format (_T("%g"), dValue);
	return Set (pszValueName, sValue);
}

/**	Sets the given value to the given double value.

	This is done by first convert it to a string for storing, this is because 
	there is no natural REG_type for double, and it is easier for humans to 
	read in the registry than pure binary data.

\param	pszValueName		The name of the value to set.
\param	pData			The data to be set.
\param	nBytes			The number of bytes in the data.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Set (const TCHAR* pszValueName, const void* pData, int nBytes)
{
	ASSERT (nBytes >= 0 );

	return Set (pszValueName, REG_BINARY, (BYTE*) pData, nBytes);
}





/**	Reads the given registry value.

\param	pszValueName		The name of the value to set.
\param	nValueType		The type of value.
\param	pData			The data to be filled.
\param	nDataSize			The size of the data.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Query (const TCHAR* pszValueName, DWORD& nValueType,
					 BYTE* pData, DWORD& nDataSize)
{
	ASSERT (m_sam && KEY_QUERY_VALUE);

	m_lError = ::RegQueryValueEx (m_hKey, pszValueName, NULL,
							 &nValueType, pData, &nDataSize);
	return m_lError == ERROR_SUCCESS;
}

BOOL CRegKey::Query (const TCHAR* pszValueName, CString& sValue)
{
	// retrieve a string value


	DWORD dwSize = m_dwMaxValueData;
	DWORD dwValType = 0;
	Query (pszValueName,	dwValType,
			  (BYTE*) sValue.GetBufferSetLength (dwSize), dwSize);
	sValue.ReleaseBuffer();

	return m_lError == ERROR_SUCCESS;
}

BOOL CRegKey::Query (const TCHAR* pszValueName, DWORD& dwValue)
{
	// Retrieve a DWORD value

	DWORD dwSize = sizeof DWORD;
	DWORD dwValType = 0;
	return Query (pszValueName, dwValType, (BYTE*) &dwValue, dwSize);
}

BOOL CRegKey::QueryDouble (const TCHAR* pszValueName, double& dValue)
{
	// Retrieve a double value.  Read as a string then convert.

	CString sValue;
	if ( Query (pszValueName, sValue) )
	{
		dValue = _tcstod (sValue, NULL);
		return TRUE;
	}
	return FALSE;
}


int	CRegKey::QueryDataSize (const TCHAR* szValue)
{
	// Returns 0 if the data isn't there or it actually is zero.

	DWORD dwSize = 0;
	DWORD dwValType = REG_BINARY;

	if ( Query (szValue, dwValType, NULL, dwSize) )
		return (int) dwSize;
	else return 0;
}

BOOL CRegKey::Query (const TCHAR* szValue, void* pData, int nBytes)
{
	// Retrieve binary data, and puts it into the pValue buffer, only if
	// there is enough room.
	ASSERT (nBytes >= 0);

	DWORD dwSize;
	if ( (dwSize=QueryDataSize (szValue)) <= (DWORD) nBytes )
	{
		DWORD dwValType = REG_BINARY;
		return Query (szValue, dwValType, (BYTE*) pData, dwSize);
	}
	return FALSE;
}





/**	Reads the given registry value.

\param	pszValueName		The name of the value to set.
\param	sData			The data to be filled.
\param	nDataSize			The size of the data.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::Query (const TCHAR* pszValueName, CString& sData, int& nDataSize)
{
	ASSERT (m_sam && KEY_QUERY_VALUE);

	nDataSize = QueryDataSize (pszValueName);
	if ( nDataSize )
	{
		DWORD dwValType = REG_BINARY;
		DWORD dwSize = nDataSize;
		BYTE* pData = (BYTE*) sData.GetBuffer (nDataSize);
		BOOL bRet = Query (pszValueName, dwValType, pData, dwSize);
		nDataSize = dwSize;
		sData.ReleaseBuffer (nDataSize);
		return bRet;
	}

	return false;
}




BOOL CRegKey::Close()
{

	LONG lRet = ::RegCloseKey (m_hKey);
	Initialise();
	return lRet == ERROR_SUCCESS;
}


BOOL CRegKey::EnumerateKeys (const DWORD dwSubkeyIndex,
                             CString& strSubkeyName,
                             CString& strClassName )
{

     TCHAR szSubkeyName [2048];
     TCHAR szClassName [2048];

     DWORD dwSizeSubkeyName = sizeof szSubkeyName  - 1;
     DWORD dwSizeClassName  = sizeof szClassName   - 1;

     ::ZeroMemory (szSubkeyName, sizeof szSubkeyName);
     ::ZeroMemory (szClassName,  sizeof szClassName);

     FILETIME fileTime;  // Throw this value away
     m_lError = ::RegEnumKeyEx (m_hKey, 
                                dwSubkeyIndex, 
                                szSubkeyName, 
                                &dwSizeSubkeyName,
                                NULL,
                                szClassName,
                                &dwSizeClassName,
                                &fileTime);

     if ( m_lError == ERROR_SUCCESS )
     {
          strSubkeyName = szSubkeyName;
          strClassName  = szClassName;

          return TRUE;
     }
     return FALSE;
}

BOOL CRegKey::EnumerateValues (const DWORD    dwValueIndex,
                               CString&       sValueName,
                               DWORD*         pdwValueType /*= NULL*/,
                               BYTE*          pbDataBuffer /*= NULL*/,
                               DWORD*         pdwSizeDataBuffer /*= NULL*/)
{
     // pbDataBuffer and dwSizeDataBuffer can be NULL

     TCHAR szTempName [2048];

     ::ZeroMemory (szTempName, sizeof szTempName);
     DWORD dwTempNameSize = sizeof szTempName;

     m_lError = ::RegEnumValue (m_hKey,
                                dwValueIndex,
                                szTempName,
                                &dwTempNameSize,
                                NULL,
                                pdwValueType,
                                pbDataBuffer,
                                pdwSizeDataBuffer );

     if ( m_lError == ERROR_SUCCESS )
     {
          sValueName  = szTempName;
          return TRUE;
     }

     return FALSE;
}


BOOL CRegKey::DeleteValue (const TCHAR* pszValue)
{
	ASSERT ( m_hKey != NULL );         // must have valid HKEY 
	ASSERT ( m_bOpen );
     ASSERT ( m_sam & KEY_WRITE );

     return ::RegDeleteValue (m_hKey, pszValue) == ERROR_SUCCESS;
}



/**	Enumerates and deletes all the values in this key, in two separate passes. 
	
	The first collects the value strings to delete, and the second does the 
	dirty deed.  It is done this way because RegEnumValue() suffers no one
	to change change the key whilst it is enumerating.

\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::DeleteAllValues()
{
	ASSERT ( m_hKey != NULL );         // must have valid HKEY 
	ASSERT ( m_bOpen );
     ASSERT ( m_sam & KEY_WRITE );

     // Collect the values...
	DWORD dwCount = 0;
     CString sValue;
     CStringList listValues;
	while ( EnumerateValues (dwCount++, sValue) )
		listValues.AddTail (sValue);

     // Value deletion pass...
     BOOL bRet = TRUE;
     for (POSITION pos = listValues.GetHeadPosition(); pos; )
     {
          sValue = listValues.GetNext (pos);
          if ( !DeleteValue (sValue) )
               bRet = FALSE;
     }
     return bRet;
}


/////////////////////////////////////////////////////////////////////////////
// Useful helper functions.

const TCHAR szFontName[] = _T("Name");
const TCHAR szFontHeight[] = _T("Height");
const TCHAR szFontWidth[] = _T("Width");
const TCHAR szFontWeight[] = _T("Weight");
const TCHAR szFontItalic[] = _T("Italic");
const TCHAR szFontPitchAndFamily[] = _T("Pitch");
const TCHAR szFontEscapement[] = _T("Escapement");
const TCHAR szFontOrientation[] = _T("Orientation");
const TCHAR szFontUnderline[] = _T("Underline");
const TCHAR szFontStrikeOut[] = _T("StrikeOut");
const TCHAR szFontCharSet[] = _T("CharSet");
const TCHAR szFontOutPrecision[] = _T("OutPrecision");
const TCHAR szFontClipPrecision[] = _T("ClipPrecision");
const TCHAR szFontQuality[] = _T("Quality");


/**	Reads a font specification from the given key under the current key.

\param	szFont		The name of the font key to read from.
\param	lf			The LOGFONT structure to fill with the font details.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::ReadFont (const TCHAR* szFont, LOGFONT& lf)
{
     ASSERT ( this );
     CRegKey rkFont (*this, szFont);

	CString sFontName;
	DWORD dwHeight;
	DWORD dwWidth;
	DWORD dwWeight;
	DWORD dwItalic;
	DWORD dwPitchAndFamily;
	DWORD dwEscapement;
	DWORD dwOrientation;
	DWORD dwUnderline;
	DWORD dwStrikeOut;
	DWORD dwCharSet;
	DWORD dwOutPrecision;
	DWORD dwClipPrecision;
	DWORD dwQuality;

     if ( rkFont.IsOpen() &&
          rkFont.Query (szFontName,           sFontName) &&
          rkFont.Query (szFontHeight,         dwHeight) &&
          rkFont.Query (szFontWidth,          dwWidth) &&
          rkFont.Query (szFontWeight,         dwWeight) &&
          rkFont.Query (szFontItalic,         dwItalic) &&
          rkFont.Query (szFontPitchAndFamily, dwPitchAndFamily) )
	{
		ZeroMemory (&lf, sizeof LOGFONT);

          _tcscpy (lf.lfFaceName, sFontName);
          lf.lfHeight = dwHeight;
		lf.lfWidth = dwWidth;
		lf.lfWeight = dwWeight;
		lf.lfItalic = (BYTE) dwItalic;
		lf.lfPitchAndFamily = (BYTE) dwPitchAndFamily;

          // These settings were added after the ones above,
          // and are tested separately for backward compatibility.

          if (rkFont.Query (szFontEscapement,     dwEscapement) &&
              rkFont.Query (szFontOrientation,    dwOrientation) &&
              rkFont.Query (szFontUnderline,      dwUnderline) &&
              rkFont.Query (szFontStrikeOut,      dwStrikeOut) &&
              rkFont.Query (szFontCharSet,        dwCharSet) &&
              rkFont.Query (szFontOutPrecision,   dwOutPrecision) &&
              rkFont.Query (szFontClipPrecision,  dwClipPrecision) &&
              rkFont.Query (szFontQuality,        dwQuality) )
          {
		     lf.lfEscapement = dwEscapement;
		     lf.lfOrientation = dwOrientation;
		     lf.lfUnderline = (BYTE) dwUnderline;
		     lf.lfStrikeOut = (BYTE) dwStrikeOut;
		     lf.lfCharSet = (BYTE) dwCharSet;
		     lf.lfOutPrecision = (BYTE) dwOutPrecision;
		     lf.lfClipPrecision = (BYTE) dwClipPrecision;
		     lf.lfQuality = (BYTE) dwQuality;
          }

          return TRUE;
	}

     return FALSE;
}


/**	Writes the given font specification to the given key under the current key.

\param	szFont		The name of the font key to write to.
\param	lf			The LOGFONT structure with the font details to be 
					stored in the registry.
\return	\b TRUE if successful, \b FALSE if not.
*/
BOOL CRegKey::WriteFont (const TCHAR* szFont, const LOGFONT& lf)
{
     ASSERT ( this );

	CRegKey rkFont (*this, szFont, KEY_ALL_ACCESS);

	return rkFont.IsOpen() && 
	       rkFont.Set (szFontName,           lf.lfFaceName) &&
     	  rkFont.Set (szFontHeight,         lf.lfHeight) &&
     	  rkFont.Set (szFontWidth,          lf.lfWidth) &&
		  rkFont.Set (szFontWeight,         lf.lfWeight) &&
		  rkFont.Set (szFontItalic,         lf.lfItalic) &&
		  rkFont.Set (szFontPitchAndFamily, lf.lfPitchAndFamily) &&
     	  rkFont.Set (szFontEscapement,     lf.lfEscapement) &&
     	  rkFont.Set (szFontOrientation,    lf.lfOrientation) &&
     	  rkFont.Set (szFontUnderline,      lf.lfUnderline) &&
     	  rkFont.Set (szFontStrikeOut,      lf.lfStrikeOut) &&
     	  rkFont.Set (szFontCharSet,        lf.lfCharSet) &&
     	  rkFont.Set (szFontOutPrecision,   lf.lfOutPrecision) &&
     	  rkFont.Set (szFontClipPrecision,  lf.lfClipPrecision) &&
     	  rkFont.Set (szFontQuality,        lf.lfQuality);
}


/**	Copies the current registry key and branch to the destination key.

\param	rkDest		The destination hive.
\param	bDeleteFirst	If this is \b true, then the destination hive is 
					deleted first before the copy.

\return	\b TRUE if successful, \b FALSE if not.

*/
BOOL CRegKey::CopyTo (CRegKey& rkDest, BOOL bDeleteFirst /*= FALSE*/)
{
	ASSERT ( m_hKey != NULL );         // must have valid HKEY 
	ASSERT ( m_bOpen );
	ASSERT ( rkDest.m_sam & KEY_SET_VALUE );

     if ( rkDest.IsOpen() )
     {
          bool bResult = true;          // This gets set to false if anything
                                        // bad happens.
          if ( bDeleteFirst )
          {
               // Clean up the destination first.
               rkDest.DeleteAllValues();
               rkDest.DeleteAllKeys();
          }

          // Copy values...
	     DWORD dwCount = 0;
          CString sValue;
          BYTE* pData = new BYTE [m_dwMaxValueData];
          while ( EnumerateValues (dwCount++, sValue) )
          {
               DWORD nType;
               DWORD nDataSize = m_dwMaxValueData;     // Initialise each time because
                                                       // it's an in/out parameter.
               if ( Query (sValue, nType, pData, nDataSize) )
               {
                    if ( !rkDest.Set (sValue, nType, pData, nDataSize) )
                         bResult = false;
               }
               else bResult = false;
          }
          delete [] pData;

          // Now the keys...
          dwCount = 0;
          CString sSubkey;
          CString sClassName;
          while ( EnumerateKeys (dwCount++, sSubkey, sClassName) )
          {
               CRegKey rkSourceSubKey (*this, sSubkey);
               CRegKey rkDestSubKey (rkDest, sSubkey, KEY_ALL_ACCESS);
               if ( rkSourceSubKey.IsOpen() &&
                    rkDestSubKey.IsOpen() )
               {
                    if ( !rkSourceSubKey.CopyTo (rkDestSubKey, bDeleteFirst) )
                         bResult = false;
               }
          }
          return bResult;
     }
     return false;
}

/**	Move the source (this) to the destination.

	We simply do a copy first, and if that is successful, we delete all our 
	values and keys.
*/
BOOL CRegKey::MoveTo (CRegKey& rkDest)
{
     if ( CopyTo (rkDest, true) )
     {
          DeleteAll();
          return TRUE;
     }
     return FALSE;
}