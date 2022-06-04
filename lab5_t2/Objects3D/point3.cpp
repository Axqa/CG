#include "point3.h"
#include <QGraphicsEllipseItem>
#include "../myassert.h"
#include <QDebug>

Point3::Point3()
{

}

Point3::Point3(float x, float y, float z)
    : pos(x,y,z)
{
}
Point3::Point3(float x, float y, float z, QColor color)
    : pos(x,y,z), color(color)
{
}

Point3::Point3(float x, float y, float z, float rad)
    : pos(x,y,z), rad(rad)
{
}
Point3::Point3(float x, float y, float z, float rad, QColor color)
    : pos(x,y,z), rad(rad), color(color)
{
}

void Point3::Set(float x, float y, float z, float rad, QColor color)
{
    Set({x,y,z}, rad, color);
}

void Point3::Set(float3 pos, float rad, QColor color)
{
    this->pos = pos; this->rad = rad; this-> color = color;
}

QRectF Point3::RectForPainter()
{
    float left = pos.x - rad;
    float top  = pos.y - rad;

    return QRectF(left, top, rad*2, rad*2);
}

QGraphicsItemGroup* Point3::DrawOnCameraView(Camera &cam)
{
    MatrixF prPoint = cam.ProjectOnScreen(this);
    Point3 nPoint (pos.x,pos.y,pos.z,rad,color);
    nPoint.FromMatrix(prPoint);

    if (!visible) {
        return new QGraphicsItemGroup();
    }

    QGraphicsEllipseItem *el = new QGraphicsEllipseItem(nPoint.RectForPainter());

    cam.sImage.DrawSphere(nPoint.pos, 5, color);

    return new QGraphicsItemGroup();
}

MatrixF Point3::ToMatrix()
{
    MatrixF m ({
        {pos.x, pos.y, pos.z, 1}
    });
    return m;
}

void Point3::FromMatrix(MatrixF m)
{
    rad = rad / m[0][3];
    m = m.NormalizedW();
    // hope m is 1x4 or 1x3
    massert(m.ColCount() > 2, "Wrong dims matrix");
    pos.x = m[0][0];
    pos.y = m[0][1];
    pos.z = m[0][2];

}

void Point3::Normalize()
{
}
