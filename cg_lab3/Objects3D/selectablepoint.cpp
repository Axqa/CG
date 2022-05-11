#include "selectablepoint.h"
#include "line3d.h"
#include "../MathEngine/ray.h"
#include "../MathEngine/Intersections.h"

SelectablePoint::SelectablePoint()
{
    selected = false;
    initProj();
}

void SelectablePoint::initProj()
{
    projX.pos = {0, pos.y, pos.z}; projX.color = projColor; projX.rad = projRad;
    projY.pos = {pos.x, 0, pos.z}; projY.color = projColor; projY.rad = projRad;
    projZ.pos = {pos.x, pos.y, 0}; projZ.color = projColor; projZ.rad = projRad;
//    selected = true;
}


QGraphicsItemGroup *SelectablePoint::DrawOnCameraView(Camera &cam)
{
    MatrixF prPoint = cam.ProjectOnScreen(this);
    SelectablePoint nPoint;
//    qDebug() << prPoint;
    nPoint.FromMatrix(prPoint);

    QGraphicsEllipseItem *el = new QGraphicsEllipseItem(nPoint.RectForPainter());
    el->setPen(QColor(Qt::black));
    el->setBrush(color);
    el->setZValue(nPoint.pos.z + nPoint.rad);

    QGraphicsItemGroup *group = new QGraphicsItemGroup();
    itemOnScene = group;

    group->addToGroup(el);

    if (selected) {
        QGraphicsEllipseItem *prX = new QGraphicsEllipseItem(nPoint.projX.RectForPainter());
        prX->setPen(QColor(Qt::black));
        prX->setBrush(projColor);
        prX->setZValue(nPoint.projX.pos.z + nPoint.projX.rad);
        group->addToGroup(prX);
        QGraphicsEllipseItem *prY = new QGraphicsEllipseItem(nPoint.projY.RectForPainter());
        prY->setPen(QColor(Qt::black));
        prY->setBrush(projColor);
        prY->setZValue(nPoint.projY.pos.z + nPoint.projY.rad);
        group->addToGroup(prY);
        QGraphicsEllipseItem *prZ = new QGraphicsEllipseItem(nPoint.projZ.RectForPainter());
        prZ->setPen(QColor(Qt::black));
        prZ->setBrush(projColor);
        prZ->setZValue(nPoint.projZ.pos.z + nPoint.projZ.rad);
        group->addToGroup(prZ);

        QList<Line3D*> lines ({
                                  new Line3D(nPoint.pos, nPoint.projX.pos),
                                  new Line3D(nPoint.pos, nPoint.projY.pos),
                                  new Line3D(nPoint.pos, nPoint.projZ.pos),
                                  new Line3D(nPoint.unitX.pos, nPoint.projZ.pos),
                                  new Line3D(nPoint.unitY.pos, nPoint.projZ.pos),
                                  new Line3D(nPoint.unitX.pos, nPoint.projY.pos),
                                  new Line3D(nPoint.unitZ.pos, nPoint.projY.pos),
                                  new Line3D(nPoint.unitY.pos, nPoint.projX.pos),
                                  new Line3D(nPoint.unitZ.pos, nPoint.projX.pos),
        });


        for (auto i : lines) {
            i->LineToSegments(i->p1, i->p2, 10, QPen(supColor, 1), group);
        }
        for (auto i : lines) {
            delete i;
        }
    }


    return group;
}

MatrixF SelectablePoint::ToMatrix()
{
    MatrixF m ({
        {pos.x, pos.y, pos.z, 1},
           projX.pos.ToVector4(),
           projY.pos.ToVector4(),
           projZ.pos.ToVector4(),
                   float3{pos.x,0,0}.ToVector4(),
                   float3{0,pos.y,0}.ToVector4(),
                   float3{0,0,pos.z}.ToVector4(),

    });
    return m;
}

void SelectablePoint::FromMatrix(MatrixF m)
{
    MatrixF norm = m.NormalizedW();
    pos = float3(norm[0]);
    rad = rad / m[0][3];

    projX.pos = float3(norm[1]);
    projX.rad /= m[1][3];

    projY.pos = float3(norm[2]);
    projY.rad /= m[2][3];

    projZ.pos = float3(norm[3]);
    projZ.rad /= m[3][3];

    unitX.pos = float3(norm[4]);
    unitY.pos = float3(norm[5]);
    unitZ.pos = float3(norm[6]);
}

void SelectablePoint::Normalize()
{
}

void SelectablePoint::setSelected(bool state)
{
    selected = state;
    SelectionChanged(selected);
}

