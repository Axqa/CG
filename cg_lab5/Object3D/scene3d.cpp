#include "scene3d.h"
#include "object3d.h"
#include "camera.h"

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

Object3D * Scene3D::addObject(Object3D *el)
{
    elems << el;
    connect(el, &Object3D::ObjectChanged, this, &Scene3D::SceneChanged);

//    if (dynamic_cast<SelectablePoint*>(el)) {
//        selectables << (SelectablePoint*)el;
//        qDebug() << "add selectable point " << el;
//    } else if (dynamic_cast<BezierPlane*>(el)) {
//        selectables << (BezierPlane*)el;
//        qDebug() << "add selectable plane " << el;
//    }
//    switch (el->type()) {
//    case Type3d::SelectablePoint:
//        selectables << (SelectablePoint*)el;
//        qDebug() << "add selectable";
//        break;
//    }

    SceneChanged();

    return el;
}

Camera * Scene3D::addCamera(Camera *cam)
{
    cams << cam;
    cam->scene = this;

//    connect(this, &Scene3D::SceneChanged, cam, &Camera::SceneChanged);

//    connect(cam, &Camera::MoveObject, this, &Scene3D::MoveRay);
//    connect(cam, &Camera::MousePress, this, &Scene3D::PressRay);

    return cam;
}

QList<Object3D *> Scene3D::items()
{
    return elems;
}

QList<Camera *> Scene3D::cameras()
{
    return cams;
}

void Scene3D::MoveRay(Ray &from, Ray &to)
{
//    qDebug() << "Scene gets rays";

//    if (selectables.size() == 0)
//        return;
//    if (selected != nullptr)
//        selected->MovingRay(from,to);
}

void Scene3D::PressRay(Ray &ray)
{
//    qDebug() << "press ray"<<selectables.size();
//    if (selectables.size() == 0)
//        return;
//    float minD = FLOAT_INF;
//    float curD;
//    Selectable* curP = nullptr;
//    for (auto i : selectables) {

//        if (auto j = dynamic_cast<BezierPlane*>(i)) {
//            for (auto row : j->controlPoints) {
//                for (auto bezierPoint : row) {
//                    if (bezierPoint->isIntersects(ray, curD)) {
//                        if (curD < minD) {
//                            curP = bezierPoint;
//                            minD = curD;
//                        }
//                    }
//                }
//            }

//        } else {

//            if (i->isIntersects(ray, curD)) {
//                if (curD < minD) {
//                    curP = i;
//                    minD = curD;
//                }
//            }
//        }
//    }
//    if (curP == nullptr) {
//        if (selected){
//            selected->setSelected(false);
//            selected = nullptr;
//            SceneChanged();
//        }
//    } else {
//        if (curP != selected) {
//            if (selected)
//                selected->setSelected(false);
//            curP->setSelected(true);
//            selected = curP;
//            SceneChanged();
//        }
//    }
}
