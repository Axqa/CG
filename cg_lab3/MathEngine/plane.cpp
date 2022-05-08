#include "plane.h"
#include "somemath.h"

Plane::Plane()
{

}

Plane::Plane(const float3 &normal_, float d_)
:normal(normal_), d(d_)
{
}

Plane::Plane(const float3 &v1, const float3 &v2, const float3 &v3)
{
    Set(v1, v2, v3);
}

Plane::Plane(const float3 &point, const float3 &normal_)
{
    Set(point, normal_);
}

void Plane::Set(const float3 &v1, const float3 &v2, const float3 &v3)
{
    normal = (v2-v1).Cross(v3-v1);
    float len = normal.Length();
    normal /= len;
    d = normal.Dot(v1);
}

void Plane::Set(const float3 &point, const float3 &normal_)
{
    normal = normal_;
    d = point.Dot(normal);
}

MatrixF Plane::ProjectionMatrix()
{
    float angleAroundX = 35.26439;
    float angleAroundY = 45;

    MatrixF rotX = MatrixF::RotateX(ToRad(angleAroundX));
    MatrixF rotY = MatrixF::RotateY(ToRad(angleAroundY));

    return rotY*rotX; // maybe *ProjectZ() // i still dont have ProjectZ()
}
