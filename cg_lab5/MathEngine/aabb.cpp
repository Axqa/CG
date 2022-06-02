#include "aabb.h"
#include "somemath.h"
#include "line.h"
#include "linesegment.h"
#include "plane.h"
#include "ray.h"

AABB::AABB(const vec &minPoint_, const vec &maxPoint_)
:minPoint(minPoint_), maxPoint(maxPoint_)
{
}

void AABB::SetNegativeInfinity()
{
    minPoint = {FLOAT_INF,FLOAT_INF,FLOAT_INF};
    maxPoint = {-FLOAT_INF,-FLOAT_INF,-FLOAT_INF};
}

void AABB::SetFromCenterAndSize(const vec &center, const vec &size)
{
    vec halfSize = 0.5f * size;
    minPoint = center - halfSize;
    maxPoint = center + halfSize;
}

void AABB::SetFrom(const vec *pointArray, int numPoints)
{
    assert(pointArray || numPoints == 0);
    SetNegativeInfinity();
    if (!pointArray)
        return;
    for(int i = 0; i < numPoints; ++i)
        Enclose(pointArray[i]);
}



bool AABB::IsDegenerate() const
{
#ifdef _MSC_VER
    // MSVC generates code that asserts nans can't be present - add an explicit check for that case.
    return IsNan(minPoint.x) || IsNan(minPoint.y) || IsNan(minPoint.z) ||
        IsNan(maxPoint.x) || IsNan(maxPoint.y) || IsNan(maxPoint.z) ||
        !(minPoint.x < maxPoint.x && minPoint.y < maxPoint.y && minPoint.z < maxPoint.z);
#else
    return !(minPoint.x < maxPoint.x && minPoint.y < maxPoint.y && minPoint.z < maxPoint.z);
#endif
}

vec AABB::CenterPoint() const
{
    return (minPoint + maxPoint) * 0.5f;
}

LineSegment AABB::Edge(int edgeIndex) const
{
    assert(0 <= edgeIndex && edgeIndex <= 11);
    switch(edgeIndex)
    {
        default: // For release builds where assert() is disabled, return always the first option if out-of-bounds.
        /* For documentation, here's the segments that are returned:
        case 0: return LineSegment(CornerPoint(0), CornerPoint(1));
        case 1: return LineSegment(CornerPoint(0), CornerPoint(2));
        case 2: return LineSegment(CornerPoint(0), CornerPoint(4));
        case 3: return LineSegment(CornerPoint(1), CornerPoint(3));
        case 4: return LineSegment(CornerPoint(1), CornerPoint(5));
        case 5: return LineSegment(CornerPoint(2), CornerPoint(3));
        case 6: return LineSegment(CornerPoint(2), CornerPoint(6));
        case 7: return LineSegment(CornerPoint(3), CornerPoint(7));
        case 8: return LineSegment(CornerPoint(4), CornerPoint(5));
        case 9: return LineSegment(CornerPoint(4), CornerPoint(6));
        case 10: return LineSegment(CornerPoint(5), CornerPoint(7));
        case 11: return LineSegment(CornerPoint(6), CornerPoint(7));
        */
        // Force-optimize to avoid calling to CornerPoint for another switch-case statement.
        case 0: return LineSegment(minPoint, float3(minPoint.x, minPoint.y, maxPoint.z));
        case 1: return LineSegment(minPoint, float3(minPoint.x, maxPoint.y, minPoint.z));
        case 2: return LineSegment(minPoint, float3(maxPoint.x, minPoint.y, minPoint.z));
        case 3: return LineSegment(float3(minPoint.x, minPoint.y, maxPoint.z), float3(minPoint.x, maxPoint.y, maxPoint.z));
        case 4: return LineSegment(float3(minPoint.x, minPoint.y, maxPoint.z), float3(maxPoint.x, minPoint.y, maxPoint.z));
        case 5: return LineSegment(float3(minPoint.x, maxPoint.y, minPoint.z), float3(minPoint.x, maxPoint.y, maxPoint.z));
        case 6: return LineSegment(float3(minPoint.x, maxPoint.y, minPoint.z), float3(maxPoint.x, maxPoint.y, minPoint.z));
        case 7: return LineSegment(float3(minPoint.x, maxPoint.y, maxPoint.z), maxPoint);
        case 8: return LineSegment(float3(maxPoint.x, minPoint.y, minPoint.z), float3(maxPoint.x, minPoint.y, maxPoint.z));
        case 9: return LineSegment(float3(maxPoint.x, minPoint.y, minPoint.z), float3(maxPoint.x, maxPoint.y, minPoint.z));
        case 10: return LineSegment(float3(maxPoint.x, minPoint.y, maxPoint.z), maxPoint);
        case 11: return LineSegment(float3(maxPoint.x, maxPoint.y, minPoint.z), maxPoint);
    }
}

