// TestUtils.h      Testing utilities

#if ! defined  __TESTUTILS_H
#define        __TESTUTILS_H

class CTestUtils
{
public:
     template<typename Functor>
     static bool Await (int msecs, Functor predicate)
     {
          clock_t start = ::clock() * 1000 / CLOCKS_PER_SEC;
          while (! predicate() )
          {
               ::Sleep (100);
               clock_t now = ::clock() * 1000 / CLOCKS_PER_SEC;
               if ( now - start > msecs )
                    return false;
          }
          return true;
     }
};

#endif    //   __TESTUTILS_H
