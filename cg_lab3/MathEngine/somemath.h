#ifndef SOMEMATH_H
#define SOMEMATH_H

#include "../commondec.h"
#include "math.h"
#include "MathConstants.h"



/// Returns the square root of x.
FORCE_INLINE float Sqrt(float x)
{
    return sqrtf(x);
}

/// Degrees to radians
FORCE_INLINE float ToRad(float degree) {
    return degree * (pi / 180);
}

FORCE_INLINE float Max(float x, float y) {
    return fmax(x,y);
}

FORCE_INLINE float Sin(float angleRadians)
{
    return sinf(angleRadians);
}

FORCE_INLINE float Cos(float angleRadians)
{
    return cosf(angleRadians);
}

FORCE_INLINE float Tan(float angleRadians)
{
    return tanf(angleRadians);
}

FORCE_INLINE void SinCos(float angleRadians, float &outSin, float &outCos) {
    outSin = Sin(angleRadians);
    outCos = Cos(angleRadians);
}

template<typename T>
FORCE_INLINE T Abs(const T &a)
{
    return a >= 0 ? a : -a;
}

#endif // SOMEMATH_H
