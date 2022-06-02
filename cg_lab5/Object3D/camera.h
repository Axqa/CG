#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "../MathEngine/float3.h"
#include "../MathEngine/plane.h"
#include "scene3d.h"


class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

    vec pos   ;
    vec normal;

    float width = 600;    // dx in world coords
    float height = 400;   // dy in world coords

    Scene3D *scene;

public:
    SceneView& UpdateView(SceneView& view);

public slots:
    void MousePressed(Qt::MouseButton btn, QPoint pos);

signals:

};

#endif // CAMERA_H
