#include "camera.h"
#include "../customgraphicsscene.h"
#include "../MathEngine/ray.h"
#include "line3d.h"
#include <QDebug>
#include <QElapsedTimer>

Camera::Camera()
{
//    plane = Plane({100,0,0},{0,100,0},{0,0,100});
//        plane = Plane({0,0,100},{0,100,0},{0,0,0});
    plane = Plane(float3(1,1,1).Normalized(), 300);

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

    connect(gScene, &CustomGraphicsScene::mouseMove, this, &Camera::mouseMove);
    connect(gScene, &CustomGraphicsScene::mousePress, this, &Camera::mousePress);
    connect(gScene, &CustomGraphicsScene::mouseRelease, this, &Camera::mouseRelease);

    QElapsedTimer timer;
    timer.start();
    for (auto i: scene->items()) {
        for (auto j : i->DrawOnCameraView(*this)->childItems())
            gScene->addItem(j);
//        gScene->addItem(i->DrawOnCameraView(*this));

    }
    for (auto i : extraItems) {
        for (auto j : i->DrawOnCameraView(*this)->childItems())
                gScene->addItem(j);
    }
    qDebug() << "adding elems:" << timer.elapsed() << "ms";



    return gScene;
}

MatrixF Camera::ProjectOnScreen(Object3D *obj)
{
    return obj->MatrixTransform(plane.ProjectionMatrix(camPoint, camDist, addPerspective));
}

Ray Camera::CastRayFromPoint(QPointF point)
{
    float x = point.x(), y = point.y();
    if (plane.normal.z < 0) {
        std::swap(x, y);
    }
    float3 onPlane = plane.Point(x, y);
    qDebug() << "point in casting" << onPlane;
    if (!addPerspective){
        return Ray(onPlane, plane.normal * -1);
    } else {
        float3 viewPoint = plane.normal * (plane.d + camDist);
        return Ray(onPlane, (onPlane - viewPoint).Normalized());
    }

}


void Camera::SceneChanged()
{
//    CameraView();
    ViewChanged();
}

void Camera::mousePress(Qt::MouseButton btn, QPointF pos)
{
    if (btn & Qt::MouseButton::LeftButton) {
        qDebug() << "Mouse press in space:" << plane.Point(pos.x(),pos.y());
        Ray ray = CastRayFromPoint(pos);
        extraItems << new Line3D(ray.pos, ray.GetPoint(500));
        MousePress(ray);
    }
    if (btn & Qt::MouseButton::MidButton) {
        for (auto i: extraItems) {
            delete i;
        }
        extraItems.clear();
    }
}

void Camera::mouseRelease(Qt::MouseButton btn)
{

}

void Camera::mouseMove(Qt::MouseButtons btns, QPointF from, QPointF to)
{
//    qDebug() << "from" << from << "to" << to;
    if (btns == 0) {
        return;
    }
    if (btns & Qt::MouseButton::RightButton) {
        /// Here must be rotation

        float3 planeFrom = plane.Point(from.x(), from.y());
        float3 planeTo = plane.Point(to.x(), to.y());
        qDebug() << "planeFrom:" << planeFrom << "planeTo:" << planeTo;
        qDebug() << "angle between" << planeFrom.AngleBetween(planeTo);

        if (plane.normal.z < 0) {
            std::swap(planeFrom, planeTo);
        }

//        float3 offset = planeFrom - plane.normal * planeFrom.Length();
//        float3 newTo = planeTo - offset;
//        float3 newTo = plane.normal * planeTo.Length() + (planeFrom - planeTo);
        float3 newTo = plane.normal.RodriguesRotation(planeFrom.Cross(planeTo).Normalized(), planeFrom.AngleBetween(planeTo));
        qDebug() << "norm angle:" << plane.normal.AngleBetweenNorm(newTo.Normalized());
//                 << "old normal:" << plane.normal.x << plane.normal.y << plane.normal.z
         qDebug()<< "new normal" << newTo.Normalized();
        plane.normal = newTo.Normalized() ;
        ViewChanged();
        return;
    }
    if (btns.testFlag(Qt::MouseButton::MiddleButton)) {
        /// Here must be moving

        return;
    }
    if (btns.testFlag(Qt::MouseButton::LeftButton)) {
        /// Here must be emiting signal to scene with casted ray

        Ray rayFrom = CastRayFromPoint(from);
        Ray rayTo = CastRayFromPoint(to);

        MoveObject(rayFrom, rayTo);

        return;
    }
}
