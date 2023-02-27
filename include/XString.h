//	XString.h


#ifndef	__MFCX_CXSTRING_H
#define	__MFCX_CXSTRING_H

/// CXString string class
class CXString
{
public:
	// Constructors
	
	/// Constructs an empty CXString object.
	CXString() {}
	
	/// Copy constructor.  Creates a new CXString object and initialises it to 
	/// the given value.
	CXString (const TCHAR* str) : _string (str) {}
	
	/// Copy constructor.  Creates a new CXString object and initialises it to 
	/// the given value.
	CXString (const CString s) : _string (s) {}
	
// Assignment

	/// Sets the CXString object to the given value.
	/**
		Sets the CXString object to the given value.
	\param	str		The value to set the value to.
	\return	Returns a reference to self.
	*/
	CXString& operator= (const TCHAR* str)  
		{ _string = str; return *this; }

     CXString& operator= (const CXString& str)
          { _string = str._string; return(*this); }


	/// Sets the CXString object to the given value.
	/**
		Sets the CXString object to the given value.
	\param	s		The value to set the value to.
	\return	Returns a reference to self.
	*/
	CXString& operator= (const CString s) { _string = s; return *this; }

     CXString& operator+ (TCHAR ch) { _string += ch; return *this; }
     CXString& operator+ (LPCTSTR s) { _string += s; return *this; }
     CXString& operator+ (const CString& s) { _string += s; return *this; }
     CXString& operator+ (const CXString& s) { _string += s._string; return *this; }

     friend CString operator+(LPCTSTR psz1, const CXString& str2)
     {
          CString strResult = psz1 + str2._string;
          return(strResult);
     }

     const CXString& operator+= (CString s) { _string += s; return *this; }


     friend bool operator==(const CXString& s1, const CXString& s2) throw()
     {
          return s1._string == s2._string;
     }

     friend bool operator==(const CXString& s1, LPCTSTR psz2) throw()
     {
          return s1._string == psz2;
     }


 //    friend CXString& operator+ (LPCTSTR psz, const CXString& s)
 //         { CXString sResult = psz + s; return sResult; }



     /// Returns a pointer to the string's data.
     /** Returns a pointer to the string's data.
     
          This is a useful casting operator that allows you to access the 
          internal data.  Be careful with this pointer, because if you change 
          the data, then the data may be re-allocated to a different memory 
          location, leaving your pointer pointing to invalid data.

     \return   A pointer to the string's data.
     */
	virtual operator LPCTSTR() const { return (LPCTSTR) _string; }

     virtual operator CString&() { return _string; }

     virtual void Empty() throw() { _string.Empty(); }
     virtual BOOL IsEmpty() const { return _string.IsEmpty(); }
     virtual CString& MakeLower() { return _string.MakeLower(); }
     virtual CString& MakeUpper() { return _string.MakeUpper(); }
     virtual int GetLength() const { return _string.GetLength(); }
     virtual LPTSTR GetBuffer(int minBufLen) { return _string.GetBuffer(minBufLen); }
     virtual LPTSTR GetBufferSetLength(int nLength) { return _string.GetBufferSetLength(nLength); }
     virtual void ReleaseBuffer(int len = -1) { _string.ReleaseBuffer(len); }

     //void __cdecl Format(UINT nFormatID...);
     virtual void __cdecl Format(LPCTSTR pszFormat, ...);

     virtual CString Left(int nCount) const { return _string.Left(nCount); }
     virtual CString Right(int nCount) const { return _string.Right(nCount); }

     // Useful stuff

	/// Creates a message box.	
	//int MessageBox (int type = MB_OK, int nIDHelp = 0)
	//	{ return AfxMessageBox ((LPCTSTR)(*this), type, nIDHelp); }
	
	///// Chop newline(s) off end
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

	/// Parses a time.
	CTime GetTime() const { return GetTime ((const TCHAR*) _string); }

	/// Parses a time.
	static CTime GetTime (const TCHAR* psz);

     /// Returns true if the given string is made up of whitespace characters.
	static bool PASCAL IsSpace (const TCHAR* s);

     /// Returns true if this string is made up of whitespace characters.
	bool IsSpace() const 
	     { return IsSpace (static_cast<const TCHAR*>(_string)); }

     /// Returns true if the string is composed entirely of digits.
	static bool PASCAL IsDigit (const TCHAR* s);	
	static bool PASCAL IsNumber (const TCHAR *s, 
						    int cSeparator = ',',
						    int cDecimalPoint = '.');
	bool IsDigit() const 
	     { return IsDigit (static_cast<const TCHAR*>(_string)); }
	bool IsNumber (int cSeparator = ',',
				int cDecimalPoint = '.') const
		{ return IsNumber (_string, cSeparator, cDecimalPoint); }

     bool IsUpper() const 
          { return IsUpper (static_cast<const TCHAR*>(_string)); }
     bool IsLower() const 
          { return IsLower (static_cast<const TCHAR*>(_string)); }
     bool IsCapitalised() const 
          { return IsCapitalised (static_cast<const TCHAR*>(_string)); }
	static bool PASCAL IsUpper (const TCHAR* s);	
	static bool PASCAL IsLower (const TCHAR* s);	
	static bool PASCAL IsCapitalised (const TCHAR* s);	

     int Find (LPCTSTR pszSub, int iStart = 0) const throw()
          {  return _string.Find (pszSub, iStart); }

	bool FindReplace (const TCHAR* pszFind, const TCHAR* pszReplace);
	bool FindReplace (int nIndex, int nLen, const TCHAR* pszReplace);

     int  GetMaxLineLength() const { 
          return GetMaxLineLength (static_cast<const TCHAR*>(_string)); }
     static int GetMaxLineLength (const TCHAR* psz);

     // Companions for LoadString (to get the first and second parts of the resource string).
     bool LoadDescription (UINT nIDS);
     bool LoadToolTip (UINT nIDS);


	void MakeSameCase (const CString& sSource);
	static void MakeSameCase (CString& sTarget, const CString& sSource);

	static void ExpandEnvStr (CString& sEnv);
	static void ExpandMacroStr (CString sText, 
						   const CMapStringToString& mapMacros,
						   const TCHAR* pszStartDelim,
						   const TCHAR* pszEndDelim = NULL);

	/// Returns a hex string representing the given string.
	CString HexStr() const { return HexStr (_string, _string.GetLength()); }
	static CString HexStr (const TCHAR* psz, int nLen);

	CString FromHex() const { return FromHex (_string); }
	static CString FromHex (const TCHAR* pszHex);

      // Searches for the last occurance of ch in this CXString object.
     int ReverseFind (TCHAR ch) const { return _string.ReverseFind (ch); }

     /// Searches for the last occurance of s in this CXString object.
     int ReverseFind (const CString& sTarget) const;

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
protected:
     CString _string;
};

#endif	// __MFCX_CXSTRING_H
