#ifndef POINT3_H
#define POINT3_H

#include "clss.h"
#include "../MathEngine/float3.h"
#include "object3d.h"

class Point3 : public Object3D
{
    Q_OBJECT
public:
    float3 pos;
    float rad=10;
    QColor color = Qt::yellow;
public:
    Point3();
    Point3(float x, float y, float z);
    Point3(float x, float y, float z, QColor color);
    Point3(float x, float y, float z, float rad);
    Point3(float x, float y, float z, float rad, QColor color);

    QRectF RectForPainter();

    // Object3D interface
public:
    QGraphicsItemGroup* DrawOnCameraView(Camera &cam) override;

    // Object3D interface
public:
    Matrix<float> ToMatrix() override;
    void FromMatrix(Matrix<float> m) override;
    void Normalize() override;

protected:
    Type3d _type = Type3d::Point;
};

#endif // POINT3_H
