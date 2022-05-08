#ifndef COMMONDEC_H
#define COMMONDEC_H

#include <assert.h>

typedef unsigned char u8; ///< a single byte: 0-255.
typedef unsigned short u16; ///< 2 bytes: 0 - 65535.
typedef unsigned int u32; ///< 4 bytes: 0 - 4,294,967,295 ~ 4000 million or 4e9.
typedef unsigned long long u64; ///< 8 bytes: 18,446,744,073,709,551,615 ~1.8e19.

typedef char s8; ///< a single byte: -128 - 127.
typedef short s16; ///< 2 bytes: -32768 - 32767.
typedef int s32; ///< 4 bytes signed: max 2,147,483,647 ~ 2000 million or 2e9.
typedef long long s64; ///< 8 bytes signed. 9,223,372,036,854,775,807 ~ 9e18.

// Looking at disasm, compilers have been seen to be stupid about inlining some single-instruction SIMD intrinsics functions, so use this to force.
#ifdef _DEBUG
#define FORCE_INLINE inline
#elif defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline __attribute__((always_inline))
#endif

// The CONST_WIN32 is a #define which resolves to 'const' on Windows, and null on other
// platforms. This #define is used on Windows to detect accidental programming errors
// occurring from an expression "const float3 vec; vec[1] = 5;". Trying to return
// const float from operator[] on GCC gives a warning "type qualifiers ignored on function return type",
// so hence this is only enabled on Visual Studio.
#ifdef _MSC_VER
#define CONST_WIN32 const
#else
#define CONST_WIN32
#endif

#endif // COMMONDEC_H