vec AABB::CornerPoint(int cornerIndex) const
{
    assert(0 <= cornerIndex && cornerIndex <= 7);
    switch(cornerIndex)
    {
        default: // For release builds where assert() is disabled, return always the first option if out-of-bounds.
        case 0: return minPoint;
        case 1: return float3(minPoint.x, minPoint.y, maxPoint.z);
        case 2: return float3(minPoint.x, maxPoint.y, minPoint.z);
        case 3: return float3(minPoint.x, maxPoint.y, maxPoint.z);
        case 4: return float3(maxPoint.x, minPoint.y, minPoint.z);
        case 5: return float3(maxPoint.x, minPoint.y, maxPoint.z);
        case 6: return float3(maxPoint.x, maxPoint.y, minPoint.z);
        case 7: return maxPoint;
    }
}

vec AABB::ExtremePoint(const vec &direction) const
{
    return float3((direction.x >= 0.f ? maxPoint.x : minPoint.x),
                     (direction.y >= 0.f ? maxPoint.y : minPoint.y),
                     (direction.z >= 0.f ? maxPoint.z : minPoint.z));
}

vec AABB::ExtremePoint(const vec &direction, float &projectionDistance) const
{
    vec extremePoint = ExtremePoint(direction);
    projectionDistance = extremePoint.Dot(direction);
    return extremePoint;
}

vec AABB::PointOnEdge(int edgeIndex, float u) const
{
    assert(0 <= edgeIndex && edgeIndex <= 11);
    assert(0 <= u && u <= 1.f);

    vec d = maxPoint - minPoint;
    switch(edgeIndex)
    {
    default: // For release builds where assert() is disabled, return always the first option if out-of-bounds.
    case 0: return float3(minPoint.x, minPoint.y, minPoint.z + u * d.z);
    case 1: return float3(minPoint.x, maxPoint.y, minPoint.z + u * d.z);
    case 2: return float3(maxPoint.x, minPoint.y, minPoint.z + u * d.z);
    case 3: return float3(maxPoint.x, maxPoint.y, minPoint.z + u * d.z);

    case 4: return float3(minPoint.x, minPoint.y + u * d.y, minPoint.z);
    case 5: return float3(maxPoint.x, minPoint.y + u * d.y, minPoint.z);
    case 6: return float3(minPoint.x, minPoint.y + u * d.y, maxPoint.z);
    case 7: return float3(maxPoint.x, minPoint.y + u * d.y, maxPoint.z);

    case 8: return float3(minPoint.x + u * d.x, minPoint.y, minPoint.z);
    case 9: return float3(minPoint.x + u * d.x, minPoint.y, maxPoint.z);
    case 10: return float3(minPoint.x + u * d.x, maxPoint.y, minPoint.z);
    case 11: return float3(minPoint.x + u * d.x, maxPoint.y, maxPoint.z);
    }
}

vec AABB::FaceCenterPoint(int faceIndex) const
{
    assert(0 <= faceIndex && faceIndex <= 5);

    vec center = (minPoint + maxPoint) * 0.5f;
    switch(faceIndex)
    {
    default: // For release builds where assert() is disabled, return always the first option if out-of-bounds.
    case 0: return float3(minPoint.x, center.y, center.z);
    case 1: return float3(maxPoint.x, center.y, center.z);
    case 2: return float3(center.x, minPoint.y, center.z);
    case 3: return float3(center.x, maxPoint.y, center.z);
    case 4: return float3(center.x, center.y, minPoint.z);
    case 5: return float3(center.x, center.y, maxPoint.z);
    }
}

