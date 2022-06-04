#include "camera.h"
#include "../customgraphicsscene.h"
#include "../MathEngine/ray.h"
#include "../MathEngine/somemath.h"
#include "line3d.h"
#include <QDebug>
#include <QElapsedTimer>
#include "point3.h"
#include <qmath.h>

Camera::Camera()

{
//    plane = Plane({100,0,0},{0,100,0},{0,0,100});
//        plane = Plane({0,0,100},{0,100,0},{0,0,0});
    plane = Plane(float3(1,1,1).Normalized(), 100);
//    cam->Set(camPoint,5,Qt::black);
    cam = new Point3(0,0,0,5,QColor(Qt::black));
    connect(&sImage, &SceneImage::UpdateImage, this, &Camera::ViewChanged);
//    cam->rad = 5;
}

Camera::Camera(Plane p)
    : plane(p)
{
}

Camera::~Camera()
{
    delete cam;
//    delete scene;
}

CustomGraphicsScene *Camera::CameraView()
{
    CustomGraphicsScene *gScene = new CustomGraphicsScene();
    gScene->setSceneRect(-400,-300,800, 600);

    connect(gScene, &CustomGraphicsScene::mouseMove, this, &Camera::mouseMove);
    connect(gScene, &CustomGraphicsScene::mousePress, this, &Camera::mousePress);
    connect(gScene, &CustomGraphicsScene::mouseRelease, this, &Camera::mouseRelease);

    sImage.Clear();

    QElapsedTimer timer;
    timer.start();
    for (auto i: scene->items()) {
        auto group = i->DrawOnCameraView(*this);
//        for (auto j : group->childItems())
//            gScene->addItem(j);
//        qDebug() << group;
        delete group;
    }
    for (auto i : extraItems) {
        auto group = i->DrawOnCameraView(*this);
//        for (auto j : group->childItems())
//                gScene->addItem(j);
        delete group;
    }
//    qDebug() << "scene" << gScene->sceneRect();
//    qDebug() << "image sceneRect" << sImage.rect << "orig" << sImage.image.rect();
//    pm = gScene->addPixmap(QPixmap::fromImage(sImage.image.scaled(sImage.rect.width(),sImage.rect.height(),
//                                                                       Qt::IgnoreAspectRatio)));
//    pm->setX(-sImage.rect.center().x());
//    pm->setY(sImage.rect.center().y());

//    qDebug() << "pm pos:" << pm->pos() ;
//    pm->setX()

//    pm->setPos(-sImage.rect.center());

    // Camera point
//    Point3 cPoint (camPoint.x,camPoint.y,camPoint.z, 10);
    qDebug() << "adding elems:" << timer.elapsed() << "ms";

    if (cam){
//        qDebug() << "try add camera";
//        for (auto j : cam->DrawOnCameraView(*this)->childItems())
//                gScene->addItem(j);
//        gScene->addItem(cam->DrawOnCameraView(*this));
    }



    return gScene;
}

MatrixF Camera::ProjectOnScreen(Object3D *obj)
{
    return obj->MatrixTransform(plane.ProjectionMatrix(camPoint, camDist, addPerspective));
}


