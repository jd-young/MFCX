//	XString.h


#ifndef	__MFCX_CXSTRING_H
#define	__MFCX_CXSTRING_H

/// CXString Extended string class
class CXString : public CString
{
public:
	// Constructors
	
	/// Constructs an empty CXString object.
	CXString(){}
	
	/// Copy constructor.  Creates a new CXString object and initialises it to 
	/// the given value.
	CXString (const TCHAR* str) : CString (str){}
	
	/// Copy constructor.  Creates a new CXString object and initialises it to 
	/// the given value.
	CXString (const CString s): CString (s){}
	
// Assignment

	/// Sets the CXString object to the given value.
	/**
		Sets the CXString object to the given value.
	\param	str		The value to set the value to.
	\return	Returns a reference to self.
	*/
	const CXString& operator= (const TCHAR* str)  
		{ CString::operator=(str); return *this; }


	/// Sets the CXString object to the given value.
	/**
		Sets the CXString object to the given value.
	\param	s		The value to set the value to.
	\return	Returns a reference to self.
	*/
	const CXString& operator= (CString s) 
		{ CString::operator=(s); return *this; }

     /// Returns a pointer to the string's data.
     /** Returns a pointer to the string's data.
     
          This is a useful casting operator that allows you to access the 
          internal data.  Be careful with this pointer, because if you change 
          the data, then the data may be re-allocated to a different memory 
          location, leaving your pointer pointing to invalid data.

     \return   A pointer to the string's data.
     */
	operator LPCTSTR() const
          { return CString::operator LPCTSTR (); }

// Useful stuff

	/// Creates a message box.	
	int MessageBox (int type = MB_OK, int nIDHelp = 0)
		{ return AfxMessageBox ((LPCTSTR)(*this), type, nIDHelp); }
	
	/// Chop newline(s) off end
	int Chop();
	
	/// Chop to the given length.
	int Chop (int nChars, bool bEllipsis = TRUE);

     /// Chops the string to the given width.
	int Chop (CDC* pDC, int nWidth, bool bEllipsis = TRUE);

     /// Chops any newlines off the end of the string.
	int Chomp() { return Chop(); };

     /// Limits the string to the given number of characters.
	int Limit (int nChars);

	/// Puts commas into a number.
	void Commatise();

     /// Strips any commas from a number.
     void Decommatise();

     /// Strips the commas from a number.
     static void Decommatise (TCHAR* psz);

     /// Makes the first letter of each word upper case, the rest lower.
     void Capitalise();

     /// Makes the first letter of each word upper case, the rest lower.
     static void Capitalise (TCHAR* psz);

     /// Makes the first letter of each word upper case, the rest lower.
     static void Capitalise (CString& s);

     /// Toggles the case.
     void ToggleCase();

     static void ToggleCase (TCHAR* psz);    ///< Toggles the case.
     static void ToggleCase (CString& s);    ///< Toggles the case.

	CTime GetTime();

     /// Returns true if the given string is made up of whitespace characters.
	static bool PASCAL IsSpace (const TCHAR* s);

     /// Returns true if this string is made up of whitespace characters.
	bool IsSpace() const { return IsSpace (m_pchData); }

     /// Returns true if the string is composed entirely of digits.
	static bool PASCAL IsDigit (const TCHAR* s);	
	static bool PASCAL IsNumber (const TCHAR *s, 
						    int cSeparator = ',',
						    int cDecimalPoint = '.');
	bool IsDigit() const { return IsDigit (m_pchData); }
	bool IsNumber (int cSeparator = ',',
				int cDecimalPoint = '.') const
		{ return IsNumber (m_pchData, cSeparator, cDecimalPoint); }

     bool IsUpper() const;
     bool IsLower() const;
     bool IsCapitalised() const;

	bool FindReplace (const TCHAR* pszFind, const TCHAR* pszReplace);
	bool FindReplace (int nIndex, int nLen, const TCHAR* pszReplace);

     int  GetMaxLineLength() const { return GetMaxLineLength ((const TCHAR*) this); }
     static int GetMaxLineLength (const TCHAR* psz);

     // Companions for LoadString (to get the first and second parts of the resource string).
     bool LoadDescription (UINT nIDS);
     bool LoadToolTip (UINT nIDS);


	void MakeSameCase (const CXString& sSource);
	static void MakeSameCase (CXString& sTarget, const CXString& sSource);

	static void ExpandEnvStr (CString& sEnv);
	static void ExpandMacroStr (CString sText, 
						   const CMapStringToString& mapMacros,
						   const TCHAR* pszStartDelim,
						   const TCHAR* pszEndDelim = NULL);

	/// Returns a hex string representing the given string.
	CString HexStr() const
		{	return HexStr ((const TCHAR*) this, GetLength()); }
	static CString HexStr (const TCHAR* psz, int nLen);


	CString FromHex() const 
		{	return FromHex ((const TCHAR*) this); }
	static CString FromHex (const TCHAR* pszHex);

     /// Searches for the last occurance of ch in this CXString object.
     int ReverseFind (TCHAR ch) const
          { return CString::ReverseFind (ch); }

     /// Searches for the last occurance of s in this CXString object.
     int ReverseFind (const CString& s) const;

private:
	static bool CalcStringEllipsis(HDC hdc, 
							 LPTSTR pszString, 
							 int nLen, 
							 int nMaxBuf, 
							 UINT nMaxWidth);

	static CString ExpandMacroStrImp (CString sText, 
							    const CMapStringToString* pmapMacros,
							    const CString& sStartDelim, 
							    const CString& sEndDelim);
};

#endif	// __MFCX_CXSTRING_H
