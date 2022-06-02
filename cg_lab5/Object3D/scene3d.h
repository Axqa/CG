#ifndef SCENE3D_H
#define SCENE3D_H

#include <QObject>
#include "classes.h"
#include "../MathEngine/ray.h"
#include "sceneview.h"
#include <QColor>

class Scene3D : public QObject
{
    Q_OBJECT
public:
    Scene3D();
    ~Scene3D();

    QList <Object3D*> elems;
    QList <Camera*> cams;

public:

    Object3D * addObject(Object3D *el);
    Camera * addCamera(Camera *cam);

    // to have foreach-like loop
    decltype(elems.begin()) begin() {return elems.begin();};
    decltype(elems.end()) end() {return elems.end();};

    QList<Object3D*> items();
    QList<Camera*> cameras();



public slots:
        void MoveRay(Ray &from, Ray &to);
        void PressRay(Ray &ray);

signals:
        void SceneChanged();

};

#endif // SCENE3D_H
