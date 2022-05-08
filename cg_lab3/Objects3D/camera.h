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
    // maybe change on angle in camera and add width and height for screen
    float3 camPoint;

    Plane plane;

    Scene3D* scene;

public:
    Camera();
    Camera(Plane p);
    ~Camera();

    CustomGraphicsScene* CameraView();
    MatrixF ProjectOnScreen(Object3D *obj);

public slots:
    void SceneChanged();

signals:
    void ViewChanged();
};

#endif // CAMERA_H
