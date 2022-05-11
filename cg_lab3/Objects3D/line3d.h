#ifndef LINE3D_H
#define LINE3D_H

#include "object3d.h"
#include <QObject>
#include "clss.h"
#include "../MathEngine/float3.h"

class Line3D : public Object3D
{
    Q_OBJECT
public:
    float3 p1;
    float3 p2;
    QColor color = Qt::black;

public:
    Line3D();
    Line3D(float3 p1, float3 p2);
    Line3D(float3 p1, float3 p2, QColor color);

    // Object3D interface
public:
    QGraphicsItemGroup *DrawOnCameraView(Camera &cam) override;
    Matrix<float> ToMatrix() override;
    void FromMatrix(Matrix<float> m) override;
    void Normalize() override;

    QGraphicsItemGroup* LineToSegments(const float3 &p1, const float3 &p2, int segmCount, QPen pen,QGraphicsItemGroup *group = nullptr);


};

#endif // LINE3D_H
