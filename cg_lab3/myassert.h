#ifndef MYASSERT_H
#define MYASSERT_H

#include <iostream>
#include <assert.h>

#ifndef NDEBUG
#   define massert(Expr, Msg) \
    __M_Assert(Expr, #Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

inline void __M_Assert( bool expr,const char* expr_str, const char* file, int line, const char* msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

// example : massert(ptr != nullptr, "MyFunction: requires non-null argument");

#endif // MYASSERT_H
