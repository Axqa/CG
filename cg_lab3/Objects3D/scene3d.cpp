#include "scene3d.h"
#include "../MathEngine/ray.h"
#include "../MathEngine/MathConstants.h"
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

    if (dynamic_cast<SelectablePoint*>(el)) {
        selectables << (SelectablePoint*)el;
        qDebug() << "add selectable";
    }
//    switch (el->type()) {
//    case Type3d::SelectablePoint:
//        selectables << (SelectablePoint*)el;
//        qDebug() << "add selectable";
//        break;
//    }

    SceneChanged();
}

void Scene3D::addCamera(Camera *cam)
{
    cams << cam;
    cam->scene = this;

    connect(this, &Scene3D::SceneChanged, cam, &Camera::SceneChanged);

    connect(cam, &Camera::MoveObject, this, &Scene3D::MoveRay);
    connect(cam, &Camera::MousePress, this, &Scene3D::PressRay);
}

QList<Object3D *> Scene3D::items()
{
    return elems;
}

void Scene3D::MoveRay(Ray &from, Ray &to)
{
//    qDebug() << "Scene gets rays";
//    for (auto i : selectables) {
//        i->MovingRay(from, to);
//    }
    if (selectables.size() == 0)
        return;
    if (selected != nullptr)
        selected->MovingRay(from,to);
}

void Scene3D::PressRay(Ray &ray)
{
//    qDebug() << "press ray";
    if (selectables.size() == 0)
        return;
    float minD = FLOAT_INF;
    float curD;
    SelectablePoint* curP = nullptr;
    for (auto i : selectables) {
        if (i->isIntersects(ray, curD)) {
            if (curD < minD) {
                curP = i;
                minD = curD;
            }
        }
    }
    if (curP == nullptr) {
        if (selected){
            selected->setSelected(false);
            selected = nullptr;
            SceneChanged();
        }
    } else {
        if (curP != selected) {
            if (selected)
                selected->setSelected(false);
            curP->setSelected(true);
            selected = curP;
            SceneChanged();
        }
    }
}


