#include "ray.h"
#include "float3.h"
#include "somemath.h"

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

float3 Ray::ClosestPoint(const float3 &targetPoint, float &d) const
{
    d = Max(0.f, Dot(targetPoint - pos, dir));
    return GetPoint(d);
}
