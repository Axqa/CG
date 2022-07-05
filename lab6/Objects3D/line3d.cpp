#include "line3d.h"
#include "../MathEngine/somemath.h"

Line3D::Line3D()
{
}
Line3D::Line3D(float3 p1, float3 p2)
    : p1(p1), p2(p2)
{

}
Line3D::Line3D(float3 p1, float3 p2, QColor color)
    : p1(p1), p2(p2), color(color)
{
}


QGraphicsItemGroup *Line3D::DrawOnCameraView(Camera &cam)
{
    MatrixF prLine = cam.ProjectOnScreen(this);
    Line3D nLine(p1,p2,color);
    nLine.FromMatrix(prLine);

    if (!visible) {
        return new QGraphicsItemGroup();
    }

    cam.sImage.DrawLine(nLine.p1,nLine.p2,color);

//    float dx = p1.x - p2.x;
//    float dy = p1.y - p2.y;
//    int lineSegmCount =  Sqrt(dx*dx + dy*dy);
    QGraphicsItemGroup *group = new QGraphicsItemGroup();
//    group->addToGroup(line);

//    itemOnScene = group;

    return group;
}

MatrixF Line3D::ToMatrix()
{
    return MatrixF ({
        {p1.x, p1.y, p1.z, 1},
        {p2.x, p2.y, p2.z, 1}
    });
//    Row<float> a ({1,2,3});
}

void Line3D::FromMatrix(MatrixF m)
{
    m = m.NormalizedW();
    p1.x = m[0][0]; p2.x = m[1][0];
    p1.y = m[0][1]; p2.y = m[1][1];
    p1.z = m[0][2]; p2.z = m[1][2];
}

void Line3D::Normalize()
{

}

QGraphicsItemGroup *Line3D::LineToSegments(const float3 &p1, const float3 &p2, int segmCount, QPen pen, QGraphicsItemGroup *group)
{
    if (group == nullptr)
        group= new QGraphicsItemGroup();
    float3 prevPoint = p1;
    float dx = (p2.x - p1.x) / segmCount;
    float dy = (p2.y - p1.y) / segmCount;
    float dz = (p2.z - p1.z) / segmCount;
    for (int i = 0; i < segmCount; ++i) {
        float x1 = prevPoint.x, x2 = prevPoint.x + dx;
        float y1 = prevPoint.y, y2 = prevPoint.y + dy;
        float z = prevPoint.z;
        QGraphicsLineItem *lineSegm = new QGraphicsLineItem(x1, y1, x2, y2);
        lineSegm->setPen(pen);
        lineSegm->setZValue(z);
        group->addToGroup(lineSegm);

        prevPoint += {dx,dy,dz};
    }
    return group;
}
