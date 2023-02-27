// MFCXTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		fprintf (stderr, "Fatal Error: MFC initialization failed\n");
		return 1;
	}

     printf ("Running MFCX tests...\n");
     testing::InitGoogleTest (&argc, argv);
     return RUN_ALL_TESTS();
}