Ray Camera::CastRayFromPoint(QPointF point)
{
    float x = point.x(), y = point.y();
//    if (plane.normal.z < 0) {
//        x *= -1;
//        y *= -1;
//    }
    Plane p;
    p.normal = plane.normal;
    p.d = 1000;
    float3 onPlane = plane.Point(x, y);
//    qDebug() << point <<  "point in casting" << onPlane << "dir" << plane.normal * -1;
    if (!addPerspective){
        return Ray(p.Point(x,y) + camPoint, plane.normal * -1);
//        return Ray(onPlane, plane.normal * -1);
    } else {
        float3 viewPoint = plane.normal * (plane.d - camDist);
        return Ray(viewPoint, -(viewPoint - onPlane).Normalized());
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
        extraItems << new Line3D(ray.pos, ray.GetPoint(1000));
        MousePress(ray);
    }
    if (btn & Qt::MouseButton::MidButton) {
        for (auto i: extraItems) {
            delete i;
        }
        extraItems.clear();
    }
    ViewChanged();
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

//        if (plane.normal.z < 0) {
//            std::swap(planeFrom, planeTo);
//        }

        float3 planeFrom = plane.Point(from.x(), from.y());
        float3 planeTo = plane.Point(to.x(), to.y());
        float angle = planeFrom.AngleBetween(planeTo);;
        qDebug() << "planeFrom:" << planeFrom << "planeTo:" << planeTo;
        qDebug() << "angle between" << angle;

        if (plane.normal.z < 0) {
//            std::swap(planeFrom, planeTo);
//            angle *= -1;
        }

        if (Abs(planeFrom.AngleBetween(planeTo)) < 1e-6) {
            return;
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
        float3 planeFrom = plane.Point(from.x(), from.y());
        float3 planeTo = plane.Point(to.x(), to.y());

//        camPoint = camPoint + (planeFrom - planeTo);
//        cam->pos = camPoint;
        sImage.rect.moveCenter(sImage.rect.center() + from - to);

        ViewChanged();
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

QPoint Camera::MapToImage(QPointF pointOnLabel)
{
    QPointF res = pointOnLabel - sImage.rect.topLeft();

    return res.toPoint();
}


void Camera::labelMousePress(QMouseEvent *event)
{
    qDebug() << "label mouse press at" << event->pos() << "in geom" << sImage.viewRect
             << "in plane" << sImage.MapToScene(event->pos()) << "in rect" << sImage.rect;
    mousePress(event->button(), sImage.MapToScene(event->pos()));

//    if (event->button() == Qt::MouseButton::RightButton) {
        lastPos = event->pos();
//    }
}

void Camera::labelMouseRelease(QMouseEvent *event)
{
    mouseRelease(event->button());
}

void Camera::labelMouseMove(QMouseEvent *event)
{
    mouseMove(event->buttons(), sImage.MapToScene(lastPos), sImage.MapToScene(event->pos()));
    lastPos = event->pos();

    QPointF delta = target_viewport_pos - event->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
        target_viewport_pos = event->pos();
        target_scene_pos =  sImage.MapToScene(event->pos());
    }
}

void Camera::labelWheel(QWheelEvent *event)
{
    qDebug() << "mouse wheel at" << event->pos() << "at scene" << sImage.MapToScene(event->pos()) << "on" << event->angleDelta().y();
    double _zoom_factor_base = 1.0015;
    if (event->orientation() == Qt::Vertical) {
      double angle = event->angleDelta().y();
      double factor = qPow(_zoom_factor_base, angle);
      gentle_zoom(factor, event->pos());
      ViewChanged();
    }
}

void Camera::gentle_zoom(double factor, QPointF pos) {

    QPointF target_scene_pos = sImage.MapToScene(pos.toPoint());
//    QPointF target_viewport_pos = pos;

    sImage.rect.setHeight(sImage.rect.height()*factor);
    sImage.rect.setWidth(sImage.rect.width()*factor);
//    sImage.rect.moveCenter(target_scene_pos);
//    QPointF delta_viewport_pos = target_viewport_pos - sImage.viewRect.center();
//    QPointF viewport_center = sImage.MapFromScene(target_scene_pos) - delta_viewport_pos;
//    sImage.rect.moveCenter(sImage.MapToScene(viewport_center.toPoint()));

    qDebug() << "image.rect after zoom" << sImage.rect << "center" << sImage.rect.center();

//    _view->scale(factor, factor);
//    _view->centerOn(target_scene_pos);
//    QPointF delta_viewport_pos = target_viewport_pos - QPointF(_view->viewport()->width() / 2.0,
//                                                               _view->viewport()->height() / 2.0);
//    QPointF viewport_center = _view->mapFromScene(target_scene_pos) - delta_viewport_pos;
//    _view->centerOn(_view->mapToScene(viewport_center.toPoint()));



//    qDebug() << "Zoomed to" << _view->viewport()->rect() << "with center" << target_scene_pos << "rect" << _view->mapToScene(_view->viewport()->rect());

//    QPolygonF polRect = _view->mapToScene(_view->viewport()->rect());

//    emit zoomed();
//    emit zoomedRect(QRectF(polRect.at(0), polRect.at(2)), _view->viewport()->rect());

}

void Camera::labelKeyPress(QKeyEvent *event)
{

}

void Camera::labelKeyRelease(QKeyEvent *event)
{

}

void Camera::labelResize(QResizeEvent *event)
{
    qDebug() << "resize to size" << event->size();
    sImage.viewRect.setHeight( event->size().height());
    sImage.viewRect.setWidth( event->size().width());
    qDebug() << "viewRect after resize" << sImage.viewRect;
}
