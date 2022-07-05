#include "float3.h"
#include "somemath.h"
#include <QDebug>

float3::float3(float x_, float y_, float z_)
:x(x_), y(y_), z(z_)
{
}

float3::float3(float scalar)
:x(scalar), y(scalar), z(scalar)
{
}

float3::float3(const float2 &xy, float z_)
:x(xy.x), y(xy.y), z(z_)
{
}

float3::float3(const float *data)
{
    if (data == nullptr) return;
    x = data[0];
    y = data[1];
    z = data[2];
}

float float3::LengthSq() const
{
    return x*x + y*y + z*z;
}

float float3::Length() const
{
    return Sqrt(LengthSq());
}


float float3::Dot(const float3 &rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

/** dst = A x B - The standard cross product:
\code
        |a cross b| = |a||b|sin(alpha)

        i		j		k		i		j		k		units (correspond to x,y,z)
        a		b		c		a		b		c		this vector
        d		e		f		d		e		f		vector v
        -cei	-afj	-bdk	bfi	cdj	aek	result

        x = bfi - cei = (bf-ce)i;
        y = cdj - afj = (cd-af)j;
        z - aek - bdk = (ae-bd)k;
\endcode

Cross product is anti-commutative, i.e. a x b == -b x a.
It distributes over addition, meaning that a x (b + c) == a x b + a x c,
and combines with scalar multiplication: (sa) x b == a x (sb).
i x j == -(j x i) == k,
(j x k) == -(k x j) == i,
(k x i) == -(i x k) == j. */
float3 float3::Cross(const float3 &rhs) const
{
    return float3(y * rhs.z - z * rhs.y,
                  z * rhs.x - x * rhs.z,
                  x * rhs.y - y * rhs.x);
}

float3 float3::ProjectTo(const float3 &direction) const
{
    assert(!direction.IsZero());
    return direction * this->Dot(direction) / direction.LengthSq();
}

float3 float3::ProjectToNorm(const float3 &direction) const
{
    Q_ASSERT(direction.IsNormalized());
    return direction * this->Dot(direction);
}

float float3::AngleBetween(const float3 &other) const
{
    float cosa = Dot(other) / Sqrt(LengthSq() * other.LengthSq());
    if (cosa >= 1.f)
        return 0.f;
    else if (cosa <= -1.f)
        return pi;
    else
        return acos(cosa);
}

float float3::AngleBetweenNorm(const float3 &other) const
{
    // this and other must be normalized
    float cosa = Dot(other);
    if (cosa >= 1.f)
        return 0.f;
    else if (cosa <= -1.f)
        return pi;
    else
        return acos(cosa);
}

float3 float3::operator +(const float3 &rhs) const
{
    return float3(x + rhs.x, y + rhs.y, z + rhs.z);
}

float3 float3::operator -(const float3 &rhs) const
{
    return float3(x - rhs.x, y - rhs.y, z - rhs.z);
}

float3 float3::operator -() const
{
    return float3(-x, -y, -z);
}

float3 float3::operator *(float scalar) const
{
    return float3(x * scalar, y * scalar, z * scalar);
}

float3 operator *(float scalar, const float3 &rhs)
{
    return float3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
}

float3 float3::operator /(float scalar) const
{
    float invScalar = 1.f / scalar;

    return float3(x * invScalar, y * invScalar, z * invScalar);
}

float3 &float3::operator =(const float3 &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

float3 &float3::operator +=(const float3 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

float3 &float3::operator -=(const float3 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

float3 &float3::operator *=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
}

bool float3::IsNormalized(float epsilonSq) const
{
    return Abs(LengthSq()-1.f) <= epsilonSq;
}

bool float3::IsZero(float epsilonSq) const
{
    return LengthSq() <= epsilonSq;
}

float3 float3::Add(float scalar) const
{
    return float3(x + scalar, y + scalar, z + scalar);
}

float3 float3::Sub(float scalar) const
{
    return float3(x - scalar, y - scalar, z - scalar);
}

float3 float3::SubLeft(float scalar) const
{

    return float3(scalar - x, scalar - y, scalar - z);
}

float3 float3::Mul(const float3 &rhs) const
{
    return float3(x * rhs.x, y * rhs.y, z * rhs.z);
}

float3 float3::Div(const float3 &rhs) const
{
    return float3(x / rhs.x, y / rhs.y, z / rhs.z);
}

float3 float3::DivLeft(float scalar) const
{
    return float3(scalar / x, scalar / y, scalar / z);
}

float3 &float3::operator /=(float scalar)
{
    float invScalar = 1.f / scalar;

    x *= invScalar;
    y *= invScalar;
    z *= invScalar;

    return *this;
}

float3 float3::Recip() const
{
    return float3(1.f/x, 1.f/y, 1.f/z);
}

float3 float3::Min(float ceil) const
{
    return float3(__min(x, ceil), __min(y, ceil), __min(z, ceil));

}

float3 float3::Min(const float3 &ceil) const
{
    return float3(__min(x, ceil.x), __min(y, ceil.y), __min(z, ceil.z));

}

float3 float3::Max(float floor) const
{
    return float3(__max(x, floor), __max(y, floor),__max(z, floor));
}

float3 float3::Max(const float3 &floor) const
{
    return float3(__max(x, floor.x), __max(y, floor.y),__max(z, floor.z));
}

float3 float3::Clamp(const float3 &floor, const float3 &ceil) const
{
    return Min(ceil).Max(floor);
}

float3 float3::Clamp01() const
{
    return Clamp(0.f, 1.f);
}

float3 float3::Clamp(float floor, float ceil) const
{
    return Min(ceil).Max(floor);
}

float float3::Distance(const float3 &rhs) const
{
    return Sqrt(DistanceSq(rhs));
}

float float3::DistanceSq(const float3 &rhs) const
{
    float dx = x - rhs.x;
    float dy = y - rhs.y;
    float dz = z - rhs.z;
    return dx*dx + dy*dy + dz*dz;
}

float float3::Normalize()
{
    float length = Length();
    if (length > 1e-6f)
    {
        *this *= 1.f / length;
        return length;
    }
    else
    {
        Set(1.f, 0.f, 0.f); // We will always produce a normalized vector.
        return 0; // But signal failure, so user knows we have generated an arbitrary normalization.
    }
}

float3 float3::Normalized() const
{

    float3 copy = *this;
    float oldLength = copy.Normalize();
    assert(oldLength > 0.f && "float3::Normalized() failed!");
    return copy;

}

float3 float3::Reflect(const float3 &normal) const
{
    assert(normal.IsNormalized());
    assert(normal.Length());
    return 2.f * this->ProjectToNorm(normal) - *this;
}

void float3::PerpendicularBasis(float3 &outB, float3 &outC) const
{
    // Pixar orthonormal basis code: https://graphics.pixar.com/library/OrthonormalB/paper.pdf
//    float sign = copysignf(1.0f, z);
//    const float a = -1.0f / (sign + z);
//    const float b = x * y * a;
//    outB = float3(1.0f + sign * x * x * a, sign * b,             -sign * x);
//    outC = float3(                      b, sign + y * y * a,            -y);

    /// Костыль, чтобы outC была соноправлена оси Y
    float3 yLike (0,1,0);
    outB = yLike.Cross(*this).Normalized();
    outC = Cross(outB).Normalized();
}

float3 float3::Perpendicular(const float3 &hint, const float3 &hint2) const
{
    assert(!this->IsZero());
    assert(hint.IsNormalized());
    assert(hint2.IsNormalized());
    float3 v = this->Cross(hint);
    float len = v.Normalize();
    if (len == 0)
        return hint2;
    else
        return v;
}

float3 float3::AnotherPerpendicular(const float3 &hint, const float3 &hint2) const
{
    assert(!this->IsZero());
    assert(hint.IsNormalized());
    assert(hint2.IsNormalized());
    float3 firstPerpendicular = Perpendicular(hint, hint2);
    float3 v = this->Cross(firstPerpendicular);
    return v.Normalized();
}

void float3::Set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void float3::RotateAroundX(float angle)
{
    float sin, cos;
    SinCos(angle, sin, cos);
    float3 res(x, y, z);
    res.y = y * cos + z * sin;
    res.z = y * -sin + z * cos;
    operator= (res);
}

void float3::RotateAroundY(float angle)
{
    float sin, cos;
    SinCos(angle, sin, cos);
    float3 res(x, y, z);
    res.x = x * cos + z * -sin;
    res.z = x * sin + z * cos;
    operator= (res);
}

void float3::RotateAroundZ(float angle)
{
    float sin, cos;
    SinCos(angle, sin, cos);
    float3 res(x, y, z);
    res.x = x * cos + y * sin;
    res.y = x * -sin + y * cos;
    operator= (res);
}

float3 float3::RodriguesRotation(float3 unit, float angle)
{
    float sin, cos;
    SinCos(angle, sin, cos);

    float3 rot = Mul(cos) + Cross(unit) * sin + unit.Mul(Mul(unit)) * (1-cos);
    return rot;
}
