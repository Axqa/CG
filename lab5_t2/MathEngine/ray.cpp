#include "ray.h"
#include "float3.h"
#include "somemath.h"
#include "line.h"
#include "linesegment.h"

// dir must be normalized
Ray::Ray(const float3 &pos_, const float3 &dir_)
:pos(pos_), dir(dir_)
{
}

float3 Ray::GetPoint(float d) const
{
    return pos + dir*d;
}

float Ray::Distance(const float3 &point, float &d) const
{
    return ClosestPoint(point, d).Distance(point);
}

float Ray::Distance(const Ray &other, float &d, float &d2) const
{
    vec c = ClosestPoint(other, d, d2);
    return c.Distance(other.GetPoint(d2));
}

float Ray::Distance(const Line &other, float &d, float &d2) const
{
    vec c = ClosestPoint(other, d, d2);
    return c.Distance(other.GetPoint(d2));
}

float Ray::Distance(const LineSegment &other, float &d, float &d2) const
{
    vec c = ClosestPoint(other, d, d2);
    return c.Distance(other.GetPoint(d2));
}


float3 Ray::ClosestPoint(const float3 &targetPoint, float &d) const
{
    d = Max(0.f, Dot(targetPoint - pos, dir));
    return GetPoint(d);
}

vec Ray::ClosestPoint(const Ray &other, float &d, float &d2) const
{
    Line::ClosestPointLineLine(pos, dir, other.pos, other.dir, d, d2);
    if (d < 0.f && d2 < 0.f)
    {
        vec closestPoint = ClosestPoint(other.pos, d);
        vec closestPoint2 = other.ClosestPoint(pos, d2);
        if (closestPoint.DistanceSq(other.pos) <= closestPoint2.DistanceSq(pos))
        {
            d2 = 0.f;
            return closestPoint;
        }
        else
        {
            d = 0.f;
            return pos;
        }
    }
    else if (d < 0.f)
    {
        d = 0.f;
        other.ClosestPoint(pos, d2);
        d2 = Max(0.f, d2);
        return pos;
    }
    else if (d2 < 0.f)
    {
        vec pt = ClosestPoint(other.pos, d);
        d = Max(0.f, d);
        d2 = 0.f;
        return pt;
    }
    else
    {
        return GetPoint(d);
    }
}

vec Ray::ClosestPoint(const Line &other, float &d, float &d2) const
{
    Line::ClosestPointLineLine(pos, dir, other.pos, other.dir, d, d2);
    if (d < 0.f)
    {
        d = 0.f;
        other.ClosestPoint(pos, d2);
        return pos;
    }
    else
        return GetPoint(d);
}

vec Ray::ClosestPoint(const LineSegment &other, float &d, float &d2) const
{
    Line::ClosestPointLineLine(pos, dir, other.a, other.b - other.a, d, d2);
    if (d < 0.f)
    {
        d = 0.f;
        if (d2 >= 0.f && d2 <= 1.f)
        {
            other.ClosestPoint(pos, d2);
            return pos;
        }

        vec p;
        float t2;

        if (d2 < 0.f)
        {
            p = other.a;
            t2 = 0.f;
        }
        else // u2 > 1.f
        {
            p = other.b;
            t2 = 1.f;
        }

        vec closestPoint = ClosestPoint(p, d);
        vec closestPoint2 = other.ClosestPoint(pos, d2);
        if (closestPoint.DistanceSq(p) <= closestPoint2.DistanceSq(pos))
        {
            d2 = t2;
            return closestPoint;
        }
        else
        {
            d = 0.f;
            return pos;
        }
    }
    else if (d2 < 0.f)
    {
        d2 = 0.f;
        return ClosestPoint(other.a, d);
    }
    else if (d2 > 1.f)
    {
        d2 = 1.f;
        return ClosestPoint(other.b, d);
    }
    else
        return GetPoint(d);
}
