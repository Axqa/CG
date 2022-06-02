#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QObject>
#include "../MathEngine/ray.h"
#include "../MathEngine/aabb.h"

class Object3D : public QObject
{
    Q_OBJECT
public:
    explicit Object3D(QObject *parent = nullptr);

    bool selected = true;

    AABB boundingBox;

    /// Returns true if interscects and set color to color of this,
    /// dist to dist from ray origin
    virtual bool GetColorByRay(Ray &ray, QColor &color, float &dist) = 0;
    virtual bool RayAABBIntersect(Ray &ray);
    virtual void RecalcBB() = 0;

signals:
    void ObjectChanged();

};

#endif // OBJECT3D_H
