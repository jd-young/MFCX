//
//	XStringTest.h
//

#ifndef   XSTRINGTEST_H
#define   XSTRINGTEST_H

#include "UnitTest.h"

BEGIN_DECLARE_TEST(CTest_XString, "CXString") 
     CString TestChop();
END_DECLARE_TEST()


BEGIN_DECLARE_UNITTEST(CXStringTest, "CXString")
     ADD_TEST(CTest_XString);
END_DECLARE_UNITTEST()

#endif    // XSTRINGTEST_H
