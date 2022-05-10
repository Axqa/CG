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

QGraphicsItemGroup* Point3::DrawOnCameraView(Camera &cam)
{
    MatrixF prPoint = cam.ProjectOnScreen(this);
    Point3 nPoint;
    nPoint.FromMatrix(prPoint);

    float left = nPoint.pos.x - nPoint.rad;
    float top  = nPoint.pos.y - nPoint.rad;

    QGraphicsEllipseItem *el = new QGraphicsEllipseItem(left, top, nPoint.rad*2, nPoint.rad*2);

    el->setPen(QColor(Qt::black));
    el->setBrush(color);

    QGraphicsItemGroup *group = new QGraphicsItemGroup();
    group->addToGroup(el);

    itemOnScene = group;

    return group;
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
