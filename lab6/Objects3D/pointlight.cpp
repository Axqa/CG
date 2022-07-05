#include "pointlight.h"


PointLight::PointLight()
{

}


QGraphicsItemGroup *PointLight::DrawOnCameraView(Camera &cam)
{
    MatrixF prPoint = cam.ProjectOnScreen((SelectablePoint*)this);
    SelectablePoint nPoint (pos.x,pos.y,pos.z,rad,color);
//    qDebug() << prPoint;
    nPoint.FromMatrix(prPoint);
    nPoint.setSelected( selected );


    return nPoint.ToGraphGroup(cam);
}

float PointLight::IntensityForPointOnPlane(vec p, Plane plane, vec cam, Material &mat)
{
    vec cNorm = plane.normal;

    if (plane.IsOnPositiveSide(p)) {
        cNorm *= -1;
    }

    bool onSameSide = plane.AreOnSameSide(pos, cam);
    if (!onSameSide) {
        return 0;
    }

    float NormLigthAngle = cNorm.AngleBetween(pos-p);
    if (cos(NormLigthAngle) < 0) {
        NormLigthAngle = acos(-cos(NormLigthAngle));
    }
//    float ViewLightAngle = (pos-p).AngleBetween((pos-cam)) + 2 * NormLigthAngle;
    auto v = pos - p;
    auto nV = -v + cNorm.Mul(cNorm.Mul(v)) *2;

    float ViewLightAngle = nV.AngleBetween(cam - p);
//    float ViewLightAngle = -(pos-p).AngleBetween(cam - p) + 2 * NormLigthAngle;
    float P = 200; // [1..200]

//    float lum = intensity / (pos.Distance(p) ) * ( (mat.Kdif * cos(NormLigthAngle)) + mat.Ks * powf(cos(ViewLightAngle), P));
    float lum = intensity / (pos.Distance(p) ) * ( (mat.Kdif * cos(NormLigthAngle)) + 1 * powf(cos(ViewLightAngle), P));


    if (lum < 0) {
        qDebug() << pos << p << "dist:" << pos.Distance(p) << "kd"<< mat.Kdif * cos(NormLigthAngle)
                 << "ks" << mat.Ks * powf(cos(ViewLightAngle), P)<<"lum:" <<lum;

    }

    return lum;
}

bool PointLight::isIntersects(const Ray &ray, float &dist)
{
    return SelectablePoint::isIntersects(ray, dist);
}

void PointLight::MovingRay(Ray &from, Ray &to)
{
    SelectablePoint::MovingRay(from, to);
}

Matrix<float> PointLight::ToMatrix()
{
    return SelectablePoint::ToMatrix();
}

void PointLight::FromMatrix(MatrixF m)
{
    SelectablePoint::FromMatrix(m);
}

void PointLight::Normalize()
{
}
