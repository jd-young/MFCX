/*!
	\file	Crypt.cpp      Encryption / decryption using RC4
	\author	John Young
	\date	13 June 2000
	Copyright (c) 2000 Thistle Softare
*/


#include "stdafx.h"
#include <stdlib.h>
#include "../include/Crypt.h"




/**  A encryption / decryption class using RC4.

     Initialises the CCrypt object and (optionally) supplies a key.

	NOTES ON KEYS (from the CipherSaber web page).

	To insure adequate mixing of the initialization vector and user key, we 
 	recommend you select a user key of 54 bytes or less. A user key with a 
	minimum of 14 random letters, or 5 short words selected at random from a 
	dictionary, should be used for medium security (64 bit entropy). For high 
	security, use 20 random letters or seven random words. (90 bit entropy).

\param	pszKey	The key.
*/
CCrypt::CCrypt (const char* pszKey /*= NULL*/)
{
	if ( pszKey )
		SetKey (pszKey);
}


CCrypt::~CCrypt()
{
}

void CCrypt::SetKey (const char* pszKey)
{
	m_sKey = pszKey;
}




/**  Encrypts the given plain text data.  The output data is prepended by a 10
	bype initialisation vector.

	To use...
\verbatim
          CCrypt crypt (szCryptKey);
          CString sPassword = crypt.Encrypt (pPlainText, nLen) )
\endverbatim

\param    pPlain		The data to encrypt.
\param	nLen			The length of the data to encrypt.
\result   The encrypted data.
*/
CString CCrypt::Encrypt (const TCHAR* pPlain, int nLen)
{
	CString sPlain (pPlain, nLen);
	return Encrypt (sPlain);
}


/**  Encrypts the given plain text data.  The output data is prepended by a 10
	bype initialisation vector.

	To use...
\verbatim
          CCrypt crypt (szCryptKey);
          CString sPassword = crypt.Encrypt (sPlainText) )
\endverbatim

\param    sPlain		The data to encrypt.
\result   The encrypted data.
*/
CString CCrypt::Encrypt (const CString& sPlain)
{
     // Create the IV.
     CString sRandom;
     TCHAR* s = sRandom.GetBuffer (10);
     srand ((unsigned int) time (NULL));
     for (int i = 0; i < 10; i++)
          *s++ = rand() * 256 / RAND_MAX;
     sRandom.ReleaseBuffer (10);

     CString sKey = m_sKey + sRandom;
     CString sData = sPlain;
     Crypt (sKey, sData); 
     sData = sRandom + sData;      // Prepend the IV.
	return sData;
}



/**  Decrypts the given encrypted data.  The first 10 bytes of the data is 
	expected to have the IV.
	
	To use...
\verbatim
		CCrypt crypt (szCryptKey);
	     CString sPlainText = crypt.Decrypt (pPassword, nLen);
\endverbatim

\param    sEncrypted		The data to be decrypted.
\result   The plain text if successful, the empty string if otherwise.  The only 
		reason this can fail is if the data is shorter than 10 bytes, i.e. the 
		pre-pended random number portion of the key.  If so, then the returned 
		string is empty.
*/
CString CCrypt::Decrypt (const TCHAR* pEncrypted, int nLen)
{
	CString sEncrypted (pEncrypted, nLen);
	return Decrypt (sEncrypted);
}