vec AABB::FacePoint(int faceIndex, float u, float v) const
{
    assert(0 <= faceIndex && faceIndex <= 5);
    assert(0 <= u && u <= 1.f);
    assert(0 <= v && v <= 1.f);

    vec d = maxPoint - minPoint;
    switch(faceIndex)
    {
    default: // For release builds where assert() is disabled, return always the first option if out-of-bounds.
    case 0: return float3(minPoint.x, minPoint.y + u * d.y, minPoint.z + v * d.z);
    case 1: return float3(maxPoint.x, minPoint.y + u * d.y, minPoint.z + v * d.z);
    case 2: return float3(minPoint.x + u * d.x, minPoint.y, minPoint.z + v * d.z);
    case 3: return float3(minPoint.x + u * d.x, maxPoint.y, minPoint.z + v * d.z);
    case 4: return float3(minPoint.x + u * d.x, minPoint.y + v * d.y, minPoint.z);
    case 5: return float3(minPoint.x + u * d.x, minPoint.y + v * d.y, maxPoint.z);
    }
}

vec AABB::FaceNormal(int faceIndex) const
{
    assert(0 <= faceIndex && faceIndex <= 5);
    switch(faceIndex)
    {
    default: // For release builds where assert() is disabled, return always the first option if out-of-bounds.
    case 0: return float3(-1,  0,  0);
    case 1: return float3( 1,  0,  0);
    case 2: return float3( 0, -1,  0);
    case 3: return float3( 0,  1,  0);
    case 4: return float3( 0,  0, -1);
    case 5: return float3( 0,  0,  1);
    }
}

Plane AABB::FacePlane(int faceIndex) const
{
    assert(0 <= faceIndex && faceIndex <= 5);
    return Plane(FaceCenterPoint(faceIndex), FaceNormal(faceIndex));
}

void AABB::GetCornerPoints(vec *outPointArray) const
{
    assert(outPointArray);
    for(int i = 0; i < 8; ++i)
        outPointArray[i] = CornerPoint(i);
}

void AABB::GetFacePlanes(Plane *outPlaneArray) const
{
    assert(outPlaneArray);
    for(int i = 0; i < 6; ++i)
        outPlaneArray[i] = FacePlane(i);
}

AABB AABB::MinimalEnclosingAABB(const vec *pointArray, int numPoints)
{
    AABB aabb;
    aabb.SetFrom(pointArray, numPoints);
    return aabb;
}


AABB AABB::FromCenterAndSize(const vec &aabbCenterPos, const vec &aabbSize)
{
    vec halfSize = aabbSize * 0.5f;
    return AABB(aabbCenterPos - halfSize, aabbCenterPos + halfSize);
}

vec AABB::Size() const
{
    return maxPoint - minPoint;
}

vec AABB::HalfSize() const
{
    return Size() * 0.5f;
}

float AABB::Volume() const
{
    vec sz = Size();
    return sz.x * sz.y * sz.z;
}

float AABB::SurfaceArea() const
{
    vec size = Size();
    return 2.f * (size.x*size.y + size.x*size.z + size.y*size.z);
}


void AABB::Scale(const vec &centerPoint, float scaleFactor)
{
    minPoint = (minPoint - centerPoint) * scaleFactor + centerPoint;
    maxPoint = (maxPoint - centerPoint) * scaleFactor + centerPoint;
}


vec AABB::ClosestPoint(const vec &targetPoint) const
{

    return targetPoint.Clamp(minPoint, maxPoint);
}

float AABB::Distance(const vec &point) const
{
    ///@todo This function could be slightly optimized. See Christer Ericson's
    /// Real-Time Collision Detection, p.131.
    return ClosestPoint(point).Distance(point);
}

bool AABB::Contains(const vec &point) const
{
// Benchmarking this code is very difficult, since branch prediction makes the scalar version
// look very good. In isolation the scalar version might be better, however when joined with
// other SSE computation, the SIMD variants are probably more efficient because the data is
// already "hot" in the registers. Therefore favoring the SSE version over the scalar version
// when possible.

    // Benchmark 'AABBContains_positive': AABB::Contains(point) positive
    //    Best: 2.108 nsecs / 3.6022 ticks, Avg: 2.232 nsecs, Worst: 4.638 nsecs
    // Benchmark 'AABBContains_negative': AABB::Contains(point) negative
    //    Best: 1.988 nsecs / 3.361 ticks, Avg: 2.148 nsecs, Worst: 4.457 nsecs
    // Benchmark 'AABBContains_unpredictable': AABB::Contains(point) unpredictable
    //    Best: 3.554 nsecs / 6.0764 ticks, Avg: 3.803 nsecs, Worst: 6.264 nsecs
    return minPoint.x <= point.x && point.x <= maxPoint.x &&
           minPoint.y <= point.y && point.y <= maxPoint.y &&
           minPoint.z <= point.z && point.z <= maxPoint.z;
}

