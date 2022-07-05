#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"
#include "selectablepoint.h"
#include "material.h"

class PointLight : public SelectablePoint,  public Light
{

public:
    PointLight();
    PointLight(vec p) {this->pos = p;};

    QColor color = QColor(Qt::white);
    float rad = 3;

    float intensity = 100 ;

    // Object3D interface
public:
    QGraphicsItemGroup *DrawOnCameraView(Camera &cam) override;

    // Light interface
public:
    float IntensityForPointOnPlane(vec p, Plane plane, vec cam, Material &mat) override;

    // Selectable interface
public:
    bool isIntersects(const Ray &ray, float &dist) override;
    void MovingRay(Ray &from, Ray &to) override;

    // Object3D interface
public:
    Matrix<float> ToMatrix() override;
    void FromMatrix(MatrixF m) override;
    void Normalize() override;
};

#endif // POINTLIGHT_H
