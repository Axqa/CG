#include "float2.h"
#include "somemath.h"
#include "myassert.h"

float2::float2(float x_, float y_)
:x(x_), y(y_)
{
}

float2::float2(float scalar)
:x(scalar), y(scalar)
{
}

float2::float2(const float *data)
{
    if (data != nullptr) {
        x = data[0];
        y = data[1];
    }
}


float float2::LengthSq() const
{
    return x*x + y*y;
}

float float2::Length() const
{
    return Sqrt(LengthSq());
}


float float2::AimedAngle() const
{
    assert(!IsZero());
    return atan2(y, x);
}

float float2::Normalize()
{
//    assert(IsFinite());
    float lengthSq = LengthSq();
    if (lengthSq > 1e-6f)
    {
        float length = Sqrt(lengthSq);
        *this *= 1.f / length;
        return length;
    }
    else
    {
        Set(1.f, 0.f); // We will always produce a normalized vector.
        return 0; // But signal failure, so user knows we have generated an arbitrary normalization.
    }
}

float2 float2::Normalized() const
{
    float2 copy = *this;
    float oldLength = copy.Normalize();
    MARK_UNUSED(oldLength);
    return copy;
}

float float2::ScaleToLength(float newLength)
{
    float length = LengthSq();
    if (length < 1e-6f)
        return 0.f;

    length = Sqrt(length);
    float scalar = newLength / length;
    x *= scalar;
    y *= scalar;
    return length;
}

float2 float2::ScaledToLength(float newLength) const
{
    assert(!IsZero());

    float2 v = *this;
    v.ScaleToLength(newLength);
    return v;
}

bool float2::IsNormalized(float epsilonSq) const
{
    return Abs(LengthSq()-1.f) <= epsilonSq;
}

bool float2::IsZero(float epsilonSq) const
{
    return LengthSq() <= epsilonSq;
}

//bool float2::IsFinite() const
//{
//	return IsFinite(x) && IsFinite(y);
//}

bool float2::IsPerpendicular(const float2 &other, float epsilonSq) const
{
    float dot = Dot(other);
    return dot*dot <= epsilonSq * LengthSq() * other.LengthSq();
}


float float2::DistanceSq(const float2 &rhs) const
{
    float dx = x - rhs.x;
    float dy = y - rhs.y;
    return dx*dx + dy*dy;
}

float float2::Distance(const float2 &rhs) const
{
    return Sqrt(DistanceSq(rhs));
}

float float2::Dot(const float2 &rhs) const
{
    return x * rhs.x + y * rhs.y;
}

float2 float2::Perp() const
{
    return float2(-y, x);
}



float float2::AngleBetween(const float2 &other) const
{
    return acos(Dot(other)) / Sqrt(LengthSq() * other.LengthSq());
}

float float2::AngleBetweenNorm(const float2 &other) const
{
    assert(this->IsNormalized());
    assert(other.IsNormalized());
    return acos(Dot(other));
}

float2 float2::Lerp(const float2 &b, float t) const
{
    assert(0.f <= t && t <= 1.f);
    return (1.f - t) * *this + t * b;
}

float2 float2::Lerp(const float2 &a, const float2 &b, float t)
{
    return a.Lerp(b, t);
}

float float2::Cross(const float2 &w) const
{
    return x*w.y - y*w.x;
}



void float2::Set(float x_, float y_)
{
    x = x_;
    y = y_;
}

void float2::Rotate90CW()
{
    float oldX = x;
    x = y;
    y = -oldX;
}

float2 float2::Rotated90CW() const
{
    return float2(y, -x);
}

void float2::Rotate90CCW()
{
    float oldX = x;
    x = -y;
    y = oldX;
}

float2 float2::Rotated90CCW() const
{
    return float2(-y, x);
}


float2 float2::operator +(const float2 &rhs) const
{
    return float2(x + rhs.x, y + rhs.y);
}

float2 float2::operator -(const float2 &rhs) const
{
    return float2(x - rhs.x, y - rhs.y);
}

float2 float2::operator -() const
{
    return float2(-x, -y);
}

float2 float2::operator *(float scalar) const
{
    return float2(x * scalar, y * scalar);
}

float2 operator *(float scalar, const float2 &rhs)
{
    return float2(scalar * rhs.x, scalar * rhs.y);
}

float2 float2::operator /(float scalar) const
{
    float invScalar = 1.f / scalar;
    return float2(x * invScalar, y * invScalar);
}

float2 &float2::operator =(const float2 &rhs)
{
    x = rhs.x;
    y = rhs.y;

    return *this;
}

float2 &float2::operator +=(const float2 &rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

float2 &float2::operator -=(const float2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

float2 &float2::operator *=(float scalar)
{
    x *= scalar;
    y *= scalar;

    return *this;
}

float2 float2::Add(float s) const
{
    return float2(x + s, y + s);
}

float2 float2::Sub(float s) const
{
    return float2(x - s, y - s);
}

float2 float2::SubLeft(float s) const
{
    return float2(s - x, s - y);
}


float2 float2::Mul(const float2 &rhs) const
{
    return float2(x * rhs.x, y * rhs.y);
}

float2 float2::Div(const float2 &rhs) const
{
    return float2(x / rhs.x, y / rhs.y);
}

float2 &float2::operator /=(float scalar)
{
    float invScalar = 1.f / scalar;
    x *= invScalar;
    y *= invScalar;

    return *this;
}

const float2 float2::zero = float2(0, 0);
const float2 float2::one = float2(1, 1);
const float2 float2::unitX = float2(1, 0);
const float2 float2::unitY = float2(0, 1);
const float2 float2::nan = float2(FLOAT_NAN, FLOAT_NAN);
const float2 float2::inf = float2(FLOAT_INF, FLOAT_INF);
