#include "line.h"
#include "../myassert.h"
#include "ray.h"
#include "linesegment.h"

/// Computes the closest point pair on two lines.
/** The first line is specified by two points start0 and end0. The second line is specified by
    two points start1 and end1.
    The implementation of this function follows http://paulbourke.net/geometry/lineline3d/ .
    @param v0 The starting point of the first line.
    @param v10 The direction vector of the first line. This can be unnormalized.
    @param v2 The starting point of the second line.
    @param v32 The direction vector of the second line. This can be unnormalized.
    @param d [out] Receives the normalized distance of the closest point along the first line.
    @param d2 [out] Receives the normalized distance of the closest point along the second line.
    @return Returns the closest point on line start0<->end0 to the second line.
    @note This is a low-level utility function. You probably want to use ClosestPoint() or Distance() instead.
    @see ClosestPoint(), Distance(). */
void Line::ClosestPointLineLine(const vec &v0, const vec &v10, const vec &v2, const vec &v32, float &d, float &d2)
{
    assert(!v10.IsZero());
    assert(!v32.IsZero());
    vec v02 = v0 - v2;
    float d0232 = v02.Dot(v32);
    float d3210 = v32.Dot(v10);
    float d3232 = v32.Dot(v32);
    assert(d3232 != 0.f); // Don't call with a zero direction vector.
    float d0210 = v02.Dot(v10);
    float d1010 = v10.Dot(v10);
    float denom = d1010*d3232 - d3210*d3210;
    if (denom != 0.f)
        d = (d0232*d3210 - d0210*d3232) / denom;
    else
        d = 0.f;
    d2 = (d0232 + d * d3210) / d3232;
}

Line::Line(const vec &pos_, const vec &dir_)
:pos(pos_), dir(dir_)
{
}

Line::Line(const Ray &ray)
:pos(ray.pos), dir(ray.dir)
{
}

Line::Line(const LineSegment &lineSegment)
:pos(lineSegment.a), dir(lineSegment.Dir())
{
}

vec Line::GetPoint(float d) const
{
    return pos + dir * d;
}

float Line::Distance(const vec &point, float &d) const
{
    return ClosestPoint(point, d).Distance(point);
}

float Line::Distance(const Ray &other, float &d, float &d2) const
{
    vec c = ClosestPoint(other, d, d2);
    return c.Distance(other.GetPoint(d2));
}

float Line::Distance(const Line &other, float &d, float &d2) const
{
    vec c = ClosestPoint(other, d, d2);
    return c.Distance(other.GetPoint(d2));
}

float Line::Distance(const LineSegment &other, float &d, float &d2) const
{
    vec c = ClosestPoint(other, d, d2);
    assert(d2 >= 0.f);
    assert(d2 <= 1.f);
    return c.Distance(other.GetPoint(d2));
}

vec Line::ClosestPoint(const vec &targetPoint, float &d) const
{
    d = Dot(targetPoint - pos, dir);
    return GetPoint(d);
}

vec Line::ClosestPoint(const Ray &other, float &d, float &d2) const
{
    ClosestPointLineLine(pos, dir, other.pos, other.dir, d, d2);
    if (d2 >= 0.f)
        return GetPoint(d);
    else
    {
        d2 = 0.f;
        return ClosestPoint(other.pos, d);
    }
}

vec Line::ClosestPoint(const Line &other, float &d, float &d2) const
{
    ClosestPointLineLine(pos, dir, other.pos, other.dir, d, d2);
    return GetPoint(d);
}

vec Line::ClosestPoint(const LineSegment &other, float &d, float &d2) const
{
    ClosestPointLineLine(pos, dir, other.a, other.b - other.a, d, d2);
    if (d2 < 0.f)
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

Ray Line::ToRay() const
{
    return Ray(pos, dir);
}

LineSegment Line::ToLineSegment(float d) const
{
    return LineSegment(pos, GetPoint(d));
}

LineSegment Line::ToLineSegment(float dStart, float dEnd) const
{
    return LineSegment(GetPoint(dStart), GetPoint(dEnd));
}
