#include "selectablepoint.h"
#include "line3d.h"
#include "../MathEngine/ray.h"
#include "../MathEngine/Intersections.h"
#include <functional>

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
    SelectablePoint nPoint (pos.x,pos.y,pos.z,rad,color);
//    qDebug() << prPoint;
    nPoint.FromMatrix(prPoint);
    nPoint.selected = selected;
//    if (!visible) {
//        return new QGraphicsItemGroup();
//    }

//    QGraphicsEllipseItem *el = new QGraphicsEllipseItem(nPoint.RectForPainter());
//    el->setPen(QColor(selected ? Qt::white : Qt::black));
//    el->setBrush(color);
//    el->setZValue(nPoint.pos.z + nPoint.rad);

//    QGraphicsItemGroup *group = new QGraphicsItemGroup();
//    itemOnScene = group;

//    group->addToGroup(el);

//    if (selected) {
//        QGraphicsEllipseItem *prX = new QGraphicsEllipseItem(nPoint.projX.RectForPainter());
//        prX->setPen(QColor(Qt::black));
//        prX->setBrush(projColor);
//        prX->setZValue(nPoint.projX.pos.z + nPoint.projX.rad);
//        group->addToGroup(prX);
//        QGraphicsEllipseItem *prY = new QGraphicsEllipseItem(nPoint.projY.RectForPainter());
//        prY->setPen(QColor(Qt::black));
//        prY->setBrush(projColor);
//        prY->setZValue(nPoint.projY.pos.z + nPoint.projY.rad);
//        group->addToGroup(prY);
//        QGraphicsEllipseItem *prZ = new QGraphicsEllipseItem(nPoint.projZ.RectForPainter());
//        prZ->setPen(QColor(Qt::black));
//        prZ->setBrush(projColor);
//        prZ->setZValue(nPoint.projZ.pos.z + nPoint.projZ.rad);
//        group->addToGroup(prZ);

//        QList<Line3D*> lines ({
//                                  new Line3D(nPoint.pos, nPoint.projX.pos),
//                                  new Line3D(nPoint.pos, nPoint.projY.pos),
//                                  new Line3D(nPoint.pos, nPoint.projZ.pos),
//                                  new Line3D(nPoint.unitX.pos, nPoint.projZ.pos),
//                                  new Line3D(nPoint.unitY.pos, nPoint.projZ.pos),
//                                  new Line3D(nPoint.unitX.pos, nPoint.projY.pos),
//                                  new Line3D(nPoint.unitZ.pos, nPoint.projY.pos),
//                                  new Line3D(nPoint.unitY.pos, nPoint.projX.pos),
//                                  new Line3D(nPoint.unitZ.pos, nPoint.projX.pos),
//        });


//        for (auto i : lines) {
//            i->LineToSegments(i->p1, i->p2, 10, QPen(supColor, 1), group);
//        }
//        for (auto i : lines) {
//            delete i;
//        }
//    }


    return nPoint.ToGraphGroup();
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

