// MFCXTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MFCXTest.h"
#include "UnitTest.h"
#include "XStringTest.h"

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
	else
	{
		printf ("Running MFCX tests...\n");
		
		CTestList tests;
		
		// TODO: Maybe replace this with an #include file which gets 
		//       automatically filled by a script that scrapes the header 
		//       files for all classes derived from CTest
		// Add all the tests here:
		tests.Add (new CXStringTest());

          TRY
          {	     
     	     CUnitTester tester (tests);
     		return tester.RunTests() ? 0 : 2;  // Why 2?
          }
          CATCH (CException, e)  // 
          {
               TCHAR   szErrorMessage[512];
               UINT    nHelpContext;
               
               if ( e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage), &nHelpContext))
                    printf ("Exception '%s' (help context %d)\n", szErrorMessage, nHelpContext);

               e->Delete();
          }
          END_CATCH
	}
}