bool SelectablePoint::isIntersects(const Ray &ray, float &dist)
{
    if (!selected) {
        bool fl = ray.Distance(pos,dist) < rad;
        qDebug() << "dist =" << dist;
        return fl;
    }

    float dToC;
    float3 clToC = ray.ClosestPoint(pos, dToC);
    float sqdToC = clToC.DistanceSq(pos);
    float dToX;
    float3 clToX = ray.ClosestPoint(projX.pos, dToX);
    float sqdToX = clToX.DistanceSq(projX.pos);
    float dToY;
    float3 clToY = ray.ClosestPoint(projY.pos, dToY);
    float sqdToY = clToY.DistanceSq(projY.pos);
    float dToZ;
    float3 clToZ = ray.ClosestPoint(projZ.pos, dToZ);
    float sqdToZ = clToZ.DistanceSq(projZ.pos);

    if (dToC < __min(__min(dToX,dToY),dToZ)) {
        if (sqdToC < rad*rad * 4) {
            qDebug() << "press in center";
            return true;
        }
    }
    if (dToX < __min(dToY, dToZ)) {
        if (sqdToX < projX.rad * projX.rad * 4) {
            qDebug() << "press in prX";
            return true;
        }
    }
    if (dToY < dToZ) {
        if (sqdToY < projY.rad * projY.rad * 4) {
            qDebug() << "press in prY";
            return true;
        }

    }
    if (1) {
        if (sqdToZ < projZ.rad * projZ.rad * 4) {
            qDebug() << "press in prZ";
            return true;
        }

    }
    return false;


}

void SelectablePoint::MovingRay(Ray &from, Ray &to)
{
    qDebug() << "move ray";
    if (!selected)
        return;
    Ray ray = from;

    float dToC;
    float3 clToC = ray.ClosestPoint(pos, dToC);
    float sqdToC = clToC.DistanceSq(pos);

    float dToX;
    float3 clToX = ray.ClosestPoint(projX.pos, dToX);
    float sqdToX = clToX.DistanceSq(projX.pos);

    float dToY;
    float3 clToY = ray.ClosestPoint(projY.pos, dToY);
    float sqdToY = clToY.DistanceSq(projY.pos);

    float dToZ;
    float3 clToZ = ray.ClosestPoint(projZ.pos, dToZ);
    float sqdToZ = clToZ.DistanceSq(projZ.pos);

    if (dToC < __min(__min(dToX,dToY),dToZ)) {
        qDebug() << "1" << sqdToC;
        if (sqdToC < rad*rad*4) {
            qDebug() << "1_2";
            /// interact with center
            vec newPos = to.GetPoint(dToC) /*+ (pos - clToC)*/;
            pos = newPos;
            initProj();
            ObjectChanged();
            return;
        }
    }
    if (dToX < __min(dToY, dToZ)) {
        qDebug() << "2" << sqdToX;
        if (sqdToX < projX.rad * projX.rad*10) {
            qDebug() << "2_2";
            /// interact with projX
            vec newPos;
            Plane plane({1,0,0},0);

            if (RayPlaneIntersection(to, plane, newPos)) {
                newPos = newPos /*+ (projX.pos - clToX)*/;
                pos.y = newPos.y;
                pos.z = newPos.z;
                initProj();
                ObjectChanged();
            }
            return;
        }
    }
    if (dToY < dToZ) {
        qDebug() << "3" << sqdToY;
        if (sqdToY < projY.rad * projY.rad*10) {
            qDebug() << "3_2";
            /// interact with projY
            vec newPos;
            Plane plane({0,1,0},0);

            if (RayPlaneIntersection(to, plane, newPos)) {
                newPos = newPos /*+ (projY.pos - clToY)*/;
                pos.x = newPos.x;
                pos.z = newPos.z;
                initProj();
                ObjectChanged();
            }
            return;
        }

    }
    if (1) {
        qDebug() << "4" << sqdToZ;
        if (sqdToZ < projZ.rad * projZ.rad*10) {
            qDebug() << "4_2";
            /// interact with projZ
            vec newPos;
            Plane plane({0,0,1},0);

            if (RayPlaneIntersection(to, plane, newPos)) {
                newPos = newPos /*+ (projZ.pos - clToZ)*/;
                pos.y = newPos.y;
                pos.x = newPos.x;
                qDebug() << "new pos " << pos;
                initProj();
                ObjectChanged();
            }
            return;
        }

    }

    return;
}

float SelectablePoint::MousePressRay(Ray &ray)
{
    if (!selected) {
        if (ray.Distance(pos) < rad) {
            selected = true;
        }
    } else {
        float d;
        selected = isIntersects(ray, d);
        return d;
    }
    return 0;
}
