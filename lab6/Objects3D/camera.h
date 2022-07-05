#ifndef CAMERA_H
#define CAMERA_H

#include "clss.h"

#include "../MathEngine/float3.h"
#include "scene3d.h"
//#include "point3.h"
#include "../MathEngine/plane.h"
#include "QGraphicsScene"
#include "../customgraphicsscene.h"
#include <QObject>
#include "Graphics_view_zoom.h"
#include "sceneimage.h"

class Camera : public QObject
{
    Q_OBJECT
public:
    /// Center of all interaction. I.e. rotation around it.
    float3 camPoint = {0,0,0};
    /// Distance for perspective view
    float camDist = -500;
    /// Add perspectie view to proj matrix
    bool addPerspective = false;

    Plane plane;

    Scene3D* scene;

    QList<Object3D*> extraItems;
    Point3* cam = nullptr;

    SceneImage sImage;
    QGraphicsPixmapItem *pm = nullptr;

    QPoint lastPos;
    QPointF target_scene_pos, target_viewport_pos;

public:
    Camera();
    Camera(Plane p);
    ~Camera();

    CustomGraphicsScene* CameraView();
    MatrixF ProjectOnScreen(Object3D *obj);

    QPoint MapToImage(QPointF pointOnLabel);

private:
    /// Creates ray through point according to [non-]perspective view
    Ray CastRayFromPoint(QPointF point);

    void gentle_zoom(double factor, QPointF pos);

public slots:
    void SceneChanged();
    void mousePress(Qt::MouseButton btn, QPointF pos);
    void mouseRelease(Qt::MouseButton btn);
    void mouseMove(Qt::MouseButtons btns, QPointF from, QPointF to);

    void labelMousePress(QMouseEvent *event)  ;
    void labelMouseRelease(QMouseEvent *event);
    void labelMouseMove(QMouseEvent *event)   ;
    void labelWheel(QWheelEvent *event)       ;
    void labelKeyPress(QKeyEvent *event)      ;
    void labelKeyRelease(QKeyEvent *event)    ;
    void labelResize(QResizeEvent *event)     ;

signals:
    void ViewChanged();
    void MoveObject(Ray &from, Ray &to);
    void MousePress(Ray &ray);
};

#endif // CAMERA_H