bool AABB::Contains(const LineSegment &lineSegment) const
{
    return Contains(Min(lineSegment.a, lineSegment.b), Max(lineSegment.a, lineSegment.b));
}

bool AABB::Contains(const vec &aabbMinPoint, const vec &aabbMaxPoint) const
{
    return minPoint.x <= aabbMinPoint.x && maxPoint.x >= aabbMaxPoint.x &&
           minPoint.y <= aabbMinPoint.y && maxPoint.y >= aabbMaxPoint.y &&
           minPoint.z <= aabbMinPoint.z && maxPoint.z >= aabbMaxPoint.z;
}

bool AABB::IntersectLineAABB(const vec &linePos, const vec &lineDir, float &tNear, float &tFar) const
{
    // Never call the SSE version here. The SSE version does not output tNear and tFar, because
    // the memory stores have been profiled to make it slower than the CPP version. Therefore the SSE
    // version does not output tNear and tFar (profile shows it to be about 10x faster than the CPP version).
    return IntersectLineAABB_CPP(linePos, lineDir, tNear, tFar);
}

bool AABB::Intersects(const Line &line) const
{
    float tNear = -FLOAT_INF;
    float tFar = FLOAT_INF;

    return IntersectLineAABB_CPP(line.pos, line.dir, tNear, tFar);
}

bool AABB::Intersects(const Ray &ray) const
{
    float tNear = 0;
    float tFar = FLOAT_INF;

    return IntersectLineAABB_CPP(ray.pos, ray.dir, tNear, tFar);
}

bool AABB::Intersects(const LineSegment &lineSegment) const
{
    vec dir = lineSegment.b - lineSegment.a;
    float len = dir.Length();
    if (len <= 1e-4f) // Degenerate line segment? Fall back to point-in-AABB test.
        return Contains(lineSegment.a);

    float invLen = 1.f / len;
    dir *= invLen;
    float tNear = 0.f, tFar = len;
#ifdef MATH_SIMD
    return IntersectLineAABB_SSE(lineSegment.a, dir, tNear, tFar);
#else
    return IntersectLineAABB_CPP(lineSegment.a, dir, tNear, tFar);
#endif
}

bool AABB::IntersectLineAABB_CPP(const vec &linePos, const vec &lineDir, float &tNear, float &tFar) const
{
    assert(lineDir.IsNormalized());
    assert(lineDir.LengthSq());
    assert(tNear <= tFar && "AABB::IntersectLineAABB: User gave a degenerate line as input for the intersection test!");
//    assert(tNear);
//    assert(tFar);
    // The user should have inputted values for tNear and tFar to specify the desired subrange [tNear, tFar] of the line
    // for this intersection test.
    // For a Line-AABB test, pass in
    //    tNear = -FLOAT_INF;
    //    tFar = FLOAT_INF;
    // For a Ray-AABB test, pass in
    //    tNear = 0.f;
    //    tFar = FLOAT_INF;
    // For a LineSegment-AABB test, pass in
    //    tNear = 0.f;
    //    tFar = LineSegment.Length();

    // Test each cardinal plane (X, Y and Z) in turn.
    if (!EqualAbs(lineDir.x, 0.f))
    {
        float recipDir = 1/lineDir.x;
        float t1 = (minPoint.x - linePos.x) * recipDir;
        float t2 = (maxPoint.x - linePos.x) * recipDir;

        // tNear tracks distance to intersect (enter) the AABB.
        // tFar tracks the distance to exit the AABB.
        if (t1 < t2)
            tNear = Max(t1, tNear), tFar = Min(t2, tFar);
        else // Swap t1 and t2.
            tNear = Max(t2, tNear), tFar = Min(t1, tFar);

        if (tNear > tFar)
            return false; // Box is missed since we "exit" before entering it.
    }
    else if (linePos.x < minPoint.x || linePos.x > maxPoint.x)
        return false; // The ray can't possibly enter the box, abort.

    if (!EqualAbs(lineDir.y, 0.f))
    {
        float recipDir =1/lineDir.y;
        float t1 = (minPoint.y - linePos.y) * recipDir;
        float t2 = (maxPoint.y - linePos.y) * recipDir;

        if (t1 < t2)
            tNear = Max(t1, tNear), tFar = Min(t2, tFar);
        else // Swap t1 and t2.
            tNear = Max(t2, tNear), tFar = Min(t1, tFar);

        if (tNear > tFar)
            return false; // Box is missed since we "exit" before entering it.
    }
    else if (linePos.y < minPoint.y || linePos.y > maxPoint.y)
        return false; // The ray can't possibly enter the box, abort.

    if (!EqualAbs(lineDir.z, 0.f)) // ray is parallel to plane in question
    {
        float recipDir = 1/lineDir.z;
        float t1 = (minPoint.z - linePos.z) * recipDir;
        float t2 = (maxPoint.z - linePos.z) * recipDir;

        if (t1 < t2)
            tNear = Max(t1, tNear), tFar = Min(t2, tFar);
        else // Swap t1 and t2.
            tNear = Max(t2, tNear), tFar = Min(t1, tFar);
    }
    else if (linePos.z < minPoint.z || linePos.z > maxPoint.z)
        return false; // The ray can't possibly enter the box, abort.

    return tNear <= tFar;
}

