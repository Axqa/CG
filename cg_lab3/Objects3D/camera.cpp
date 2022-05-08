#include "camera.h"
#include "../customgraphicsscene.h"

Camera::Camera()
{
//    plane = Plane({100,0,0},{0,100,0},{0,0,100});
//        plane = Plane({0,0,100},{0,100,0},{0,0,0});
    plane = Plane(float3(1,1,1).Normalized(), 0);
}

Camera::Camera(Plane p)
    : plane(p)
{
}

Camera::~Camera()
{
//    delete scene;
}

CustomGraphicsScene *Camera::CameraView()
{
    CustomGraphicsScene *gScene = new CustomGraphicsScene();
    gScene->setSceneRect(-400,-300,800, 600);

    for (auto i: *scene) {
        gScene->addItem(i->DrawOnCameraView(*this));

    }

    return gScene;
}

MatrixF Camera::ProjectOnScreen(Object3D *obj)
{
    return obj->MatrixTransform(plane.ProjectionMatrix());
}

void Camera::SceneChanged()
{
//    CameraView();
    ViewChanged();
}
