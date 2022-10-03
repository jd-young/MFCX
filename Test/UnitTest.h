//
//	UnitTest.h	A unit test framework for C++
//

#ifndef	_UNITTEST_H
#define	_UNITTEST_H

#include "StackWalker.h"

#define   ASSERT_EQUALS(exp, act)\
     if ( exp != act ) \
          return CTest::ReportEqualsFailure ("", exp, act, __FILE__, __LINE__)

#define   ASSERT_EQ1(desc, exp, act)\
     if ( exp != act ) \
          return CTest::ReportEqualsFailure (desc, exp, act, __FILE__, __LINE__)

#define   ASSERT_POINT_EQ(exp, act)\
     if ( exp.x != act.x && exp.y != exp.y ) \
          return CTest::ReportEqualsFailure ("", exp, act, __FILE__, __LINE__)

#define   ASSERT_POINT_EQ1(desc, exp, act)\
     if ( exp.x != act.x && exp.y != exp.y ) \
          return CTest::ReportEqualsFailure (desc, exp, act, __FILE__, __LINE__)

#define   ASSERT_RECT_EQ(exp, act)\
     if ( exp.top != act.top && exp.left != exp.left && \
          exp.bottom != act.bottom && exp.right != exp.right) \
          return CTest::ReportEqualsFailure ("", exp, act, __FILE__, __LINE__)

#define   ASSERT_RECT_EQ1(desc, exp, act)\
     if ( exp.top != act.top && exp.left != exp.left && \
          exp.bottom != act.bottom && exp.right != exp.right) \
          return CTest::ReportEqualsFailure (desc, exp, act, __FILE__, __LINE__)

#define   ASSERT_TP_EQ(exp, act)\
     if ( exp.nLine != act.nLine && exp.nCol != exp.nCol ) \
          return CTest::ReportEqualsFailure ("", exp, act, __FILE__, __LINE__)

#define   ASSERT_TP_EQ1(desc, exp, act)\
     if ( exp.nLine != act.nLine && exp.nCol != exp.nCol ) \
          return CTest::ReportEqualsFailure (desc, exp, act, __FILE__, __LINE__)


#define   ASSERT_TRUE(condition)\
     if ( ! (condition) ) \
          return CTest::ReportTrueFailure ("Expected true but got false", __FILE__, __LINE__)

#define   ASSERT_TRUE1(desc, condition)\
     if ( ! (condition) ) \
          return CTest::ReportTrueFailure (desc, __FILE__, __LINE__)


#define   ASSERT_FALSE(condition)\
     if ( condition ) \
          return CTest::ReportFalseFailure ("Expected false but got true", __FILE__, __LINE__)

#define   ASSERT_FALSE1(desc, condition)\
     if ( condition ) \
          return CTest::ReportFalseFailure (desc, __FILE__, __LINE__)

#define   ASSERT_NULL    ASSERT_FALSE
#define   ASSERT_NULL1   ASSERT_FALSE1



