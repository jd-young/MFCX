// MFCXTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MFCXTest.h"
#include "..\include\MFCX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


static bool TestFilename();
static bool TestCrypt();


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{

		printf ("MFCX tester\n");

		if ( TestFilename() &&
			TestCrypt() )
			printf ("Done - all tests passed ok!\n");
		else  
		{
			printf ("Done.\n");
			nRetCode = 1;		// Failed
		}
	}

	return nRetCode;
}



static bool TestCanonPath (const char* pszPath, const char* pszOK);

static bool TestFilename()
{
	// CanonPath test...
	if ( TestCanonPath ("c:\\dir1\\\\dir2\\file.ext", "C:\\dir1\\dir2\\file.ext") &&
		TestCanonPath ("//server/share/dir1/../dir2/file.ext", "\\\\server\\share\\dir2\\file.ext") &&
		TestCanonPath ("dir1/../dir2/file.ext", "dir2\\file.ext") &&
		TestCanonPath ("./reldir/dir1/file", "reldir\\dir1\\file") &&
		TestCanonPath ("./", ".\\") )
	{
		printf ("CFilename passed ok\n");
		return true;
	}
	return false;
}


static bool TestCanonPath (const char* pszPath, const char* pszOK)
{
	CString sPath = CFilename::CanonPath (pszPath);
	if ( sPath != pszOK )
	{
		printf ("CFilename::CanonPath ('%s') FAILED!\n"
			   "   Should be '%s'\n"
			   "   but was   '%s'\n",
				pszPath, pszOK, sPath);
		return false;
	}
	return true;
}


static bool TestCrypt (const TCHAR* pszPlainfile, const TCHAR* pszKey);

static bool TestCrypt()
{
	const TCHAR* PLAINFILE = "EncryptText-plain.txt";
	const TCHAR* ENCRYPTKEY = "PrEditorTestKey";

	if ( !TestCrypt ("EncryptText-plain.txt", "PrEditorTestKey") ||
		!TestCrypt ("EncryptText-plain.cpp", "abc") )
		return false;

	return true;
}

static bool TestCrypt (const TCHAR* pszPlainfile, const TCHAR* pszKey)
{
	CStdioFile inFile;
	if ( !inFile.Open (pszPlainfile, CFile::modeRead | CFile::shareDenyNone) )
	{
		printf ("Cannot open file %s for reading.\n", pszPlainfile);
		return false;
	}

	CString sPlain;
	int nLen = inFile.GetLength();
	inFile.Read (sPlain.GetBuffer (nLen), nLen);
	sPlain.ReleaseBuffer (nLen);
	

	CCrypt crypt1 (pszKey);
	CString sEncrypted = crypt1.Encrypt (sPlain);
	
	CFile outFile;
	CString sOutFile = pszPlainfile;
	sOutFile += ".encrypted";
	if ( !outFile.Open (sOutFile, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive) )
		printf ("Cannot open file %s for writing.\n", sOutFile);
	else outFile.Write (sEncrypted, sEncrypted.GetLength());
	
	CString sTest = crypt1.Decrypt (sEncrypted);
		
	if ( sTest != sPlain )
	{
		printf ("En/Decrypt failed!\n");
		return false;
	}	

	CCrypt crypt2 (pszKey);
	sTest = crypt2.Decrypt (sEncrypted);
	if ( sTest != sPlain )
	{
		printf ("En/Decrypt failed!\n");
		return false;
	}	


	printf ("CCrypt (%s / %s) passed ok\n", pszPlainfile, pszKey);
	return true;
}