QGraphicsItemGroup *SelectablePoint::ToGraphGroup()
{
    if (!visible) {
        return new QGraphicsItemGroup();
    }

    QGraphicsEllipseItem *el = new QGraphicsEllipseItem(RectForPainter());
    el->setPen(QColor(selected ? Qt::white : Qt::black));
    el->setBrush(color);
    el->setZValue(pos.z + rad);

    QGraphicsItemGroup *group = new QGraphicsItemGroup();
    itemOnScene = group;

    group->addToGroup(el);

    if (selected) {
        QGraphicsEllipseItem *prX = new QGraphicsEllipseItem(projX.RectForPainter());
        prX->setPen(QColor(Qt::black));
        prX->setBrush(projColor);
        prX->setZValue(projX.pos.z + projX.rad);
        group->addToGroup(prX);
        QGraphicsEllipseItem *prY = new QGraphicsEllipseItem(projY.RectForPainter());
        prY->setPen(QColor(Qt::black));
        prY->setBrush(projColor);
        prY->setZValue(projY.pos.z + projY.rad);
        group->addToGroup(prY);
        QGraphicsEllipseItem *prZ = new QGraphicsEllipseItem(projZ.RectForPainter());
        prZ->setPen(QColor(Qt::black));
        prZ->setBrush(projColor);
        prZ->setZValue(projZ.pos.z + projZ.rad);
        group->addToGroup(prZ);

        QList<Line3D*> lines ({
                                  new Line3D(pos,       projX.pos),
                                  new Line3D(pos,       projY.pos),
                                  new Line3D(pos,       projZ.pos),
                                  new Line3D(unitX.pos, projZ.pos),
                                  new Line3D(unitY.pos, projZ.pos),
                                  new Line3D(unitX.pos, projY.pos),
                                  new Line3D(unitZ.pos, projY.pos),
                                  new Line3D(unitY.pos, projX.pos),
                                  new Line3D(unitZ.pos, projX.pos),
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

    struct dists{
        float rayD;
        float closD;
        float rad;
    } ;

    std::vector<dists> vec(4);

    float dToC;
    float3 clToC = ray.ClosestPoint(pos, dToC);
    float sqdToC = clToC.DistanceSq(pos);
    vec.push_back({dToC, sqdToC, rad});

    float dToX;
    float3 clToX = ray.ClosestPoint(projX.pos, dToX);
    float sqdToX = clToX.DistanceSq(projX.pos);
    vec.push_back({dToX, sqdToX, projX.rad});

    float dToY;
    float3 clToY = ray.ClosestPoint(projY.pos, dToY);
    float sqdToY = clToY.DistanceSq(projY.pos);
    vec.push_back({dToY, sqdToY, projY.rad});

    float dToZ;
    float3 clToZ = ray.ClosestPoint(projZ.pos, dToZ);
    float sqdToZ = clToZ.DistanceSq(projZ.pos);
    vec.push_back({dToZ, sqdToZ, projZ.rad});

    qDebug() << "Dist to cent" << sqdToC << "rad" <<rad     << "D:" << dToC;
    qDebug() << "Dist to x" << sqdToX << "rad" << projX.rad<< "D:" << dToX;
    qDebug() << "Dist to y" << sqdToY<< "rad" << projY.rad<< "D:" << dToY;
    qDebug() << "Dist to z" << sqdToZ<< "rad" << projZ.rad<< "D:" << dToZ;

    std::sort(vec.begin(), vec.end(), [&](dists d1, dists d2) {return d1.rayD > d2.rayD;});

    for (auto i : vec) {
        if (i.closD < i.rad*i.rad) {
            dist = i.rayD;
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

    struct dists{
        float rayD;
        float closD;
        float rad;
        std::function<void()>func;
    } ;

    std::vector<dists> vec(4);

    float dToC;
    float3 clToC = ray.ClosestPoint(pos, dToC);
    float sqdToC = clToC.DistanceSq(pos);
    vec.push_back({dToC, sqdToC, rad, [&](){/*vec newPos = to.GetPoint(dToC)*/ /*+ (pos - clToC)*/;
                                            pos = to.GetPoint(dToC);
                                            initProj();
                                            ObjectChanged();}});

    float dToX;
    float3 clToX = ray.ClosestPoint(projX.pos, dToX);
    float sqdToX = clToX.DistanceSq(projX.pos);
    vec.push_back({dToX, sqdToX, projX.rad, [&](){/*vec newPos = to.GetPoint(dToC)*/ /*+ (pos - clToC)*/;
                                                  float3 newPos;
                                                  Plane plane({1,0,0},0);

                                                  if (RayPlaneIntersection(to, plane, newPos)) {
                                                      /*newPos = newPos + (projX.pos - clToX)*/;
                                                      pos.y = newPos.y;
                                                      pos.z = newPos.z;
                                                      initProj();
                                                      ObjectChanged();}
                   }
                  });

    float dToY;
    float3 clToY = ray.ClosestPoint(projY.pos, dToY);
    float sqdToY = clToY.DistanceSq(projY.pos);
    vec.push_back({dToY, sqdToY, projY.rad, [&](){/*vec newPos = to.GetPoint(dToC)*/ /*+ (pos - clToC)*/;
                                                  float3 newPos;
                                                  Plane plane({0,1,0},0);

                                                  if (RayPlaneIntersection(to, plane, newPos)) {
                                                      /*newPos = newPos*/ /*+ (projY.pos - clToY)*/;
                                                      pos.x = newPos.x;
                                                      pos.z = newPos.z;
                                                      initProj();
                                                      ObjectChanged();
                                                  }
                   }
                  });

    float dToZ;
    float3 clToZ = ray.ClosestPoint(projZ.pos, dToZ);
    float sqdToZ = clToZ.DistanceSq(projZ.pos);
    vec.push_back({dToZ, sqdToZ, projZ.rad, [&](){/*vec newPos = to.GetPoint(dToC)*/ /*+ (pos - clToC)*/;
                                                  float3 newPos;
                                                  Plane plane({0,0,1},0);

                                                  if (RayPlaneIntersection(to, plane, newPos)) {
                                                      /*newPos = newPos*/ /*+ (projZ.pos - clToZ)*/;
                                                      pos.y = newPos.y;
                                                      pos.x = newPos.x;
//                                                      qDebug() << "new sel point pos " << pos;
                                                      initProj();
                                                      ObjectChanged();
                                                  }
                   }
                  });


    if (sqdToC < rad*rad) {
        pos = to.GetPoint(dToC);
        initProj();
        ObjectChanged();
        return;
    }

    std::sort(vec.begin(), vec.end(), [&](dists d1, dists d2) {return d1.rayD > d2.rayD;});

    for (auto i : vec) {
        if (i.closD < i.rad*i.rad) {
            i.func();
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
