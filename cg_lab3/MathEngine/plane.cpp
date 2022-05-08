#include "plane.h"
#include "somemath.h"
#include <QDebug>

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
//    float angleAroundX = 35.26439;
//    float angleAroundY = 45;

    CalculateProjMatrix();
    return projMatrix;
}

void Plane::CalculateProjMatrix()
{
    float angleAroundX = 0;
    float angleAroundY = 0;

//    qDebug() << "Normal: " << normal.x << normal.y << normal.z;

    vec yz0 = normal; yz0.x = 0;
    if (yz0.IsZero())
        angleAroundX = 0;
    else {
        angleAroundX = yz0.AngleBetween({0,0,1});
        if (normal.y < 0) angleAroundX *= -1;
    }
    vec xz0 = normal; xz0.y = 0;
    if (xz0.IsZero())
        angleAroundY = 0;
    else {
        angleAroundY = xz0.AngleBetween({0,0,1});
        if (normal.x > 0) angleAroundY *= -1;

    }
//    qDebug() << "rot (arX, arY):" << angleAroundX << angleAroundY;
    MatrixF rotX = MatrixF::RotateX(angleAroundX);
    MatrixF rotY = MatrixF::RotateY(angleAroundY);

    projMatrix = rotY*rotX;
}
