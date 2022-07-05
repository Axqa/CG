#include "scene3d.h"
#include "../MathEngine/ray.h"
#include "../MathEngine/MathConstants.h"
#include "selectablepoint.h"
#include "bezierplane.h"
#include "pointlight.h"

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

    if (dynamic_cast<SelectablePoint*>(el)) {
        selectables << (SelectablePoint*)el;
        qDebug() << "add selectable point " << el;
    } else if (dynamic_cast<BezierPlane*>(el)) {
        selectables << (BezierPlane*)el;
        qDebug() << "add selectable plane " << el;
    }
    if (dynamic_cast<PointLight*>(el))
    {
        lights << (PointLight*)(el);
    }

    SceneChanged();

    return el;
}

Camera * Scene3D::addCamera(Camera *cam)
{
    cams << cam;
    cam->scene = this;

    connect(this, &Scene3D::SceneChanged, cam, &Camera::SceneChanged);

    connect(cam, &Camera::MoveObject, this, &Scene3D::MoveRay);
    connect(cam, &Camera::MousePress, this, &Scene3D::PressRay);

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

float Scene3D::ComputeLightForPointOnPlane(vec p, Plane plane)
{
    Material mat {0.8,0.9};

    float intensity = baseLightIntensity;


    if (lights.size() > 0)
    for (auto l : lights) {
        intensity += l->IntensityForPointOnPlane(p, plane, cams.at(0)->plane.normal*10000, mat);
    }

    intensity = __max(__min(1, intensity), 0);
    return intensity;
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
    qDebug() << "press ray"<<selectables.size();
    if (selectables.size() == 0)
        return;
    float minD = FLOAT_INF;
    float curD;
    Selectable* curP = nullptr;
    for (auto i : selectables) {

        if (auto j = dynamic_cast<BezierPlane*>(i)) {
            for (auto row : j->controlPoints) {
                for (auto bezierPoint : row) {
                    if (bezierPoint->isIntersects(ray, curD)) {
                        if (curD < minD) {
                            curP = bezierPoint;
                            minD = curD;
                        }
                    }
                }
            }

        } else {

            if (i->isIntersects(ray, curD)) {
                if (curD < minD) {
                    curP = i;
                    minD = curD;
                }
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


