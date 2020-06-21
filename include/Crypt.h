//
//	crypt.h		Implementation of the RC4 encryption / decryption algorithm 
//				described in the CipherSaber home page 
//				(http://ciphersaber.gurus.com/)
//	

#ifndef	__MFCX_CRYPT_H
#define	__MFCX_CRYPT_H


/**	An encryption / decryption class that uses the RC4 algorithm.  For more 
	information on RC4 see http://ciphersaber.gurus.com/.

	To encrypt...
	
\verbatim
          CCrypt crypt (szCryptKey);
          CString sPassword = crypt.Encrypt (sPlainText) )
\endverbatim
	
	To decrypt...

\verbatim
          CCrypt crypt (szCryptKey);
          CString sPlainText = crypt.Decrypt (sPassword);
\endverbatim

*/
class CCrypt
{
public:
	CCrypt (const char* pszKey = NULL);
	~CCrypt();

     /// This can be use to change the key
	void SetKey (const char* pszKey);

	/// Encrypts the given data.
	CString Encrypt (const CString& sData);
	CString Encrypt (const TCHAR* pData, int nLen);

	/// Decrypts the given data.
	CString Decrypt (const CString& sData);
	CString Decrypt (const TCHAR* pData, int nLen);

private:
     /// The key to use for en / decryption.
	CString m_sKey;

     /// Does the en / decryption on the data with the given key.
	void Crypt (const CString& sKey, CString& sData);
};

#endif // __MFCX_CRYPT_H
