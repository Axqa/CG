#include "line3d.h"

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
    Line3D nLine;
    nLine.FromMatrix(prLine);

    QGraphicsLineItem *line = new QGraphicsLineItem(nLine.p1.x, nLine.p1.y, nLine.p2.x, nLine.p2.y);

    line->setPen(QPen(QColor(color),2));

    QGraphicsItemGroup *group = new QGraphicsItemGroup();
    group->addToGroup(line);

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
    p1.x = m[0][0]; p2.x = m[1][0];
    p1.y = m[0][1]; p2.y = m[1][1];
    p1.z = m[0][2]; p2.z = m[1][2];
}

void Line3D::Normalize()
{
}
