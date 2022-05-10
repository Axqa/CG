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

void Plane::ReverseNormal()
{
    normal = -normal;
    d = -d;
}

vec Plane::PointOnPlane() const
{
    return normal * d;
}

vec Plane::Point(float u, float v) const
{
    vec b1, b2;
    normal.PerpendicularBasis(b1, b2);
    vec res = PointOnPlane() + b1 * u + b2 * v;
//    res.y = - res.y;
    return res;
}


void Plane::Translate(const vec &offset)
{
    d -= normal.Dot(offset);
}

MatrixF Plane::ProjectionMatrix(float3 camPoint, float perspectiveDist, bool addPersp)
{
//    float angleAroundX = 35.26439;
//    float angleAroundY = 45;

    CalculateProjMatrix(camPoint, perspectiveDist, addPersp);
    return projMatrix;
}

void Plane::CalculateProjMatrix(float3 camPoint, float perspectiveDist, bool addPersp)
{
    float angleAroundX = 0;
    float angleAroundY = 0;

//    qDebug() << "Normal: " << normal.x << normal.y << normal.z;

    MatrixF move = MatrixF::MoveBy(-camPoint.x, -camPoint.y, -camPoint.z);

    MatrixF persp = MatrixF::CreateEye(4);

    if (addPersp) {
        persp.at(2,3) = 1/perspectiveDist;
    }

    float angleY = 0, angleX = 0;

    vec cNorm = normal;
    vec x0z = cNorm; x0z.y = 0;

//    if (normal.z >= 0) {

//        if (!x0z.IsZero()){
//            angleY = x0z.AngleBetween({0,0,1});
//            if (normal.x > 0) angleY *= -1;
//        }
//        cNorm.RotateAroundY(-angleY);
//    //    qDebug() << "after 1st rot:" << cNorm;
//        angleX = cNorm.AngleBetween({0,0,1});
//        if (normal.y < 0) angleX *= -1;

//        cNorm.RotateAroundX(-angleX);
//    }
//    else {
//        if (!x0z.IsZero()) {
//            angleY = x0z.AngleBetween({0,0,-1});
//            if (normal.x < 0) angleY *= -1;
//        }
//        cNorm.RotateAroundY(-angleY);
//    //    qDebug() << "after 1st rot:" << cNorm;
//        angleX = cNorm.AngleBetween({0,0,-1});
//        if (normal.y > 0) angleX *= -1;

//        cNorm.RotateAroundX(-angleX);
//    }

//    qDebug() << "after 2nd rot:" << cNorm;
//    cNorm.RotateAroundX(angleX);
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
//    if (normal.z < 0) {
////        angleAroundX *= -1;
////        angleAroundY *= -1;
//    }
//    qDebug() << "rot for z:" << float3(normal.x, normal.y, 0).AngleBetween({1,0,0});
//    qDebug() << "rot (arX, arY):" << angleAroundX << angleAroundY;
    MatrixF rotY = MatrixF::RotateY(angleAroundY);
    MatrixF rotX = MatrixF::RotateX(angleAroundX);
//    MatrixF rotZ = MatrixF::RotateZ(-float3(normal.x, normal.y, 0).AngleBetween({1,0,0}));
    projMatrix = move*rotY*rotX*persp;
}
