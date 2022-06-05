#ifndef POLYGON3D_H
#define POLYGON3D_H

#include "object3d.h"
#include "../MathEngine/float3.h"
#include "../MathEngine/plane.h"

class Polygon3D : public Object3D
{
public:
    Polygon3D();
    Polygon3D(QVector<float3> points);
    void init();

    int NumVertex();

    void SetColor(QColor col);
    QColor GetColor(vec p = vec());

    void AddPoint(vec p);
    QColor NormToColor(vec p = vec());
    void RecalcCenter();
    void RecalcPlane();

    Plane PolygonPlane();

    void DrawWithoutTransform(Camera &cam);

    float3 center;
    Plane plane;
    QVector<float3> points;

    QColor color;
    bool calcColor = true;

    // Object3D interface
public:
    QGraphicsItemGroup *DrawOnCameraView(Camera &cam) override;
    MatrixF ToMatrix() override;
    void FromMatrix(MatrixF m) override;
    void Normalize() override;



};

#endif // POLYGON3D_H
