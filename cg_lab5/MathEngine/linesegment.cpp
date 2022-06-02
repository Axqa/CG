#include "linesegment.h"
#include "ray.h"
#include "line.h"
#include "somemath.h"

LineSegment::LineSegment(const vec &a_, const vec &b_)
:a(a_), b(b_)
{
}

LineSegment::LineSegment(const Ray &ray, float d)
:a(ray.pos), b(ray.GetPoint(d))
{
}

LineSegment::LineSegment(const Line &line, float d)
:a(line.pos), b(line.GetPoint(d))
{
}

vec LineSegment::GetPoint(float d) const
{
    return (1.f - d) * a + d * b;
}

vec LineSegment::CenterPoint() const
{
    return (a + b) * 0.5f;
}

void LineSegment::Reverse()
{
    Swap(a, b);
}

vec LineSegment::Dir() const
{
    return (b - a).Normalized();
}

vec LineSegment::ExtremePoint(const vec &direction) const
{
    return Dot(direction, b-a) >= 0.f ? b : a;
}

vec LineSegment::ExtremePoint(const vec &direction, float &projectionDistance) const
{
    vec extremePoint = ExtremePoint(direction);
    projectionDistance = extremePoint.Dot(direction);
    return extremePoint;
}

float LineSegment::Length() const
{
    return a.Distance(b);
}

float LineSegment::LengthSq() const
{
    return a.DistanceSq(b);
}

vec LineSegment::ClosestPoint(const vec &point, float &d) const
{
    vec dir = b - a;
    d = Clamp01(Dot(point - a, dir) / dir.LengthSq());
    return a + d * dir;
}

vec LineSegment::ClosestPoint(const Ray &other, float &d, float &d2) const
{
    other.ClosestPoint(*this, d2, d);
    return GetPoint(d);
}

vec LineSegment::ClosestPoint(const Line &other, float &d, float &d2) const
{
    Line::ClosestPointLineLine(other.pos, other.dir, a, b - a, d2, d);
    if (d < 0.f)
    {
        d = 0.f;
        other.ClosestPoint(a, d2);
        return a;
    }
    else if (d > 1.f)
    {
        d = 1.f;
        other.ClosestPoint(b, d2);
        return b;
    }
    else
        return GetPoint(d);
}

vec LineSegment::ClosestPoint(const LineSegment &other, float &d, float &d2) const
{
    vec dir = b - a;
    Line::ClosestPointLineLine(a, b - a, other.a, other.b - other.a, d, d2);
    if (d >= 0.f && d <= 1.f && d2 >= 0.f && d2 <= 1.f)
        return a + dir * d;
    else if (d >= 0.f && d <= 1.f) // Only d2 is out of bounds.
    {
        vec p;
        if (d2 < 0.f)
        {
            d2 = 0.f;
            p = other.a;
        }
        else
        {
            d2 = 1.f;
            p = other.b;
        }
        return ClosestPoint(p, d);
    }
    else if (d2 >= 0.f && d2 <= 1.f) // Only d is out of bounds.
    {
        vec p;
        if (d < 0.f)
        {
            d = 0.f;
            p = a;
        }
        else
        {
            d = 1.f;
            p = b;
        }

        other.ClosestPoint(p, d2);
        return p;
    }
    else // Both u and u2 are out of bounds.
    {
        vec p;
        if (d < 0.f)
        {
            p = a;
            d = 0.f;
        }
        else
        {
            p = b;
            d = 1.f;
        }

        vec p2;
        if (d2 < 0.f)
        {
            p2 = other.a;
            d2 = 0.f;
        }
        else
        {
            p2 = other.b;
            d2 = 1.f;
        }

        float T, T2;
        vec closestPoint = ClosestPoint(p2, T);
        vec closestPoint2 = other.ClosestPoint(p, T2);

        if (closestPoint.DistanceSq(p2) <= closestPoint2.DistanceSq(p))
        {
            d = T;
            return closestPoint;
        }
        else
        {
            d2 = T2;
            return p;
        }
    }
}

float LineSegment::Distance(const vec &point, float &d) const
{
    /// See Christer Ericson's Real-Time Collision Detection, p.130.
    vec closestPoint = ClosestPoint(point, d);
    return closestPoint.Distance(point);
}

float LineSegment::DistanceSq(const vec &point) const
{
    float d;
    /// See Christer Ericson's Real-Time Collision Detection, p.130.
    vec closestPoint = ClosestPoint(point, d);
    return closestPoint.DistanceSq(point);
}


float LineSegment::Distance(const Ray &other, float &d, float &d2) const
{
    ClosestPoint(other, d, d2);
    return GetPoint(d).Distance(other.GetPoint(d2));
}

float LineSegment::Distance(const Line &other, float &d, float &d2) const
{
    vec closestPoint2 = other.ClosestPoint(*this, d, d2);
    vec closestPoint = GetPoint(d2);
    return closestPoint.Distance(closestPoint2);
}

float LineSegment::DistanceSq(const LineSegment &other) const
{
    float d, d2;
    ClosestPoint(other, d, d2);
    return GetPoint(d).DistanceSq(other.GetPoint(d2));
}

Ray LineSegment::ToRay() const
{
    return Ray(a, Dir());
}

Line LineSegment::ToLine() const
{
    return Line(a, Dir());
}
