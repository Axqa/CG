#include "line3d.h"
#include "../MathEngine/linesegment.h"

Line3D::Line3D(QObject *parent) : Object3D(parent)
{
    init();
}

void Line3D::init()
{
    RecalcBB();
}


bool Line3D::GetColorByRay(Ray &ray, QColor &color, float &dist)
{
    float d;

    float distToLine = ray.Distance(LineSegment(p1,p2), dist, d);

    color = this->color;
    color.setAlphaF(distToLine <= width ? ((width-distToLine) / width) : 1);

    return distToLine < width;
}


void Line3D::RecalcBB()
{
    boundingBox.SetFrom(&p1,2);
    boundingBox.minPoint -= float3(width/2);
    boundingBox.maxPoint += float3(width/2);
//    boundingBox.Scale(boundingBox.CenterPoint(), )
}
