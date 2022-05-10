#ifndef CAMERA_H
#define CAMERA_H

#include "clss.h"

#include "../MathEngine/float3.h"
#include "scene3d.h"
#include "../MathEngine/plane.h"
#include "QGraphicsScene"
#include "../customgraphicsscene.h"
#include <QObject>

class Camera : public QObject
{
    Q_OBJECT
public:
    /// Center of all interaction. I.e. rotation around it.
    float3 camPoint;
    /// Distance for perspective view
    float camDist = 10;
    /// Add perspectie view to proj matrix
    bool addPerspective = false;

    Plane plane;

    Scene3D* scene;

public:
    Camera();
    Camera(Plane p);
    ~Camera();

    CustomGraphicsScene* CameraView();
    MatrixF ProjectOnScreen(Object3D *obj);

private:
    /// Creates ray through point according to [non-]perspective view
    Ray CastRayFromPoint(QPointF point);

public slots:
    void SceneChanged();
    void mousePress(Qt::MouseButton btn, QPointF pos);
    void mouseRelease(Qt::MouseButton btn);
    void mouseMove(Qt::MouseButtons btns, QPointF from, QPointF to);

signals:
    void ViewChanged();
};

#endif // CAMERA_H
