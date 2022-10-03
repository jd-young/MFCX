/*!
     \file     UnitTest.cpp
     \author   John Young
     \date     17 July 2019
     
     Copyright (c) 2019 Thistle Software.
*/

#include "stdafx.h"
#include "UnitTest.h"
#include "../include/Regex.h"
#include "../include/StringUtil.h"

// DEBUG ONLY
#include <typeinfo>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define   SW_OPTIONS RetrieveSymbol | RetrieveLine 


CString GetPointStr (const CPoint& pt)
{
     CString sPoint;
     sPoint.Format ("CPoint (%d, %d)", pt.x, pt.y);
     return sPoint;
}


//| SymBuildPath
//
//    // Try to retrieve the module-infos
//    RetrieveModuleInfo = 4,
//
//    // Also retrieve the version for the DLL/EXE
//    RetrieveFileVersion = 8,
//
//    // Generate a "good" symbol-search-path
//    SymBuildPath = 0x10,
//
//    // Also use the public Microsoft-Symbol-Server
//    SymUseSymSrv = 0x20,



CTestingStackWalker::CTestingStackWalker()
 :   StackWalker (SW_OPTIONS), _bStore (false)
{
}


void CTestingStackWalker::Reset()
{
     _bStore = false;
     _lstStackEntries.RemoveAll();
}


void CTestingStackWalker::OnOutput (LPCSTR pszText)
{
     CRegex regex;
     
     // _bStore determines whether we store output or not.  We're not interested
     // in stack traces outside PrEditor code (so everything between main() and
     // ShowCallstack()).
     if ( _bStore )
     {
          _lstStackEntries.AddTail (pszText);
          if ( regex.Match ("main$", pszText) )
               _bStore = false;     
     }
     else if ( regex.Match ("StackWalker::ShowCallstack$", pszText) )
               _bStore = true;
}


CString CTestingStackWalker::GetStackTrace() const
{
     return CStringUtil::Join (_lstStackEntries, "    ");
}




CTest::CTest (CTestingStackWalker& stackWalker)
 :  _stackWalker (stackWalker)
{
}



CString CTest::RunTest()
{
     CStringList lstErrors;
     RunAllTests (lstErrors);
     return CStringUtil::Join (lstErrors, "\n    ");
}



CString CTest::ReportTrueFailure (const char* pszDesc, 
                                  const char* file, int line_nr)
{
     CString err;
     err.Format ("%s(%d) : %s in test %s", file, line_nr, pszDesc, GetName());

     _stackWalker.Reset();
     _stackWalker.ShowCallstack();
     CString sTrace = _stackWalker.GetStackTrace();
     err += sTrace;

     return err;
}


CString CTest::ReportFalseFailure (const char* pszDesc, 
                                   const char* file, int line_nr)
{
     CString err;
     err.Format ("%s(%d) : %s in test %s", file, line_nr, pszDesc, GetName());

     _stackWalker.Reset();
     _stackWalker.ShowCallstack();
     CString sTrace = _stackWalker.GetStackTrace();
     err += sTrace;

     return err;
}



CString CTest::ReportEqualsFailure (const char* pszDesc, 
                                    int exp, int act, 
                                    const char* file, int line_nr)
{
     CString err;
     err.Format ("%s(%d) : Expected %d but got %d in test %s", file, line_nr, 
                    exp, act, GetName());
     if ( pszDesc && strlen (pszDesc) )
     {
          err += ": ";
          err += pszDesc;
     }
     err += "\n    ";

     _stackWalker.Reset();
     _stackWalker.ShowCallstack();
     CString sTrace = _stackWalker.GetStackTrace();
     err += sTrace;

     return err;
}


CString CTest::ReportEqualsFailure (const char* pszDesc, 
                                    const CRect& exp, const CRect& act, 
                                    const char* file, int line_nr)
{
     CString err;
     err.Format ("%s(%d) : Expected (%d,%d)..(%d,%d) but got (%d,%d)..(%d,%d) in test %s", 
                    file, line_nr, 
                    exp.left, exp.top, exp.right, exp.bottom,
                    act.left, act.top, act.right, act.bottom, GetName());
     if ( pszDesc && strlen (pszDesc) )
     {
          err += ": ";
          err += pszDesc;
     }
     err += "\n    ";

     _stackWalker.Reset();
     _stackWalker.ShowCallstack();
     CString sTrace = _stackWalker.GetStackTrace();
     err += sTrace;

     return err;
}

CString CTest::ReportEqualsFailure (const char* pszDesc, 
                                    const CString& exp, const CString& act, 
                                    const char* file, int line_nr)
{
     CString sPrefix;
     sPrefix.Format ("%s(%d) : in test %s", file, line_nr, GetName());
     if ( pszDesc && strlen (pszDesc) )
     {
          CString sDesc;
          sDesc.Format ("(%s)", pszDesc);
          sPrefix += sDesc;
     }

     CString err;
     err.Format ("%s\n"
                 "  Expected '%s'\n"
                 "   but got '%s' ", 
                    sPrefix, exp, act);
     err += "\n    ";

     _stackWalker.Reset();
     _stackWalker.ShowCallstack();
     CString sTrace = _stackWalker.GetStackTrace();
     err += sTrace;

     return err;
}



