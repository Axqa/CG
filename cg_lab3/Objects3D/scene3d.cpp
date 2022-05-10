#include "scene3d.h"
#include "../MathEngine/ray.h"
#include "selectablepoint.h"

Scene3D::Scene3D()
{

}

Scene3D::~Scene3D()
{
    for (auto i: elems)
        delete i;
    elems.clear();

    for (auto i: cams)
        delete i;
    cams.clear();

}

void Scene3D::addObject(Object3D *el)
{
    elems << el;
    connect(el, &Object3D::ObjectChanged, this, &Scene3D::SceneChanged);

    switch (el->type()) {
    case Type3d::SelectablePoint:
        selectables << (SelectablePoint*)el;
        qDebug() << "add selectable";
        break;
    }

    SceneChanged();
}

void Scene3D::addCamera(Camera *cam)
{
    cams << cam;
    cam->scene = this;

    connect(this, &Scene3D::SceneChanged, cam, &Camera::SceneChanged);
}

QList<Object3D *> Scene3D::items()
{
    return elems;
}

void Scene3D::MoveRay(Ray &from, Ray &to)
{
//    qDebug() << "Scene gets rays";
    for (auto i : selectables) {
        i->MovingRay(from, to);
    }
}


