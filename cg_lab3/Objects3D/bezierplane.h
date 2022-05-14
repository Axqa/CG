#ifndef BEZIERPLANE_H
#define BEZIERPLANE_H

#include "objectgroup.h"


class BezierPlane : public ObjectGroup, public Selectable
{
    Q_OBJECT
public:
    int n = 3; // row deg
    int m = 3; // col deg

    bool showControlLines = true;

    int nSub = 25;
    int mSub = 25;

    QVector<QVector<SelectablePoint*>> controlPoints;

    float3** surfPoints = nullptr;

public:
    BezierPlane();
    BezierPlane(int n, int m);
    ~BezierPlane();

    void RecalcPoints();

public slots:
    void setN(int newN);
    void setM(int newM);

    void RotateXDeg(int value);
    void RotateYDeg(int value);
    void RotateZDeg(int value);

    void SetShowLines(bool state);

    void NeedRecalc();

    // Object3D interface
public:
    QGraphicsItemGroup *DrawOnCameraView(Camera &cam) override;
    Matrix<float> ToMatrix() override;
    void FromMatrix(MatrixF m) override;
    void Normalize() override;

    // Selectable interface
public:
    bool isIntersects(const Ray &ray, float &dist) override;
    void MovingRay(Ray &from, Ray &to) override;

    // Object3D interface
public:
    MatrixF MatrixTransform(MatrixF m) override;
};

#endif // BEZIERPLANE_H