CString CTest::ReportEqualsFailure (const char* pszDesc, 
                                    const CPoint& exp, const CPoint& act, 
                                    const char* file, int line_nr)
{
     CString sExp = GetPointStr (exp);
     CString sAct = GetPointStr (act);
     return ReportEqualsFailure (pszDesc, sExp, sAct, file, line_nr);
}


void CTest::AddFailure (CStringList& lstFailures, const CString& sMsg) const
{
     if ( !sMsg.IsEmpty() )
          lstFailures.AddTail (sMsg);
}


void CTestFailureList::CopyFrom (const CTestFailureList& other)
{
     _list.RemoveAll();
     for (POSITION pos = other._list.GetHeadPosition(); pos; )
     {
          CString failure = _list.GetNext(pos);
          _list.AddTail (failure);
     }
}


void CTestFailureList::Add (CString failure)
{
     _list.AddTail (failure);
}



CTestResult::CTestResult (const CString& name, 
                          int total, 
                          CTestFailureList failures)
     : _name(name), _failures (failures)
{
     _nTestCount = total;
}


/**
 * Copy constructor
 */
/*public*/ CTestResult::CTestResult (const CTestResult& from)
{
     _name = from._name;
     _nTestCount = from._nTestCount;
     _failures = from._failures;
}


void CTestResult::PrintReport()
{
     printf ("   '%s' Unit Test Report: ", (LPCTSTR) _name);
     int failed = _failures.GetCount();
     if ( failed )
     {
          printf ("   %d failed out of %d tests\n", failed, _nTestCount);
          for (POSITION pos = _failures.GetHeadPosition(); pos; )
          {
               CString msg = _failures.GetNext (pos);
               printf ("    %s\n", (LPCTSTR) msg);
          }
     }
     else
     {
          printf ("  All %d tests passed\n", _nTestCount);
     }
}



CUnitTest::~CUnitTest()
{
     for (POSITION pos = _tests.GetHeadPosition(); pos; )
     {
          CTest* test = _tests.GetNext(pos);
          delete test;
     }
     _tests.RemoveAll();
}



CTestResult CUnitTest::RunTest()
{
     int testCount = 0;
     CTestFailureList failures;
     for (POSITION pos = _tests.GetHeadPosition(); pos; )
     {
          CTest* test = _tests.GetNext (pos);
          TRY
          {
               printf ("    Running test %s\n", (LPCTSTR) test->GetName());
               CString err = test->RunTest();
               if ( !err.IsEmpty() )
                    failures.Add (err);
          }
          CATCH (CException, e)
          {
               // TODO: typeid just returns CException (which we already know).
               //CXString type (typeid(e).name());
               //type.FindReplace ("class ", "");
               //type.FindReplace (" *", "");

               CRuntimeClass* rtc = e->GetRuntimeClass();
               CString sType (rtc->m_lpszClassName);
               CString err;
               err.Format ("%s caught: '%s'", sType, GetErrorMessage (e));
               failures.Add (err);
          }
          END_CATCH
          testCount++;
     }
     
     CTestResult result (_title, testCount, failures);
     return result;
}


CString CUnitTest::GetErrorMessage (CException* e)
{
     CString err;
     e->GetErrorMessage (err.GetBuffer(256), 255);
     err.ReleaseBuffer();
     return err;
}



CTestList::~CTestList()
{
     for (POSITION pos = _list.GetHeadPosition(); pos; )
     {
          CUnitTest* test = _list.GetNext(pos);
          delete test;
     }
     _list.RemoveAll();
}



BOOL CUnitTester::RunTests()
{
     int nTestsTotal = 0;
     int nTestsPassed = 0;
     int nTestsFailed = 0;
     int nTestCaseTotal = 0;
     int nTestCasePassed = 0;
     int nTestCaseFailed = 0;

     for (POSITION pos = _tests.GetHeadPosition(); pos; )
     {
     	CUnitTest* test = _tests.GetNext (pos);

          printf ("  Running tests for '%s'\n", (LPCTSTR) test->GetTitle());
          CTestResult result = test->RunTest();
          result.PrintReport();
          if ( result.IsSuccess() )
               nTestCasePassed++;
          else nTestCaseFailed++;
               
          nTestsPassed += result.PassedCount();
          nTestsFailed += result.FailureCount();
          nTestsTotal += result.TestCount();
          nTestCaseTotal++;
     }
     
     if ( nTestsFailed == 0 )
          printf ("All %d test cases (%d tests) passed\n", 
                    nTestCaseTotal, nTestsTotal);
     else 
     {
          printf ("%d of %d test cases failed (%d passed)\n", 
                    nTestCaseFailed, nTestCaseTotal, nTestCasePassed);
          printf ("Total tests: %d, failed: %d, and %d passed\n", 
                    nTestsTotal, nTestsFailed, nTestsPassed);
     }

     return nTestsFailed == 0;
}
