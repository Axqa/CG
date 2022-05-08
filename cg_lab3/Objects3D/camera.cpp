#include "camera.h"
#include "../customgraphicsscene.h"

Camera::Camera()
{

}

Camera::Camera(Plane p)
    : plane(p)
{
}

Camera::~Camera()
{
//    delete scene;
}

QGraphicsScene *Camera::CameraView()
{
    QGraphicsScene *gScene = new CustomGraphicsScene();
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
