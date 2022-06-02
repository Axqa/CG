#ifndef SOMEMATH_H
#define SOMEMATH_H

#include "../commondec.h"
#include "math.h"
#include "MathConstants.h"

template<typename T>
FORCE_INLINE void Swap(T &a, T &b)
{
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

/// Returns the square root of x.
FORCE_INLINE float Sqrt(float x)
{
    return sqrtf(x);
}

/// Degrees to radians
FORCE_INLINE float ToRad(float degree) {
    return degree * (pi / 180);
}

/// Radians to degrees
FORCE_INLINE float ToDeg(float rad) {
    return rad * (180 / pi);
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

FORCE_INLINE bool EqualAbs(float a, float b, float epsilon= 1e-4f)
{
    return Abs(a-b) < epsilon;
}
template<typename T>
FORCE_INLINE T Max(T &a, T &b)
{
    return __max(a,b);
}

template<typename T>
FORCE_INLINE T Min(T &a, T &b)
{
    return __min(a,b);
}

/// Clamps the given input value to the range [min, max].
/** @see Clamp01(), Min(), Max(). */
template<typename T>
FORCE_INLINE T Clamp(const T &val, const T &floor, const T &ceil)
{
    assert(floor <= ceil);
    return val <= ceil ? (val >= floor ? val : floor) : ceil;
}

/// Clamps the given input value to the range [0, 1].
/** @see Clamp(), Min(), Max(). */
template<typename T>
FORCE_INLINE T Clamp01(const T &val) { return Clamp(val, T(0), T(1)); }


#endif // SOMEMATH_H