// Usage: CMyClass
//
//   BEGIN_DECLARE_TEST(CTest_MyClass, "My Test")
//        CString TestFirstThingy();
//        CString TestSecondThingy();
//         :
//        CString TestLastThingy();
//   // Helpers
//         :
//   END_DECLARE_TEST()
//
//   BEGIN_DECLARE_UNITTEST(CMyClassTest, "My Unit Test")
//        ADD_TEST(CTest_MyClass);
//        :
//   END_DECLARE_UNITTEST()
//
#define BEGIN_DECLARE_TEST(test_name, desc)\
 class test_name : public CTest\
 {\
 public:\
     test_name (CTestingStackWalker& stackWalker);\
     CString GetName() const override { return desc; }\
 protected:\
     void RunAllTests (CStringList& lstFailures);

#define END_DECLARE_TEST()    };

#define BEGIN_DECLARE_UNITTEST(test_name, desc)\
 class test_name : public CUnitTest\
 {\
 public:\
     test_name() : CUnitTest(desc)\
     {

#define ADD_TEST(test_name) _tests.AddTail (new test_name (_stackWalker));

#define END_DECLARE_UNITTEST()\
     }\
 };



// Usage: CTest_MyClass
//
//   BEGIN_IMPLEMENT_TEST(CTest_MyClass)
//        CALL_TEST(TestFirstThingy);
//        CALL_TEST(TestSecondThingy)
//         :
//        CALL_TEST(TestLastThingy);
//   END_IMPLEMENT_TEST()
//
#define BEGIN_IMPLEMENT_TEST(test_name)\
 test_name::test_name (CTestingStackWalker& sw) : CTest (sw) {}\
 void test_name::RunAllTests (CStringList& lstFailures)\
 {\
     CString sMsg;
#define CALL_TEST(test_func) sMsg = test_func(); AddFailure (lstFailures, sMsg);
#define END_IMPLEMENT_TEST() }



class CTestingStackWalker : public StackWalker
{
public:
     CTestingStackWalker();

     void Reset();

     CString GetStackTrace() const;

protected:
     virtual void OnOutput (LPCSTR szText);

     bool _bStore;
     CStringList _lstStackEntries;
};



//class /*interface*/ ITestResult
//{
//public:
//     virtual ~ITestResult() {}
//     
//     virtual int FailedCount() const = 0;
//};



/**
 * A single test.  This can call Assert as many times as it wants, but on the 
 * first failure, execution stops.
 */
class CTest
{
public:
     CTest (CTestingStackWalker& stackWalker);
     virtual CString GetName() const = 0;
     virtual CString RunTest();

protected:
     virtual void RunAllTests (CStringList& lstFailures) {};

     CString ReportTrueFailure (const char* pszDesc, 
                                const char* file, int line_nr);
     CString ReportFalseFailure (const char* pszDesc, 
                                 const char* file, int line_nr);
     CString ReportEqualsFailure (const char* pszDesc, 
                                  int exp, int act, 
                                  const char* file, int line_nr);
     CString ReportEqualsFailure (const char* pszDesc, 
                                  const CRect& exp, const CRect& act, 
                                  const char* file, int line_nr);
     CString ReportEqualsFailure (const char* pszDesc, 
                                  const CString& exp, const CString& act, 
                                  const char* file, int line_nr);
     CString ReportEqualsFailure (const char* pszDesc, 
                                  const CPoint& exp, const CPoint& act, 
                                  const char* file, int line_nr);

     void AddFailure (CStringList& lstFailures, const CString& sMsg) const;
     
     CTestingStackWalker& _stackWalker;
};


class CTestFailureList
{
public:
     CTestFailureList() {};
     CTestFailureList (const CTestFailureList& other) { CopyFrom(other); }

     void Add (CString failure);
     int GetCount() { return _list.GetCount(); }
     POSITION GetHeadPosition() { return _list.GetHeadPosition(); }
     CString GetNext(POSITION& pos) { return _list.GetNext (pos); }

   	CTestFailureList& operator= (const CTestFailureList& from)
		{ CopyFrom (from);	return *this; }
     
private:
     CStringList _list;
     
     void CopyFrom (const CTestFailureList& other);
};


/**
 *   The result of one test.
 */
class CTestResult //: public ITestResult
{
public:
     CTestResult (const CString& name, int total, CTestFailureList failures);
     CTestResult (const CTestResult& from);  ///< Copy constructor

     /// Returns TRUE if the test was successful.
     BOOL IsSuccess() { return _failures.GetCount() == 0; }

     int TestCount() { return _nTestCount; }
     int PassedCount() { return _nTestCount - _failures.GetCount(); }
     int FailureCount() { return _failures.GetCount(); }

     /// Reports the test results to stdout.
     void PrintReport();

private:
     CString _name;

     int _nTestCount;
     CTestFailureList _failures;
};


#if 0
/**
 *   The result of multiple tests.
 */
class CMultiTestResult : public ITestResult
{
public:
     CMultiTestResult (const CString& name, int total, CTestFailureList failures);
     CMultiTestResult (const CMultiTestResult& from);  ///< Copy constructor

//     /// Returns TRUE if the test was successful.
//     BOOL IsSuccess() { return _failures.GetCount() == 0; }
//
//     int TestCount() { return _nTestCount; }
//     int PassedCount() { return _nTestCount - _failures.GetCount(); }
//     int FailureCount() { return _failures.GetCount(); }

     /// Reports the test results to stdout.
     void PrintReport();

//private:
//     CString _name;
//
//     int _nTestCount;
//     CTestFailureList _failures;
};
#endif





/// A unit testing interface.
/**  All unit tests should derive from this class.
 * 
 */
class CUnitTest
{
public:
     CUnitTest (CString title) { _title = title; }
     ~CUnitTest();

     /*!
      * \return     The title of the test.
      */
     CString GetTitle() { return _title; }
     
     /*!
      *   Runs the test and returns a test result.
      *
      * \return  A test result.
      */
     CTestResult RunTest();

protected:
     CList<CTest*, CTest*> _tests;
     CTestingStackWalker _stackWalker;

private:
     CString _title;

     /**
      * Extracts the error message from the given exception.
      * 
      * @param e         The exception from which to get the error message.
      * @return the error message as a CString.
      */
     CString GetErrorMessage (CException* e);
};



class CTestList
{
private:
     CList<CUnitTest*, CUnitTest*> _list;
     
public:
     ~CTestList();

   	POSITION Add(CUnitTest* newElement) { return _list.AddTail (newElement); }
   	POSITION GetHeadPosition() { return _list.GetHeadPosition(); }
   	CUnitTest* GetNext (POSITION& pos) { return _list.GetNext (pos); }
};


class CUnitTester
{
public:
     CUnitTester (CTestList& tests) : _tests(tests) {};

     BOOL RunTests();

private:
     CTestList& _tests;
};


#endif    // _UNITTEST_H