bool AABB::Intersects(const Ray &ray, float &dNear, float &dFar) const
{
    dNear = 0.f;
    dFar = FLOAT_INF;
    return IntersectLineAABB(ray.pos, ray.dir, dNear, dFar);
}

bool AABB::Intersects(const Line &line, float &dNear, float &dFar) const
{
    dNear = -FLOAT_INF;
    dFar = FLOAT_INF;
    return IntersectLineAABB(line.pos, line.dir, dNear, dFar);
}

bool AABB::Intersects(const LineSegment &lineSegment, float &dNear, float &dFar) const
{
    vec dir = lineSegment.b - lineSegment.a;
    float len = dir.Length();
    if (len <= 1e-4f) // Degenerate line segment? Fall back to point-in-AABB test.
    {
        dNear = 0.f;
        dFar = 1.f;
        return Contains(lineSegment.a);
    }
    float invLen = 1.f / len;
    dir *= invLen;
    dNear = 0.f;
    dFar = len;
    bool hit = IntersectLineAABB(lineSegment.a, dir, dNear, dFar);
    dNear *= invLen;
    dFar *= invLen;
    return hit;
}

bool AABB::Intersects(const AABB &aabb) const
{

    // Benchmark 'AABBIntersectsAABB_positive': AABB::Intersects(AABB) positive
    //    Best: 2.108 nsecs / 3.588 ticks, Avg: 2.310 nsecs, Worst: 5.481 nsecs
    // Benchmark 'AABBIntersectsAABB_random': AABB::Intersects(AABB) random
    //    Best: 7.529 nsecs / 12.8282 ticks, Avg: 8.892 nsecs, Worst: 16.323 nsecs

    // If any of the cardinal X,Y,Z axes is a separating axis, then
    // there is no intersection.
    return minPoint.x < aabb.maxPoint.x &&
           minPoint.y < aabb.maxPoint.y &&
           minPoint.z < aabb.maxPoint.z &&
           aabb.minPoint.x < maxPoint.x &&
           aabb.minPoint.y < maxPoint.y &&
           aabb.minPoint.z < maxPoint.z;
}

void AABB::ProjectToAxis(const vec &axis, float &dMin, float &dMax) const
{
    vec c = (minPoint + maxPoint) * 0.5f;
    vec e = maxPoint - c;


    // Compute the projection interval radius of the AABB onto L(t) = aabb.center + t * plane.normal;
    float r = Abs(e[0]*Abs(axis[0]) + e[1]*Abs(axis[1]) + e[2]*Abs(axis[2]));
    // Compute the distance of the box center from plane.
    float s = axis.Dot(c);
    dMin = s - r;
    dMax = s + r;
}


void AABB::Enclose(const vec &point)
{
    minPoint = Min(minPoint, point);
    maxPoint = Max(maxPoint, point);
}

void AABB::Enclose(const LineSegment &lineSegment)
{
    Enclose(Min(lineSegment.a, lineSegment.b), Max(lineSegment.a, lineSegment.b));
}

void AABB::Enclose(const vec &aabbMinPoint, const vec &aabbMaxPoint)
{
    minPoint = Min(minPoint, aabbMinPoint);
    maxPoint = Max(maxPoint, aabbMaxPoint);
}

void AABB::Enclose(const vec *pointArray, int numPoints)
{
    assert(pointArray || numPoints == 0);
    if (!pointArray)
        return;
    for(int i = 0; i < numPoints; ++i)
        Enclose(pointArray[i]);
}

AABB AABB::Intersection(const AABB &aabb) const
{
    return AABB(Max(minPoint, aabb.minPoint), Min(maxPoint, aabb.maxPoint));
}
