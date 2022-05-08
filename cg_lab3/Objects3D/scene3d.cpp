#include "scene3d.h"

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
    SceneChanged();
}

void Scene3D::addCamera(Camera *cam)
{
    cams << cam;
    cam->scene = this;

    connect(this, &Scene3D::SceneChanged, cam, &Camera::SceneChanged);
}


