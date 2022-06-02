#include "object3d.h"

Object3D::Object3D(QObject *parent) : QObject(parent)
{

}

bool Object3D::RayAABBIntersect(Ray &ray)
{
    return boundingBox.Intersects(ray);
}