/**  Decrypts the given encrypted data.  The first 10 bytes of the data is 
	expected to have the IV.
	
	To use...
\verbatim
		CCrypt crypt (szCryptKey);
	     CString sPlainText = crypt.Decrypt (sPassword);
\endverbatim

\param    sEncrypted		The data to be decrypted.
\result   The plain text if successful, the empty string if otherwise.  The only 
		reason this can fail is if the data is shorter than 10 bytes, i.e. the 
		pre-pended random number portion of the key.  If so, then the returned 
		string is empty.
*/
CString CCrypt::Decrypt (const CString& sEncrypted)
{
	// The first 10 bytes of the input data should be the random number part of
	// the key.
	
	if ( sEncrypted.GetLength() < 10 )
		return "";

	// Skip past the first 10 bytes to the actual encrypted data
	CString sKey = m_sKey;
	int origLen = sKey.GetLength();
	TCHAR* s = sKey.GetBuffer (origLen + 10) + origLen;
	for (int i = 0; i < 10; i++)
		*s++ = sEncrypted [i];
	sKey.ReleaseBuffer (origLen + 10);

	CString sData = sEncrypted.Right (sEncrypted.GetLength() - 10);
	
	Crypt (sKey, sData);
	return sData;
}


/**  En / decrypts the given data according to the given key.  Cryption is done 
 *   using Ron Rivest's RC4 algorithm (RC4 stands for Ron's Code 4).
 *
 *   RC4 uses two arrays of eight bit bytes. The "state" array is 256 bytes long 
 *   and holds a permutation of the numbers 0 through 255. The "key" array can 
 *   be of any length up to 256 bytes. RC4 also uses two index variables i and j 
 *   that start off as zero. All variables are eight bits long and all addition 
 *   is performed modulo 256.
 *
 *   RC4 has two phases: key setup and ciphering. The setup phase is only done 
 *   once per message and starts by initializing the entire state array so that 
 *   the first state element is zero, the second is one, the third is two, and 
 *   so on.
 *
 *   The state array is then subjected to 256 mixing operations using a loop 
 *   that steps i through the values from zero to 255. Each mixing operation 
 *   consists of two steps:
 *
 *        Add to the variable j the contents of the ith element of the state 
 *        array and the nth element of the key, where n is equal to i modulo the 
 *        length of the key. 
 *        
 *        Swap the ith and jth elements of the state array. 
 *
 *   After the entire mixing loop is completed, i and j are set to zero.
 *
 *   During the ciphering operation, the following steps are performed for each 
 *   byte of the message:
 *
 *   - The variable i is incremented by one 
 *   - The contents of the ith element of the state array is then added to j 
 *   - The ith and jth elements of the state array are swapped and their 
 *     contents are added together to form a new value n. 
 *   - The nth element of the state array is then combined with the message 
 *     byte, using a bit by bit exclusive-or operation, to form the output byte. 
 *   - The same ciphering steps are performed for encryption and for decryption.
 *
 *   Note that in CipherSaber the RC4 key array consists of the user's 
 *   CipherSaber key followed by the 10 byte initialization vector (IV).
 *
 *   When you are encrypting a file, you generate a new IV randomly and write 
 *   out the 10 bytes before you write out the encrypted file bytes. 
 *   When you are decrypting the file, you read the IV from the first 10 bytes 
 *   of the encrypted file. 
 *
 *   That's all there is!
 *
 *   @param	sKey		The key with which to encrypt / decrypt the data.
 *	@param	sData	The data to encrypt / decrypt.
 */
void CCrypt::Crypt (const CString& sKey, CString& sData)
{	
	// Initialse the state array...
	int	perm [256];
	for (int i = 0; i < 256; i++)
		perm [i] = i;

	int keylen = sKey.GetLength();
	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		j += perm [i] + sKey [i % keylen];
		j &= 255;
		int tmp = perm [i];
		perm [i] = perm [j];
		perm [j] = tmp;
	}

	// Now do the transform...
	int x = 0;
	int y = 0;
     int datalen = sData.GetLength();
	for (int i = 0; i < datalen; i++)
	{
		x = (x + 1) & 255;
		y = (y + perm [x]) & 255;
		int a = perm [x];
          int b = perm [y];
		perm[x] = b;
		perm[y] = a;
		int n = perm [(a + b) & 255];
		sData.SetAt (i, ((unsigned char) sData [i] ^ n) & 255);
	}  
}


