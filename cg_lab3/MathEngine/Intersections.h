#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "MathAll.h"
#include <QDebug>

static bool RayPlaneIntersection(Ray &ray, Plane &plane, vec &point) {
    vec p = plane.PointOnPlane();
    qDebug() << "RP int: p=" << p;
    float denom = -plane.normal.Dot(-ray.dir);
    qDebug() << "denom" << denom;
    if (denom > 1e-6) {
        vec pl = p - ray.pos;
        qDebug() << "pl" << pl;
        float t = pl.Dot(plane.normal) / denom;
        qDebug() << "t = " << t;
        if (t >= 0) {
            point = ray.GetPoint(t);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

#endif // INTERSECTIONS_H
